#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#define MAX 5;


pthread_mutex_t mutex;
pthread_cond_t condf, condc;

int feed = 0;
int daysGoneBy = 0;

void newDay()
{
    daysGoneBy++;
    printf("--- Wake up. Day: %d --- \n", daysGoneBy);
}

bool continueRunning()
{
    if (daysGoneBy < 5) {
        return true;
    }
    return false;
}


void* chicken(void* arg)
{
    int i;
    //wait
    for (i = 1; i <= 5; i++) {
        pthread_mutex_lock(&mutex);
        while (feed == 0) pthread_cond_wait(&condc, &mutex);
        printf("Chickens \tChickens Eating..\n");
        feed--;
        pthread_cond_signal(&condf);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

void* farmer(void* arg)
{
    int i;
    for (i = 1; i <= 5; i++) {
        pthread_mutex_lock(&mutex);
        while (feed != 0) pthread_cond_wait(&condf, &mutex);
        printf("Farmer \t\tFeed is low. Buying Feed...\n");
        feed++;
        pthread_cond_signal(&condc);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

int main()
{
    pthread_t farm, chick;
    //init mutex and conditions
    pthread_mutex_init(&mutex, 0);
    pthread_cond_init(&condf,0);
    pthread_cond_init(&condc,0);
    pthread_create(&farm,0,farmer,0);
    pthread_create(&chick,0,chicken,0);
    // Wait for termination
    pthread_join(farm,0);
    pthread_join(chick, 0);
    // destroy conditions and mutex
    pthread_cond_destroy(&condf);
    pthread_cond_destroy(&condc);
    pthread_mutex_destroy(&mutex);
    return 0;
}
