/* This is supposed to be done on Windoze. Lol. */

/* Well, I've got some time before class. Maybe I could knock this out */

/* Gonna work off the class slides first. */

#include <windows.h>
#include <stdio.h>

/* Need to make a thread args struct somehow... */

/* threaded function */
DWORD WINAPI fibonacci(LPVOID Param) {

  /* need to figure out how to declare local struct ptr and cast Param */
  local;

  for (DWORD i = 2 ; i < local->size ; i++ ) {
    local->array[i] = local->array[i-1] + local->array[i-2];
  }

}

int main(int argc, char *argv[]) {

  DWORD ThreadId;
  HANDLE ThreadHandle;
  
  struct thread_args;

  ThreadHandle = CreateThread(
    NULL,
    0,
    &thread_args,
    0,
    &ThreadId);

  if (ThreadHandle != NULL) {
    WaitForSingleObject(ThreadHandle,INFINITE);
    CloseHandle(ThreadHandle);
  }

  /* Cycle through the array */

}
