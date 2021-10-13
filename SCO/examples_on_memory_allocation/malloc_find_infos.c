
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

/* ----------------------------------------------------------------- *
 *                                                                   *
 * with this code we simply find that some infos are readed before   *
 * an allocated block of memory                                      *
 * ( that may be strongly implementation dependent, though; this is  *
 *   only for didactical purposes )                                  *
 *                                                                   *
 * ----------------------------------------------------------------- */



#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>


int main ( int argc, char **argv )
{
#define SIZE 1024

  printf("\nI'm allocating 2 chunks of %d integers each"
	 " (supposed to require %lu bytes each, then)\n\n", SIZE, SIZE*sizeof(int));

  // allocate two chunks of memory
  // much likely they will be subsequent in memory
  //
  int *chunk1 = (int*)calloc( SIZE, sizeof(int) );
  int *chunk2 = (int*)calloc( SIZE, sizeof(int) );

  // place a guard value at the beginning of the second chunk
  // 
  chunk2[0] = 2;

  // get some info from the malloc call about the true
  // allocate memory
  //
  unsigned long long int chunk1_size = malloc_usable_size(chunk1);
  unsigned long long int chunk2_size = malloc_usable_size(chunk2);


  // find the distance btw the 2 chunks in units of ints
  int mem_delta = chunk2 - (chunk1+SIZE);
  
  // print infos
  //
  printf("chunk1: %p (usable size is %lld bytes, %g integers)\n"
	 "chunk2: %p (usable size is %lld bytes, %g integers)\n"
	 "\ndelta mem is: %d integers, %td bytes\n\n",
	 chunk1, chunk1_size, (double)chunk1_size/sizeof(int),
	 chunk2, chunk2_size, (double)chunk2_size/sizeof(int),
	 mem_delta, (size_t)(mem_delta*sizeof(int)) );  
  
  // now, let us mimic an array overflow of chunk1..
  //
  for ( int i = 0; i < SIZE + mem_delta; i++ )
    {
      if ( i >= SIZE )
  	printf("writing at %td\n", (void*)&chunk1[i] - (void*)chunk2 );
      chunk1[i] = 1;
    }

  // verify that we actually did not overrun chunk2
  //
  printf("chunk2[0] is still %d\n\n", chunk2[0] );

  // release memory
  //
  
  printf( "releasing chunk 2.. " ); fflush(stdout);
  free( chunk2 );     /* <-- this should cause a "double free or corruption" -like error */
  printf( "done\nreleasing chunk 1.." ); fflush(stdout);
  free( chunk1 );     /* <-- you never reach this */

  // try to uncomment the following block,
  // you'll get a slightly different messages
  //
  
  //printf( "releasing chunk 1.." ); fflush(stdout);
  //free( chunk1 );     /* <-- this may cause a "invalid next size" -like error */
  //printf( "done\nreleasing chunk 1.." ); fflush(stdout);
  //free( chunk2 );  

  printf( "done\n" );
  return 0;
}
