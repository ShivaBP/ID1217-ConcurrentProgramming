#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <omp.h>

#define MATRIXSIZE 100
#define ITERS 20
#define WORKERS 4
#define BOUNDARY 1.0
#define INTERIOR 0.0

//define variables
int matrixSize, iterations, i, j, count, totalSize, numWorkers;
double start_time, end_time, error, difference;
FILE *file;
bool go = false;
int threadsArrived = 0;
void barrier(){
#pragma omp atomic  
        threadsArrived++;
    while (threadsArrived < numWorkers)
    {}// wait
    if (threadsArrived == numWorkers)
    {
        go = true;
    }
}

int main(int argc, char const *argv[])
{

    //command line arguments
    matrixSize = (argc > 1) ? atoi(argv[1]) : MATRIXSIZE;
    printf("The size of the matrix without boundaries is: %d.\n\n", matrixSize);
    iterations = (argc > 2) ? atoi(argv[2]) : ITERS;
    printf("The number of iterations used is: %d.\n\n", iterations);
    numWorkers = (argc > 3) ? atoi(argv[3]) : WORKERS;
    printf("The number of threads used is: %d.\n\n", numWorkers);

    //inititalize the matrix
    totalSize = matrixSize + 1;
    double grid[totalSize][totalSize];
    double new[totalSize][totalSize];
    for (i = 0; i < totalSize; i++)
    {
        for (j = 0; j < totalSize; j++)
        {
            if (i == 0 || j == 0)
            {
                grid[i][j] = BOUNDARY;
                new[i][j] = BOUNDARY;
            }
            else if (i == matrixSize || j == matrixSize)
            {
                grid[i][j] = BOUNDARY;
                new[i][j] = BOUNDARY;
            }
            else
            {
                grid[i][j] = INTERIOR;
                new[i][j] = INTERIOR;
            }
        }
    }

    start_time = omp_get_wtime();
    //do the jacobi calculations'
    
    for (count = 0; count < iterations / 2; count++)
    {
#pragma omp parallel num_threads(numWorkers)
        {
#pragma omp for 
            for (i = 1; i < matrixSize; i++)
            {
                for (j = 1; j < matrixSize; j++)
                {
                    new[i][j] = (grid[i - 1][j] + grid[i + 1][j] + grid[i][j - 1] + grid[i][j + 1]) * 0.25;
                }
            } // implicit barrier
            barrier();
#pragma omp for  
            for (i = 1; i < matrixSize; i++)
            {
                for (j = 1; j < matrixSize; j++)
                {
                    grid[i][j] = (new[i - 1][j] + new[i + 1][j] + new[i][j - 1] + new[i][j + 1]) * 0.25;
                }
            } // implicit barrier
            barrier();
        }
    }
// compute maximum error locally

// compute the maximum difference globally
#pragma omp master
    {
        for (i = 1; i < matrixSize; i++)
        {
            for (j = 1; j < matrixSize; j++)
            {

                difference = grid[i][j] - new[i][j];

                if (0 > difference)
                {
                    difference = 0.0 - difference;
                }

                if (difference > error)
                {
                    error = difference;
                }
            }
        }
    }
    end_time = omp_get_wtime();
    printf("The maximum error is:  %.15f.\n\n", error);
    printf("The execution time is %g sec\n", end_time - start_time);

    //write to output file
    file = fopen("jacobiP.out", "w");
    fprintf(file, "This is the grid matrix.\n\n");
    for (i = 0; i < totalSize; i++)
    {
        for (j = 0; j < totalSize; j++)
        {
            fprintf(file, "%f ", grid[i][j]);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n\n\n");
    fprintf(file, "This is the new matrix.\n\n");
    for (i = 0; i < totalSize; i++)
    {
        for (j = 0; j < totalSize; j++)
        {
            fprintf(file, "%f ", new[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    return 0;
}