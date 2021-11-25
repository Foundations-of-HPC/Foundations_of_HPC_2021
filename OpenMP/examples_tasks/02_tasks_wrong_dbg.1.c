
/* ────────────────────────────────────────────────────────────────────────── *
 │                                                                            │
 │ This file is part of the exercises for the Lectures on                     │
 │   "Foundations of High Performance Computing"                              │
 │ given at                                                                   │
 │   Master in HPC and                                                        │
 │   Master in Data Science and Scientific Computing                          │
 │ @ SISSA, ICTP and University of Trieste                                    │
 │                                                                            │
 │ contact: luca.tornatore@inaf.it                                            │
 │                                                                            │
 │     This is free software; you can redistribute it and/or modify           │
 │     it under the terms of the GNU General Public License as published by   │
 │     the Free Software Foundation; either version 3 of the License, or      │
 │     (at your option) any later version.                                    │
 │     This code is distributed in the hope that it will be useful,           │
 │     but WITHOUT ANY WARRANTY; without even the implied warranty of         │
 │     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          │
 │     GNU General Public License for more details.                           │
 │                                                                            │
 │     You should have received a copy of the GNU General Public License      │
 │     along with this program.  If not, see <http://www.gnu.org/licenses/>   │
 │                                                                            │
 * ────────────────────────────────────────────────────────────────────────── */


#if defined(__STDC__)
#  if (__STDC_VERSION__ >= 199901L)
#     define _XOPEN_SOURCE 700
#  endif
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <omp.h>


#define N_default      2000   // how long is the main array
#define max_default    2000   // the maximum argument to heavy_work_? functions

#if defined(_OPENMP)
#define CPU_TIME (clock_gettime( CLOCK_REALTIME, &ts ), (double)ts.tv_sec + \
		  (double)ts.tv_nsec * 1e-9)

#define CPU_TIME_th (clock_gettime( CLOCK_THREAD_CPUTIME_ID, &myts ), (double)myts.tv_sec +	\
		     (double)myts.tv_nsec * 1e-9)

#else

#define CPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec + \
		  (double)ts.tv_nsec * 1e-9)
#endif

#if !defined(NTHREADS)    // threads for the first level of parallelism
                          // this value should be equal to the number_of_sockets-1
#define NTHREADS 3
#endif



#if defined(DEBUG)
#define PRINTF(...) printf(__VA_ARGS__);
#else
#define PRINTF(...)
#endif
  
typedef unsigned int uint;
double heavy_work_0( uint );
double heavy_work_1( uint );
double heavy_work_2( uint );



int main( int argc, char **argv )
{

  int      N         = N_default;
  int      max_value = max_default;
  int      nthreads  = 1;
  
  struct  timespec ts, myts;

  /*  -----------------------------------------------------------------------------
   *   initialize 
   *  -----------------------------------------------------------------------------
   */

  // check whether some arg has been passed on
  if ( argc > 1 )
    {
      N = atoi( *(argv+1) );
      if( argc > 2 )
	max_value = atoi( *(argv+2) );
    }

  srand48(1234321);
  double result = 0;

  int *array = (int*)malloc( N*sizeof(double) );
  
  
  // this mimic a stream of number you can not
  // initialize in parallel
  // note: that also means that data resides
  //       in one's thread DRAM
  //
  for( int ii = 0; ii < N; ii++ )
    array[ii] = 100 + (lrand48() % max_value);

  
#if !defined(_OPENMP)
  
  printf("serial summation\n" );

  double tstart = CPU_TIME;
  
 #if defined(DEBUG)
  double result1 = 0, result2 = 0, result3 = 0;
  for( int ii = 0; ii < N; ii++ )
    result1 += heavy_work_0(array[ii]);

  for( int ii = 0; ii < N; ii++ )
    result2 += heavy_work_1(array[ii]);
  
  for( int ii = 0; ii < N; ii++ )
    result3 += heavy_work_2(array[ii]);

  printf("result1: %g, result2: %g, result3: %g\n",
	 result1, result2, result3 );
  result = result1 + result2 + result3;
 #else
  for( int ii = 0; ii < N; ii++ )
    result += heavy_work_0(array[ii]) +
      heavy_work_1(array[ii]) +
      heavy_work_2(array[ii]) ;
 #endif
  
 #else
			   
  printf("omp summation\n" );

  			    
  double tstart = CPU_TIME;
  
 #pragma omp parallel shared(result)
  {
   #if defined(DEBUG)
    int me = omp_get_thread_num();
   #endif
    double result1, result2, result3;

    PRINTF("-- Thread %d: %p %p %p\n", me,
	   &result1, &result2, &result3 );
    #pragma omp barrier
    
   #pragma omp single
    {
      PRINTF(" : Thread %d is generating the tasks\n", me);
      
     #pragma omp task
      {
	PRINTF(" + Thread %d is executing T1\n", omp_get_thread_num());
       	for( int jj = 0; jj < N; jj++ )
       	  result1 += heavy_work_0( array[jj] );
	PRINTF(" + Thread %d has got T1 = %g\n", omp_get_thread_num(), result1);
      }

     #pragma omp task
      {
	PRINTF(" + Thread %d is executing T2\n", omp_get_thread_num());
       	for( int jj = 0; jj < N; jj++ )
       	  result2 += heavy_work_1( array[jj] );
	PRINTF(" + Thread %d has got T2 = %g\n", omp_get_thread_num(), result2);
      }

     #pragma omp task
      {
	PRINTF(" + Thread %d is executing T3\n", omp_get_thread_num());
       	for( int jj = 0; jj < N; jj++ )
       	  result3 += heavy_work_2(array[jj] );
	PRINTF(" + Thread %d has got T3 = %g\n", omp_get_thread_num(), result3);
      }

    }

   #pragma omp barrier
    PRINTF("\tThread %d is here (%g %g %g)\n", me, result1, result2, result3 );
    
    #pragma omp atomic update
    result += result1;
    #pragma omp atomic update
    result += result2;
    #pragma omp atomic update
    result += result3;

    PRINTF("\tThread %d is here (%g)\n", me, result );
  }



#endif
  
  double tend = CPU_TIME;


  /*  -----------------------------------------------------------------------------
   *   finalize
   *  -----------------------------------------------------------------------------
   */

  free(array);
  
  printf("The result is %g\nrun took %g of wall-clock time\n\n",
	 result, tend - tstart );


return 0;
}



double heavy_work_0( uint N )
{
  double guess = 3.141572 / 3;
  
  for( int i = 0; i < N; i++ )
    {
      guess = exp( guess );
      guess = sin( guess );

    }

  return guess;
}

double heavy_work_1( uint N )
{
  double guess = 3.141572 / 3;

  for( int i = 0; i < N; i++ )
    {
      guess = log( guess );
      guess = exp( sqrt(guess)/guess );
    }

  return guess;
}

double heavy_work_2( uint N  )
{
  double guess = 3.141572 / 3;

  for( int i = 0; i < N; i++ )
    {
      guess = sqrt( guess );
      guess = exp( 1+1.0/guess );
    }

  return guess;
}
