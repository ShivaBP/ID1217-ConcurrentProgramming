#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#define MATRIXSIZE 100
#define ITERS 20
#define BOUNDARY 1.0
#define INTERIOR 0.0

/* timer */
double read_timer()
{
  static bool initialized = false;
  static struct timeval start;
  struct timeval end;
  if (!initialized)
  {
    gettimeofday(&start, NULL);
    initialized = true;
  }
  gettimeofday(&end, NULL);
  return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

int main(int argc, char const *argv[])
{
  //define variables
  int matrixSize, iterations, i, j, count, totalSize;
  double start_time, end_time, error, difference;
  FILE *file;
  

  //command line arguments
  matrixSize = (argc > 1) ? atoi(argv[1]) : MATRIXSIZE;
  printf("The size of the matrix without boundaries is: %d.\n\n", matrixSize);
  iterations = (argc > 2) ? atoi(argv[2]) : ITERS;
  printf("The number of iterations used is: %d.\n\n", iterations);

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

  //Computations
  start_time = read_timer();
  //do the jacobi calculations
  for (count = 0; count < iterations / 2; count++)
  {
    for (i = 1; i < matrixSize; i++)
    {
      for (j = 1; j < matrixSize; j++)
      {
        new[i][j] = (grid[i - 1][j] + grid[i + 1][j] + grid[i][j - 1] + grid[i][j + 1]) * 0.25;
      }
    }
    for (i = 1; i < matrixSize; i++)
    {
      for (j = 1; j < matrixSize; j++)
      {
        grid[i][j] = (new[i - 1][j] + new[i + 1][j] + new[i][j - 1] + new[i][j + 1]) * 0.25;
      }
    }
  }


  // compute the maximum difference
  for (i = 1; i < matrixSize; i++)
  {
    for (j = 1; j < matrixSize; j++)
    {
   
      difference = grid[i][j] - new[i][j] ;

      if( 0 > difference){
        difference = 0.0 - difference ;
      }
     
      if (difference > error)
      {
        error = difference;


      }
    }
  } 
  end_time = read_timer();
  printf("The maximum error is:  %.15f.\n\n", error);
  

  //write to output file
  printf("The execution time is %g sec\n", end_time - start_time);
  file = fopen("jacobiS.out", "w");
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