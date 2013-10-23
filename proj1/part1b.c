/* I don't know if this compiles or not. I based this largely off the 
 * slides for Chapter 4.
 */

#include <windows.h>
#include <stdio.h>

/* Global rray to hold the fibonacci numbers */
DWORD array[50] = {-1}; 

/* threaded function */
DWORD WINAPI fibonacci(LPVOID Param) {

  DWORD  limit = *(DWORD*)Param;

  DWORD i;
  for ( i = 2 ; i < limit ; i++ ) {
    array[i] = array[i-1] + array[i-2];
  }

  return 0;

}

int main(int argc, char *argv[]) {

  DWORD ThreadId;
  HANDLE ThreadHandle;
  int Param;

  /* Seed the array */
  array[0] = 0;
  array[1] = 1;

  /* Grab user input */
  if (argc != 2) {
    printf("An integer parameter is required. `./part1b $n` where $n is a positive integer greater than or equal to two and less than or equal to 47.\n");
    return -1;
  }

  Param = atoi(argv[1]);

  /* Validate input */
  if (Param < 2 || Param > 47) {
    printf("An integer >=2 or <=47 is required.\n");
    return -1;
  }
 
  /* Create the thread */ 
  ThreadHandle = CreateThread(
    NULL, /* Default security stuff */
    0, /* Default stack size */
    fibonacci, /* thread function */
    &Param, /* thread function parameter */
    0, /* Default creation flags */
    &ThreadId); /* Returns the thread identifier */

  /* Wait for it to finish */
  if (ThreadHandle != NULL) {
    WaitForSingleObject(ThreadHandle,INFINITE);
    CloseHandle(ThreadHandle);
  }

  /* Cycle through the finished array */
  DWORD i;
  for ( i=0 ; i < size ; i++) {
    printf("%d\n",array[i];
  }

  return 0;

}
