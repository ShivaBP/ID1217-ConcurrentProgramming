#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h>

#define MAXBEES 20
#define MAXHONEY 10
#define SHARED 1

void *bees(void *);
void *bear(void *);

sem_t *pot_is_available_for_more_honey;
sem_t *pot_is_full;
sem_t *pot_is_empty;

//global variables
int maxBees = MAXBEES;
int maxHoney = MAXHONEY;
int honeyPortion;

int main(int argc, char *argv[])
{
    int i;
    pthread_t bearThread;
    pthread_t beeThreads[maxBees ];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    honeyPortion = 0;
    pot_is_available_for_more_honey = sem_open("/dish_is_free", O_EXCL, S_IWOTH, 1);
    pot_is_full= sem_open("/empty_dish", O_EXCL, S_IWOTH, 0);
    pot_is_empty = sem_open("/dish_is_filled", O_EXCL, S_IWOTH, 0);

    pthread_create(&bearThread, &attr, bear, NULL);
    for (i = 0; i <= maxBees-1; i++)
    {
        pthread_create(&beeThreads, &attr, bees, (void *)i);
    }
    for (i = 0; i <= maxBees-1; i++)
    {
        pthread_join(beeThreads[i], NULL);
    }
}

void *bear(void *arg)
{
    while (true)
    {
         
        sem_wait(pot_is_full);
        sleep(rand() % 4);
        honeyPortion = 0;
        printf("The honey pot is now empty and ready to be filled again.\n");
        sem_post(pot_is_empty);
       
    }
}

void *bees(void *arg)
{
    int i;
    int fairnessCheck = 0;
    for ( i=1 ; i<= maxBees ; i++){
       // printf ("Bee %d starts filling the pot.\n", (int) arg);
        sem_wait(pot_is_available_for_more_honey);
        if(honeyPortion >= maxHoney){
            // printf ("Bee %d noticed the full pot when it has %d honey portions.\n", (int) arg, honeyPortion);
            printf("Bee %d awakes the bear.\n", (int)arg);
            sem_post(pot_is_full);
           sleep(rand() % 4);
            sem_wait(pot_is_empty);
        }
        else if(honeyPortion>= 0 && honeyPortion < MAXHONEY){
        
            honeyPortion ++ ;
            fairnessCheck ++ ;
            printf ("Bee %d added a honey portion and there are in total %d honey portions.\n", (int) arg, honeyPortion);
            printf ("Bee %d has added %d honey portions.\n", (int) arg, fairnessCheck);
            sem_post(pot_is_available_for_more_honey);
        }
// printf ("Bee %d is done filling the honey portion.\n", (int) arg);
    }
}