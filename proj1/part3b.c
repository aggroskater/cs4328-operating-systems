#include <pthread.h> /* For pthreads */
#include <stdio.h> /* printf */
#include <stdlib.h> /* exit, srand, rand */
#include <time.h> /* clock_t types, CLOCKS_PER_SEC constant */

#define NUM_THREADS 4

/* Thread struct */
struct thread_args {
//  int thread_id;
//  pthread_mutex_t* locker;
//  int* counter;
  int counter;
  int* array;
  unsigned int start;
  unsigned int finish;
//  double time_spent;
};

/* Threaded Code */
void *findThe99s(void* threadArgs) {

  struct thread_args *local;
  local = (struct thread_args *) threadArgs;

  unsigned int start,finish;
  start = local->start;
  finish = local->finish;

//  int* globalCount;
//  globalCount = local->counter;

//  pthread_mutex_t* locker;
//  locker = local->locker;

//  printf("THREAD: In thread %d.\n",local->thread_id);
//  printf("Checking from %llu to %llu.\n",start,finish);

//  clock_t begin,end;
//  begin = clock();
  for( start ; start < finish ; start++ ) {
    if (local->array[start] == 99) {
//      pthread_mutex_lock(locker);
//      *globalCount = *globalCount + 1;
//      pthread_mutex_unlock(locker);
//        local->counter = local->counter + 1;
        local->counter++;
    } 
  }
//  end = clock();
//  local->time_spent = ((double)(end-begin))/CLOCKS_PER_SEC;

}

int main(int argc, char *argv[]) {

  int count;
  count = 0;
  unsigned int const size = 10000000; /* 1M */
  int *array = malloc(sizeof(unsigned int)*size);

  unsigned int i;
  /* Fill the array with random values */
  srand((unsigned)time(NULL));
  for ( i = 0 ; i < size ; i++) {
    array[i] = rand() % 100;
  }

  /* Figure out the number of 99s serially */
  int countSerial;
  countSerial = 0;

//  clock_t serialStart;
//  clock_t serialEnd;
//  double serialTime;

  struct timespec start, finish;
  double elapsed;

//  serialStart = clock(); 
  clock_gettime(CLOCK_MONOTONIC, &start);
  for( i = 0; i < size ; i++) {
    if( array[i] == 99 ) {
      countSerial++;
    }
  }
  clock_gettime(CLOCK_MONOTONIC, &finish);
//  serialEnd = clock();

//  serialTime = ((double)(serialEnd - serialStart)) / CLOCKS_PER_SEC;

  elapsed = (finish.tv_sec - start.tv_sec);
  elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

  /* Do it with openmp */

  double omp_start, omp_end;
  int countOMP;
  countOMP = 0;

  struct timespec start3, finish3;
  double elapsed3;

  clock_gettime(CLOCK_MONOTONIC, &start3);
//  omp_start = omp_get_wtime();
  int n;
  #pragma omp parallel for private(n) num_threads(4) reduction(+:countOMP)
  for ( n=0 ; n < size ; n++) {
    if ( array[n] == 99 ) {
      countOMP += 1;
    }
  }
//  omp_end = omp_get_wtime();
  clock_gettime(CLOCK_MONOTONIC, &finish3);
  elapsed3 = (finish3.tv_sec - start3.tv_sec);
  elapsed3 += (finish3.tv_nsec - start3.tv_nsec) / 1000000000.0 ;
  

  /* Now Perform data parallelism. */
  /* Each thread chews through a piece of the arrray. */ 
 
  /* Figure out if a thread needs to handle left-overs */ 
  unsigned int remainder;
  remainder = size % NUM_THREADS;

  unsigned int dividend;
  dividend = size / NUM_THREADS;

  /* Declare the threads */
  pthread_t threads[NUM_THREADS];

  /* Declare their argument structs */
  struct thread_args threadArgs[NUM_THREADS];

  /* Declare a mutex */
  pthread_mutex_t locker;
  pthread_mutex_init(&locker, NULL);

  /* testing something */
//  clock_t testStart, testEnd;
//  double testTime;

//  testStart = clock();

  struct timespec start2, finish2;
  double elapsed2;
  clock_gettime(CLOCK_MONOTONIC, &start2);
  /* Initialize the threads */
  int retval;
  for ( i = 0 ; i < NUM_THREADS ; i++) {

    /* We want each thread to have the address of the shared counter */
    /* and the address of the (single) mutex to protect access to counter. */
//    threadArgs[i].thread_id = i;
//    threadArgs[i].locker = &locker;
//    threadArgs[i].counter = &count;
    threadArgs[i].counter = 0;
    threadArgs[i].array = array;
    threadArgs[i].start = i*dividend;
    threadArgs[i].finish = (i+1)*dividend;
//    threadArgs[i].time_spent = 0;
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
    //int ret;
    //void* result;
    //ret = pthread_join(threads[i],&result);
    pthread_join(threads[i],NULL);
    //if(ret) {
    //  printf("MAIN: ERROR: pthread_join() returned %d\n",ret);
    //}
  }
//  testEnd = clock();
//  testTime = ((double)(testEnd-testStart))/ CLOCKS_PER_SEC;

  clock_gettime(CLOCK_MONOTONIC, &finish2);
  elapsed2 = (finish2.tv_sec - start2.tv_sec);
  elapsed2 += (finish2.tv_nsec - start2.tv_nsec) / 1000000000.0;

  /* Crunch total time with threads */
  double concurrentTime;
  concurrentTime = 0;
//  for ( i = 0 ; i < NUM_THREADS ; i++) {
//    concurrentTime += threadArgs[i].time_spent;
//  }

  count = 0;
  /* Crunch total occurences from threads */
  for ( i = 0 ; i < NUM_THREADS ; i++) {
    count += threadArgs[i].counter;
  }

  /* End with the count */
  printf("Found %d 99s concurrently.\n", count);
  printf("Found %d 99s serially.\n", countSerial);
  printf("Found %d 99s with OMP.\n", countOMP);

  printf("Took %f time concurrently.\n", concurrentTime);
//  printf("Took %f time serially.\n", serialTime);
  printf("Took %f time serially.\n", elapsed);
//  printf("Took %f time test.\n", testTime);
  printf("Took %f time test.\n", elapsed2);
  printf("Speedup is %f.\n" , (double)(elapsed/elapsed2));

  printf("OMP time is %f.\n", elapsed3);
  printf("OMP speedup is %f.\n", (double)(elapsed/elapsed3));

}
