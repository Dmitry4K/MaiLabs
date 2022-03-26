#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include<iostream>
#include<thrust/extrema.h>
#include <thrust/device_vector.h>
#include<vector>
#include<cmath>
#include<algorithm>
#include <iomanip>
#include <random>
//#define BLOCKS_2D dim3(32,32)
//#define THREADS_2D dim3(32,32)
//#define BLOCKS_1D 256
//#define THREADS_1D 512
#define BLOCKS_2D dim3(8,8)
#define THREADS_2D dim3(8,8)
#define BLOCKS_1D 32
#define THREADS_1D 32
#define EPS 0.0000001

#define CUDA_ERROR_HANDLER(err) do { if (err != cudaSuccess) { std::cout << cudaGetErrorString(err) << std::endl; exit(1);} } while (0)
#define CEH(err) CUDA_ERROR_HANDLER(err)

#include<chrono>


namespace chrono_timer {
	template<class T = std::chrono::nanoseconds>
	class timer {
		std::chrono::steady_clock::time_point start_point = std::chrono::high_resolution_clock::now();
		std::chrono::steady_clock::time_point end_point = start_point;
		bool is_stoped = true;
	public:
		using time_type = T;
		void start() {
			start_point = std::chrono::high_resolution_clock::now();
			is_stoped = false;
		}
		void stop() {
			end_point = std::chrono::high_resolution_clock::now();
			is_stoped = true;
		}
		long long time() {
			return is_stoped ? std::chrono::duration_cast<time_type>(end_point - start_point).count() : std::chrono::duration_cast<time_type>(std::chrono::high_resolution_clock::now() - start_point).count();
		}
		long long measure(void f(void)) {
			this->start();
			f();
			return this->time();
		}
	};
	long long nanoseconds_time(void f(void)) {
		timer<> timer;
		timer.start();
		f();
		return timer.time();
	}

	template<class T>
	long long measure_time(void f(void)) {
		timer<T> timer;
		timer.start();
		f();
		return timer.time();
	}

	using timer_nns = timer <>;
	using timer_mls = timer < std::chrono::milliseconds >;
	using timer_mcs = timer < std::chrono::microseconds >;
	using timer_sec = timer < std::chrono::seconds >;
};
using namespace chrono_timer;

double drand(double b, double e)
{
	return b + (std::rand() % static_cast<unsigned int>((e - b) * 1000)) / 1000.;
}

__global__ void div_row_kernel(double* matrix, double lead, int row, int n) {
	int id = blockDim.x * blockIdx.x + threadIdx.x;
	int offset = blockDim.x * gridDim.x;
	for (int i = row + id; i < n; i += offset) {
		matrix[row + i * n] /= lead;
	}
}

__global__ void sub_rows_kernel(double* matrix, int lead_row, int n) {
	int idx = blockDim.x * blockIdx.x + threadIdx.x;
	int idy = blockDim.y * blockIdx.y + threadIdx.y;
	int offsetx = blockDim.x * gridDim.x;
	int offsety = blockDim.y * gridDim.y;
	for (int i = lead_row + idx + 1; i < n; i += offsetx) {
		for (int j = lead_row + idy + 1; j < n; j += offsety) {
			matrix[j * n + i] -= matrix[lead_row*n + i] * matrix[lead_row + j*n];
		}
	}
}

__global__ void swap_rows_kernel(double* matrix, int first, int second, int off, int n) {
	int id = blockDim.x * blockIdx.x + threadIdx.x;
	int offset = blockDim.x * gridDim.x;
	for (int i = off + id; i < n; i += offset) {
		double tmp = matrix[i * n + first];
		matrix[i * n + first] = matrix[i * n + second];
		matrix[i * n + second] = tmp;
	}
}

__global__ void set_zeros(double* matrix, int column, int n) {
	int id = blockDim.x * blockIdx.x + threadIdx.x;
	int offset = blockDim.x * gridDim.x;
	for (int i = column + id + 1; i < n; i += offset) {
		matrix[i + column * n] = 0.0;
	}
}

struct comparator
{
	__host__ __device__ bool operator()(double a, double b)
	{
		return std::fabs(a) < std::fabs(b);
	}
};
double get_det(std::vector<double>& multipliers) {
	comparator comp;
	std::sort(multipliers.begin(), multipliers.end(), comp);
	double ans = 1.0;
	int begin = 0, end = (int)multipliers.size() - 1;

	while(begin <= end) {
		if (std::abs(ans) < EPS) {
			ans *= multipliers[end--];
		}
		else {
			ans *= multipliers[begin++];
		}
	}
	return ans;
}

int main() {
	int n;
	comparator comp;
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cin >> n;
	double* matrix = new double[n * n];
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			//std::cin >> matrix[i + j * n];
			matrix[i + j * n] = drand(0, 1);
		}
	}
	std::cout << std::setprecision(10);
	std::cout.setf(std::ios::scientific);
	if (n == 1) {
		std::cout << matrix[0];
		return 0;
	}
	double* dev_matrix = nullptr;
	CEH(cudaMalloc(&dev_matrix, sizeof(double) * n * n));
	CEH(cudaMemcpy(dev_matrix, matrix, sizeof(double) * n * n, cudaMemcpyHostToDevice));
	std::vector<double> multipliers(n);
	bool is_zero = false;
	thrust::device_ptr<double> begin;
	thrust::device_ptr<double> max_ptr;
	timer_nns timer;
	timer.start();
	for (int i = 0; i < n; ++i) {
		begin = thrust::device_pointer_cast(dev_matrix + i + i * n);
		double multiplier = *begin;
		if (std::abs(multiplier) <= EPS) {
			max_ptr = thrust::max_element(begin, begin + (n - i), comp);
			double max_el = *max_ptr;
			int max_index = max_ptr - begin;
			if (std::abs(max_el) <= EPS) {
				is_zero = true;
				break;
			}
			swap_rows_kernel << < BLOCKS_1D, THREADS_1D >> > (dev_matrix, i, max_index+i, i, n);
			multipliers[i] = -1.0 * max_el;
			multiplier = max_el;
		}
		else {
			multipliers[i] = multiplier;
		}
		div_row_kernel << < BLOCKS_1D, THREADS_1D >> > (dev_matrix, multiplier, i, n);
		sub_rows_kernel << <BLOCKS_2D, THREADS_2D >> > (dev_matrix, i, n);
		//set_zeros <<< BLOCKS_1D, THREADS_1D >> > (dev_matrix, i, n);
	}
	double determinant = is_zero ? 0 : get_det(multipliers);
	//std::cout << timer.time();
	std::cout << determinant;
	CEH(cudaFree(dev_matrix));
	delete[] matrix;
	exit(0);
}
