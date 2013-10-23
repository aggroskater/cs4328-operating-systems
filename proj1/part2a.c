#include <pthread.h> /* For pthreads */
#include <stdio.h> /* For printf */
#include <stdlib.h> /* For exit */

/* Constants as defined in the Project Description */
#define M 3
#define K 2
#define N 3
#define NUM_THREADS M*N

int A[M][K] = {{1,4},{2,5},{3,6}};
int B[K][N] = {{8,7,6},{5,4,3}};
int C[M][N] = {0}; /* Need to initialize to zero for this to work */

/* Thread argument struct */
struct thread_args {

  int thread_id;
  int row;
  int column;

};

/* Threaded code */
void *matrixCruncher(void* threadArgs) {

  struct thread_args *local;
  local = (struct thread_args *) threadArgs;

  int row, column;
  row = local->row;
  column = local->column; 

  printf("THREAD: In thread %d crunching C[%d][%d].\n",local->thread_id,row,column);

  int i;
  for( i=0 ; i < K ; i++ ) {
    C[row][column] += A[row][i] + B[i][column];
  }

  printf("THREAD: Exiting thread %d\n",local->thread_id);

}

int main(int argc, char *argv[]) {

  /* Create an array of structs. The structs hold each thread's args */
  struct thread_args init[M*N];

  /* Create an array of thread-types */
  pthread_t threads[M*N];
//  pthread_t *threads = malloc(sizeof(pthread_t)*(M*N));

  /* Give all threads same attributes. Namely, joinable */
//  pthread_attr_t attr;
//  pthread_attr_init(&attr);
//  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  /* Joinable by default (in Linux anyway). */

  int count,retval;
  count = 0;
  int i,j;
  for ( i=0 ; i < M ; i++ ) {

    for ( j=0 ; j < N ; j++ ) {

      /* Initialize the thread's data */
      init[count].thread_id = count;
      init[count].row = i;
      init[count].column = j;

      /* Create the thread */
      retval = pthread_create(&threads[count], NULL, matrixCruncher, 
        (void *) &init[count]);
      if (retval) {
        printf("MAIN: ERROR: pthread_create() returned %d\n",retval);
        exit(-1);
      }

      count++;

    }

  }

  printf("MAIN: Waiting for threads to finish.\n");

  /* Wait for the threads to finish */
    for ( i=0 ; i < M*N ; i++) {
    int ret;
    void* result;
    ret = pthread_join(threads[i],&result);
    if(ret) {
      printf("MAIN: ERROR: pthread_join() returned %d on thread_id %d\n",ret,i);
    }
  }

  printf("MAIN: Finished waiting on threads.\n");

  /* Output the final matrix */
  for ( i=0 ; i < M ; i++ ) {

    for ( j=0 ; j < N ; j++ ) {

      printf("%5d",C[i][j]);

    }

    printf("\n");

  }

}
