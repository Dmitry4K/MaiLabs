#include <stdio.h>
#include <vector>
#include <cuda_runtime_api.h>
#include <device_launch_parameters.h>
#include <thrust/scan.h>
#include <thrust/extrema.h>
#include <thrust/device_ptr.h>
#include <stack>
#include <iostream>

#define FLT_MAX 3.402823466e+38F
#define FLT_MIN 1.175494351e-38F

#define REDUCE_BLOCKS 256
#define REDUCE_TRHREADS_COUNT 256
#define BITONIC_SORT_BLOCKS_COUNT 512
#define BITONIC_SORT_THREADS_COUNT 512
#define RESET_BLOCKS_COUNT 32
#define RESET_THREADS_COUNT 32
#define COPY_BLOCKS_COUNT 64
#define THREADS_COPY_COUNT 32
#define COUNTING_SORT_BLOCKS 64
#define COUNTING_SORT_THREADS 32
#define POCKET_SIZE 1024


struct block_view {
    int start, end;
    __host__ __device__ int length() {
        return end - start + 1;
    }
};

__device__ void load_block_from_shared_to_global(
    volatile float* shared, volatile float* global, int tid, block_view view
) {
    if (tid < view.length()) global[view.start + tid] = shared[tid];
    if (tid + blockDim.x < view.length()) global[view.start + tid + blockDim.x] = shared[tid + blockDim.x];
    __syncthreads();
}

__device__ void load_block_from_global_to_shared(
    volatile float* shared, volatile float* global, int tid, block_view view
) {
    shared[tid] = (tid < view.length() ? global[view.start + tid] : FLT_MAX);
    shared[tid + blockDim.x] = (tid + blockDim.x < view.length() ? global[view.start + tid + blockDim.x] : FLT_MAX);
    __syncthreads();
}

__device__ void bytonic_sort_for_shared(volatile float* shared) { 
    for (int bitonic_length = 2; bitonic_length <= 2 * blockDim.x; bitonic_length *= 2) { 
        for (int block_size = bitonic_length; block_size >= 2; block_size /= 2) { 

            int filter_offset = block_size / 2;
            int block_offset = threadIdx.x / filter_offset * block_size;
            int filter_index = threadIdx.x % filter_offset;
            int first = block_offset + filter_index;
            int second = first + filter_offset;

            float min = fmin(shared[first], shared[second]);
            float max = fmax(shared[first], shared[second]);

            if (first & bitonic_length) {
                shared[first] = max;
                shared[second] = min;
            }
            else {
                shared[first] = min;
                shared[second] = max;
            }
            __syncthreads();
        }
    }
}

__global__ void bitonic_sort_global(float* global, block_view* blocks, int blocks_size) {
    extern __shared__ float shared[];
    for (int block_id = blockIdx.x; block_id < blocks_size; block_id += gridDim.x) {
        load_block_from_global_to_shared(shared, global, threadIdx.x, blocks[block_id]);
        bytonic_sort_for_shared(shared);
        load_block_from_shared_to_global(shared, global, threadIdx.x, blocks[block_id]);
    }
}

__device__ void reduce_for_mins_small(volatile float* mins) {
    for (int offset = 32; offset >= 1; offset /= 2)
        mins[threadIdx.x] = fmin(mins[threadIdx.x], mins[threadIdx.x + offset]);
}

__device__ void reduce_for_maxs_small(volatile float* maxs) {
    for (int offset = 32; offset >= 1; offset /= 2) 
        maxs[threadIdx.x] = fmax(maxs[threadIdx.x], maxs[threadIdx.x + offset]);
}

__device__ void reduce_for_mins_big(volatile float* shared_mins, volatile float* g_mem, int n) { 
    int idx = 2 * blockIdx.x * blockDim.x + threadIdx.x; 
    int offset = 2 * blockDim.x * gridDim.x; 
    shared_mins[threadIdx.x] = FLT_MAX; 
    for (; (idx + blockDim.x) < n; idx += offset) { 
        shared_mins[threadIdx.x] = fmin(fmin(shared_mins[threadIdx.x], g_mem[idx]), g_mem[idx + blockDim.x]); 
    }

    if (idx < n) {
        shared_mins[threadIdx.x] = fmin(shared_mins[threadIdx.x], g_mem[idx]);
    }
    __syncthreads();
}

__device__ void reduce_for_maxs_big(
    volatile float* shared_maxs, volatile float* g_mem, int n
) {
    int idx = 2 * blockIdx.x * blockDim.x + threadIdx.x;
    int offset = 2 * blockDim.x * gridDim.x;
    shared_maxs[threadIdx.x] = FLT_MIN;
    for (; (idx + blockDim.x) < n; idx += offset) {
        shared_maxs[threadIdx.x] = fmax(fmax(shared_maxs[threadIdx.x], g_mem[idx]), g_mem[idx + blockDim.x]);
    }
    if (idx < n) {
        shared_maxs[threadIdx.x] = fmax(shared_maxs[threadIdx.x], g_mem[idx]);
    }
    __syncthreads();
}

__global__ void block_reduce_mins_kernel(float* g_mem, float* block_mins, int n) {
    extern __shared__ float s_memory[];
    reduce_for_mins_big(s_memory, g_mem, n);

    for (int offset = blockDim.x / 2; offset >= 64; offset /= 2) {
        if (threadIdx.x < offset)
            s_memory[threadIdx.x] = fmin(s_memory[threadIdx.x], s_memory[threadIdx.x + offset]);
        __syncthreads();
    }
    if (threadIdx.x < 32) reduce_for_mins_small(s_memory);

    if (threadIdx.x == 0) block_mins[blockIdx.x] = s_memory[0];
}

__global__ void block_reduce_maxs_kernel(float* g_mem, float* block_maxs, int n) {
    extern __shared__ float s_memory[];
    reduce_for_maxs_big(s_memory, g_mem, n);

    for (int offset = blockDim.x / 2; offset >= 64; offset /= 2) {
        if (threadIdx.x < offset)
            s_memory[threadIdx.x] = fmax(s_memory[threadIdx.x], s_memory[threadIdx.x + offset]);
        __syncthreads();
    }
    if (threadIdx.x < 32) reduce_for_maxs_small(s_memory);
    if (threadIdx.x == 0) block_maxs[blockIdx.x] = s_memory[0];
}


__global__ void hist_kernel(
    float* src, float* dst, int n,
    int* hist, int hist_size,
    float minimum, float maximum) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int offset = blockDim.x * gridDim.x;
    for (int i = idx;  i < n; i += offset) {
        int mult_coef = ((src[i] - minimum) / (maximum - minimum));
        int pocket = hist_size - 1;
        if (mult_coef > 1) {
            pocket *= mult_coef;
        }
        atomicAdd(hist + pocket, 1u);
        dst[i] = src[i];
    }
}


__global__ void counting_sort(
    float* src, float* dst, int n,
    int* scan, int scan_size,
    float minimum, float maximum
) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    int offset = gridDim.x * blockDim.x;
    for (int i = tid; i < n; i += offset) {
        int pocket = scan_size - 1;
        int mult_coef = (src[i] - minimum) / (maximum-minimum);
        if (mult_coef > 1) pocket *= mult_coef;
        dst[atomicAdd(scan + pocket, 1)] = src[i];
    }
}


__global__ void set_nulls(int* arr, int size) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int offset = gridDim.x * blockDim.x;
    for (int i = idx; i < size; i += offset) arr[i] = 0; 
}


float find_min(float* arr, int n) { 
    float min = arr[0]; 
    for (int i = 1; i < n; ++i) min = std::min(min, arr[i]); 
    return min; 
}

float find_max(float* arr, int n) {
    float max = arr[0];
    for (int i = 1; i < n; ++i) max = std::max(max, arr[i]);
    return max;
}

const int partition_effort = 2;
#define IOSTREAM

int main() {
    std::ios::sync_with_stdio(false);
    int arr_size;
#ifndef IOSTREAM
    fread(&arr_size, sizeof(int), 1, stdin);
#else
    std::cin >> arr_size;
#endif

    float* arr = (float*)malloc(arr_size * sizeof(float));

#ifndef IOSTREAM
    fread(arr, sizeof(float), arr_size, stdin);
    for (int i = 0; i < arr_size; ++i) std::cerr << arr[i] << ' ';
#else
    for (int i = 0; i < arr_size; ++i) arr[i] = float(arr_size - i) + 0.5;
#endif
    const int pockets_max_count = ((arr_size + POCKET_SIZE - 1) / POCKET_SIZE) * partition_effort;

    float* mins = (float*)malloc(REDUCE_BLOCKS * sizeof(float));
    float* maxs = (float*)malloc(REDUCE_BLOCKS * sizeof(float));
    int*   hist = (int*)  malloc(pockets_max_count * sizeof(int));
    int*   scan = (int*)  malloc(pockets_max_count * sizeof(int));

    float* dev_mins = nullptr;
    float* dev_maxs = nullptr;
    float* dev_array = nullptr;
    float* dev_counting_sort_buffer = nullptr;
    int* dev_hist_for_trust = nullptr;
    int* dev_scan = nullptr;

    fprintf(stderr, "%d\n", arr_size);
    cudaMalloc(&dev_mins, REDUCE_BLOCKS * sizeof(float));
    cudaMalloc(&dev_maxs, REDUCE_BLOCKS * sizeof(float));
    cudaMalloc(&dev_array, arr_size * sizeof(float));
    cudaMalloc(&dev_counting_sort_buffer, arr_size * sizeof(float));
    cudaMalloc(&dev_hist_for_trust, pockets_max_count * sizeof(int));
    cudaMalloc(&dev_scan, pockets_max_count * sizeof(int));
    
    thrust::device_ptr<int> dev_trust_hist = thrust::device_pointer_cast(dev_hist_for_trust);
    thrust::device_ptr<int> thrust_scan = thrust::device_pointer_cast(dev_scan);

    cudaMemcpy(dev_array, arr, arr_size * sizeof(float), cudaMemcpyHostToDevice);

    std::stack<block_view> pool;
    std::vector<block_view> pockets;

    if (arr_size > 0) {
        pool.push({ 0, arr_size-1 });
    }

    while (!pool.empty()) {
        block_view view = pool.top();
        pool.pop();
        if (view.length() <= POCKET_SIZE) {
            pockets.push_back(view);
            continue;
        }
        block_reduce_mins_kernel << <REDUCE_BLOCKS, REDUCE_TRHREADS_COUNT,
            2 * REDUCE_TRHREADS_COUNT * sizeof(float) >> > (dev_array + view.start, dev_mins, view.length());
        block_reduce_maxs_kernel << <REDUCE_BLOCKS, REDUCE_TRHREADS_COUNT,
            2 * REDUCE_TRHREADS_COUNT * sizeof(float) >> > (dev_array + view.start, dev_maxs, view.length());
        cudaMemcpy(mins, dev_mins, REDUCE_BLOCKS
            * sizeof(float), cudaMemcpyDeviceToHost);
        cudaMemcpy(maxs, dev_maxs, REDUCE_BLOCKS
            * sizeof(float), cudaMemcpyDeviceToHost);
        float min = find_min(mins, REDUCE_BLOCKS);
        float max = find_max(maxs, REDUCE_BLOCKS);

        if (min != max) {
            int pockets_count = ((view.length() + POCKET_SIZE - 1) / POCKET_SIZE) * partition_effort;
            set_nulls << <RESET_BLOCKS_COUNT, RESET_THREADS_COUNT >> > (dev_hist_for_trust, pockets_count);
            cudaDeviceSynchronize();

            hist_kernel << <COPY_BLOCKS_COUNT, THREADS_COPY_COUNT >> > (dev_array + view.start,
                dev_counting_sort_buffer, view.length(), dev_hist_for_trust, pockets_count, min, max);
            cudaDeviceSynchronize();

            thrust::exclusive_scan(dev_trust_hist, dev_trust_hist + pockets_count, thrust_scan);
            cudaMemcpy(hist, dev_hist_for_trust, pockets_count * sizeof(int), cudaMemcpyDeviceToHost);
            cudaMemcpy(scan, dev_scan, pockets_count * sizeof(int), cudaMemcpyDeviceToHost);

            counting_sort << <COUNTING_SORT_BLOCKS, COUNTING_SORT_THREADS >> > (
                dev_counting_sort_buffer, dev_array + view.start, view.length(),
                dev_scan, pockets_count, min, max);
            cudaDeviceSynchronize();

            for (int i = 0; i < pockets_count; ++i) {
                if (hist[i] != 0) {
                    //fprintf(stderr, "%d %d\n", (int)view.start + scan[i], (int)view.start + scan[i] + hist[i] - 1);
                    pool.push({ view.start + scan[i], view.start + scan[i] + hist[i] - 1 });
                }
            }
        }
        //fprintf(stderr, "%d\n", (int)pool.size());
    }

    cudaFree(dev_hist_for_trust);
    cudaFree(dev_scan);
    cudaFree(dev_counting_sort_buffer);
    cudaFree(dev_mins);
    cudaFree(dev_maxs);
    free(maxs);
    free(mins);
    free(scan);
    free(hist);

    block_view* dev_pockets = nullptr;
    cudaMalloc(&dev_pockets, pockets.size() *  sizeof(block_view));
    cudaMemcpy(dev_pockets, pockets.data(), pockets.size() * sizeof(block_view), cudaMemcpyHostToDevice);

    bitonic_sort_global << <BITONIC_SORT_BLOCKS_COUNT, BITONIC_SORT_THREADS_COUNT,
        2 * BITONIC_SORT_THREADS_COUNT * sizeof(float) >> > (
            dev_array, dev_pockets, pockets.size());

    cudaMemcpy(arr, dev_array, arr_size * sizeof(float), cudaMemcpyDeviceToHost);
    cudaFree(dev_pockets);

#ifndef IOSTREAM
    fwrite((char*)(arr), sizeof(float), arr_size, stdout);
#else
    for (int i = 0; i < arr_size; ++i) std::cout << arr[i] << ' ';
#endif
    free(arr);
    return 0;
}
