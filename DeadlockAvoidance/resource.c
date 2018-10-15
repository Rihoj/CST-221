/**
 * Author: James Ray
 * Solution: If the diff between the current time and when the process starts is greater than or equal to the wait time
 * we exit and start a new one.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

bool resourceInUse = false;
pthread_mutex_t mutex;
pthread_cond_t condc;
pthread_t p1, p2, p3;
int waitTime = 200;

void* processTwo(void* arg)
{
    clock_t start;
    int processNum = *((int *) arg);
    printf("Process %d Started\n", processNum);
    pthread_mutex_lock(&mutex);
    while (resourceInUse && (clock()-start) < waitTime) pthread_cond_wait(&condc, &mutex);
    if((clock()-start) >= waitTime){
        printf("Process %d Deadlocked. Restarting.\n", processNum);
        pthread_cond_signal(&condc);
        pthread_mutex_unlock(&mutex);
        pthread_create(&p3, 0,processTwo,arg);
        pthread_join(p3, 0);
    }else{
        resourceInUse=true;
        printf("Process %d gained access to resource.\n", processNum);
        sleep(10);
        resourceInUse=false;
        printf("Process %d done with resource.\n", processNum);
        pthread_cond_signal(&condc);
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
  //init mutex and conditions
  pthread_mutex_init(&mutex, 0);
  pthread_cond_init(&condc,0);
  int *p1arg = malloc(sizeof(*p1arg));
  int *p2arg = malloc(sizeof(*p2arg));
  if ( p1arg == NULL || p2arg == NULL ) {
      fprintf(stderr, "Couldn't allocate memory for threads.\n");
      exit(EXIT_FAILURE);
  }
  *p1arg = 1;
  *p2arg = 2;
  pthread_create(&p1, 0,processTwo,p1arg);
  pthread_create(&p2, 0,processTwo,p2arg);
  pthread_join(p1, 0);
  pthread_join(p2, 0);
  pthread_cond_destroy(&condc);
  pthread_mutex_destroy(&mutex);
  return 0;
}
