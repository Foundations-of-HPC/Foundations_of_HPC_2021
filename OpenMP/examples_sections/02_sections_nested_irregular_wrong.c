
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


#define N_default     20000 // how long is the main array
#define min_default   100   // the minimum argument to heavy_work_? functions
#define max_default   20000 // the maximum argument to heavy_work_? functions
#define chunk_default 5000  // the size of the small work chunks


#define NANO_PAUSE    100   // the sleeping time when checking for initialization
#define uSEC          1000  // a microsecond

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
#define PRINTFS(...) _Pragma("omp single")	\
  printf(__VA_ARGS__);
#else
#define PRINTF(...)
#define PRINTFS(...)
#endif
  
typedef unsigned int uint;
double heavy_work_0( uint );
double heavy_work_1( uint );
double heavy_work_2( uint );



int main( int argc, char **argv )
{

  int      N         = N_default;
  int      min_value = min_default;
  int      max_value = max_default;
  int      chunk     = chunk_default;
  int      nthreads  = 1;  
  
  struct  timespec ts;

  /*  -----------------------------------------------------------------------------
   *   initialize 
   *  -----------------------------------------------------------------------------
   */

  // check whether some arg has been passed on
  if ( argc > 1 )
    {
      N = atoi( *(argv+1) );
      if( argc > 2 )
	{
	  max_value = atoi( *(argv+2) );
	  if( argc > 3 )
	    chunk = atoi( *(argv+3) );
	}
    }

  srand48(1234321);
  double result = 0;

  int *array = (int*)malloc( N*sizeof(double) );
  
#if !defined(_OPENMP)
  
  printf("serial summation\n" );

  double tstart = CPU_TIME;
  
  // this mimic a stream of number you can not
  // initialize in parallel
  // note: that also means that data resides
  //       in one's thread DRAM  
  //
  for( int ii = 0; ii < N; ii++ )    
    array[ii] = min_value + lrand48() % max_value;     

  #if defined(DEBUG)  
  double partial_result1, partial_result2, partial_result3 = 0;
  for( int ii = 0; ii < N; ii++ )
    partial_result1 += heavy_work_0(array[ii]);

  for( int ii = 0; ii < N; ii++ )
    partial_result2 += heavy_work_1(array[ii]);
  
  for( int ii = 0; ii < N; ii++ )
    partial_result3 += heavy_work_2(array[ii]);

  result = partial_result1 + partial_result2 + partial_result3;

  double tend = CPU_TIME;
  
  printf("partial results are: %g %g %g\n", partial_result1, partial_result2, partial_result3 );    

  #else

  for( int ii = 0; ii < N; ii++ )
    result += heavy_work_0(array[ii]) +
      heavy_work_1(array[ii]) + heavy_work_2(array[ii]);

  double tend = CPU_TIME;
  #endif

#else
			   
  

  double tstart = CPU_TIME;
			   
  omp_set_num_threads( NTHREADS+1 );
#pragma omp parallel
#pragma omp single
  nthreads= omp_get_num_threads();
  
  PRINTF("omp summation with %d threads\n", nthreads );
  if ( nthreads < NTHREADS+1 )
    {
      printf("something odd happened: you did not get"
  	     " as many threads as requested (%d instead of %d)\n",
  	     nthreads, NTHREADS+1 );
    }
  
  // using the c11 feature, you may decleare this
  // array to be nthreads long; however, we want
  // to have a compile-time initialization because
  // its size is linked to the number of omp sections
  //
  int    semaphore = 0;

  // we impose a spread policy on the outer level
  //
#pragma omp parallel proc_bind(spread) shared(result) firstprivate(nthreads)
  {
    #pragma omp single
    {
      if( ! omp_get_nested() )
	printf(">>> WARNING: nesting is not active, so you'll loose a lot of speedup");
    }
      


    // inside each section then, we spawn a new region
    // requesting a close policy; the places should be
    // defined accordingly at the moment of running
    // the program
    //
#pragma omp sections reduction(+:result) firstprivate( chunk )
    {

      #pragma omp  section // "INITIALIZER"
      {	
	int idx   = 0;
	int first = 0;
	int last  = chunk;
        #if defined (MIMIC_SLOWER_INITIALIZATION)
	struct timespec nanot = {0, 200*uSEC};
	nanosleep( &nanot, NULL );
        #endif
        #if defined(DEBUG)
	struct timespec myts;
	double tstart = CPU_TIME_th;
	int    me     = omp_get_thread_num();
	#endif
	
	while( first < N )
	  {
	    last = (last >= N)?N:last;
	    for( int kk = first; kk < last; kk++, idx++ )
	      array[idx] = min_value + lrand48() % max_value;

	    #pragma omp atomic write
	    semaphore = last;

	    
	    PRINTF("* initializer (thread %d) : %g sec, initialized chunk from %d to %d\n",
		   me, CPU_TIME_th - tstart, first, last);
	    first += chunk;
	    last  += chunk;
	    #if defined (MIMIC_SLOWER_INITIALIZATION)
	    nanot.tv_nsec = 200*uSEC + lrand48() % 100*uSEC;
	    nanosleep( &nanot, NULL );
	    #endif
	  }

        #if defined(DEBUG)
	printf("* initializer thread: initialization lasted %g seconds\n", CPU_TIME_th - tstart );
	#endif

      }

      #pragma omp section // "SECTION 1"
      {
	
	int    last     = 0;
	double myresult = 0;

	const struct timespec nanot = {0, NANO_PAUSE};
        #pragma omp parallel reduction(+:myresult) shared(last) proc_bind(close)
	{
	  int inner_nthreads = omp_get_num_threads();
	  int me             = omp_get_thread_num();

	  PRINTFS("- \t section 1 :: %d threads active\n", inner_nthreads);
	  	  
	  while( last < N )
	    {
              #pragma omp single
	      {
		while( semaphore == last )
		  nanosleep(&nanot, NULL);	    
	      }

	      int my_chunk = (semaphore-last)/inner_nthreads;
	      int my_last  = my_chunk*(me+1);
	      int my_first = my_chunk*me;
	      my_last  = (my_last > N)? N : my_last;		  

	      PRINTFS("\t\tsection 1 :: processing from %d to %d\n", last, semaphore);
	      last = semaphore;	     

	      for( int ii = my_first; ii < my_last; ii++)
		myresult += heavy_work_0(array[ii]);	      
	    }
	}
	PRINTF("- \t section 1 :: result is %g\n", myresult);
	result += myresult;
      }  // close 2nd section


      #pragma omp section // "SECTION 2"
      {
	int    last     = 0;
	double myresult = 0;

	const struct timespec nanot = {0, NANO_PAUSE};
        #pragma omp parallel reduction(+:myresult) shared(last) proc_bind(close)
	{
	  int inner_nthreads = omp_get_num_threads();
	  int me             = omp_get_thread_num();

	  PRINTFS("- \t section 2 :: %d threads active\n", inner_nthreads);
	  	  
	  while( last < N )
	    {
              #pragma omp single
	      {
		while( semaphore == last )
		  nanosleep(&nanot, NULL);	    
	      }
	      
	      int my_chunk = (semaphore-last)/inner_nthreads;
	      int my_last  = my_chunk*(me+1);
	      int my_first = my_chunk*me;
	      my_last  = (my_last > N)? N : my_last;		  
	      
	      PRINTFS("\t\tsection 2 :: processing from %d to %d\n", last, semaphore);	      
	      last = semaphore;	      
	      for( int ii = my_first; ii < my_last; ii++)
		myresult += heavy_work_1(array[ii]);	      
	    }
	}
	PRINTF("- \t section 2 :: result is %g\n", myresult);
	result += myresult;
      }  // close 3rd section


      #pragma omp section // "SECTION 3"
      {
	int    last        = 0;
	double myresult    = 0;

	const struct timespec nanot = {0, NANO_PAUSE};
        #pragma omp parallel reduction(+:myresult) shared(last) proc_bind(close)
	{
	  int inner_nthreads = omp_get_num_threads();
	  int me             = omp_get_thread_num();
	  
	  PRINTFS("-\t section 3 :: %d threads active\n", inner_nthreads);

	  while( last < N )
	    {
              #pragma omp single
	      {
		while( semaphore == last )
		  nanosleep(&nanot, NULL);	    
	      }
	      
	      int my_chunk = (semaphore-last)/inner_nthreads;
	      int my_last  = my_chunk*(me+1);
	      int my_first = my_chunk*me;
	      my_last  = (my_last > N)? N : my_last;		  
	      
	      PRINTFS("\t\tsection 2 :: processing from %d to %d\n", last, semaphore);	      
	      last = semaphore;	      

	      for( int ii = my_first; ii < my_last; ii++)
		myresult += heavy_work_2(array[ii]);	      
	    }
	}
	PRINTF("- \t section 3 :: result is %g\n", myresult);
	result += myresult;
      }  // close 4th section

      
    } // close sections

  } // close parallel region


  double tend = CPU_TIME;
#endif
  


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
