#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 *
 * Compile like so:
 *
 * $ gcc -lpthread part1.c -g -o part1
 *
 */

struct thread_args {

  int rand;

};

int buffer[10] = {-1};

void *producer(void* thread_args) {

  struct thread_args *local;
  local = (struct thread_args *) thread_args;

  printf("Producing %d", local.rand);

}

void *consumer(void* thread_args) {

  struct thread_args *local;
  local = (struct thread_args *) thread_args;

  printf("Consuming %d", local.rand);

}

int main(int argc, char *argv[]) {

}
