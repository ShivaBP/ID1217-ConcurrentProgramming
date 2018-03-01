
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define MAXSIZE 200000
//#define MAXWORKERS 8

double start_time, end_time; /* start and end times */
int size;
int array[MAXSIZE];
//int numWorkers;
//pthread_mutex_t numWorkerLock;
/* timer */
/*double read_timer()
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
}*/

// printing the array
void arrayPrint()
{
    int j;
    printf("[ ");
    for (j = 0; j < size; j++)
    {
        printf(" %d", array[j]);
    }
    printf(" ]\n");
    printf("\n");
}

// swap 2 elements insode the array
void swap(int i, int j)
{
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

// choose pivot as last element in array
// swap all the smaller ones to begining
// position the pivot
//return pivot
int partition(int low, int high)
{
    int pivot = array[high];
    int i = low;
    int j;
    for (j = low; j < high; j++)
    {
        if (array[j] <= pivot)
        {
            swap(i, j);
            i++;
        }
    }
    swap(i, high);
    return i;
}

// sequential quicksort used for improvement measurements
void sqsort(int low, int high)
{
    if (low < high)
    {
        int pivot = partition(low, high);
        sqsort(low, pivot - 1);
        sqsort(pivot + 1, high);
    }
}

// arguments for parallel quicksort
/*struct args
{
    int low;
    int high;
    int threadID;
}; */

void pqsort(int low, int high);

// give the quicksort its arguments
/*void *initiate_thread(void *arg)
{
    pqsort(((struct args *)arg)->low, ((struct args *)arg)->high, ((struct args *)arg)->threadID);
    return 0;
}*/

// tha parallel quicksort function
void pqsort(int low, int high)
{
    if (high - low <= 1000)
    {
        sqsort(low, high);
    }
    // bool threadCreated = 0;
    else
    {
        if (low < high)
        {
            int pivot = partition(low, high);
          omp_set_dynamic(0);

     

               
                        #pragma omp task
                        {
                    
                        pqsort(low, pivot - 1);
                        }
                        //  printf("threadcreated\n");
                        // }
//#pragma omp single nowait
//     {
                       // #pragma omp task
                    
                        pqsort(pivot + 1, high);
                    
                
                // wait for the created thread to terminate
                // if mother lives , stack of child is working
                // pthread_join(thread, NULL);
            
        }
    }
}

int main(int argc, char *argv[])

{
    /* read command line args if any */
    size = (argc > 1) ? atoi(argv[1]) : MAXSIZE;

    // numWorkers = (argc > 2) ? atoi(argv[2]) : MAXWORKERS;
    // omp_set_num_threads(numWorkers);
    /*  if (size > MAXSIZE)
        size = MAXSIZE;
    if (numWorkers > MAXWORKERS)
        numWorkers = MAXWORKERS; */

    // initialize the array
    int i;
    for (i = 0; i < size; i++)
        array[i] = rand() % 99;

    /* print initial array, unless its too big */

    printf("Initial array:\n");
    //arrayPrint();

    /* get parallel execution time */
    start_time = omp_get_wtime();
    #pragma omp parallel 
    {
         #pragma omp single nowait
    pqsort(0, size - 1);
    }
    end_time = omp_get_wtime();

    /* get sequential execution time 
   starts = read_timer();
   sqsort(0, size - 1);
  ends= read_timer(); */

    // results are printed here

    printf("Sorted array:\n");
    //arrayPrint();

    /*  printf("The execution time of the sequential program is %g sec\n", ends - starts);
      printf("\n");
        printf("\n"); */

    printf("The execution time of the parallel program is %g sec\n", end_time - start_time);
}
