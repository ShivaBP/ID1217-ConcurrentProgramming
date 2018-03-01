
/* matrix summation using OpenMP

   usage with gcc (version 4.2 or higher required):
     gcc -O -fopenmp -o matrixSum-openmp matrixSum-openmp.c 
     ./matrixSum-openmp size numWorkers

*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double start_time, end_time;

#define MAXSIZE 100 /* maximum matrix size */
#define MAXWORKERS 8 /* maximum number of workers */

typedef struct
{
  int x;
  int y;
  int value;

} coordinate;

// defining global variables
coordinate globalMinimum; // maximum that will be accessed sequentially
coordinate globalMaximum; // minimum that will be accessed sequentially

// defining local variables
coordinate *threadsMinimum; //list  partial minimum values calculated by each thread
coordinate *threadsMaximum; // partial maximum values calculated by each thread
//coordinate localMinimum;
//coordinate localMaximum;

int numWorkers;
int size;
int matrix[MAXSIZE][MAXSIZE];
// void *Worker(void *);

/* read command line, initialize, and create threads */
int main(int argc, char *argv[])
{
  int i, j, threadID, total = 0;

  /* read command line args if any */
  size = (argc > 1) ? atoi(argv[1]) : MAXSIZE;
  numWorkers = (argc > 2) ? atoi(argv[2]) : MAXWORKERS;
  if (size > MAXSIZE)
    size = MAXSIZE;
  if (numWorkers > MAXWORKERS)
    numWorkers = MAXWORKERS;

  omp_set_num_threads(numWorkers);

  /* initialize the matrix */
  for (i = 0; i < size; i++)
  {
  //  printf("[ ");
    for (j = 0; j < size; j++)
    {
      matrix[i][j] = rand() % 99;
     // printf(" %d", matrix[i][j]);
    }
   // printf(" ]\n");
  }

  // initiating global variables
  globalMinimum.x = 0;
  globalMinimum.y = 0;
  globalMinimum.value = matrix[0][0];
  globalMaximum.x = 0;
  globalMaximum.y = 0;
  globalMaximum.value = matrix[0][0];

  // initiating  partial min and max storage
  threadsMinimum = (coordinate *)malloc(sizeof(coordinate) * numWorkers);
  threadsMaximum = (coordinate *)malloc(sizeof(coordinate) * numWorkers);

  /*for (i = 0; i < numWorkers; i++)
  {
    localMaximum = threadsMaximum[i];
    localMinimum = threadsMinimum[i];
  } */

  start_time = omp_get_wtime();
#pragma omp parallel for reduction(+: total) private(i, j, threadID)

  for (i = 0; i < size; i++)
  {
    // get the thread ID for each row
    threadID = omp_get_thread_num();
    //  intitialize thread local variables
    threadsMinimum[threadID].value = 100;
    threadsMinimum[threadID].x = -1;
    threadsMinimum[threadID].y = -1;
    threadsMaximum[threadID].value = -1;
    threadsMaximum[threadID].x = -1;
    threadsMaximum[threadID].y = -1;

    for (j = 0; j < size; j++)
    {
      int block = matrix[i][j];
      total += matrix[i][j];

      if (threadsMaximum[threadID].value < block)
      {
        threadsMaximum[threadID].value = block;
        threadsMaximum[threadID].x = j;
        threadsMaximum[threadID].y = i;
        // printf(" i am  %d My maximum is  %d \n", threadID , threadsMaximum[threadID].value);
      }
      // printf(" i reached the minimum \n") ;
      if (threadsMinimum[threadID].value > block)
      {
        threadsMinimum[threadID].value = block;
        threadsMinimum[threadID].x = j;
        threadsMinimum[threadID].y = i;
        // printf(" i am  %d My minimum is  %d \n", threadID , threadsMinimum[threadID].value);
      }
    }
  }

// implicit barrier
// accessing the globals sequentially
#pragma omp master
{
    for (i = 0; i < numWorkers; i++)
    {
        if (globalMaximum.value < threadsMaximum[i].value)
        {
          globalMaximum.value = threadsMaximum[i].value;
          globalMaximum.x = threadsMaximum[i].x;
          globalMaximum.y = threadsMaximum[i].y;
        }
      
        if (globalMinimum.value > threadsMinimum[i].value)
        {
          globalMinimum.value = threadsMinimum[i].value;
          globalMinimum.x = threadsMinimum[i].x;
          globalMinimum.y = threadsMinimum[i].y;
        }
      }  
}

  end_time = omp_get_wtime();

  printf("The total is %d\n", total);
  printf("The maximum is %d appears at [%d , %d]\n", globalMaximum.value, globalMaximum.x + 1, globalMaximum.y + 1);
  printf("The minimum is %d appears at [%d , %d]\n", globalMinimum.value, globalMinimum.x + 1, globalMinimum.y + 1);
  printf("It took %g seconds\n", end_time - start_time);
}
