#include <stdio.h>
#include <omp.h>
#define NUM_THREADS 12 //Probar con 3, 6, 9, 12, 15 y 18 threads.

int main()
{
        omp_set_num_threads(NUM_THREADS);
#pragma omp parallel
    {
        int ID = omp_get_thread_num();
        printf("\nproceso(%d) ", ID);
    }
}