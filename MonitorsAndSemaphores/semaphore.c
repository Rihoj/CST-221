#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>


sem_t mutex;
int feed = 0;
int daysGoneBy = 0;
pthread_t t1,t2;

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

void buyFeed()
{
    printf("Farmer \t\tFeed is low. Buying 5 Feed...\n");
    feed += 5;
}

void feedAnimals()
{
    printf("Farmer \t\tFeeding the chickens...\n");
    feed--;
    printf("Info \t\tRemaining Feed: %d\n", feed);
}


void* chicken(void* arg)
{
    //wait
    sem_wait(&mutex);
    printf("Chickens \tChickens Eating..\n");

    //critical section
    sleep(4);

    //signal
    sem_post(&mutex);
}

void releaseChickens()
{
    printf("Farmer \t\tReleasing the chickens...\n");
    pthread_create(&t2,NULL,chicken,NULL);
    printf("Info \t\tChickens are released...\n");
}

void putChickensUp()
{
    printf("Farmer \t\tPutting the chickens to bed...\n");
}

void* farmer(void* arg)
{
    newDay();
    printf("Farmer \t\tStarts his day...\n");
    //wait
    sem_wait(&mutex);
    //critical section
    releaseChickens();
    if (feed < 1) {
        buyFeed();
    }
    feedAnimals();
    //signal
    sem_post(&mutex);
    sleep(1);
    printf("Farmer \t\tDone Feeding the animals.\n");

    sem_wait(&mutex);
    putChickensUp();
    sem_post(&mutex);
    printf("Farmer \t\tGoes back to bed.\n\n");

}

int main()
{
    sem_init(&mutex, 0, 1);
    while(continueRunning()) {
        pthread_create(&t1,NULL,farmer,NULL);
        sleep(1);
        pthread_join(t1,NULL);
        pthread_join(t2,NULL);
    }
    sem_destroy(&mutex);
    return 0;
}
