/*
 * This file is part of the exercises for the Lectures on 
 *   "Foundations of High Performance Computing"
 * given at 
 *   Master in HPC and 
 *   Master in Data Science and Scientific Computing
 * @ SISSA, ICTP and University of Trieste
 *  2019
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

#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "aliasing.h"

#define ALIGN 32

int    compare  ( const void *A, const void *B );


int main ( int argc, char **argv )
{

  int N               = N_def;
  int type            = 0;
  double timing[ITER] = {0};
  double sum[ITER]    = {0};
#if defined (ASSIGN)
  char message[] = {"assignment"};
#else
  char message[] = {"summation"};
#endif
  struct timespec ts;
  clockid_t       id = CLOCK_PROCESS_CPUTIME_ID;


  // ---------------------------------------------------
  // initialization
  
  if ( argc > 1 )
    {
      type = atoi( *(argv+1) );
      
      if ( argc > 2 )
	N = atoi( *(argv+2) );
    }

  TYPE _Alignas(ALIGN) * restrict A;
  TYPE _Alignas(ALIGN) * restrict B;
  TYPE _Alignas(ALIGN) * restrict C;
  A = (TYPE*)aligned_alloc( ALIGN, N*sizeof(TYPE) );
  B = (TYPE*)aligned_alloc( ALIGN, N*sizeof(TYPE) );
  C = (TYPE*)aligned_alloc( ALIGN, N*sizeof(TYPE) );

  srand48(1010101);
  for ( int cc = 0; cc < N; cc++ )    
    {
      A[cc] = 0;
      B[cc] = 0;
      C[cc] = drand48();
    }


  // ---------------------------------------------------
  // set-up

  loopfunc function;

  if ( type < 2 )
    {
      if( type == 0)
	{
	  function = process_float_array;
	  printf("aliasing won't be explicitly excluded in %s.. ", message);
	}
      else
	{	    
	  function = process_float_array_noaliasing;
	  printf("aliasing will be explicitly excluded in %s.. ", message);
	}

      for ( int cc = 0; cc < ITER; cc++ )
	{
	  double time_start = TCPU_TIME;
	  function( N, C, A, B );
	  double time_end = TCPU_TIME - time_start;
	  
	  timing[ cc ] = time_end;
	  
	  // just to let the compiler believe we are doing something
	  for ( int ii = 0; ii < N; ii++ )
	    sum[cc] += A[ii]+B[ii];
	  
	}
    }

  qsort ( timing, ITER, sizeof(double), compare);
  
  int top = ITER/TOP_FACTOR;
  if (top < 3)
    top = 3;

  double time_avg  = 0;
  double sigma_avg = 0;
  for ( int cc = 0; cc < top; cc++ )
    {
      time_avg  += timing[cc];
      sigma_avg += timing[cc]*timing[cc];

      // just to let the compiler believe we are doing something
      printf("%g ", sum[cc]);
    }
  time_avg /= top;
  sigma_avg /= top;
  
  printf( "\n # averaging best %d of %d iterations # %g (sigma^2: %g)\n",
	  top, ITER,
	  time_avg, sigma_avg - time_avg*time_avg);
  
  free( C );
  free( B );
  free( A );
  
  return 0;
}


int compare ( const void *A, const void *B )
{
  int lt = (*(double*)A > *(double*)B);
  int st = (*(double*)A < *(double*)B);
  return (lt - st);
}
