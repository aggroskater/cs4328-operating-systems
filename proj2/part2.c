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

/************************/
/*                      */
/* LOL GLOBAL VARIABLES */
/*                      */
/************************/

struct thread_args {

  int id;

};

/* hallway_full is initialized to zero. */
sem_t hallway_full

/* hallway_empty is initialized to three, the number of seats in the hall.
 * Student threads will first check to see if the TA is busy or not. If s/he
 * is, then they will wait on this semaphore for an available slot.
 */
sem_t hallway_empty;


/* wait_count holds number of students currently waiting. */
unsigned int wait_count;

/* available_ta is a CV that says whether TA is available. */
int available_ta;

/* available_seat is a CV */
int available_seat;

/* Gonna start thinking through the student threads' logic now... */
void *student(void* thread_args) {

  lock(mutex);

  /* No available seats. Come back later. */
  while ( wait_count == 3 ) {

    printf("Student %d goes back to program.\n", local->id);
    wait(available_seat,mutex);

  }

  /* Ok, at least one seat is available. */
  while ( wait_count >= 0 && wait_count <= 2 ) {

    /* Is anyone waiting ? */
    while ( wait_count == 0 ) {
      wait_count++;
      signal(student_waiting,mutex);
      wait(available_ta,mutex);  
    }
    printf("Student %d waits for help in a chair.\n", local->id);
    wait_count++;
    signal(available_seat,mutex);
    wait(available_ta,mutex);

  }
  while ( wait_count == 0 ) {

    signal(available_seat,mutex);

  }

  unlock(mutex);

}

/* And the teacher logic... */
void *teacher(void* thread_args) {

  lock(mutex);

  while ( wait_count == 0 ) {

    printf("The TA is sleeping.\n");
    signal(available_ta,mutex);
    wait(student_waiting,mutex);

  }

  printf("The TA is woken up.\n"); 

  unlock(mutex);

}
