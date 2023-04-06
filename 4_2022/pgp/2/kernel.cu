#include<cuda.h>
#include<cuda_runtime.h>
#include<texture_types.h>
#include<device_launch_parameters.h>
#include<iostream>
#include<fstream>
#include<string>
#pragma once
#include<chrono>
template<class T = std::chrono::nanoseconds>
class Timer {
    std::chrono::steady_clock::time_point start_point = std::chrono::high_resolution_clock::now();
    std::chrono::steady_clock::time_point end_point = start_point;
    bool is_stoped = true;
public:
    using time_type = T;
    void Start() {
        start_point = std::chrono::high_resolution_clock::now();
        is_stoped = false;
    }
    void Stop() {
        end_point = std::chrono::high_resolution_clock::now();
        is_stoped = true;
    }
    long long Time() {
        return is_stoped ? std::chrono::duration_cast<time_type>(end_point - start_point).count() : std::chrono::duration_cast<time_type>(std::chrono::high_resolution_clock::now() - start_point).count();
    }
};

#define CUDA_ERROR_HANDLER(err) do { if (err != cudaSuccess) { std::cout << cudaGetErrorString(err) << std::endl; exit(0);} } while (0)
#define CEH(err) CUDA_ERROR_HANDLER(err)

#define BLOCKS dim3(4, 4)
#define THREADS dim3(4, 4)

#define TO_BLACK_WHITE(p) double((0.299*(double)p.x) + (0.587 * (double) p.y) + (0.114* (double)p.z))

texture<uchar4, cudaTextureType2D, cudaReadModeElementType> tex;

inline uchar4 getByIndexes(uchar4 * image, int x, int y, int w) {
    return image[y * w + x];
}

void previttaLinear(unsigned long long width, unsigned long long height, uchar4 * out, uchar4* image) {
    for (long long y = 1; y < height-1; y++) {
        for (long long x = 1; x < width-1; x++) {
            double gx = 0.0
                + TO_BLACK_WHITE(getByIndexes(image, x + 1, y - 1, width))
                + TO_BLACK_WHITE(getByIndexes(image, x + 1, y, width))
                + TO_BLACK_WHITE(getByIndexes(image, x + 1, y + 1, width))
                - TO_BLACK_WHITE(getByIndexes(image, x - 1, y - 1, width))
                - TO_BLACK_WHITE(getByIndexes(image, x - 1, y, width))
                - TO_BLACK_WHITE(getByIndexes(image, x - 1, y + 1, width));
            double gy = 0.0
                + TO_BLACK_WHITE(getByIndexes(image, x - 1, y + 1, width))
                + TO_BLACK_WHITE(getByIndexes(image, x, y + 1, width))
                + TO_BLACK_WHITE(getByIndexes(image, x + 1, y + 1, width))
                - TO_BLACK_WHITE(getByIndexes(image, x - 1, y - 1, width))
                - TO_BLACK_WHITE(getByIndexes(image, x, y - 1, width))
                - TO_BLACK_WHITE(getByIndexes(image, x + 1, y - 1, width));

            float r = (sqrt(gx * gx + gy * gy));
            if (r > 255.0) {
                r = 255;
            }
            out[y * width + x] = make_uchar4(r, r, r, getByIndexes(image, x, y, width).w);
        }
    }
}
__global__ void previttaKernel(unsigned long long width, unsigned long long height, uchar4* out) {
	long long idx = blockDim.x * blockIdx.x + threadIdx.x;
    long long idy = blockDim.y * blockIdx.y + threadIdx.y;
    long long offsetx = blockDim.x * gridDim.x;
    long long offsety = blockDim.y * gridDim.y;
    for (long long y = idy; y < height; y += offsety) {
        for (long long x = idx; x < width; x += offsetx) {
            double gx = 0.0
                    + TO_BLACK_WHITE(tex2D(tex, x + 1, y - 1))
                    + TO_BLACK_WHITE(tex2D(tex, x + 1, y    ))
                    + TO_BLACK_WHITE(tex2D(tex, x + 1, y + 1))
                    - TO_BLACK_WHITE(tex2D(tex, x - 1, y - 1))
                    - TO_BLACK_WHITE(tex2D(tex, x - 1, y    ))
                    - TO_BLACK_WHITE(tex2D(tex, x - 1, y + 1));
            double gy = 0.0
                    + TO_BLACK_WHITE(tex2D(tex, x - 1, y + 1))
                    + TO_BLACK_WHITE(tex2D(tex, x    , y + 1))
                    + TO_BLACK_WHITE(tex2D(tex, x + 1, y + 1))
                    - TO_BLACK_WHITE(tex2D(tex, x - 1, y - 1))
                    - TO_BLACK_WHITE(tex2D(tex, x    , y - 1))
                    - TO_BLACK_WHITE(tex2D(tex, x + 1, y - 1));

            float r = (sqrt(gx*gx + gy*gy));
            if (r > 255.0) {
                r = 255;
            }
			out[y * width + x] = make_uchar4(r, r, r, tex2D(tex, x, y).w);
		}
	}
}

int main() {
	std::string input_file_path, output_file_path;
	std::cin >> input_file_path >> output_file_path;
	std::ifstream input{ input_file_path , std::ios::in | std::ios::binary};
	unsigned int w, h;
    uchar4* image = nullptr;
    Timer<std::chrono::microseconds> timer;

    //reading
	if (input.is_open()) {
		input.read((char*)&w, sizeof(int));
		input.read((char*)&h, sizeof(int));
		image = new uchar4[(size_t)w * (size_t)h];
		input.read((char*)image, sizeof(uchar4) * (size_t)w * (size_t)h);
        input.close();
    }
	else {
		exit(1);
	}
	//malloc
    cudaArray* arr;
    uchar4* out;
	cudaChannelFormatDesc ch = cudaCreateChannelDesc<uchar4>();
	CEH(cudaMallocArray(&arr, &ch, w, h));

	//coping to device
	CEH(cudaMemcpyToArray(arr, 0, 0, image, sizeof(uchar4) * (size_t)w * (size_t)h, cudaMemcpyHostToDevice));

	//config
    tex.addressMode[0] = cudaAddressModeClamp;	// Политика обработки выхода за границы по каждому измерению
    tex.addressMode[1] = cudaAddressModeClamp;
    tex.channelDesc = ch;
    tex.filterMode = cudaFilterModePoint;		// Без интерполяции при обращении по дробным координатам
    tex.normalized = false;

    //binding
    CEH(cudaBindTextureToArray(tex, arr, ch));
    CEH(cudaMalloc(&out, sizeof(uchar4) * (size_t)w * (size_t)h));

    //executing
    timer.Start();
    previttaKernel <<< BLOCKS, THREADS>>> (w, h, out);
    CEH(cudaGetLastError());
    std::cout << "CUDA time: "<< timer.Time() << std::endl;
    uchar4* outl = new uchar4[w * h];
    timer.Start();
    previttaLinear(w, h, outl, image);
    std::cout << "Liner time: " << timer.Time();
    CEH(cudaMemcpy(image, out, sizeof(uchar4) * (size_t)w * (size_t)h, cudaMemcpyDeviceToHost));

	//formatting result
	std::ofstream output{ output_file_path ,std::ios::out | std::ios::binary};


    output.write((char*)&w, sizeof(int));
	output.write((char*)&h, sizeof(int));
    output.write((char*)image, sizeof(uchar4) * (size_t)w * (size_t)h );
    output.close();
    //unbinding
    CEH(cudaUnbindTexture(tex));
    CEH(cudaFreeArray(arr));
    CEH(cudaFree(out));

    delete[] image;
	exit(0);
}
