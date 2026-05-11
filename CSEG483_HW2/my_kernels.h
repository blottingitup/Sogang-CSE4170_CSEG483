//
// Put your kernels in this header file.
//

/*
    Independent Variable:
    _THREADS_PER_BLOCK 64, 128, 256, 512, 1024

    Controlled Variables:
    _NUMBER_OF_TESTS_ON_GPU 32
    _NUMBER_OF_ELEMENTS ((1 << 28) + 3)
    _MIN_GRID_REDUCTION_FACTOR 2
    _MAX_GRID_REDUCTION_FACTOR 32
    _MIN_KERNEL_SIZE_DEVICE 1
    _MAX_KERNEL_SIZE_DEVICE 15
*/

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

// d_kernel_weights is defined in main function

__global__ void  apply_1D_Gaussian_wrap_device_ORIGINAL(const int* d_image_in,
    int* d_image_out, int N, int kernel_width) {
    // N does not need to be a multiple of blockDim.x!
    int half_width = kernel_width / 2;
    const float* kernel = d_kernel_weights;
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    float sum = 0.0f;

    if (i < N) {
        for (int j = -half_width; j <= half_width; j++) {
            int index = i + j;
            if (index < 0) index = N + index;
            else if (index >= N) index = index - N;
            sum += d_image_in[index] * kernel[j + half_width];
        }
        d_image_out[i] = static_cast<int>(sum + 0.5f);
    }
}

__global__ void apply_1D_Gaussian_wrap_device_SHARED(const int* d_image_in, int* d_image_out, int N,
    int kernel_width) {
    // N does not need to be a multiple of blockDim.x!
    extern __shared__ int s_image_in[];
    int half_width = kernel_width / 2;
    int gid = blockDim.x * blockIdx.x + threadIdx.x;
    int block_t0 = blockDim.x * blockIdx.x;  // global thread id of first thread in each block

    if (gid < N) s_image_in[threadIdx.x + half_width] = d_image_in[gid];

    if (threadIdx.x < half_width) {  // left wing of block
        if (blockIdx.x == 0) s_image_in[threadIdx.x] = d_image_in[(N - half_width) + threadIdx.x];
        else s_image_in[threadIdx.x] = d_image_in[(block_t0 + threadIdx.x) - half_width];
    }
    else if (threadIdx.x >= blockDim.x - half_width) {  // right wing of block
        int rel_tid = threadIdx.x - (blockDim.x - half_width);  // shift threadIdx.x to start from 0
        if (blockIdx.x == gridDim.x - 1) s_image_in[(N - block_t0) + half_width + rel_tid] = d_image_in[rel_tid];
        else {
            int right_gid = block_t0 + blockDim.x + rel_tid;
            if (right_gid >= N) right_gid -= N;  // OOB
            s_image_in[blockDim.x + half_width + rel_tid] = d_image_in[right_gid];
        }
    }

    __syncthreads();

    const float* kernel = d_kernel_weights;
    if (gid < N) {
        float sum = 0.0f;
        for (int j = -half_width; j <= half_width; j++) {
            int shared_index = threadIdx.x + half_width + j;
            sum += s_image_in[shared_index] * kernel[j + half_width];
        }
        d_image_out[gid] = static_cast<int>(sum + 0.5f);
    }
}

__global__ void  apply_1D_Gaussian_wrap_device_STRIDE(const int* d_image_in, int* d_image_out, int N,
    int kernel_width) {
    // N does not need to be a multiple of blockDim.x!
    int half_width = kernel_width / 2;
    const float* kernel = d_kernel_weights;
    int start_i = blockDim.x * blockIdx.x + threadIdx.x;
    int stride = gridDim.x * blockDim.x;

    for (int i = start_i; i < N; i += stride) {
        float sum = 0.0f;
        for (int j = -half_width; j <= half_width; j++) {
            int index = i + j;
            if (index < 0) index = N + index;
            else if (index >= N) index = index - N;
            sum += d_image_in[index] * kernel[j + half_width];
        }
        d_image_out[i] = static_cast<int>(sum + 0.5f);
    }
}
