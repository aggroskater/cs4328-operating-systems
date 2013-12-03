#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 *
 * Compile like so:
 *
 * $ gcc -lpthread -lrt -g part2.c -o part2
 *
 */

#define SEATS 3

/************************/
/*                      */
/* LOL GLOBAL VARIABLES */
/*                      */
/************************/

struct thread_args {

  int id;

};

/* Keeping track of how many people need help */
unsigned int wait_count = 0;

/*****************************/
/* Threads */
/*****************************/

/* Gonna start thinking through the student threads' logic now... */
void *student(void* thread_args) {

//shitty idea below

  lock(mutex);

  /* All seats taken. Come back later. */
  while ( wait_count == SEATS ) {

    printf("Student %d goes back to program.\n", local->id);
    wait(spot_available,mutex);

  }

  /* You're next! */
  while ( wait_count == 0 ) {

    wait_count++;
    wait(next,mutex);
    printf("The TA is helping student %d.\n", local->id);
    wait_count--;
    signal(spot_available);
    unlock(mutex);

  }

  /* You're in line */
  while ( wait_count > 0 && wait_count < SEATS ) {

    printf("Student %d waits for help in a chair.\n", local->id);
    wait_count++;
    wait(ta_available,mutex);

  }

/*********************************************************************/

//New ideas below

start here;

  lock(mutex);

  while ( QUEUE_FULL ) {

    printf("Student coming back later.\n");
    wait(QUEUE_NOT_FULL,mutex);

  }

  while ( QUEUE_NOT_FULL ) {

    if ( QUEUE_EMPTY ) {
      ADD_TO_QUEUE;
      signal(QUEUE_NOT_EMPTY);
      unlock(mutex);
    }
    else {
      ADD_TO_QUEUE;
      signal(ADDED_ITEM);
      unlock(mutex);
    }

    wait(PROCESSED,mutex);

  }

  unlock(mutex);

}

/* And the teacher logic... */
void *teacher(void* thread_args) {

start here;

  lock(mutex);

  while ( QUEUE_EMPTY ) {

    printf("TA sleeps.\n");
    wait(QUEUE_NOT_EMPTY,mutex);
    printf("TA awakens.\n");

  } 

  while ( QUEUE_NOT_EMPTY ) {

    if ( QUEUE_FULL ) {
      CHEW_THROUGH_ITEM;
      signal(QUEUE_NOT_FULL);
      unlock(mutex);
    }
    else {
      CHEW_THROUGH_ITEM;
      signal(PROCESSED);
      unlock(mutex);
    }

    wait(ADDED_ITEM,mutex);

  }

go back to start here;

}
