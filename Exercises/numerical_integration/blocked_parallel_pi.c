/*

This program will numerically compute the integral of

                  4/(1+x*x) 
				  
from 0 to 1.  The value of this integral is pi -- which 
is great since it gives us an easy way to check the answer.

The is the original sequential program.  It uses the timer
from the OpenMP runtime library

History: Written by Tim Mattson, 11/99.

*/
#include <stdio.h>
#include <omp.h>
#define NUM_THREADS 2

static long num_steps = 100000000;
double step;
int main ()
{
	  int i, nthreads;
	  double pi, sum[NUM_THREADS];
	  double start_time, run_time;
	  
	  step = 1.0/(double) num_steps;
	  
	  start_time = omp_get_wtime();
          omp_set_num_threads(NUM_THREADS);
	  #pragma omp parallel
	  {
		int i, id, numthrds, istart, iend;
	  	double x;
	  	
		id = omp_get_thread_num();
                numthrds = omp_get_num_threads();
		istart = id*(num_steps/numthrds);
		iend = (id + 1)*(num_steps/numthrds);
		if (id == (numthrds-1)) iend = num_steps;
		if (id == 0)
		{
			printf("Num threads: %ld \n", numthrds);
			nthreads = numthrds; // This is only done on thread 0 because we don't want race conditions!
			// On x86, we don't need ID == 0 as all threads are writing the same thing and x86 takes care of programmers
			// This DOES come at a COST!!
			// However, on ARM or IBM power, they do not allow this
		}
		
		// BLOCKED DISTRIBUTION: MUCH BETTER!!
		
		for (i=istart, sum[id]=0; i < iend; i++)
		{
			x = (i-0.5)*step;
		  	sum[id] += 4.0/(1.0+x*x);
	       	}
	}

	  for (i=0, pi=0.0; i<nthreads; i++) pi += sum[i]*step;
	  run_time = omp_get_wtime() - start_time;
	  printf("\n pi with %ld steps is %lf in %lf seconds\n ",num_steps,pi,run_time);
}	  





