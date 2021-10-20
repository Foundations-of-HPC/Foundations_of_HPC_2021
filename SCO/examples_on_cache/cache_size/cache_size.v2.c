/* =============================================================================
 * This file is part of the exercises for the Lectures on 
 *   "Foundations of High Performance Computing"
 * given at 
 *   Master in HPC and 
 *   Master in Data Science and Scientific Computing
 * @ SISSA, ICTP and University of Trieste
 *
 * contact: luca.tornatore@inaf.it
 *
 *     This is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 3 of the License, or
 *     (at your option) any later version.
 *     This code is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License 
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#if defined(__STDC__)
#  if (__STDC_VERSION__ >= 201112L)    // c11
#    define _XOPEN_SOURCE 700
#  elif (__STDC_VERSION__ >= 199901L)  // c99
#    define _XOPEN_SOURCE 600
#  else
#    define _XOPEN_SOURCE 500          // c90
#  endif
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if _XOPEN_SOURCE >= 600
#  include <strings.h>
#endif
#include <time.h>

#include "mypapi.h"

#define CPU_TIME ({ struct timespec ts; double t; clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ); \
      t = (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9; t;})


typedef unsigned long long int ull;

#define NITER    10
#define MAX_STRIDE 64
#define WORD     sizeof(ull)

#include "mycache.h"
#define STEPSIZE (L1WORDS/8)


void insert( double *, double, int, int);
__attribute__((optimize(0))) double estimate_timing_overhead( );
__attribute__((optimize(0))) double estimate_loop0_toverhead(int, int, int, double );
__attribute__((optimize(0))) void   estimate_loop0_poverhead(int, int, int, ull *);
__attribute__((optimize(0))) ull wipe_cache( ull *, int );


int main(int argc, char **argv)
{
  int verbose  = (argc > 1 ? atoi(*(argv+1)) : 0 );
  int maxsize  = 2*L3WORDS;
  ull *array   = (ull*) calloc(maxsize, sizeof(ull));
  double overhead_t;
  
  
  PAPI_INIT;

  int n_sizesteps = 0;
  for ( ; (STEPSIZE << n_sizesteps) <= maxsize; n_sizesteps++ );
  int n_strides = 0;
  for( ; (1 << n_strides) <= MAX_STRIDE; n_strides++ );

  overhead_t = estimate_timing_overhead();
  if( verbose )
    {
      printf("timing function overhead is : %g nsec\n",
	     overhead_t * 1e9 );      
      printf("\n");
      
      printf("using %d steps for data size, from %lu to %d\n"
	     "using %d steps for stride, from %d to %d\n",
	     n_sizesteps, STEPSIZE, maxsize,
	     n_strides, 1, MAX_STRIDE );
      
      printf("-----------------------------------------------------\n\n");
    }
  
  
  double      *timings = (double*)calloc( n_sizesteps * n_strides, sizeof(double) );
  long double *results = (long double*)calloc( n_sizesteps * n_strides * PAPI_EVENTS_NUM, sizeof(long double) );

  FILE *file;
 #if defined(USE_PAPI)
  file = fopen("output.papi.dat", "w");
 #else
  file = fopen("output.dat", "w");
 #endif

  
  for ( int i = n_sizesteps-1; i >= 0; i-- )
    {
      int size = STEPSIZE << i;
      int this_n_strides = n_strides;
      while ( (this_n_strides >= 0) && ((size / (1 << this_n_strides)) <= 32) )
	this_n_strides--;

      printf("loops for size %d*%lu, max stride %d, %4.3g in L1 capacity units, %4.3g in L3 capacity units (step %d / %d )\n",
	     size, sizeof(ull), 1 << (this_n_strides-1), (double)size*WORD/L1SIZE, (double)size*WORD/L3SIZE, i+1, n_sizesteps );

      fprintf(file, "\"size ");
      double Ldsizes[3] = {(double)size/L1WORDS, (double)size/L2WORDS, (double)size/L3WORDS};
      for( int l = 0; l < 3; l++ ) {
	if( Ldsizes[l] < 0.125 )
	  fprintf( file, "<0.125L%d ", l+1);
	else
	  fprintf( file, "%4.1fL%d ", Ldsizes[l], l+1); }
      fprintf(file, "\"\n");
      //wipe_cache( alt_array, L3WORDS );
      
      for ( int stride_count = 0; stride_count < this_n_strides; stride_count++ )
	{

	  // set-up local variables
	  //
	 #define N_TIMINGS 10
	  double      elapsed[N_TIMINGS] = {0};                      // this will store the best 10 timings for the inner loop
	  double      tot_elapsed        = 0;	                     // this will store the total time spent in iterating the inner loop
	  int         repetitions        = 0;	                     // how many times the inner loop is performed
	  int         stride             = 1 << stride_count;        // the current stride
	 #if defined(USE_PAPI)    	  
	  long double values[PAPI_EVENTS_NUM] = {0.0};               // the accumulators for PAPI events
	 #endif

	  // print some useful data
	  //
	  printf("\tloops with stride %d.. ", stride ); fflush(stdout);

	  // estimate the loop timing overhed
	  //	  
	  double overhead_loop0_t = estimate_loop0_toverhead( NITER, size, stride, overhead_t);
	  if ( verbose )
	    printf("loop timing overhead is %5.4g sec.. ", overhead_loop0_t );
	  
	 #if defined(USE_PAPI)
	  // estimate the loop events overhead
	  //
	  ull overhead_loop0_p[PAPI_EVENTS_NUM];
	  estimate_loop0_poverhead( NITER, (1024), 1, overhead_loop0_p);
	  if( verbose ) {
	    printf("loop event ovrh is " );
	    for( int i = 0; i < PAPI_EVENTS_NUM; i++ )
	      printf("%llu ", overhead_loop0_p[i]);
	    printf(" | "); fflush(stdout);}
	 #endif

	  // repeat the inner loop until either at least 200 repetitions have been performed
	  // ot at least one second has elapsed
	  //
	  while( (repetitions < 200 ) || ((tot_elapsed < 1.0) && (repetitions >= 200)) )
	    {
	      // warm-up the cache
	      //
	      for ( int k = 0; k < size; k++ )
		array[k] = k;

	      // prepare variables used for the unrolling
	      //
	      int off2 = stride + stride;
	      int off3 = off2 + stride;
	      int off4 = off2 + off2;
	      int end = size - off4;

	      // take the initial time
	      //
	      double tstart = CPU_TIME;

	      // the inner loop: for NITER times it traverses the array
	      // with a given stride
	      //
	      for ( int j = 0; j < NITER; j++ )
		{
		  ull acc0 = 0, acc1 = 0, acc2 = 0, acc3 = 0;
		  PAPI_FLUSH;                                      // emtpy the counters
		  PAPI_START_CNTR;                                 // start the counters
		  for ( int k = 0; k < end; k += off4 ) {
		    acc0 += array[k];
		    acc1 += array[k+stride];
		    acc2 += array[k+off2];
		    acc3 += array[k+off3]; }
		  for ( int k = end; k < size; k++ )
		    acc0 += array[k];
		  PAPI_STOP_CNTR;                                  // stop the counters
		 #if defined(USE_PAPI)
		  if ( j > 0 )                                     // accumulate the counters
		    for ( int k = 0; k < PAPI_EVENTS_NUM; k++ )
		      if ( PAPI_GET_CNTR(k) > overhead_loop0_p[k] ) {
			if( k == 3 && (PAPI_GET_CNTR(k) > PAPI_GET_CNTR(k-1)*2) )
			  // L2_DCM counter exhibits some glitch, let's wipe it out
			  continue;
			long double v = (long double)(PAPI_GET_CNTR(k) - overhead_loop0_p[k]);
			v /= (size/stride);
			v = ( k > 1 ? v/8 : v );
			values[k] += v; }
		 #endif
		}

	      // take the time after the inner loop
	      double this_time = CPU_TIME - tstart;

	      // accumulate the total time spent in the inner loop
	      tot_elapsed += this_time;

	      // correct for the overheads
	      this_time = (this_time  - (overhead_t + overhead_loop0_t))/NITER;
	      if ( this_time > 0 )
		// if we are not at the resolution limit, insert the
		// current timing in the 10-best array
		//
		insert( elapsed, this_time, N_TIMINGS, repetitions );
	      
	      repetitions++;
	    }

	  // average the 10 best timings to estimate the inner loop timing
	  double time_elapsed = 0;
	  for( int k = 0; k < N_TIMINGS; k++ )
	    time_elapsed += elapsed[k];

	  // get the timing per processed element
	  double n_el = (double)(size/stride);
	  double t_per_el = time_elapsed/N_TIMINGS/ n_el;
	  printf(" elapsed time is %g sec (%d repetitions, ~%g sec/element)\n",
		 tot_elapsed, repetitions, t_per_el );
	  
	  timings[ i*n_strides + stride_count ] = t_per_el;
	  
	 #if defined(USE_PAPI)
	  // get the counters values
	  for ( int k = 0; k < PAPI_EVENTS_NUM; k++ )
	    results[ (i*n_strides + stride_count)*PAPI_EVENTS_NUM + k ] = values[k]/repetitions;
	 #endif	  
	}    // closes the loop over the strides

      // write relevant informations on the output file
      //
      for( int stride_count = 0; stride_count < n_strides; stride_count++ ) {
	fprintf(file, "%d %.3g ", 1<<stride_count, timings[i*n_strides + stride_count]);
       #if defined(USE_PAPI)
	for( int k = 0; k < PAPI_EVENTS_NUM; k++ )
	  fprintf(file, "%.4Lg ", results[(i*n_strides + stride_count)*PAPI_EVENTS_NUM+k]);
       #endif
	fprintf(file, "\n"); }
      
      fprintf(file, "\n\n");
      
    }      // closes the loop over the sizes
  
  fclose(file);
  
  
  free( results );
  free( timings );
  //free( alt_array );
  free( array );
  
  
  return 0;
}


void insert( double *elapsed, double t, int N, int r)
/*
 * a simple insertion sort
 */
{
  int last;
  if( r < N ) {
    elapsed[r] = t;
    last = r-1; }
  else
    last = N-1;
  
  if( elapsed[last] > t )
    {
      if( r < N )
	elapsed[r] = elapsed[last];
      elapsed[last] = t;
      last--;
      while( (last >= 0) && (elapsed[last] > elapsed[last+1]) )
	{
	  double _t_ = elapsed[last];
	  elapsed[last] = elapsed[last+1];
	  elapsed[last+1] = _t_;
	  last--;
	}
    }

  return;
}
  

__attribute__((optimize(0))) double estimate_timing_overhead( )
{
 #define MANY_ITER 125000
  double tstart = CPU_TIME;
  for( unsigned int j = 0; j < MANY_ITER; j++ )
    {
      double t0 = CPU_TIME;
      double t1 = CPU_TIME;
      double t2 = CPU_TIME;
      double t3 = CPU_TIME;
      t0 = CPU_TIME;
      t1 = CPU_TIME;
      t2 = CPU_TIME;
      t3 = CPU_TIME;
    }
  double overhead1 = (CPU_TIME - tstart)/(MANY_ITER*8+2);

  double overhead2 = 0;
  for( unsigned int j = 0; j < MANY_ITER; j++ )
    {
      double t0 = CPU_TIME;
      double t1 = CPU_TIME;
      overhead2 += t1-t0;
      t0 = CPU_TIME;
      t1 = CPU_TIME;
      overhead2 += t1-t0;
      t0 = CPU_TIME;
      t1 = CPU_TIME;
      overhead2 += t1-t0;
      t0 = CPU_TIME;
      t1 = CPU_TIME;
      overhead2 += t1-t0;
    }
  overhead2 /= (MANY_ITER*8);
  
 #undef MANY_ITER

  return (overhead1+overhead2)/2;
}



__attribute__((optimize(0))) double estimate_loop0_toverhead(int N, int size, int stride, double tovrhd )
{
  #define MANYLOOPS 100

 #if defined(USE_PAPI)    
  long double values[PAPI_EVENTS_NUM];
 #endif
  int off2 = stride + stride;
  int off3 = off2 + stride;
  int off4 = off2 + off2;
  int end = size - off4;
  
  double tstart = CPU_TIME;
  for( int k = 0; k < MANYLOOPS; k++ )
    {
      for ( int j = 0; j < N; j++ )
	{
	  ull acc0 = 0, acc1 = 0, acc2 = 0, acc3 = 0;
	  
	  PAPI_START_CNTR;
	  for ( int k = 0; k < end; k+=off4 ) {
	    acc0 += 1; acc1 += off2; acc2 += off2; acc3 += off3;}
	  for( int k = end; k < size; k++ )
	    acc0 += k;
	  PAPI_STOP_CNTR;
	 #if defined(USE_PAPI)    
	  if ( j > 0 )
	    for ( int k = 0; k < PAPI_EVENTS_NUM; k++ ) values[k] += (long double)PAPI_GET_CNTR(k);
	 #endif
	  PAPI_FLUSH;
	}
    }
  double overhead = (CPU_TIME - tstart)/MANYLOOPS - tovrhd;  
  #undef MANYLOOPS
  return overhead;
}


__attribute__((optimize(0))) void estimate_loop0_poverhead(int N, int size, int stride, ull *povrhd )
{
 #if defined(USE_PAPI)
 #define MANYLOOPS 100

  long double values[PAPI_EVENTS_NUM] = {0};
  int off2 = stride + stride;
  int off3 = off2 + stride;
  int off4 = off2 + off2;
  int end = size - off4;
  
  PAPI_FLUSH;
  PAPI_START_CNTR;
  for( int k = 0; k < MANYLOOPS; k++ )
    {
      for ( int j = 0; j < N; j++ )
	{
	  ull acc0 = 0, acc1 = 0, acc2 = 0, acc3 = 0;
	  for ( int k = 0; k < end; k+=off4 ) {
	    acc0 += 1; acc1 += off2; acc2 += off2; acc3 += off3;}
	  for( int k = end; k < size; k++ )
	    acc0 += k;
	  if ( j > 0 )
	    for ( int k = 0; k < PAPI_EVENTS_NUM; k++ ) values[k] += (long double)PAPI_GET_CNTR(k);	  
	}
    }
  PAPI_STOP_CNTR;
  for ( int k = 0; k < PAPI_EVENTS_NUM; k++ ) povrhd[k] = PAPI_GET_CNTR(k)/(MANYLOOPS*N);
  PAPI_FLUSH;
  
 #undef MANYLOOPS
 #else

 #endif
  return;
}


__attribute__((optimize(0))) ull wipe_cache( ull *base, int N ) 
{
  ull sum = 0;
  ull loopover = N/10;
  for ( int i = 0; i < N; i++ )
    sum += (base[i] = i % loopover);
  return sum;
}

