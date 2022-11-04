#include <stdio.h>
#include <omp.h>
#include <math.h>


#define NUM_THREADS 6
#define N 2500000 

FILE *fptr;

void operaciones(int op_type);

static double w0 = 0.5, a = 0, b = 2;
static double h, t[N], w[N], ab;

int i, id;
int main() {
   
    omp_set_num_threads(NUM_THREADS);

    fptr=fopen("Euler_n_0.txt","w");

 
    printf("\nNumero de pasos: %d \n", N);
    fprintf(fptr, "Datos que encuentra el metodo de Euler (variable ind.\t variable dep.\t numero de thread).\n");

    
    double t1, t2, f_time; 

    
    const double startTime = omp_get_wtime();
    t1 = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
                (void)operaciones(1);
        }
    }

    
    t2 = omp_get_wtime();

    
    f_time = t2 - t1;

    
    for(i=0; i<N; i++)
    {
        fprintf(fptr, "%f\t %f\n", t[i], w[i]);
    }

    
    fclose(fptr);

    
    printf("Le tomo al programa (%lf) segundos\n", f_time);
}



void operaciones(int op_type) {
    id = omp_get_thread_num();
    printf("ID Thread [%d]\n", id);
   
    h = (b-a) / N;
    w[0] = w0;
    t[0] = a;

    for(i=0; i<N; i++) 
    {
        t[i] = a + (h*i);
        switch (op_type)
        {
            case 1: 
                w[i] = exp2((w[i-1]+h)*(1+t[i-1]*(t[i-1]-w[i-1])));
                
                break;
            case 2: 
                w[i] = w[i-1]+h*(1+t[i-1]*pow(t[i-1]-w[i-1], 2));
                break;
            case 3: 
                w[i] = sin(w[i-1]+h*(1+t[i-1]*t[i-1]-w[i-1]));
                break;
            case 4: 
                w[i] = cos(w[i-1]+h*(1+t[i-1]*t[i-1]-w[i-1]));
                break;
            case 5: 
                w[i] = sqrt(w[i-1]+h*(1+t[i-1]*t[i-1]-w[i-1]));
                break;
            case 6: 
                w[i] = sinh(w[i-1]+h*(1+t[i-1]*t[i-1]-w[i-1]));
                break;
            case 7: 
                w[i] = cosh(w[i-1]+h*(1+t[i-1]*t[i-1]-w[i-1]));
                break;
            default:
                w[i]=w[i-1]+h*(1+t[i-1]*t[i-1]-w[i-1]);
                break;
        }
    }
}
