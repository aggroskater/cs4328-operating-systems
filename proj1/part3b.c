#include <pthread.h> /* For pthreads */
#include <stdio.h> /* printf */
#include <stdlib.h> /* exit, srand, rand */
#include <time.h> /* clock_t types, CLOCKS_PER_SEC constant */

/* Compile with the following:
 *
 * gcc -g -lpthread -lrt -fopenmp part3b.c -o part3b
 *
 */

#define NUM_THREADS 4

/* Thread struct */
struct thread_args {
  int counter;
  int* array;
  unsigned int start;
  unsigned int finish;
};

/* Threaded Code */
void *findThe99s(void* threadArgs) {

  struct thread_args *local;
  local = (struct thread_args *) threadArgs;

  unsigned int start,finish;
  start = local->start;
  finish = local->finish;

  for( start ; start < finish ; start++ ) {
    if (local->array[start] == 99) {
        local->counter++;
    } 
  }

}

int main(int argc, char *argv[]) {

  /* Allocate our array */
  unsigned int const size = 10000000; /* 10M */
  int *array = malloc(sizeof(unsigned int)*size);

  /* Seed the PRNG */
  srand((unsigned)time(NULL));

  /* Fill the array with random values */
  unsigned int i;
  for ( i = 0 ; i < size ; i++) {
    array[i] = rand() % 100;
  }

  /*****************************************/
  /*                                       */
  /* Figure out the number of 99s serially */
  /*                                       */
  /*****************************************/

  int countSerial;
  countSerial = 0;

  /* timers for serial */
  struct timespec startSerial, finishSerial;
  double elapsedSerial;

  /* Start timing */
  clock_gettime(CLOCK_MONOTONIC, &startSerial);

  /* Serial execution */
  for( i = 0; i < size ; i++) {
    if( array[i] == 99 ) {
      countSerial++;
    }
  }

  /* End timing */
  clock_gettime(CLOCK_MONOTONIC, &finishSerial);

  /* Crunch serial execution time */
  elapsedSerial = (finishSerial.tv_sec - startSerial.tv_sec);
  elapsedSerial += (finishSerial.tv_nsec - startSerial.tv_nsec) / 1000000000.0;

  /********************************************/
  /*                                          */
  /* Figure out the number of 99s with openMP */
  /*                                          */
  /********************************************/

  int countOMP;
  countOMP = 0;

  /* timers for openMP */
  struct timespec startOMP, finishOMP;
  double elapsedOMP;

  /* Start timing */
  clock_gettime(CLOCK_MONOTONIC, &startOMP);

  int n;
  #pragma omp parallel for private(n) num_threads(NUM_THREADS) reduction(+:countOMP)
  for ( n=0 ; n < size ; n++) {
    if ( array[n] == 99 ) {
      countOMP += 1;
    }
  }

  /* End timing */
  clock_gettime(CLOCK_MONOTONIC, &finishOMP);

  /* Crunch openMP execution time */
  elapsedOMP = (finishOMP.tv_sec - startOMP.tv_sec);
  elapsedOMP += (finishOMP.tv_nsec - startOMP.tv_nsec) / 1000000000.0 ;
  
  /**********************************************/
  /*                                            */
  /* Figure out the number of 99s with pthreads */
  /*                                            */
  /**********************************************/

  int countPthreads;
  countPthreads = 0;
 
  /* Figure out if a thread needs to handle left-overs. */ 
  /* I.e., size/NUM_THREADS isn't a whole number */
  unsigned int remainder;
  remainder = size % NUM_THREADS;

  /* Figure out the chunk size. I.e., the portion of the array 
   * each thread will work on.
   */
  unsigned int dividend;
  dividend = size / NUM_THREADS;

  /* Declare the threads */
  pthread_t threads[NUM_THREADS];

  /* Declare their argument structs */
  struct thread_args threadArgs[NUM_THREADS];

  /* timers for pthreads */
  struct timespec startPthreads, finishPthreads;
  double elapsedPthreads;

  /* Start timing ; Have to start timing here to include thread creation */
  clock_gettime(CLOCK_MONOTONIC, &startPthreads);

  /* Initialize the threads */
  int retval;
  for ( i = 0 ; i < NUM_THREADS ; i++) {

    threadArgs[i].counter = 0;
    threadArgs[i].array = array;
    threadArgs[i].start = i*dividend;
    threadArgs[i].finish = (i+1)*dividend;

    /* In case size/NUM_THREADS wasn't evenly divisible */
    if (i == NUM_THREADS -1 && remainder) {
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
    pthread_join(threads[i],NULL);
    if(ret) {
      printf("MAIN: ERROR: pthread_join() returned %d\n",ret);
    }
  }

  /* Threads are done. Crunch total occurences from threads.
   * We had each thread update a local counter to avoid 
   * mutexing, which slowed the threads down considerably.
   */
  for ( i = 0 ; i < NUM_THREADS ; i++) {
    countPthreads += threadArgs[i].counter;
  }

  /* Stop timing */
  clock_gettime(CLOCK_MONOTONIC, &finishPthreads);

  /* Crunch pthreads execution time */
  elapsedPthreads = (finishPthreads.tv_sec - startPthreads.tv_sec);
  elapsedPthreads += (finishPthreads.tv_nsec - startPthreads.tv_nsec) / 1000000000.0;

  /********************************/
  /*                              */ 
  /* Print out useful information */
  /*                              */
  /********************************/

  printf("Using %d threads.\n", NUM_THREADS);

  printf("Found %d 99s serially.\n", countSerial);
  printf("Found %d 99s with openMP.\n", countOMP);
  printf("Found %d 99s with pthreads.\n", countPthreads);
 
  printf("Took %f time serially.\n", elapsedSerial);
  printf("Took %f time with openMP.\n", elapsedOMP);
  printf("Took %f time with pthreads.\n", elapsedPthreads);

  printf("Pthreads speedup is %f.\n" , (double)(elapsedSerial/elapsedPthreads));
  printf("OMP speedup is %f.\n", (double)(elapsedSerial/elapsedOMP));

  return 0;

}
