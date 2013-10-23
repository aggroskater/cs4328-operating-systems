#include <pthread.h> /* For pthreads */
#include <stdio.h> /* printf */
#include <stdlib.h> /* exit, srand, rand */
#include <time.h> /* clock_t types, CLOCKS_PER_SEC constant */

#define NUM_THREADS 4

static int numberOfZeros;

/* Thread struct */
struct thread_args {
  int thread_id;
  pthread_mutex_t* locker;
  int* counter;
  int* array;
  int start;
  int finish;
};

/* Threaded Code */
void *findThe99s(void* threadArgs) {

  struct thread_args *local;
  local = (struct thread_args *) threadArgs;

  int start,finish;
  start = local->start;
  finish = local->finish;

  int* globalCount;
  globalCount = local->counter;

  pthread_mutex_t* locker;
  locker = local->locker;

  printf("THREAD: In thread %d.\n",local->thread_id);
  printf("Checking from %d to %d.\n",start,finish);

  for( start ; start < finish ; start++ ) {
    if (local->array[start] == 99) {
      pthread_mutex_lock(locker);
      *globalCount = *globalCount + 1;
      pthread_mutex_unlock(locker);
    } 
  }

}

int main(int argc, char *argv[]) {

  /* The static declaration should initialize this to zero. */
  /* But let's play it safe */
  numberOfZeros = 0;

  int count, i;
  count = 0;
  int const size = 100000; /* 100K */
  int array[size];

  /* Fill the array with random values */
  for ( i = 0 ; i < size ; i++) {
    array[i] = rand() % 100;
  }

  /* Figure out the number of 99s serially */
  int countSerial;
  countSerial = 0;

  for( i = 0; i < size ; i++) {
    if( array[i] == 99 ) {
      countSerial++;
    }
  }

  /* Now Perform data parallelism. */
  /* Each thread chews through a piece of the arrray. */ 
 
  /* Figure out if a thread needs to handle left-overs */ 
  int remainder;
  remainder = size % NUM_THREADS;

  int dividend;
  dividend = size / NUM_THREADS;

  /* Declare the threads */
  pthread_t threads[NUM_THREADS];

  /* Declare their argument structs */
  struct thread_args threadArgs[NUM_THREADS];

  /* Declare a mutex */
  pthread_mutex_t locker;
  pthread_mutex_init(&locker, NULL);

  /* Initialize the threads */
  int retval;
  for ( i = 0 ; i < NUM_THREADS ; i++) {

    /* We want each thread to have the address of the shared counter */
    /* And the same mutex */
    threadArgs[i].thread_id = i;
    threadArgs[i].locker = &locker;
    threadArgs[i].counter = &count;
    threadArgs[i].array = array;
    threadArgs[i].start = i*dividend;
    threadArgs[i].finish = (i+1)*dividend;
    if (remainder) {
      threadArgs[i].finish += remainder;
      remainder = 0;
    }

    /* Make the thread */
    retval = pthread_create(&threads[i], NULL, findThe99s,
      (void *) &threadArgs[i]);
    if(retval) {
      printf("MAIN: ERROR: pthread_create() returned %d\n",retval);
    }

  }

  /* Gotta wait for the threads to finish */
  for ( i=0 ; i < NUM_THREADS ; i++) {
    int ret;
    void* result;
    ret = pthread_join(threads[i],&result);
    if(ret) {
      printf("MAIN: ERROR: pthread_join() returned %d\n",ret);
    }
  }

  /* End with the count */
  printf("Found %d 99s concurrently.\n", count);
  printf("Found %d 99s serially.\n", countSerial);

}
