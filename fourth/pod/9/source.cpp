//#include "cuda_runtime.h"
//#include "device_launch_parameters.h"
#include <mpi.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <string>
#include <sstream>
#include <omp.h>

//#define IOSTREAM

namespace SIDES {
    int LEFT = 0;
    int RIGHT = 1;
    int UP = 2;
    int DOWN = 3;
    int FRONT = 4;
    int BACK = 5;
}

struct vec3 { int x, y, z; };

int blockId, blocksTotalCount, procNameLen, stringLen;

std::string outFileName = "";

double epsilon, lX, lY, lZ, u0, uLeft, uRight, uUp, uBack, uFront, uDown;

vec3 inBlokcPos, blockPos, gridEqDim, blockEqDim;

#define _idx(i, j, k) ( ((k) + 1) * (blockEqDim.y + 2) * (blockEqDim.x + 2) + ((j) + 1) * (blockEqDim.x + 2) + (i) + 1 )
#define _idxblock(i, j, k) ( (k) * (gridEqDim.x * gridEqDim.y) + (j) * gridEqDim.x + (i) )

void printToCerr(const char* str) {
    fprintf(stderr, "[%d]%s\n", blockId, str);
}

vec3 getPositionByBlockId(int blockId) {
    int z = blockId / (gridEqDim.x * gridEqDim.y);
    int y = (blockId - z * (gridEqDim.x * gridEqDim.y)) / (gridEqDim.x);
    int x = blockId - z * (gridEqDim.x * gridEqDim.y) - y * gridEqDim.x;
    //fprintf(stderr, "[%d]%d,%d,%d\n", blockId, x, y, z);
    return { x, y, z };
}

vec3 getPositionInBlock(int num) {
    int z = num / (blockEqDim.x * blockEqDim.y);
    int y = (num - z * (blockEqDim.x * blockEqDim.y)) / (blockEqDim.x);
    int x = num - z * (blockEqDim.x * blockEqDim.y) - y * blockEqDim.x;
    return { x, y, z };
}

int getTagForSideByBlockId(int side, int blockId) {
    return blockId * 10 + side;
}

int getTagForPrinting(int blockId, int indexZY) {
    return blockId * blockEqDim.z * blockEqDim.y + indexZY;
}

bool isLeftSideBlock(int blockId) { return getPositionByBlockId(blockId).x == 0; }
bool isRightSideBlock(int blockId) { return getPositionByBlockId(blockId).x == gridEqDim.x - 1; }
bool isFrontSideBlock(int blockId) { return getPositionByBlockId(blockId).y == 0; }
bool isBackSideBlock(int blockId) { return getPositionByBlockId(blockId).y == gridEqDim.y - 1; }
bool isUpSideBlock(int blockId) { return getPositionByBlockId(blockId).z == gridEqDim.z - 1; }
bool isDownSideBlock(int blockId) { return getPositionByBlockId(blockId).z == 0; }

void sendSide(double* buff, long long buffCount, int destBlockX, int destBlockY, int destBlockZ, int side) {
    MPI_Request request;
    MPI_Status status;
    //fprintf(stderr, "[%d]s%d,%d,%d\n", blockId, destBlockX, destBlockY, destBlockZ);
    int destBlockId = _idxblock(destBlockX, destBlockY, destBlockZ);
    MPI_Isend(buff, buffCount, MPI_DOUBLE, destBlockId, getTagForSideByBlockId(side, blockId), MPI_COMM_WORLD, &request);
    MPI_Wait(&request, &status);
}

void recvSide(double* buff, long long buffCount, int destBlockX, int destBlockY, int destBlockZ, int side) {
    MPI_Request request;
    MPI_Status status;
    //fprintf(stderr, "[%d]r%d,%d,%d\n", blockId, destBlockX, destBlockY, destBlockZ);
    int destBlockId = _idxblock(destBlockX, destBlockY, destBlockZ);
    MPI_Irecv(buff, buffCount, MPI_DOUBLE, destBlockId, getTagForSideByBlockId(side, destBlockId), MPI_COMM_WORLD, &request);
    MPI_Wait(&request, &status);
}

void reciveAndSendLeftSide(double* block) {
    //printToCerr("left exchanging");
    long long buffCount = (long long)blockEqDim.z * blockEqDim.y;
    double* buff = new double[buffCount];
    if (!isLeftSideBlock(blockId)) {
        for (int k = 0; k < blockEqDim.z; ++k) {
            for (int j = 0; j < blockEqDim.y; ++j) {
                buff[k * blockEqDim.y + j] = block[_idx(0, j, k)];
            }
        }
        sendSide(buff, buffCount, blockPos.x - 1, blockPos.y, blockPos.z, SIDES::LEFT);
    }
    if (!isRightSideBlock(blockId)) {
        recvSide(buff, buffCount, blockPos.x + 1, blockPos.y, blockPos.z, SIDES::LEFT);
        for (int k = 0; k < blockEqDim.z; ++k) {
            for (int j = 0; j < blockEqDim.y; ++j) {
                block[_idx(blockEqDim.x, j, k)] = buff[k * blockEqDim.y + j];
            }
        }
    }
    else {
        //fill right side uRight
        for (int k = 0; k < blockEqDim.z; ++k) {
            for (int j = 0; j < blockEqDim.y; ++j) {
                block[_idx(blockEqDim.x, j, k)] = uRight;
            }
        }
    }
    delete[] buff;
}
void reciveAndSendRightSide(double* block) {
    //printToCerr("right exchanging");
    long long buffCount = (long long)blockEqDim.z * blockEqDim.y;
    double* buff = new double[buffCount];
    if (!isRightSideBlock(blockId)) {
        for (int k = 0; k < blockEqDim.z; ++k) {
            for (int j = 0; j < blockEqDim.y; ++j) {
                buff[k * blockEqDim.y + j] = block[_idx(blockEqDim.x - 1, j, k)];
            }
        }
        sendSide(buff, buffCount, blockPos.x + 1, blockPos.y, blockPos.z, SIDES::RIGHT);
    }
    if (!isLeftSideBlock(blockId)) {
        recvSide(buff, buffCount, blockPos.x - 1, blockPos.y, blockPos.z, SIDES::RIGHT);
        for (int k = 0; k < blockEqDim.z; ++k) {
            for (int j = 0; j < blockEqDim.y; ++j) {
                block[_idx(-1, j, k)] = buff[k * blockEqDim.y + j];
            }
        }
    }
    else {
        //fill left side
        for (int k = 0; k < blockEqDim.z; ++k) {
            for (int j = 0; j < blockEqDim.y; ++j) {
                block[_idx(-1, j, k)] = uLeft;
            }
        }
    }
    delete[] buff;
}
void reciveAndSendUpSide(double* block) {
    //printToCerr("up exchanging");
    long long buffCount = (long long)blockEqDim.y * blockEqDim.x;
    double* buff = new double[buffCount];
    if (!isUpSideBlock(blockId)) {
        for (int j = 0; j < blockEqDim.y; ++j) {
            for (int i = 0; i < blockEqDim.x; ++i) {
                buff[j * blockEqDim.x + i] = block[_idx(i, j, blockEqDim.z - 1)];
            }
        }
        sendSide(buff, buffCount, blockPos.x, blockPos.y, blockPos.z + 1, SIDES::UP);
    }
    if (!isDownSideBlock(blockId)) {
        recvSide(buff, buffCount, blockPos.x, blockPos.y, blockPos.z - 1, SIDES::UP);
        for (int j = 0; j < blockEqDim.y; ++j) {
            for (int i = 0; i < blockEqDim.x; ++i) {
                block[_idx(i, j, -1)] = buff[j * blockEqDim.x + i];
            }
        }
    }
    else {
        // fill Down side
        for (int j = 0; j < blockEqDim.y; ++j) {
            for (int i = 0; i < blockEqDim.x; ++i) {
                block[_idx(i, j, -1)] = uDown;
            }
        }
    }
    delete[] buff;
}
void reciveAndSendDownSide(double* block) {
    //printToCerr("down exchanging");
    long long buffCount = (long long)blockEqDim.y * blockEqDim.x;
    double* buff = new double[buffCount];
    if (!isDownSideBlock(blockId)) {
        for (int j = 0; j < blockEqDim.y; ++j) {
            for (int i = 0; i < blockEqDim.x; ++i) {
                buff[j * blockEqDim.x + i] = block[_idx(i, j, 0)];
            }
        }
        sendSide(buff, buffCount, blockPos.x, blockPos.y, blockPos.z - 1, SIDES::DOWN);
    }
    if (!isUpSideBlock(blockId)) {
        recvSide(buff, buffCount, blockPos.x, blockPos.y, blockPos.z + 1, SIDES::DOWN);
        for (int j = 0; j < blockEqDim.y; ++j) {
            for (int i = 0; i < blockEqDim.x; ++i) {
                block[_idx(i, j, blockEqDim.z)] = buff[j * blockEqDim.x + i];
            }
        }
    }
    else {
        // fill Up side
        for (int j = 0; j < blockEqDim.y; ++j) {
            for (int i = 0; i < blockEqDim.x; ++i) {
                block[_idx(i, j, blockEqDim.z)] = uUp;
            }
        }
    }
    delete[] buff;
}
void reciveAndSendFrontSide(double* block) {
    //printToCerr("front exchanging");
    long long buffCount = (long long)blockEqDim.z * blockEqDim.x;
    double* buff = new double[buffCount];
    if (!isFrontSideBlock(blockId)) {
        for (int k = 0; k < blockEqDim.z; ++k) {
            for (int i = 0; i < blockEqDim.x; ++i) {
                buff[k * blockEqDim.x + i] = block[_idx(i, 0, k)];
            }
        }
        sendSide(buff, buffCount, blockPos.x, blockPos.y - 1, blockPos.z, SIDES::FRONT);
    }
    if (!isBackSideBlock(blockId)) {
        recvSide(buff, buffCount, blockPos.x, blockPos.y + 1, blockPos.z, SIDES::FRONT);
        for (int k = 0; k < blockEqDim.z; ++k) {
            for (int i = 0; i < blockEqDim.x; ++i) {
                block[_idx(i, blockEqDim.y, k)] = buff[k * blockEqDim.x + i];
            }
        }
    }
    else {
        // fill back side
        for (int k = 0; k < blockEqDim.z; ++k) {
            for (int i = 0; i < blockEqDim.x; ++i) {
                block[_idx(i, blockEqDim.y, k)] = uBack;
            }
        }
    }
    delete[] buff;
}
void reciveAndSendBackSide(double* block) {
    //printToCerr("back exchanging");
    long long buffCount = (long long)blockEqDim.z * blockEqDim.x;
    double* buff = new double[buffCount];
    if (!isBackSideBlock(blockId)) {
        for (int k = 0; k < blockEqDim.z; ++k) {
            for (int i = 0; i < blockEqDim.x; ++i) {
                buff[k * blockEqDim.x + i] = block[_idx(i, blockEqDim.y - 1, k)];
            }
        }
        sendSide(buff, buffCount, blockPos.x, blockPos.y + 1, blockPos.z, SIDES::BACK);
    }
    if (!isFrontSideBlock(blockId)) {
        recvSide(buff, buffCount, blockPos.x, blockPos.y - 1, blockPos.z, SIDES::BACK);
        for (int k = 0; k < blockEqDim.z; ++k) {
            for (int i = 0; i < blockEqDim.x; ++i) {
                block[_idx(i, -1, k)] = buff[k * blockEqDim.x + i];
            }
        }
    }
    else {
        // fill front side
        for (int k = 0; k < blockEqDim.z; ++k) {
            for (int i = 0; i < blockEqDim.x; ++i) {
                block[_idx(i, -1, k)] = uFront;
            }
        }
    }
    delete[] buff;
}

void printExclusiveBlock(double* block) {
    for (int k = -1; k <= blockEqDim.z; k++) {
        for (int j = -1; j <= blockEqDim.y; j++) {
            for (int i = -1; i <= blockEqDim.x; i++) {
                std::cout << block[_idx(i, j, k)] << ' ';
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void printBlock(double* block) {
    for (int k = 0; k < blockEqDim.z; k++) {
        for (int j = 0; j < blockEqDim.y; j++) {
            for (int i = 0; i < blockEqDim.x; i++) {
                std::cout << block[_idx(i, j, k)] << ' ';
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}
void printGlobalVariableToCerr() {
    if (blockId == 0) {
        std::cerr << gridEqDim.x << ' ' << gridEqDim.y << ' ' << gridEqDim.z << ' ';
        std::cerr << blockEqDim.x << ' ' << blockEqDim.y << ' ' << blockEqDim.z << ' ';
        std::cerr << outFileName << ' ' << epsilon << ' ';
        std::cerr << lX << ' ' << lY << ' ' << lZ << ' ';
        std::cerr << uDown << ' ' << uUp << ' ' << uLeft << ' ' << uRight << ' ' << uFront << ' ' << uBack << ' ';
        std::cerr << u0 << std::endl;
    }
}

void multiPrint(double* data) {
    std::stringstream stringStream;
    stringStream << std::scientific << std::setprecision(6);

    int bufferSize = 20;
    for (int k = 0; k < blockEqDim.z; k++) {
        for (int j = 0; j < blockEqDim.y; j++) {
            for (int i = 0; i < blockEqDim.x; i++) {
                int prev = stringStream.str().length();
                stringStream << data[_idx(i, j, k)];
                int after = stringStream.str().length();
                for (int n = 0; n < bufferSize - (after - prev) - 1; ++n) {
                    stringStream << ' ';
                }
                if (i == blockEqDim.x - 1 && blockPos.x == gridEqDim.x - 1) {
                    stringStream << '\n';
                }
                else {
                    stringStream << ' ';
                }
            }
        }
    }
    MPI_Datatype filetype;
    int array_of_sizes[3] = {
        blockEqDim.z * gridEqDim.z, blockEqDim.y * gridEqDim.y, blockEqDim.x * gridEqDim.x * bufferSize
    };
    int array_of_subsizes[3] = { blockEqDim.z, blockEqDim.y, blockEqDim.x * bufferSize };
    int array_of_starts[3] = {
        blockPos.z * blockEqDim.z, blockPos.y * blockEqDim.y, blockPos.x * blockEqDim.x * bufferSize
    };
    printToCerr((
        std::to_string(array_of_starts[2]) + ' ' +
        std::to_string(array_of_starts[1]) + ' ' +
        std::to_string(array_of_starts[0])
        ).c_str());
    MPI_Type_create_subarray(3, array_of_sizes, array_of_subsizes, array_of_starts, MPI_ORDER_C, MPI_CHAR, &filetype);
    MPI_Type_commit(&filetype);

    MPI_File fp;
    MPI_File_delete(outFileName.c_str(), MPI_INFO_NULL);
    MPI_File_open(MPI_COMM_WORLD, outFileName.c_str(), MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fp);
    MPI_File_set_view(fp, 0, MPI_CHAR, filetype, "native", MPI_INFO_NULL);
    MPI_File_write_all(fp, stringStream.str().c_str(), stringStream.str().length(), MPI_CHAR, MPI_STATUS_IGNORE);
    MPI_File_close(&fp);
}

void castFileName() {
    char tmpChar = ' ';
    MPI_Bcast(&stringLen, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (blockId != 0) {
        outFileName.resize(stringLen);
    }
    for (int i = 0; i < stringLen; ++i) {
        if (blockId == 0) {
            tmpChar = outFileName[i];
        }
        MPI_Bcast(&tmpChar, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
        outFileName[i] = tmpChar;
    }
    printToCerr(outFileName.c_str());
}

void ompCalculation(double* uCurr, double* uNext, double* diffs, double hX, double hY, double hZ) {
#pragma omp parallel
    {
        int threadId = omp_get_thread_num();
        int threadCount = omp_get_num_threads();
        for (int it = threadId; it < blockEqDim.x * blockEqDim.y * blockEqDim.z; it += threadCount) {
            vec3 pos = getPositionInBlock(it);
            int i = pos.x, j = pos.y, k = pos.z;
            uNext[_idx(i, j, k)] =
                ((uCurr[_idx(i + 1, j, k)] + uCurr[_idx(i - 1, j, k)]) / (hX * hX) +
                    (uCurr[_idx(i, j + 1, k)] + uCurr[_idx(i, j - 1, k)]) / (hY * hY) +
                    (uCurr[_idx(i, j, k + 1)] + uCurr[_idx(i, j, k - 1)]) / (hZ * hZ)) /
                (2.0 * ((1.0 / (hX * hX)) + (1.0 / (hY * hY)) + (1.0 / (hZ * hZ))));
            diffs[it] = std::abs(uNext[_idx(i, j, k)] - uCurr[_idx(i, j, k)]);
        }
    }
}

int main(int argc, char** argv) {
    std::cout << std::scientific << std::setprecision(6);
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &blocksTotalCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &blockId);

    //input
    if (blockId == 0) {
        std::cin >> gridEqDim.x >> gridEqDim.y >> gridEqDim.z;
        std::cin >> blockEqDim.x >> blockEqDim.y >> blockEqDim.z;
        std::cin >> outFileName;
        std::cin >> epsilon;
        std::cin >> lX >> lY >> lZ;
        std::cin >> uDown >> uUp >> uLeft >> uRight >> uFront >> uBack >> u0;
        stringLen = outFileName.length();
    }
    MPI_Bcast(&gridEqDim.x, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&gridEqDim.y, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&gridEqDim.z, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Bcast(&blockEqDim.x, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&blockEqDim.y, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&blockEqDim.z, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Bcast(&epsilon, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Bcast(&lX, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&lY, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&lZ, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Bcast(&uDown, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&uUp, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&uLeft, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&uRight, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&uFront, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&uBack, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Bcast(&u0, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    printGlobalVariableToCerr();
    castFileName();
    MPI_Barrier(MPI_COMM_WORLD);
    blockPos = getPositionByBlockId(blockId);
    //-----
    int blockInsideSize = blockEqDim.x * blockEqDim.y * blockEqDim.z;
    double* uCurr = new double[(blockEqDim.x + 2LL) * (blockEqDim.y + 2LL) * (blockEqDim.z + 2LL)];
    double* uNext = new double[(blockEqDim.x + 2LL) * (blockEqDim.y + 2LL) * (blockEqDim.z + 2LL)];
    double* diffs = new double[blocksTotalCount];
    double* diffs_inside_block = new double[blockInsideSize];

    double hX = lX / (double)((long long)gridEqDim.x * blockEqDim.x);
    double hY = lY / (double)((long long)gridEqDim.y * blockEqDim.y);
    double hZ = lZ / (double)((long long)gridEqDim.z * blockEqDim.z);


    for (int i = -1; i <= blockEqDim.x; i++) {
        for (int j = -1; j <= blockEqDim.y; j++) {
            for (int k = -1; k <= blockEqDim.z; k++) {
                uCurr[_idx(i, j, k)] = u0;
            }
        }
    }

    double max_diff = 0.0;
    do {
        MPI_Barrier(MPI_COMM_WORLD);
        reciveAndSendUpSide(uCurr);
        reciveAndSendDownSide(uCurr);
        reciveAndSendBackSide(uCurr);
        reciveAndSendFrontSide(uCurr);
        reciveAndSendLeftSide(uCurr);
        reciveAndSendRightSide(uCurr);

        //calculating
        ompCalculation(uCurr, uNext, diffs_inside_block, hX, hY, hZ);
        max_diff = 0.0;
        for (int i = 0; i < blockInsideSize; ++i) {
            max_diff = std::max(max_diff, diffs_inside_block[i]);
        }
        MPI_Allgather(&max_diff, 1, MPI_DOUBLE, diffs, 1, MPI_DOUBLE, MPI_COMM_WORLD);
        max_diff = 0.0;
        for (int i = 0; i < blocksTotalCount; ++i) {
            max_diff = std::max(max_diff, diffs[i]);
        }

        //swapping
        double* tmp = uCurr;
        uCurr = uNext;
        uNext = tmp;
    } while (max_diff > epsilon);
    MPI_Barrier(MPI_COMM_WORLD);
    multiPrint(uCurr);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    delete[] uCurr;
    delete[] uNext;
    delete[] diffs;
    return 0;
}
