#include <pthread.h>
#include <stdio.h> /* for printf() */
#include <stdlib.h> /* for exit() */

void *fibonacciCruncher(void *threadId) {

  long id;
  id = (long)threadId;
  printf("Hello world from thread %d\n", id);
  pthread_exit(NULL);

}


int main(int argc, char *argv[]) {

  /* My local machine has 4 physical cores */
  pthread_t threads[4];
  int returnval;
 
  long i; 
  for(i = 0; i < 4 ; i++) {

    printf("In main: creating thread %d\n", i);
    returnval = pthread_create(&threads[i], NULL, fibonacciCruncher, (void *)i);

    /* Something went wrong during thread creation */
    if (returnval) {

      printf("ERROR: pthread_create() returned %d\n", returnval);
      exit(-1);

    } 

  }

  pthread_exit(NULL);

}

