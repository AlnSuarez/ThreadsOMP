#include<stdio.h>
#include "omp.h"

int main() 
{
    FILE *f;  
    f = fopen("alan.txt", "w");
    fprintf(f, "Alan Suárez Santamaría.\nIngeniería en Sistemas Digitales y Robótica.");
    fclose(f);
}