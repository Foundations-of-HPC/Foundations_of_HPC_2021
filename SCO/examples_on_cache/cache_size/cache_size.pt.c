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

#include "ptiming.h"


#define NEVENTS  3
#define NRUNS    50
#define L1WORDS  4096  // guess for the double capacity of L1 ( 32KB )
#define STEPSIZE (L1WORDS/8)
#define L2WORDS  32768 // guess for the double capacity of L2 ( 256K )
#define L2WORDS2 (4*L2WORDS)


double *array;


int main(int argc, char **argv)
{

  uint64_t clk;


  init_cycles_counter();
  
  /* declare an array that is more than twice the L2 cache size */
  array     = (double*) malloc(L2WORDS2*sizeof(double));

  
  for ( int size = STEPSIZE; size < L2WORDS2; size += STEPSIZE)
    {
      /* warm-up the cache by dragging the whole array through it */
      for ( int n = 0; n < L2WORDS2; n++) array[n] = (double)0.0;
      
      /* now load the data in the highest cache level that fits */
      for ( int n = 0; n < size; n++) array[n] = (double)1.0;
      
      

      unsigned long long int cycles = 0;
      for( int r = 0; r < NRUNS; r++ ) {
	
	/* now load the data in the highest cache level that fits */
	for ( int n = 0; n < size; n++) array[n] = (double)1.0;

	clk = read_cyc_counter();
	for (int j = 0; j < size; j++ ) (array[j] = 2.3*array[j]+1.2);
	cycles += read_cyc_counter() - clk - clock_info.clock_overhead_direct;
      }
      
      printf("size: %d tot_cycles %9.5f cycles_per_loc: %9.5f\n",
	     size, (double)cycles/NRUNS, (double)cycles/(NRUNS*size) );
    }

  free(array);

  return 0;
}
