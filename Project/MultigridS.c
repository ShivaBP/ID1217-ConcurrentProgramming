#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#define MATRIXSIZE 100
#define ITERS 20

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

//Jacobian method used to update the points on a fine grid
// ioterations is the iteration on fine grid points which will be set to 4
void jacobi(double **grid, double **new, int gridSize, int iterations)
{
  int count, i, j;
  for (count = 0; count < iterations; count++)
  {
    for (i = 1; i < gridSize + 1; i++)
    {
      for (j = 1; j < gridSize + 1; j++)
      {
        new[i][j] = (grid[i - 1][j] + grid[i + 1][j] + grid[i][j - 1] + grid[i][j + 1]) * 0.25;
      }
    }
    for (int i = 1; i < gridSize + 1; i++)
    {
      for (int j = 1; j < gridSize + 1; j++)
      {
        grid[i][j] = (new[i - 1][j] + new[i + 1][j] + new[i][j - 1] + new[i][j + 1]) * 0.25;
      }
    }
  }
}

// restriction : transfer fine grid to coarse frid
void restriction(double **fine, double **coarse, int coarseSize)
{
  for (int i = 1; i < coarseSize + 1; i++)
  {
    int m = i << 1; // multiply by 2
    for (int j = 1; j < coarseSize + 1; j++)
    {
      int n = j << 1; // multiply by 2
      coarse[i][j] = fine[m][n] * 0.5 + (fine[m - 1][n] + fine[m + 1][n] + fine[m][n - 1] + fine[m][n + 1]) * 0.125;
    }
  }
}

void interpolation(double **coarse, double **fine, int coarseSize, int fineSize)
{
  // copy the points
  for (int i = 1; i < coarseSize + 1; i++)
  {
    int m = i << 1;
    for (int j = 1; j < coarseSize + 1; j++)
    {
      int n = j << 1;
      fine[m][n] = coarse[i][j];
    }
  }
  //set the point
  for (int j = 2; j < fineSize + 1; j += 2)
  {
    for (int i = 1; i < fineSize + 1; i += 2)
    {
      fine[i][j] = (fine[i - 1][j] + fine[i + 1][j]) * 0.5;
    }
  }
  for (int i = 1; i < fineSize + 1; i++)
  {
    for (int j = 1; j < fineSize + 1; j += 2)
    {
      fine[i][j] = (fine[i][j - 1] + fine[i][j + 1]) * 0.5;
    }
  }
}

int main(int argc, char const *argv[])
{
  //define variables
  int i, j, iterations, gridSize, gridSize2, gridSize3, gridSize4, totalSize, count;
  double start_time, end_time, error, difference;
  FILE *file;
  //command line arguments
  gridSize = (argc > 1) ? atoi(argv[1]) : MATRIXSIZE;
  printf("The size of the matrix without boundaries is: %d.\n\n", gridSize);
  iterations = (argc > 2) ? atoi(argv[2]) : ITERS;
  printf("The number of iterations used is: %d.\n\n", iterations);
  gridSize2 = 2 * gridSize + 1;
  gridSize3 = 2 * gridSize2 + 1;
  gridSize4 = 2 * gridSize3 + 1;
  //initializing the grids
  double **grid = (double **) malloc((gridSize+2) * sizeof(double*));
  double **new = (double **) malloc((gridSize+2) * sizeof(double*));
  double **grid2 = (double **) malloc((gridSize2+2) * sizeof(double*));
  double **new2 = (double **) malloc((gridSize2+2) * sizeof(double*));
  double **grid3 = (double **) malloc((gridSize3+2) * sizeof(double*));
  double **new3 = (double **) malloc((gridSize3+2) * sizeof(double*));
  double **grid4 = (double **) malloc((gridSize4+2) * sizeof(double*));
  double **new4 = (double **) malloc((gridSize4+2) * sizeof(double*));
  for(int i = 0; i < gridSize+2; i++) {
    grid[i] = (double *) malloc((gridSize+2) * sizeof(double));
    new[i] = (double *) malloc((gridSize+2) * sizeof(double));
  }
  for(int i = 0; i < gridSize2+2; i++) {
    grid2[i] = (double *) malloc((gridSize2+2) * sizeof(double));
    new2[i] = (double *) malloc((gridSize2+2) * sizeof(double));
  }
  for(int i = 0; i < gridSize3+2; i++) {
    grid3[i] = (double *) malloc((gridSize3+2) * sizeof(double));
    new3[i] = (double *) malloc((gridSize3+2) * sizeof(double));
  }
  for(int i = 0; i < gridSize4+2; i++) {
    grid4[i] = (double *) malloc((gridSize4+2) * sizeof(double));
    new4[i] = (double *) malloc((gridSize4+2) * sizeof(double));
  }
  for (int i = 0; i < gridSize + 2; i++)
  {
    totalSize = gridSize + 1;
  }
  for (int i = 0; i < gridSize2 + 2; i++)
  {
    grid2[0][i]= 1;
    new2[0][i] = 1;
    grid2[gridSize2 + 1][i] = 1;
    new2[gridSize2 + 1][i] = 1;
    grid2[i][0] = 1;
    new2[i][0] = 1;
    grid2[i][gridSize2 + 1] = 1;
    new2[i][gridSize2 + 1] = 1;
  }
  for (int i = 0; i < gridSize3 + 2; i++)
  {
    grid3[0][i] = 1;
    new3[0][i] = 1;
    grid3[gridSize3 + 1][i] = 1;
    new3[gridSize3 + 1][i] = 1;
    grid3[i][0] = 1;
    new3[i][0] = 1;
    grid3[i][gridSize3 + 1] = 1;
    new3[i][gridSize3 + 1] = 1;
  }
  for (int i = 0; i < gridSize4 + 2; i++)
  {
    grid4[0][i] = 1;
    new4[0][i] = 1;
    grid4[gridSize4 + 1][i] = 1;
    new4[gridSize4 + 1][i] = 1;
    grid4[i][0] = 1;
    new4[i][0] = 1;
    grid4[i][gridSize4 + 1] = 1;
    new4[i][gridSize4 + 1] = 1;
  }
  for (int i = 1; i < gridSize4 + 1; i++)
  {
    for (int j = 1; j < gridSize4 + 1; j++)
    {
      grid4[i][j] = 0;
    }
  }
  /* start V-cycle */
  start_time = read_timer();
  jacobi(grid4, new4, gridSize4, 4);
  restriction(grid4, grid3, gridSize3);

  jacobi(grid3, new3, gridSize3, 4);
  restriction(grid3, grid2, gridSize2);

  jacobi(grid2, new2, gridSize2, 4);
  restriction(grid2, grid, gridSize);

  jacobi(grid, new, gridSize, iterations);

  interpolation(grid, grid2, gridSize, gridSize2);
  jacobi(grid2, new2, gridSize2, 4);

  interpolation(grid2, grid3, gridSize2, gridSize3);
  jacobi(grid3, new3, gridSize3, 4);

  interpolation(grid3, grid4, gridSize3, gridSize4);
  jacobi(grid4, new4, gridSize4, 4);
  /* V-cycle ended */
  // calculate maximum differnce
  for (i = 1; i < gridSize2; i++)
  {
    for (j = 1; j < gridSize2; j++)
    {

      difference = grid2[i][j] - new2[i][j];

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
  end_time = read_timer();
  printf("The maximum error is:  %.15f.\n\n", error);
  //write to output file
  printf("The execution time is %g sec\n", end_time - start_time);
  file = fopen("multigridS.out", "w");
  fprintf(file, "This is the grid matrix.\n\n");
  for (i = 0; i < gridSize + 2; i++)
  {
    for (j = 0; j < gridSize + 2; j++)
    {
      fprintf(file, "%f ", grid[i][j]);
    }
    fprintf(file, "\n");
  }
  fprintf(file, "\n\n\n");
  fprintf(file, "This is the new matrix.\n\n");
  for (i = 0; i < gridSize + 2; i++)
  {
    for (j = 0; j < gridSize + 2; j++)
    {
      fprintf(file, "%f ", new[i][j]);
    }
    fprintf(file, "\n");
  }
  fclose(file);
  return 0;
}
