
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
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <omp.h>
#include <time.h>



#if defined(_OPENMP)
#define CPU_TIME (clock_gettime( CLOCK_REALTIME, &ts ), (double)ts.tv_sec + \
                  (double)ts.tv_nsec * 1e-9)

#define CPU_TIME_th (clock_gettime( CLOCK_THREAD_CPUTIME_ID, &myts ), (double)myts.tv_sec +     \
                     (double)myts.tv_nsec * 1e-9)

#else

#define CPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec + \
                  (double)ts.tv_nsec * 1e-9)
#endif

#if defined(DEBUG)
#define VERBOSE
#endif

#if defined(VERBOSE)
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif




#define MAX( a, b ) ( (a)->data[HOT] >(b)->data[HOT]? (a) : (b) );
#define MIN( a, b ) ( (a)->data[HOT] <(b)->data[HOT]? (a) : (b) );

#if !defined(DATA_SIZE)
#define DATA_SIZE 8
#endif
#define HOT       0

#if (!defined(DEBUG) || defined(_OPENMP))
#define N_dflt    100000
#else
#define N_dflt    10000
#endif

#define TaskTh_dflt 64

typedef struct
{
  double data[DATA_SIZE];
} data_t;

typedef int (compare_t)(const void*, const void*);
typedef int (verify_t)(data_t *, int, int, int);

extern inline compare_t compare;
extern inline compare_t compare_ge;
extern inline compare_t compare_g;
verify_t  verify_partitioning;
verify_t  verify_sorting;
verify_t  show_array;

extern inline int partitioning( data_t *, int, int );
void pqsort( data_t *, int, int ); 
void insertion_sort( data_t *, int, int );

int task_cutoff      = TaskTh_dflt;
int insertion_cutoff = TaskTh_dflt / 2;

#pragma omp threadprivate( task_cutoff, insertion_cutoff )

int main ( int argc, char **argv )
{

  
  // ---------------------------------------------
  //  get the arguments
  //


  int N          = N_dflt;
  
  /* check command-line arguments */
  {
    int a = 0;
    
    if ( argc > ++a ){
      N = atoi(*(argv+a));
      if ( argc > ++a ) {
	task_cutoff = atoi(*(argv+a));	
	if ( argc > ++a ) {
	  insertion_cutoff = atoi(*(argv+a)); }
	else insertion_cutoff = task_cutoff/2;}}
  }
  
  // ---------------------------------------------
  //  generate the array
  //
  
  data_t *data = (data_t*)malloc(N*sizeof(data_t));
  long int seed;
 #if defined(_OPENMP)
 #pragma omp parallel
  {
    int me             = omp_get_thread_num();
    short int seed     = time(NULL) % ( (1 << sizeof(short int))-1 );
    short int seeds[3] = {seed-me, seed+me, seed+me*2};

   #pragma omp for
    for ( int i = 0; i < N; i++ )
      data[i].data[HOT] = erand48( seeds );
  }
 #else
  {
    seed = time(NULL);
    srand48(seed);
    
    PRINTF("ssed is % ld\n", seed);
     
    for ( int i = 0; i < N; i++ )
      data[i].data[HOT] = drand48();
  }    
 #endif

  
  // ---------------------------------------------
  //  process 
  //
  struct timespec ts;
  int    nthreads = 1;
  double tstart = CPU_TIME;
  
 #if defined(_OPENMP)

 #pragma omp parallel copyin( task_cutoff, insertion_cutoff )
  {
   #pragma omp single
    {
      nthreads = omp_get_num_threads();
      pqsort( data, 0, N );
    }
  }
  
 #else

  // uncomment the following call to use
  // exactly the same routine than the omp version
  pqsort( data, 0, N );

  // uncomment the following call to test
  // the insertion sort routine
  /* insertion_sort( data, 0, N); */

  // uncomment the following call to use
  // the library qsort routine
  /* qsort( data, N, sizeof(data_t), compare); */
  
 #endif
  
  double tend = CPU_TIME;  
  
  // ---------------------------------------------
  //  release the memory and stop
  //

  if ( verify_sorting( data, 0, N, 0) )
    printf("%d\t%g sec\n", nthreads, tend-tstart);
  else
    printf("the array is not sorted correctly\n");
  
  free( data );

  return 0;
}


 #define SWAP(A,B,SIZE) do {int sz = (SIZE); char *a = (A); char *b = (B); \
    do { char _temp = *a;*a++ = *b;*b++ = _temp;} while (--sz);} while (0)

inline int partitioning( data_t *data, int start, int end )
{
  
  // pick up the meadian of [0], [mid] and [end] as pivot
  //
  /* to be done */

  // pick up the last element as pivot
  //
  --end;  
  void *pivot = (void*)&data[end];
  
  int pointbreak = end-1;
  for ( int i = start; i <= pointbreak; i++ )
    if( compare_ge( (void*)&data[i], pivot ) )
      {
	while( (pointbreak > i) && compare_ge( (void*)&data[pointbreak], pivot ) ) pointbreak--;
	if (pointbreak > i ) 
	  SWAP( (void*)&data[i], (void*)&data[pointbreak--], sizeof(data_t) );
      }  
  pointbreak += !compare_ge( (void*)&data[pointbreak], pivot ) ;
  SWAP( (void*)&data[pointbreak], pivot, sizeof(data_t) );
  
  return pointbreak;
}


void pqsort( data_t *data, int start, int end )
{

 #if defined(DEBUG)
 #define CHECK {							\
    if ( verify_partitioning( data, start, end, mid ) ) {		\
      printf( "partitioning is wrong\n");				\
      printf("%4d, %4d (%4d, %g) -> %4d, %4d  +  %4d, %4d\n",		\
	     start, end, mid, data[mid].data[HOT],start, mid, mid+1, end); \
      show_array( data, start, end, 0 ); }}
 #define CHECK_S {						\
     if ( !verify_sorting( data, start, end, 0 ) )		\
       printf("error between %d and %d\n", start, end ); }
 #else
 #define CHECK
 #define CHECK_S
 #endif

 #define CHECKSWAP( a, b) { if ( compare_ge ( (void*)&data[start+(a)], (void*)&data[start+(b)] ) )\
      SWAP( (void*)&data[start+(a)], (void*)&data[start+(b)], sizeof(data_t) );}

  int size = end-start;

  switch ( size )
    {
    case 1: break;
    case 2: { if ( compare_ge ( (void*)&data[start], (void*)&data[end-1] ) )
	  SWAP( (void*)&data[start], (void*)&data[end-1], sizeof(data_t) ); } break;
    case 3: { CHECKSWAP( 1, 2 );
	CHECKSWAP( 0, 2 );
	CHECKSWAP( 0, 1 ); } break;
    default: { if ( size < insertion_cutoff ) {
	  insertion_sort( data, start, end );
	  CHECK_S; }
	else {
    
	  int mid = partitioning( data, start, end );
	  
	  CHECK;
	  
	  int mid_start = mid-start;
	  if ( mid_start > 0 )
	   #pragma omp task default(none) final( mid_start < task_cutoff ) mergeable \
	     shared(data) firstprivate(start, mid) untied
	    pqsort( data, start, mid );
	  
	  int end_mid = end -(mid+1);
	  if ( end_mid )
	   #pragma omp task default(none) final( end_mid < task_cutoff ) mergeable \
	     shared(data) firstprivate(mid, end) untied
	    pqsort( data, mid+1, end );
	} } break;
    }
  
}



void insertion_sort( data_t *data, int start, int end )
{
  {
    int min_idx = start;
    for ( int i = start+1; i < end; i++ )
      if ( compare_g( (void*)&data[min_idx], (void*)&data[i] ) )
	min_idx = i;
    
    SWAP( (void*)&data[start], (void*)&data[min_idx], sizeof(data_t) );
  }

  for ( int head = start+1, run = start+1; (run = ++head) < end; )
    {      
      while ( (run > 0) && compare_g( (void*)&data[run-1], (void*)&data[run] ) ) {
	SWAP( (void*)&data[run-1], (void*)&data[run], sizeof(data_t) ); --run;}
    }
  
}




 
int verify_sorting( data_t *data, int start, int end, int mid )
{
  int i = start;
  while( (++i < end) && (data[i].data[HOT] >= data[i-1].data[HOT]) );
  return ( i == end );
}

int verify_partitioning( data_t *data, int start, int end, int mid )
{
  int failure = 0;
  int fail = 0;
  
  for( int i = start; i < mid; i++ )
    if ( compare( (void*)&data[i], (void*)&data[mid] ) >= 0 )
      fail++;

  failure += fail;
  if ( fail )
    { 
      printf("failure in first half\n");
      fail = 0;
    }

  for( int i = mid+1; i < end; i++ )
    if ( compare( (void*)&data[i], (void*)&data[mid] ) < 0 )
      fail++;

  failure += fail;
  if ( fail )
    printf("failure in second half\n");

  return failure;
}


int show_array( data_t *data, int start, int end, int not_used )
{
  for ( int i = start; i < end; i++ )
    printf( "%f ", data[i].data[HOT] );
  printf("\n");
  return 0;
}


inline int compare( const void *A, const void *B )
{
  data_t *a = (data_t*)A;
  data_t *b = (data_t*)B;

  double diff = a->data[HOT] - b->data[HOT];
  return ( (diff > 0) - (diff < 0) );
}

inline int compare_ge( const void *A, const void *B )
{
  data_t *a = (data_t*)A;
  data_t *b = (data_t*)B;

  return (a->data[HOT] >= b->data[HOT]);
}

inline int compare_g( const void *A, const void *B )
{
  data_t *a = (data_t*)A;
  data_t *b = (data_t*)B;

  return (a->data[HOT] > b->data[HOT]);
}
