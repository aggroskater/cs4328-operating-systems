#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Thread argument struct */
struct thread_args{

  int thread_id;
  int seed1;
  int seed2;
  int size; /* Number of fibonacci to crunch */
  int *array; /* Array dynamically allocated. Must pass its address */

};

void *fibonacciCruncher(void *threadArgs) {

  struct thread_args *local;
  local = (struct thread_args *) threadArgs;

  /* Don't really have to do this; just wanting to make sure we can make 
   * localized copies of the stuff in the struct
   */
  int seed1, seed2;
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

  /* Make the fibonacci cruncher thread */
  pthread_t fibthread;
  int retval;

  /* Allocate the array */
  int *array;
  array = malloc(100*sizeof(int));
  if (array==NULL) {
    printf("Failed to allocate memory for array.\n");
    exit(-1);
  }

  /* Initialize the array by filling with -1 */
  int i;
  for( i = 0 ; i < 100 ; i++) {
    array[i] = -1;
  }

  /* Make the thread arg struct */
  struct thread_args init;
  init.threadid = 1337;
  init.seed1 = 0;
  init.seed2 = 1;
  init.size = 100;
  init.array = array;

  printf("In main: creating fibthread.\n");

  /* Create the thread */
  retval = pthread_create(&fibthread, NULL, fibonacciCruncher, (void *) &init);
  if (retval) {
    printf("ERROR: pthread_create() returned %d\n", retval);
    exit(-1);
  }

  /* Wait for the thread to finish by joining main() and the thread */
  pthread_join(fibthread, NULL);

  /* The following for loop should not execute until the thread is done. */

  /* Cycle through the completed array */
  for( i=0 ; i < 100 ; i++) {

    printf("%d ",array[i]);

  }

}

