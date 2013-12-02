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

  /* Gonna try doing this to fix race condition */
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

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
 
  } while(1);

}

/* This function is the filthy casual consumer */
void *remove_item(void* thread_args) {

  struct thread_args *local;
  local = (struct thread_args *) thread_args;

  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

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
 
  } while(1);

}

/*********************/
/*                   */
/* LOL MAIN FUNCTION */
/*                   */
/*********************/

int main(int argc, char *argv[]) {

  /* Seed the PRNG */
  srand(time(NULL));

  /* Command line parameters */
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

    printf("Time to sleep: %d\n", time_to_sleep);
    printf("Number of producers: %d\n", num_producers);
    printf("Number of consumers: %d\n", num_consumers);
    printf("\n\n PREPARE YOUR SCREEN BUFFER \n\n");
    sleep(3);
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

  /* The threads are running. Sleep for user-specified time */
  sleep(time_to_sleep);

  /* Since our threads are set to cancel asyncrhonously, we can 
   * simply do a pthread_cancel(). This is cheap and bad practice, 
   * since there is no cleanup. But the project description doesn't 
   * require cleanup :D And it's two in the morning. And I'm tired.
   */
  for (i=0 ; i < total_threads ; i++) {

    int ret;
    ret = pthread_cancel(threads[i]);

    if(ret) {
      printf("Error: pthread_cancel() returned %d\n", ret);
    }

  }

  printf("Done.\n");
  return 0;

  /* Note: This code does not cleanly shut down. The threads are 
   * simply told to quit and the buffer is not free()'d. But, since 
   * the assignment didn't require cleanup, it isn't here.
   */

}
