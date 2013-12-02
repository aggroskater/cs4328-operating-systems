#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>

/*
 *
 * Compile like so:
 *
 * $ gcc -lpthread -lrt -g part1.c -o part1
 *
 */

/*******************************/
/*                             */
/* LOL PREPROCESSOR DIRECTIVES */
/*                             */
/*******************************/

#define BUFFER_SIZE 10

/************************/
/*                      */
/* LOL GLOBAL VARIABLES */
/*                      */
/************************/

struct thread_args {

  int rand;

};

int buffer[BUFFER_SIZE] = {-1};
unsigned int produced = 0;
unsigned int consumed = 0;
int quitting_time = 0;

pthread_mutex_t mutex;
sem_t full;
sem_t empty;

/***************************/
/*                         */ 
/* LOL THREADED OPERATIONS */
/*                         */
/***************************/

/* This function is the glorious, job-making producer */
void *insert_item(void* thread_args) {

  struct thread_args *local;
  local = (struct thread_args *) thread_args;

  do {
    /* Get a random number */
    local->rand = rand() % 1000;    

    /* Acquire empty */
    sem_wait(&empty);

    /* Acquire mutex */
    pthread_mutex_lock(&mutex);

    /* Critical Section. Add to buffer.
     * (This modulo BUFFER_SIZE trick won't work forever. 
     * Eventually, produced and consumed would overflow.
     * Let's just pretend that can't happen.)
     * (Wait. I'm a moron. Nevermind that last comment.)
     */
    buffer[produced] = local->rand;
    printf("Producing buffer[%d] = %d\n", produced, buffer[produced]);
    produced = (produced + 1) % BUFFER_SIZE;

    /* Release mutex */
    pthread_mutex_unlock(&mutex);

    /* Update full */
    sem_post(&full); 
 
  } while(!quitting_time);

  /* quitting_time has been set by main. End the thread */
  pthread_exit(NULL);

}

/* This function is the filthy casual consumer */
void *remove_item(void* thread_args) {

  struct thread_args *local;
  local = (struct thread_args *) thread_args;

  do {
    /* Get a random number */
    local->rand = rand() % 1000;    

    /* Acquire full */
    sem_wait(&full);

    /* Acquire mutex */
    pthread_mutex_lock(&mutex);

    /* Critical Section. Add to buffer.
     * (This modulo BUFFER_SIZE trick won't work forever. 
     * Eventually, produced and consumed would overflow.
     * Let's just pretend that can't happen.)
     * (Wait. I'm a moron. Nevermind that last comment.)
     */
    buffer[consumed] = local->rand;
    printf(">>>>>Consuming buffer[%d] = %d<<<<<\n", consumed, buffer[consumed]);
    consumed = (consumed + 1) % BUFFER_SIZE;

    /* Release mutex */
    pthread_mutex_unlock(&mutex);

    /* Update empty */
    sem_post(&empty); 
 
  } while(!quitting_time);

  /* quitting_time has been set by main. End the thread */
  pthread_exit(NULL);

}

/* This thread does the sleep timing. This isn't perfect, but it 
 * should be close enough.
 */
void *timerThread(void* time_to_sleep) {

  int timer = (int) time_to_sleep;
  sleep(timer);
  quitting_time = 1;
  pthread_exit(NULL);

}

/*********************/
/*                   */
/* LOL MAIN FUNCTION */
/*                   */
/*********************/

int main(int argc, char *argv[]) {

  /* Seed the PRNG */
  srand(time(NULL));

  /* Command line parameters (time to sleep is global) */
  int time_to_sleep;
  int num_producers;
  int num_consumers;

  if (argc != 4) {
    printf("Usage: ./executable time_to_sleep number_of_producers number_of_consumers\n");
    printf("All numbers should be integers.\n");
    exit(1);
  }
  else {
    time_to_sleep = atoi(argv[1]);
    num_producers = atoi(argv[2]);
    num_consumers = atoi(argv[3]);
  }

  /* Set up the three semaphores: empty, full, and mutex.
   * 
   * These are declared GLOBALLY.
   * 
   * empty - Initialize to BUFFER_SIZE
   * full - Initialize to zero
   * mutex - Initialize to zero
   *
   */
  pthread_mutex_init(&mutex, NULL);
  sem_init(&full, 0, 0);
  sem_init(&empty, 0, BUFFER_SIZE);
 
  /* Set up the threads and their arguments. 
   * gotta figure out how many we need. 
   */ 
  int total_threads;
  total_threads = num_producers + num_consumers;
  pthread_t threads[total_threads];
  struct thread_args threadArgs[total_threads];

  /* Create the producers */
  int i;
  for (i=0 ; i < num_producers ; i++) {

    int ret;
    ret = pthread_create(&threads[i], NULL, insert_item, 
      (void *) &threadArgs[i]);

    if(ret) {
      printf("Error: pthread_create() returned %d\n", ret);
    }

  }

  /* Create the consumers */
  for (i=num_producers ; i < total_threads ; i++) {

    int ret;
    ret = pthread_create(&threads[i], NULL, remove_item, 
      (void *) &threadArgs[i]);

    if(ret) {
      printf("Error: pthread_create() returned %d\n", ret);
    }

  }

  /* Create the timer */
  int ret;
  pthread_t timer;
  ret = pthread_create(&timer, NULL, timerThread, (void *) &time_to_sleep);

  /* Wait for producers and consumers to finish */
  for (i=0 ; i < total_threads ; i++) {

    int ret;
    void* result;
    ret = pthread_join(threads[i], &result);

    if(ret) {
      printf("Error: pthread_join() returned %d\n", ret);
    }

    pthread_join(threads[i],NULL);

  }

  /* Sleep for N seconds */
//  sleep(time_to_sleep);
  
  /* Set global flag quitting_time to 1. Threads should exit. */
//  quitting_time = 1;

  printf("Done.\n");
  return 0;

}
