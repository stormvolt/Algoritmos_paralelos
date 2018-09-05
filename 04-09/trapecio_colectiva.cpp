#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(void)
{
    int my_rank, comm_sz, n = 1024, local_n;
    double a = 0.0, b = 3.0, h, local_a, local_b;
    double local_int, total_int;
    int source;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    h = (b-a)/n;
    local_n = n/comm sz;

    local_a = a + my rank*local_n*h;
    local_b = local_a + local_n*h;
    local_int = Trap(local_a, local_b, local_n, h);

    MPI_Reduce(local_int, total_int, comm_sz-1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (my rank == 0)
    {
        printf("With n = %d trapezoids, our estimaten\n", n);
        printf("of the integral from %f to %f = %.15en\n", a, b, total_int);
    }
    MPI_Finalize();
    return 0;
}
