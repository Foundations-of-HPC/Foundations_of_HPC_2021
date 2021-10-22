/*
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


/* -------------------------------------------------------------------------------------
 *   LOOK AT prefetching2.v0.C FOR MORE THOUROUGH AND DETAILED COMMENTS ABOUT THE CODE
 -------------------------------------------------------------------------------------- */

#if defined(__STDC__)
#  if (__STDC_VERSION__ >= 199901L)
#     define _XOPEN_SOURCE 700
#  endif
#endif
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "mypapi.h"

#define MAKESTR( S ) #S
#define MERGESTR( S1, S2 ) S1##S2

#define CPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), \
		  (double)ts.tv_sec +				  \
		  (double)ts.tv_nsec * 1e-9)

#ifndef _MEM_CLOCK
#define _MEM_CLOCK 1867	  // the clock of the DRAM
#endif
#ifndef _MEM_WIDTH
#define _MEM_WIDTH 64	  // the width in bits of a DRAM chunk
#endif
#ifndef _MEM_CHNS
#define _MEM_CHNS 2
#endif
#ifndef PREFETCH
#define PREFETCH 1
#endif

#define myFloat double
#define myFloat_sum double

#define NUM 10000000
#define ITER 12

#define WIPE_CACHE  for ( int i = 0; i < N_CACHE; i++ ) wipe_cache[i] = (double)i*2.0;

int main( int argc, char **argv)
{
  int     N = NUM;
  int     N_CACHE;
  int     init_array = 1;
  struct  timespec ts;

  myFloat *array;
  myFloat *wipe_cache;

  // check arguments and init memory

  if ( argc >= 2 )
    N = atoi( *(argv+1) );
  
  if ( argc >= 3 )
    init_array = atoi( *(argv+2) );
  
  N_CACHE = N + 128;    

  // allocate memory for the array that must be summed up
  if ( (array = (myFloat*)malloc(N * sizeof(myFloat)) ) == NULL )
    {
      printf("not enough memory to allow %d doubles (%llu)\n",
	     N, 
	     (unsigned long long)N * sizeof(myFloat) );
      exit(1);
    }

  // allocate memory for another array to be used to wipe the cache content
  if ( (wipe_cache = (myFloat*)malloc(N_CACHE * sizeof(myFloat)) ) == NULL )
    {
      printf("not enough memory to allow %d doubles (%llu) for cache wiping\n",
	     N_CACHE, 
	     (unsigned long long)N_CACHE * sizeof(myFloat) );
      exit(1);
    }

  
  // init the array with pseudo-random myFloats
  if ( init_array )
    {
      printf("prefetching v%d, initialize array with random floats..\n", PREFETCH);
      srand48(19751221);
      for ( int i = N-1; i >= 0; i-- )
	array [ i ] = drand48();
      WIPE_CACHE;
    }
  


  PAPI_INIT;

  // -------------------------------------------------------------------
  // -------------------------------------------------------------------
  double avg = 0, stdv = 0;

  
  // now include the right src file, depending on the
  // prefetching version that you want
#define XSTR(x) #x
#define STR(x) XSTR(x)
#define BASENAME prefetching2.v
#define SUFFIX   .c

#include STR(BASENAME PREFETCH SUFFIX)

  // -------------------------------------------------------------------
  // -------------------------------------------------------------------
  
  avg /= ITER;
  printf("%33s sum is %g, took %g secs (stdv: %g)\n",
	 "no prefetching, unrolling - ",
	 sum, avg, sqrt(stdv/ITER - avg*avg));

  // get the maximum transfer rate in GB/sec
  // _MEM_CLOCK is in Mhz                                                           
  double max_GB_per_sec          = (double)_MEM_CLOCK / 1000 *
    (_MEM_WIDTH / 8) * _MEM_CHNS;    
  // get the transfer rate obtained                                                 
  double transfer_rate_in_GB_sec = (double)N*2*sizeof(double) /                     
    (1024*1024*1024) / avg;  
  printf("transfer rate was %6.3g GB/sec "                                          
	 "(%7.4g%% of theoretical max that is %5.2g GB/sec)\n",                     
	 transfer_rate_in_GB_sec,                                                   
	 transfer_rate_in_GB_sec / max_GB_per_sec * 100, max_GB_per_sec);           

  PAPI_WRITE_COUNTERS;

  free(wipe_cache);
  free(array);
  
  return 0;
}
