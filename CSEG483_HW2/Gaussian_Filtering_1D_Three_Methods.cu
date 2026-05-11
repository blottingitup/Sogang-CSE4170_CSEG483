 /* 
 * 
 * [HW 2] 1D Gaussian Filtering: Three Methods
 *
 */

//
// Written for Sogang University CSEG483/CSE5483
//

#define _COMPARE_RESULTS 1

#define _NUMBER_OF_TESTS_ON_GPU 32
#define _NUMBER_OF_ELEMENTS  ((1 << 28) + 3)
#define _THREADS_PER_BLOCK 1024
#define _MIN_GRID_REDUCTION_FACTOR 2 // a power of two
#define _MAX_GRID_REDUCTION_FACTOR 32 // a power of two
#define _MIN_KERNEL_SIZE_DEVICE 1
#define _MAX_KERNEL_SIZE_DEVICE 15

#include <stdio.h>
#include <math.h>
#include <random>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include "measure_host_time_3.h"

// kernel width = 1, 3, 5, 7, 9, 11, 13, or 15
const float GAUSSIAN_KERNEL_1D[64] = {
    1.0f,
    1.0f / 4.0f, 2.0f / 4.0f, 1.0f / 4.0f,
    1.0f / 16.0f, 4.0f / 16.0f, 6.0f / 16.0f, 4.0f / 16.0f, 1.0f / 16.0f,
    1.0f / 64.0f, 6.0f / 64.0f, 15.0f / 64.0f, 20.0f / 64.0f,
        15.0f / 64.0f, 6.0f / 64.0f, 1.0f / 64.0f,
    1.0f / 256.0f, 8.0f / 256.0f, 28.0f / 256.0f, 56.0f / 256.0f, 70.0f / 256.0f,
        56.0f / 256.0f, 28.0f / 256.0f, 8.0f / 256.0f, 1.0f / 256.0f,
    1.0f / 1024.0f, 10.0f / 1024.0f, 45.0f / 1024.0f, 120.0f / 1024.0f, 210.0f / 1024.0f,
        252.0f / 1024.0f, 210.0f / 1024.0f, 120.0f / 1024.0f, 45.0f / 1024.0f, 10.0f / 1024.0f, 1.0f / 1024.0f,
    1.0f / 4096.0f, 12.0f / 4096.0f, 66.0f / 4096.0f, 220.0f / 4096.0f, 495.0f / 4096.0f,
        792.0f / 4096.0f, 924.0f / 4096.0f, 792.0f / 4096.0f, 495.0f / 4096.0f, 220.0f / 4096.0f,
        66.0f / 4096.0f, 12.0f / 4096.0f, 1.0f / 4096.0f,
    1.0f / 16384.0f, 14.0f / 16384.0f, 91.0f / 16384.0f, 364.0f / 16384.0f, 1001.0f / 16384.0f,
        2002.0f / 16384.0f, 3003.0f / 16384.0f, 3432.0f / 16384.0f, 3003.0f / 16384.0f, 2002.0f / 16384.0f,
        1001.0f / 16384.0f, 364.0f / 16384.0f, 91.0f / 16384.0f, 14.0f / 16384.0f, 1.0f / 16384.0f
};

void gen_uniform_distribution_int(int* array, int a, int b, int N) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(a, b);

    for (int i = 0; i < N; i++)  
        array[i] = dist(gen);
}

void apply_1D_Gaussian_wrap_host(const int* image_in, int* image_out, int N, int kernel_width) {
    // The image wraps around, where the right edge connects to the left edge
    int half_width = kernel_width / 2;
    const float* kernel = GAUSSIAN_KERNEL_1D + half_width * half_width;

    for (int i = 0; i < N; i++) {
        float sum = 0.0f;

        for (int j = -half_width; j <= half_width; j++) {
            int index = i + j;
            if (index < 0) {
                index = N + index;
            }
            else if (index >= N) {
                index = index - N;
            }
            sum += image_in[index] * kernel[j + half_width];
        }
        image_out[i] = static_cast<int>(sum + 0.5f);
    }
}

__constant__ float d_kernel_weights[_MAX_KERNEL_SIZE_DEVICE];

#include "my_kernels.h"  

void compare_results(int* h_image_out_host, int* h_image_out_device, int n_elements) {
    for (int i = 0; i < n_elements; ++i) {
        if (abs(h_image_out_host[i] - h_image_out_device[i]) > 1) {
            fprintf(stderr, "     Result verification failed at element %d(%d (H) != %d (D))!\n",
                i, h_image_out_host[i], h_image_out_device[i]);
            fprintf(stdout, "     Test FAILED\n");
            return;
        }
        //if (i < n_elements) { // print the results for visual inspection
        //    fprintf(stdout, "[%d] Host: %3d, Device: %3d\n", i, h_image_out_host[i], h_image_out_device[i]);
        //}
    }
    fprintf(stdout, "     Test PASSED\n");    
}

int main(void) {
	int n_elements = _NUMBER_OF_ELEMENTS;
    size_t size = n_elements * sizeof(int);
    float host_time, device_time_ave;
   
    // Allocate the host memory
    int* h_image_in = (int*)malloc(size);
    int* h_image_out_host = (int*)malloc(size);
    int* h_image_out_device = (int*)malloc(size);
    if (h_image_in == NULL || h_image_out_host == NULL || h_image_out_device == NULL) {
        fprintf(stderr, "^^^ Failed to allocate host vectors!\n");
        exit(EXIT_FAILURE);
    }

    gen_uniform_distribution_int(h_image_in, 0, 255, n_elements);
    // Allocate the device memory
    int* d_image_in = NULL;
    cudaMalloc((void**)&d_image_in, size);
    int* d_image_out = NULL;
    cudaMalloc((void**)&d_image_out, size);

    for (int kernel_width = _MIN_KERNEL_SIZE_DEVICE; kernel_width <= _MAX_KERNEL_SIZE_DEVICE; kernel_width += 2) {
        fprintf(stdout, "\n\n=== 1D Gaussian filtering of image of %d pixels(KERNEL WIDTH = %d) ===\n",
            n_elements, kernel_width);
		cudaDeviceSynchronize();  

        CHECK_TIME_START(_start, _freq);
        apply_1D_Gaussian_wrap_host(h_image_in, h_image_out_host, n_elements, kernel_width);
        CHECK_TIME_END(_start, _end, _freq, _compute_time);
        fprintf(stdout, "\n  [HOST] Time to filter an image of %d pixels on the host = %.3f(ms)\n",
            n_elements, _compute_time);
        host_time = _compute_time;

        int half_width = kernel_width / 2;
        // Copy the kernel weights to the constant memory on the device
        cudaMemcpyToSymbol(d_kernel_weights,
            (const float*)GAUSSIAN_KERNEL_1D + half_width * half_width,
            kernel_width * sizeof(float));

        cudaMemcpy(d_image_in, h_image_in, size, cudaMemcpyHostToDevice);

         // Launch parameters
        int threads_per_block = _THREADS_PER_BLOCK;
        int blocks_per_grid = (n_elements + threads_per_block - 1) / threads_per_block;


        ////////// ORIGINAL ////////////////////////////////////////////////////////////////////

        apply_1D_Gaussian_wrap_device_ORIGINAL << < blocks_per_grid, threads_per_block >> >
            (d_image_in, d_image_out, n_elements, kernel_width);
		cudaMemset(d_image_out, 0, size);  
        cudaDeviceSynchronize(); // what would happen if this line is missing?

        fprintf(stdout, "\n  [ORIGINAL] CUDA kernel launch with %d blocks of %d threads\n", blocks_per_grid, threads_per_block);

        CHECK_TIME_START(_start, _freq);
        for (int i = 0; i < _NUMBER_OF_TESTS_ON_GPU; i++) {
            apply_1D_Gaussian_wrap_device_ORIGINAL << < blocks_per_grid, threads_per_block >> >
                (d_image_in, d_image_out, n_elements, kernel_width);
        }
        cudaDeviceSynchronize(); // what would happen if this function call is missing?
        CHECK_TIME_END(_start, _end, _freq, _compute_time);
        device_time_ave = _compute_time / _NUMBER_OF_TESTS_ON_GPU;
        fprintf(stdout, "     Average Time to filter an image of %d pixels on the device (%d tests) = %.3f(ms) / speedup = %.3f\n",
            n_elements, _NUMBER_OF_TESTS_ON_GPU, device_time_ave, host_time / device_time_ave);
#if _COMPARE_RESULTS
        cudaMemcpy(h_image_out_device, d_image_out, size, cudaMemcpyDeviceToHost);
        compare_results(h_image_out_host, h_image_out_device, n_elements);
#endif

        ////////// SHARED ////////////////////////////////////////////////////////////////////
        apply_1D_Gaussian_wrap_device_SHARED << < blocks_per_grid, threads_per_block, 
            (threads_per_block + kernel_width - 1) * sizeof(int) >> > (d_image_in, d_image_out, n_elements, kernel_width);
		cudaMemset(d_image_out, 0, size);
        cudaDeviceSynchronize();

        fprintf(stdout, "\n  [SHARED] CUDA kernel launch with %d blocks of %d threads\n", blocks_per_grid, threads_per_block);

        CHECK_TIME_START(_start, _freq);
        for (int i = 0; i < _NUMBER_OF_TESTS_ON_GPU; i++) {
            apply_1D_Gaussian_wrap_device_SHARED << < blocks_per_grid, threads_per_block, 
                (threads_per_block + kernel_width - 1) * sizeof(int) >> > (d_image_in, d_image_out, n_elements, kernel_width);
        }
        cudaDeviceSynchronize(); // what would happen if this function call is missing?
        CHECK_TIME_END(_start, _end, _freq, _compute_time);
        device_time_ave = _compute_time / _NUMBER_OF_TESTS_ON_GPU;
        fprintf(stdout, "     Average Time to filter an image of %d pixels on the device (%d tests) = %.3f(ms) / speedup = %.3f\n",
            n_elements, _NUMBER_OF_TESTS_ON_GPU, device_time_ave, host_time / device_time_ave);

#if _COMPARE_RESULTS
        cudaMemcpy(h_image_out_device, d_image_out, size, cudaMemcpyDeviceToHost);
        compare_results(h_image_out_host, h_image_out_device, n_elements);
#endif

        ////////// STRIDE ////////////////////////////////////////////////////////////////////
        for (int grid_reduction_factor = _MIN_GRID_REDUCTION_FACTOR; grid_reduction_factor <= _MAX_GRID_REDUCTION_FACTOR;
            grid_reduction_factor *= 2) {
            int blocks_per_grid_reduced = blocks_per_grid / grid_reduction_factor;
            apply_1D_Gaussian_wrap_device_STRIDE << < blocks_per_grid_reduced, threads_per_block >> >
                (d_image_in, d_image_out, n_elements, kernel_width);
            cudaMemset(d_image_out, 0, size);
            cudaDeviceSynchronize();

            fprintf(stdout, "\n  [STRIDE(%d)] CUDA kernel launch with %d(%d) blocks of %d threads\n", grid_reduction_factor, 
                blocks_per_grid_reduced, blocks_per_grid,threads_per_block);

            CHECK_TIME_START(_start, _freq);
            for (int i = 0; i < _NUMBER_OF_TESTS_ON_GPU; i++) {
                apply_1D_Gaussian_wrap_device_STRIDE << < blocks_per_grid_reduced, threads_per_block >> >
                    (d_image_in, d_image_out, n_elements, kernel_width);
            }
            cudaDeviceSynchronize();  
            CHECK_TIME_END(_start, _end, _freq, _compute_time);
            device_time_ave = _compute_time / _NUMBER_OF_TESTS_ON_GPU;
            fprintf(stdout, "     Average Time to filter an image of %d pixels on the device (%d tests) = %.3f(ms) / speedup = %.3f\n",
                n_elements, _NUMBER_OF_TESTS_ON_GPU, device_time_ave, host_time / device_time_ave);

#if _COMPARE_RESULTS
            cudaMemcpy(h_image_out_device, d_image_out, size, cudaMemcpyDeviceToHost);
            compare_results(h_image_out_host, h_image_out_device, n_elements);
#endif
        }
    }

    // Free device memory
    cudaFree(d_image_in);
    cudaFree(d_image_out);

    // Free host memory
    free(h_image_in);
    free(h_image_out_host);
    free(h_image_out_device);

    fprintf(stdout, "\n^^^ Done\n");
    return 0;
}

