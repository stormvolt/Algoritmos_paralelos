#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define TAM 1000

int main(int argc,char **argv)
{
    int i, miRank, procs, parte, result;
    result = 0;
    int vet[MAX];
    int tag = 10;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &miRank);

    parte = (int)(TAM/(procs-1));

    for(i=0; i<TAM; i++)
	vet[i] = i;
    int results[procs-1];
    MPI_Reduce(results, result, procs-1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Finalize();
    printf("resultado = %d \n", result);
    return 0;
}
