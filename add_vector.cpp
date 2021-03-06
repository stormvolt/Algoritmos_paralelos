#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>


#define ITER 1000000

__global__
void vecAddKernel(float* d_A, float* d_B, float* d_C, int n)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    if (i < n)
    {
        d_C[i] = d_A[i] + d_B[i];
    }
}

void vecAdd(float* h_A, float* h_B, float* h_C, int n)
{
    int size = n * sizeof(float);
    float *d_A, *d_B, *d_C;

    cudaMalloc((void **) &d_A, size);
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMalloc((void **) &d_B, size);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    cudaMalloc((void **) &d_C, size);

    vecAddKernel<<<ceil(n/256.0), 256>>>(d_A, d_B, d_C, n);

    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_B);
}


int main()
{
    int *h_a, *h_b, *h_c;
    h_a = (int *)malloc(ITER * sizeof(int));
    h_b = (int *)malloc(ITER * sizeof(int));
    h_c = (int *)malloc(ITER * sizeof(int));

    for (int i = 0; i < ITER; ++i)
    {
        h_a[i] = i;
        h_b[i] = i;
        h_c[i] = i;
    }

    vecAdd(h_a, h_b, h_c, ITER);
    free(h_a);
    free(h_b);
    free(h_c);

    return 0;
}
