#ifndef _REENTRANT
#define _REENTRANT
#endif

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h>

#include <unistd.h>

#define MAXWORMS 5
#define MAXBIRDS 20
#define SHARED 1 // the semaphore is shared between forked processes

// defining procedures
void *children(void *);
void *parent(void *);

// defining semaphores
sem_t *dish_is_free;
sem_t *empty_dish;
sem_t *dish_is_filled;

// defining global variables
int maxWorms = MAXWORMS;
int maxBirds = MAXBIRDS;
int worms;

int main(int argc, char *argv[])
{
    int i;
    pthread_t parentThread;
    pthread_t babyThreads[maxBirds];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    // initializing the semaphores
    dish_is_free = sem_open("/dish_is_free", O_EXCL, S_IWOTH, 1);
    empty_dish = sem_open("/empty_dish", O_EXCL, S_IWOTH, 0);
    dish_is_filled = sem_open("/dish_is_filled", O_EXCL, S_IWOTH, 0);
    //initializing the global variable
    worms = maxWorms;

    //creating the threads
    pthread_create(&parentThread, &attr, parent, NULL);

    for (i = 0; i <= maxBirds - 1; i++)
    {
        pthread_create(&babyThreads[i], &attr, children, (void *)i);
    }

    for (i = 0; i <= maxBirds - 1; i++)
    {
        pthread_join(babyThreads[i], NULL);
    }

    // finishing off
    sem_close(dish_is_free);
    sem_close(empty_dish);
    sem_close(dish_is_filled);

    printf("main done\n");
}

void *parent(void *arg)
{
    while (true)
    {
        sem_wait(empty_dish);
        // printf("Parent received the empty dish signal.\n");
        sleep(rand() % 4);
        worms = maxWorms;
        //printf("boo\n" );
        printf("The dish is now filled with %d worms\n", worms);
        sem_post(dish_is_filled);
        //  printf("The full dish signal sent.\n");
        //  sleep();
    }
    pthread_exit(NULL);
}

void *children(void *arg)
{
    int i;
    int fairnessCheck = 0;
  //  for(i=0 ; i <= maxBirds-1 ; i++)
  while(true)
    {
      //  sleep(rand() % 4);
        //printf ("Baby bird %d starts eating.\n", (int) arg);
        sem_wait(dish_is_free);
        if (worms <= 0  )
        {
            printf("BABY BIRD %d CHRIPS.\n", (int)arg, worms);
            sem_post(empty_dish);
           sleep(rand() % 4);
            sem_wait(dish_is_filled);
        }

        // printf ("%d\n",worms);
        else if (worms> 0 && worms <= MAXWORMS) {
        worms--;
        fairnessCheck ++ ;
        printf("Baby bird %d ate a worm and there are %d worms left.\n", (int)arg, worms);
        printf("Baby bird %d has eaten %d worms.\n", (int)arg, fairnessCheck);
        sem_post(dish_is_free);
        }
        //usleep(3000000);
        //sleep(rand()%3);
        // printf ("Baby bird %d is done eating.\n", (int) arg);
    }

    pthread_exit(NULL);
}