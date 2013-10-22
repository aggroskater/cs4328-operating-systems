#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main()
{
    	int count = 0, i;
	int const size = 100000;
	int myArray [size];
	double start_time, end_time;
	
	//initialize random number generator
	srand((unsigned)time(NULL)); 
	
	// Initialize the array using random numbers
	for (i = 0; i < size; i++)
		myArray[i] = rand() % 100;
	
	//Serial Code
	start_time = omp_get_wtime();
	for (i = 0; i < size; i++)
		if (myArray[i] == 99)
			count ++; 
	end_time = omp_get_wtime();
	
	printf ("The serial code indicates that there are %d 99s in the array.\n\n", count);	
	printf ("The serial code used %f seconds to complete the execution.\n\n", end_time - start_time);
	
	count = 0;
	
	//Please write your parallel OpenMP and Pthreads code. 
	// Record the execution time and calculate the speedup when using 2 threads, 4 threads and 8 threads.

	//**********Parallel Code Below (two seperate cpp files for OpenMP and Pthreads)****************


	return 0;

}