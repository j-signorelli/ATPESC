
#include <stdio.h>
#include <omp.h>

int main()
{

  printf("I think");
  
  #pragma omp parallel
  {
     #pragma omp single
     {
        //int nthrds = omp_get_num_threads();
	//printf("Num Threads: %ld", nthrds);
	#pragma omp task
        printf(" car");

        #pragma omp task
        printf(" race");
     }
  }
  printf("s are fun\n");

}
