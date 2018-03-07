#include <stdio.h>

//global variables
void initMatrix();
//void jacobi();
void printMatrix();
void seqJacobi();
//grid size is the size of the matrix without the boundaries included
//change to input of user
int gridSize = 3;
//change to input of user
int maxIters = 20;

int main(){


  //initiate the matrix
  double matrix[gridSize+1][gridSize+1];
  initMatrix(matrix, gridSize, 1.0, 0.0);
  printf("oldMatrix\n");
  printMatrix(matrix, gridSize);
  double newMatrix[gridSize+1][gridSize+1];
  seqJacobi(&newMatrix, &matrix, gridSize, maxIters);
  printf("newMatrix\n");
  printMatrix(newMatrix, gridSize);
  return 0;
}

/*initMatrix method initializes the matrix
input parameters:
  gridSize: size of the matrix without the boundaries included
  boundaryValue: the value appointed to the boundaryValue
  innerValue: the value appointed*/

void initMatrix(double matrix[gridSize+1][gridSize+1], int gridSize, double boundaryValue, double innerValue){
  // size of matrix (n x n)
  int n = gridSize+1;
  for(int i = 0; i<n; i++){
    for(int j = 0; j<n; j++){
      if(i == 0 || j == 0){
        matrix[i][j] = boundaryValue;
      }
      else if(i == gridSize || j == gridSize){
        matrix[i][j] = boundaryValue;
      }
      else{
        matrix[i][j] = innerValue;
      }
    }
  }
}

void printMatrix(double matrix[gridSize+1][gridSize+1],int gridSize){
  int n = gridSize+1;
  for(int i = 0; i<n;i++){
      printf("[ ");
      for(int j = 0; j<n; j++){
        printf("%f ", matrix[i][j]);
      }
      printf(" ]\n");
  }
}

/*input parameters:
    oldMatrix: which is the old matrix
    gridSize: the size of the innerValues of the matrix
 return:
    newMatrix generated using jacobi method*/
/*void jacobi(double newMatrix[gridSize+1][gridSize+1],double oldMatrix[gridSize+1][gridSize+1], int gridSize){
  int n = gridSize+1;
  //int newMatrix[n][n];
  for(int i = 1; i<n; i++){
    for(int j = 1; j<n; j++){
      printf("%d\n", j);
      newMatrix[i, j] = (*oldMatrix[i, j-1] + *oldMatrix[i-1, j] + *oldMatrix[i+1, j] + *oldMatrix[i, j+1])*0.25;
    }
  }
  //return newMatrix;
}*/


void seqJacobi(double new[gridSize+1][gridSize+1],double grid[gridSize+1][gridSize+1], int gridSize, int maxIters){
  for(int iters = 1; iters< maxIters/2; iters++){
    //compute new values for all interior points
    for(int i = 1; i<gridSize; i++){
      for(int j = 1; j<gridSize; j++){
        *new[i,j] = (*grid[i-1, j] + *grid[i+1, j] + *grid[i, j-1] + *grid[i, j+1])*0.25;
      }
    }

    //compute new values again for interior points
    for(int  i = 1; i< gridSize; i++){
      for(int j = 1; j<gridSize; j++){
        *grid[i,j] = (*new[i-1, j] + *new[i+1, j] + *new[i, j-1] + *new[i, j+1]) * 0.25;
      }
    }
  }





}
