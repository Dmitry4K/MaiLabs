#include <cuda.h>
#include <stdio.h>


void cudasafe(int error, char* message, char* file, int line) {
    if (error != cudaSuccess) {
        fprintf(stderr, "CUDA Error: %s : %i. In %s line %d\n", message, error, file, line);
        exit(-1);
    }
}


int main(int argc, char** argv) {
    int deviceCount;

    cudasafe(cudaGetDeviceCount(&deviceCount), "GetDeviceCount", __FILE__, __LINE__);

    printf("Number of CUDA devices %d.\n", deviceCount);

    for (int dev = 0; dev < deviceCount; dev++) {
        cudaDeviceProp deviceProp;

        cudasafe(cudaGetDeviceProperties(&deviceProp, dev), "Get Device Properties", __FILE__, __LINE__);

        if (dev == 0) {
            if (deviceProp.major == 9999 && deviceProp.minor == 9999) {
                printf("No CUDA GPU has been detected\n");
                return -1;
            }
            else if (deviceCount == 1) {
                printf("There is 1 device supporting CUDA\n");
            }
            else {
                printf("There are %d devices supporting CUDA\n", deviceCount);
            }
        }

        printf("For device #%d\n", dev);
        printf("Device name:                %s\n", deviceProp.name);
        printf("Major revision number:      %d\n", deviceProp.major);
        printf("Minor revision Number:      %d\n", deviceProp.minor);
        printf("Total Global Memory:        %d\n", deviceProp.totalGlobalMem);
        printf("Total shared mem per block: %d\n", deviceProp.sharedMemPerBlock);
        printf("Total const mem size:       %d\n", deviceProp.totalConstMem);
        printf("Warp size:                  %d\n", deviceProp.warpSize);
        printf("Maximum block dimensions:   %d x %d x %d\n", deviceProp.maxThreadsDim[0], \
            deviceProp.maxThreadsDim[1], \
            deviceProp.maxThreadsDim[2]);

        printf("Maximum grid dimensions:    %d x %d x %d\n", deviceProp.maxGridSize[0], \
            deviceProp.maxGridSize[1], \
            deviceProp.maxGridSize[2]);
        printf("Clock Rate:                 %d\n", deviceProp.clockRate);
        printf("Number of muliprocessors:   %d\n", deviceProp.multiProcessorCount);

    }

    return 0;


}
