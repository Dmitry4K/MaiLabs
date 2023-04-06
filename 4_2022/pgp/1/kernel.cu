#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <iostream>

#define CUDA_ERROR_HANDLER(err) do { if (err != cudaSuccess) { std::cout << cudaGetErrorString(err) << std::endl; exit(0);} } while (0)
#define CEH(err) CUDA_ERROR_HANDLER(err)

#define BLOCKS dim3(100,100)
#define THREADS 1024
#define min(a, b) a < b ? a : b

__global__ void minKernel(long long n, const double* a, const double* b, double* c) {
    long long i = blockIdx.x * blockDim.x + threadIdx.x;
    int offset = blockDim.x * gridDim.x;
    for (; i < n; i += offset) {
        c[i] = min(a[i], b[i]);
    }
}

int main() {
    long long n;
    std::cin >> n;
    double* a, * b, *c;
    double* dev_a, * dev_b, * dev_c = 0;

    a = new double[n];
    b = new double[n];
    c = new double[n];

    for (long long i = 0; i < n; ++i) {
        std::cin >> a[i];
    }

    for (long long i = 0; i < n; ++i) {
        std::cin >> b[i];
    }

    CUDA_ERROR_HANDLER(cudaMalloc((void**)&dev_a, n * sizeof(double)));
    CUDA_ERROR_HANDLER(cudaMalloc((void**)&dev_b, n * sizeof(double)));
    CUDA_ERROR_HANDLER(cudaMalloc((void**)&dev_c, n * sizeof(double)));

    CUDA_ERROR_HANDLER(cudaMemcpy(dev_a, a, n * sizeof(double), cudaMemcpyHostToDevice));
    CUDA_ERROR_HANDLER(cudaMemcpy(dev_b, b, n * sizeof(double), cudaMemcpyHostToDevice));

    minKernel <<<BLOCKS, THREADS>>> (n , dev_a, dev_b, dev_c); 
    CUDA_ERROR_HANDLER(cudaDeviceSynchronize());

    CUDA_ERROR_HANDLER(cudaMemcpy(c, dev_c, n * sizeof(double), cudaMemcpyDeviceToHost));
    
    std::cout << std::scientific;
    std::cout.precision(11);
    for (long long i = 0; i < n; ++i) {
        std::cout << c[i] << ' ';
    }

    CUDA_ERROR_HANDLER(cudaFree(dev_c));
    CUDA_ERROR_HANDLER(cudaFree(dev_a));
    CUDA_ERROR_HANDLER(cudaFree(dev_b));
    delete a;
    delete b;
    delete c;
    
    exit(0);
}
