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

/* Gonna start thinking through the student threads' logic now... */
void *student(void* thread_args) {

  

}

/* And the teacher logic... */
void *teacher(void* thread_args) {



}
