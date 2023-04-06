#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

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
    long long measure_time_in_nanoseconds(void f(void)) {
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


#define CUDA_ERROR_HANDLER(err) do { if (err != cudaSuccess) { std::cout << cudaGetErrorString(err) << std::endl; exit(1);} } while (0)
#define CEH(err) CUDA_ERROR_HANDLER(err)
#define BLOCKS dim3(4, 4)
#define THREADS dim3(4, 4)

using namespace chrono_timer;

struct double3x3 {
    double x11, x12, x13, x21, x22, x23, x31, x32, x33;
};

__constant__ double3 AVG[32];
__constant__ double3x3 INV_COV[32];

__device__ __host__ double3 double3_from(uchar4 a) {
    return { (double)a.x, (double)a.y, (double)a.z };
}
__device__ __host__ double3 sum(double3 a, double3 b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}
__device__ __host__ double3x3 div(double3x3 a, double b) {
    return { a.x11 / b, a.x12 / b,a.x13 / b, a.x21 / b, a.x22 / b, a.x23 / b, a.x31 / b, a.x32 / b, a.x33 / b };
}
__device__ __host__ double3 div(double3 a, double b) {
    return { a.x / b, a.y / b, a.z / b };
}
__device__ __host__ double3 sub(double3 a, double3 b) {
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}
__device__ __host__ double3x3 mult(double3 a, double3 b) {
    return {a.x * b.x, a.x * b.y, a.x * b.z,
            a.y * b.x, a.y * b.y, a.y * b.z,
            a.z * b.x, a.z * b.y, a.z * b.z };
}

__device__ __host__ double dot(double3 a, double3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
__device__ __host__ double3 mult(double3 a, double3x3 b) {
    return { dot(a, {b.x11, b.x21,b.x31}), dot(a, {b.x12, b.x22,b.x32}), dot(a, {b.x13, b.x23,b.x33}) };
}


__device__ __host__ double3x3 sum(double3x3 a, double3x3 b) {
    return { a.x11 + b.x11, a.x12 + b.x12, a.x13 + b.x13,
             a.x21 + b.x21, a.x22 + b.x22, a.x23 + b.x23,
             a.x31 + b.x31, a.x32 + b.x32, a.x33 + b.x33};
}

__device__ __host__ double det(double3x3 a) {
    return a.x11 * a.x22 * a.x33 + a.x12 * a.x23 * a.x31 + a.x13 * a.x21 * a.x32 -
        a.x13 * a.x22 * a.x31 - a.x12 * a.x21 * a.x33 - a.x11 * a.x23 * a.x32;
}

__device__ __host__ double3x3 transpose(double3x3 a) {
    return { a.x11, a.x21, a.x31,
            a.x12, a.x22, a.x32,
            a.x13, a.x23, a.x33 };
}


__device__ __host__ double3x3 inv(double3x3 a) {
    a = transpose(a);
    double3x3 m = { (a.x22 * a.x33 - a.x32 * a.x23), -1.0* (a.x21 * a.x33 - a.x31 * a.x23), (a.x21 * a.x32 - a.x31 * a.x22),
                   -1.0 * (a.x12 * a.x33 - a.x32 * a.x13), (a.x11 * a.x33 - a.x31 * a.x13), -1.0* (a.x11 * a.x32 - a.x31 * a.x12),
                    (a.x12 * a.x23 - a.x22 * a.x13), -1.0 * (a.x11 * a.x23 - a.x21 * a.x13), (a.x11 * a.x22 - a.x21 * a.x12) };
    return div(m, det(a));
}

__device__ __host__ int argMax(uchar4 pixel, int nc) {
    double result, max;
    double3 tmp = sub(double3_from(pixel), AVG[0]);
    int clas = 0;
    max = -1.0 * dot(mult(tmp, INV_COV[0]), tmp);
    for (int i = 1; i < nc; ++i) {
        tmp = sub(double3_from(pixel), AVG[i]);
        result = -1.0 * dot(mult(tmp, INV_COV[i]), tmp);
        if (result > max) {
            max = result;
            clas = i;
        }
    }
    return clas;
}

void linearMahalanobisaMethod(uchar4* image, long long w, long long h, int nc) {
    for (long long y = 0; y < h; ++y) {
        for (long long x = 0; x < w; ++x) {
            //image[x + y * w].w 
        }
    }
}


__global__ void mahalanobisaMethod(uchar4* dev_image, long long w, long long h, int nc) {
    long long idx = blockDim.x * blockIdx.x + threadIdx.x;
    long long idy = blockDim.y * blockIdx.y + threadIdx.y;
    long long offsetx = blockDim.x * gridDim.x;
    long long offsety = blockDim.y * gridDim.y;

    for (long long y = idy; y < h; y += offsety) {
        for (long long x = idx; x < w; x += offsetx) {
            dev_image[x + y * w].w = argMax(dev_image[x + y * w], nc);
            /*int clas = argMax(dev_image[x + y * w], nc);
            dev_image[x + y * w] = make_uchar4(0, 0, 0, 255);
            if (clas == 0)
                dev_image[x + y * w].x = 240;
            if (clas == 1)
                dev_image[x + y * w].y = 240;
            if (clas == 2)
                dev_image[x + y * w].z = 240;*/
        }
    }
}

void print(double3x3 a) {
    std::cout << a.x11 << ' ' << a.x12 << ' ' << a.x13 << std::endl;
    std::cout << a.x21 << ' ' << a.x22 << ' ' << a.x23 << std::endl;
    std::cout << a.x31 << ' ' << a.x32 << ' ' << a.x33 << std::endl;
}

void print(double a) {
    std::cout << a << std::endl;
}

int main()
{
    std::string input_file_path, output_file_path;
    int nc, np;
    std::cin >> input_file_path >> output_file_path >> nc;
    std::vector<std::vector<int2>> classes(nc);
    for (auto& clas : classes) {
        std::cin >> np;
        clas.resize(np);
        for (auto& pair : clas) {
            std::cin >> pair.x >> pair.y;
        }
    }

    uchar4* image = nullptr;
    int w, h;

    std::ifstream input{ input_file_path , std::ios::in | std::ios::binary };
    if (input.is_open()) {
        input.read((char*)&w, sizeof(int));
        input.read((char*)&h, sizeof(int));
        image = new uchar4[(size_t)w * h];
        input.read((char*)image, sizeof(uchar4) * w * h);
        input.close();
    }
    else {
        exit(1);
    }

    std::vector<double3> avg(nc, { 0.0, 0.0, 0.0 });
    for (int j = 0; j < avg.size(); ++j) {
        double3 s = { 0.0, 0.0, 0.0 };
        auto& clas = classes[j];
        int np = (int)clas.size();
        for (auto& el : clas) {
            uchar4 ps = image[el.y * w + el.x];
            s = sum(s, double3_from(ps));
        }
        avg[j] = div(s, np);
    }


    std::vector<double3x3> inv_cov(nc, { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 });
    for (int j = 0; j < inv_cov.size(); ++j) {
        double3x3 s = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
        auto& clas = classes[j];
        int np = (int) clas.size();
        for (auto& el : clas) {
            uchar4 ps = image[el.y * w + el.x];
            double3 tmp = sub(double3_from(ps), avg[j]);
            s = sum(s, mult(tmp, tmp));
        }
        inv_cov[j] = inv(div(s, np - 1));
    }
    timer_nns timer;
    CEH(cudaMemcpyToSymbol(AVG, avg.data(), sizeof(double3) * avg.size()));
    CEH(cudaMemcpyToSymbol(INV_COV, inv_cov.data(), sizeof(double3x3) * inv_cov.size()));
    uchar4* dev_image = nullptr;
    CEH(cudaMalloc(&dev_image, sizeof(uchar4) * h * w));
    CEH(cudaMemcpy(dev_image, image, sizeof(uchar4) * h * w, cudaMemcpyHostToDevice));
    timer.start();
    mahalanobisaMethod << < BLOCKS, THREADS >> > (dev_image, w, h, nc);
    std::cout << "Cuda time: " << timer.time() << std::endl;
    //timer.start();
    //linearMahalanobisaMethod(image, w, h, nc);
    //std::cout << "Linear time: " << timer.time() << std::endl;
    CEH(cudaMemcpy(image, dev_image, sizeof(uchar4) * h * w, cudaMemcpyDeviceToHost));
    for (long long y = 0; y < h; y ++) {
        for (long long x = 0; x < w; x ++) {
            
            //*
            int clas = image[x + y * w].w;
                image[x + y * w] = make_uchar4(0, 0, 0, 255);
            if (clas == 0)
                image[x + y * w].x = 240;
            if (clas == 1)
                image[x + y * w].y = 240;//*/
        }
    }
    std::ofstream output{ output_file_path ,std::ios::out | std::ios::binary };
    if (output.is_open()) {
        output.write((char*)&w, sizeof(int));
        output.write((char*)&h, sizeof(int));
        output.write((char*)image, sizeof(uchar4) * w * h);
        output.close();
    }
    else {
        exit(1);
    }
    CEH(cudaFree(dev_image));
    delete[] image;
    return 0;
}
