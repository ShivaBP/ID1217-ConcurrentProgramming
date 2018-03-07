#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#define MAXSIZE 100 /* maximum matrix size */
#define MAXWORKERS 10 /* maximum number of workers */
//pthread_mutex_t barrier; /* mutex lock for the barrier */
//pthread_cond_t go;       /* condition variable for leaving */
int numWorkers;          /* number of workers */
//int numArrived = 0;      /* number who have arrived */

//typedef struct
//{

//  int x;
//int y;
//int value;

//} coordinate;



/*void Barrier()
{
    pthread_mutex_lock(&barrier);
    numArrived++;
    if (numArrived == numWorkers)
    {
        numArrived = 0;
        pthread_cond_broadcast(&go);
    }
    else
        pthread_cond_wait(&go, &barrier);
    pthread_mutex_unlock(&barrier);
}*/

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

double start_time, end_time; /* start and end times */

int size, stripSize; /* assume size is multiple of numWorkers */
pthread_mutex_t sumLock, maxLock, minLock;

//int sums[MAXWORKERS];         /* partial sums */
//coordinate maxes[MAXWORKERS]; // partial max
//coordinate mins[MAXWORKERS];  // partial min
int globalSum, globalMax, globalMin, globalXMax, globalYMax, globalXMin, globalYMin;
int matrix[MAXSIZE][MAXSIZE];

void *Worker(void *);

/* read command line, initialize, and create threads */
int main(int argc, char *argv[])
{
    int i, j;
    long l; /* use long in case of a 64-bit system */
    pthread_attr_t attr;
    pthread_t workerid[MAXWORKERS];

    /* set global thread attributes */
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    /* initialize mutexs */
    pthread_mutex_init(&sumLock, NULL);
    pthread_mutex_init(&maxLock, NULL);
    pthread_mutex_init(&minLock, NULL);

  /* read command line args if any */
    size = (argc > 1) ? atoi(argv[1]) : MAXSIZE;
    numWorkers = (argc > 2) ? atoi(argv[2]) : MAXWORKERS;
    if (size > MAXSIZE)
        size = MAXSIZE;
    if (numWorkers > MAXWORKERS)
        numWorkers = MAXWORKERS;
    stripSize = size / numWorkers;

    /* initialize the matrix */
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            matrix[i][j] = rand() % 99;
        }
    }
  

    /* print the matrix */
    #ifdef DEBUG

    for (i = 0; i < size; i++)
    {
        printf("[ ");
        for (j = 0; j < size; j++)
        {
            printf(" %d", matrix[i][j]);
        }
        printf(" ]\n");
    }

    #endif

    // initiate globals
    // globalXMin = -1 ;
    // globalYMin =-1 ;
    //  globalXMax = -1 ;

    //  globalYMax =-1 ;
    globalSum = 0;
    globalMin = globalMax = matrix[0][0];
     globalXMin = 0 ;
 globalXMax = 0 ;
 globalYMin = 0 ;
 globalYMax = 0 ;
    /* do the parallel work: create the workers */
    start_time = read_timer();
    for (l = 0; l < numWorkers; l++)
        pthread_create(&workerid[l], &attr, Worker, (void *)l);

    for (i = 0; i < numWorkers; i++)
        pthread_join(workerid[i], NULL);
    /* get end time */
    end_time = read_timer();

    // print results
    printf("The total is %d\nThe max is %d appears at [%d , %d]\nThe min is %d appears at [%d , %d]\n", globalSum, globalMax, globalXMax + 1, globalYMax + 1, globalMin, globalXMin + 1, globalYMin + 1);
    printf("The execution time is %g sec\n", end_time - start_time);
}

/* Each worker sums the values in one strip of the matrix.
   After a barrier, worker(0) computes and prints the total */
void *Worker(void *arg)
{
    long myid = (long)arg;

    int total, i, j, first, last;
    //variables that will be manipulated iteratively and sequentially
    // int min, max, maxX, minX, maxY, minY;

    // local variables
    int max, min, xmax, ymax, xmin, ymin;

#ifdef DEBUG
    printf("worker %d (pthread id %d) has started\n", myid, pthread_self());
#endif
    /* determine first and last rows of my strip */
    first = myid * stripSize;
    last = (myid == numWorkers - 1) ? (size - 1) : (first + stripSize - 1);
    /* sum values in my strip */

    // because we know the values are between 0 and 99
    max = min = matrix[first][0];
total = 0 ;
   xmin = 0;
    ymin = first;
     xmax = 0;
    ymax = first;

    for (i = first; i <= last; i++)
        for (j = 0; j < size; j++)
        {

            int block = matrix[i][j];

            total += matrix[i][j];

            // find max
            if (max < block)
            {
                max = matrix[i][j];
                xmax = j;
                ymax = i;
            }

            // find min
            if (min > block)
            {
                min = matrix[i][j];
                xmin = j;
                ymin = i;
            }
        }

    // send the results back to main thread
    // use locks

    // the sum value
    pthread_mutex_lock(&sumLock);
    globalSum += total;

    //  printf("my global is  %d\n", globalSum) ;

    //   printf("my  total is %d\n", total) ;

    pthread_mutex_unlock(&sumLock);
    if (globalMax < max)
    {
        pthread_mutex_lock(&maxLock);
        if (globalMax < max)
        {
            globalMax = max;
            globalXMax = xmax;
            globalYMax = ymax;
        }
        pthread_mutex_unlock(&maxLock);
    }

    if (globalMin > min)
    {
        pthread_mutex_lock(&minLock);
        if (globalMin > min)
        {
            globalMin = min;
            globalXMin = xmin;
            globalYMin = ymin;
        }
        pthread_mutex_unlock(&minLock);
    }

    /*Barrier();
    if (myid == 0)
    {
        max = maxes[0].value;
        maxX = maxes[0].x;
        maxY = maxes[0].y;

        min = mins[0].value;
        minX = mins[0].x;
        minY = mins[0].y;

        total = 0;
        for (i = 0; i < numWorkers; i++)
        {
            // global max
            if (max < maxes[i].value)
            {
                max = maxes[i].value;
                maxX = maxes[i].x;
                maxY = maxes[i].y;
            }
            // global min
            if (min > mins[i].value)
            {
                min = mins[i].value;
                minX = mins[i].x;
                minY = mins[i].y;

                //     printf("i am %d\n",  min);
                //   printf("x is  %d\n",  minX);
                // printf("y is %d\n",  minY);
            }

            total += sums[i];
        }

        end_time = read_timer();
     
        printf("The total is %d\n", total);

        printf("The max is %d appears first at [%d , %d]\n", max, maxX + 1, maxY + 1);
        printf("The min is %d appears first at [%d , %d]\n", min, minX + 1, minY + 1);

        printf("The execution time is %g sec\n", end_time - start_time);
    }  */

    /*   maxes[myid].x = maximumx;
    maxes[myid].y = maximumy;
    mins[myid].x = minimumx;
    mins[myid].y = minimumy;

    maxes[myid].value = maximum;
    mins[myid].value = minimum;

    sums[myid] = total;

    pthread_exit(NULL); */
}
