#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>


#define ITER 100
#define TILE_WIDTH 4

__global__ void MatrixMulKernel(float* d_M, float* d_N, float* d_P, int Width)
{
	__shared__ float Mds[TILE_WIDTH][TILE_WIDTH];
	__shared__ float Nds[TILE_WIDTH][TILE_WIDTH];
	int bx = blockIdx.x; int by = blockIdx.y;
	int tx = threadIdx.x; int ty = threadIdx.y;
	// Identify the row and column of the d_P element to work on
	int Row = by * TILE_WIDTH + ty;
	int Col = bx * TILE_WIDTH + tx;
	float Pvalue = 0;
	// Loop over the d_M and d_N tiles required to compute d_P element
	for (int ph = 0; ph < Width/TILE_WIDTH; ++ph)
	{
		// Collaborative loading of d_M and d_N tiles into shared memory
		Mds[ty][tx] = d_M[Row*Width + ph*TILE_WIDTH + tx];
		Nds[ty][tx] = d_N[(ph*TILE_WIDTH + ty)*Width + Col];
		__syncthreads();
		for (int k = 0; k < TILE_WIDTH; ++k)
		{
			Pvalue += Mds[ty][k] * Nds[k][tx];
		}
		__syncthreads();
	}
	d_P[Row*Width + Col] = Pvalue;
}


int main()
{
    float *h_a, *h_b, *h_c;
    h_a = (float *)malloc(ITER * sizeof(float));
    h_b = (float *)malloc(ITER * sizeof(float));
    h_c = (float *)malloc(ITER * sizeof(float));

    for (int i = 0; i < ITER; ++i)
    {
        h_a[i] = i;
        h_b[i] = i;
        h_c[i] = i;
    }

    MatrixMulKernel<<<ceil(ITER/256.0), 256>>>(h_a, h_b, h_c, ITER);
	
    free(h_a);
    free(h_b);
    free(h_c);

    return 0;
}
