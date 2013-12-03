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

int seats[SEATS+1] = {-1};
unsigned int wait_count;

/* available_ta is a CV that says whether TA is available. */
int available_ta;

/* available_seat is a CV */
int available_seat;




/* Gonna start thinking through the student threads' logic now... */
void *student(void* thread_args) {

  lock(mutex);

  /* No available seats. Come back later. */
  while ( wait_count == SEATS ) {

    printf("Student %d goes back to program.\n", local->id);
    sleep(1);

  }

//  lock(mutex);

  /* Seat available. See if TA is busy. The TA will be busy if he is 
   * helping someone. 
   */
  seats[wait_count] = local->id;  
  wait_count++;
  signal(student_waiting,mutex);

  unlock(mutex);

  wait(available_ta,mutex2);

//  wait(available_ta,mutex);

//  printf("The TA is helping student %d.\n", local->id);
  

}

/* And the teacher logic... */
void *teacher(void* thread_args) {

  lock(mutex);

  while ( wait_count == 0 ) {

    printf("The TA is sleeping.\n");
//    signal(available_ta,mutex);
    wait(student_waiting,mutex);

  }

//  lock(mutex);

  printf("The TA is woken up.\n");

  while ( wait_count != 0 ) {

    printf("The TA is helping student %d.\n", seats[--wait_count]);

  }

  unlock(mutex);

}
