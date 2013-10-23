#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* The man pages and the following website were very helpful in quickly 
 * getting up to speed on pthreads:
 *
 * https://computing.llnl.gov/tutorials/pthreads/
 *
 * Compile like so:
 *
 * $ gcc -lpthread part1a.c -o part1a
 *
 */

/* Thread argument struct */
struct thread_args{

  int thread_id;
  unsigned long long int seed1;
  unsigned long long int seed2;
  int size; /* Number of fibonacci to crunch */
  unsigned long long int *array; /* Array dynamically allocated. Must pass its address */

};

void *fibonacciCruncher(void *threadArgs) {

  struct thread_args *local;
  local = (struct thread_args *) threadArgs;

  /* Don't really have to do this; just wanting to make sure we can make 
   * localized copies of the stuff in the struct
   */
  unsigned long long int seed1, seed2;
  seed1 = local->seed1;
  seed2 = local->seed2;
  local->array[0] = seed1;
  local->array[1] = seed2;

  printf("In thread %d crunching fibonacci sequence.\n", local->thread_id);

  int i;
  for( i=2 ; i < local->size ; i++) {
   local->array[i] = local->array[i-1] + local->array[i-2]; 
  } 

}

int main(int argc, char *argv[]) {

  /* Grab user input */
  int size;

  if (argc != 2) {
    printf("Invalid usage. Correct usage is `./part1a $n`, where $n is a positive integer greater than or equal to 2 and less than or equal to 94.\n");
    exit(-1);
  }
  else {
    size = atoi(argv[1]);
    if( size > 94 || size < 2) {
      printf("Supplied integer either less than 2 or greater than 94. Try again.\n");
      exit(-1);
    }
  }

  /* Allocate the array that will hold the fibonacci numbers. */
  unsigned long long int *array;
  array = malloc(size*sizeof(long long int));
  if (array==NULL) {
    printf("Failed to allocate memory for array.\n");
    exit(-1);
  }

  /* Initialize the array by filling with -1 */
  /* (If thread doesn't work, main should spit out -1 for all elements) */
  int i;
  for( i = 0 ; i < size ; i++) {
    array[i] = -1;
  }

  /* Make the thread arg struct */
  struct thread_args init;
  init.thread_id = 1337;
  init.seed1 = 0;
  init.seed2 = 1;
  init.size = size;
  init.array = array;

  printf("In main: creating fibthread.\n");

  /* Create the thread */
  pthread_t fibthread;
  int retval;
  retval = pthread_create(&fibthread, NULL, fibonacciCruncher, (void *) &init);
  if (retval) {
    printf("ERROR: pthread_create() returned %d\n", retval);
    exit(-1);
  }

  /* Wait for the thread to finish by joining main() and the thread */
  pthread_join(fibthread, NULL);

  /* The following for loop should not execute until the thread is done. */

  /* Cycle through the completed array */
  for( i=0 ; i < size ; i++) {

    printf("%llu\n",array[i]);

  }

}

