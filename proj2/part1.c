#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphores.h>

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
int produced = 0;
int consumed = 0;

/***************************/
/*                         */ 
/* LOL THREADED OPERATIONS */
/*                         */
/***************************/

void *insert_item(void* thread_args) {

  struct thread_args *local;
  local = (struct thread_args *) thread_args;

  printf("Producing %d", local.rand);

}

void *remove_item(void* thread_args) {

  struct thread_args *local;
  local = (struct thread_args *) thread_args;

  printf("Consuming %d", local.rand);

}

/*********************/
/*                   */
/* LOL MAIN FUNCTION */
/*                   */
/*********************/

int main(int argc, char *argv[]) {

  /* Use three semaphores: empty, full, and mutex.
   * 
   * empty - Initialize to BUFFER_SIZE
   * full - Initialize to zero
   * mutex - Initialize to zero
   *
   */

}
