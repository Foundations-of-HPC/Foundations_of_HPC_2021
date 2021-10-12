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


#if defined(__STDC__)
#  if (__STDC_VERSION__ >= 201112L)    // c11
#    define _XOPEN_SOURCE 700
#  elif (__STDC_VERSION__ >= 199901L)  // c99
#    define _XOPEN_SOURCE 600
#  else
#    define _XOPEN_SOURCE 500          // c90
#  endif
#endif

#define _GNU_SOURCE                // this is to avoid problems with the 
                                   //   definition of alloca()


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if _XOPEN_SOURCE >= 600
#  include <strings.h>
#endif
#include <sys/resource.h>

#define STACKSMASH  8*1024*1024    // <<--- set this to the number of bytes
				   // needed to smash the stack limit


void func1(unsigned int S)
/*
 * array here is dynamically allocated, and as such
 * memory is allocated into the heap, not into the
 * stack.
 * that's why it does not lead to a seg fault when
 * called even if S*sizeof(int) is larger than the
 * stack size
 */
{
  char *array;

  array = (char*)malloc(S * sizeof(char));

  for ( int i = 0; i < S; i++ )
	array[i] = i;

  free(array);
  
  printf("dynamic allocation of %zd bytes on the heap has been done\n", S * sizeof(char)); fflush(stdout);
  return;	
}


void func2(void)
/*
 * usually the linux stack limit is set at 8192KB;
 * if this is still the case at the moment of executing
 * this routine, it will cause a seg fault when called.
 * In fact, it will allocate a local array of 8192KB
 *
 * try to set a different stack limit through
 *   ulimit -s <limit>
 * or to decrease the dimension of array 
 * (decreasing STACKSMASH) and magically the 
 * segfault will disappear
 */
{
  char array[STACKSMASH]; // try to get 8192K on the stack

  for ( int i = 0; i < STACKSMASH; i++ )
	array[i] = i;
  
  printf("static  allocation of %zd bytes on the stack has been done\n", STACKSMASH * sizeof(char)); fflush(stdout);
  return;
}


void func3(unsigned int S)
/*
 * This routine will try to dynamically allocate S
 * integer on the stack.
 * If you succeeded in enlarging the stack, if needed,
 * there will be no segfault.
 * Otherwise...
 */
{
  char *array; // try to get 8192K on the stack

  array = (char*) alloca ( S* sizeof(char) );
  for ( int i = 0; i <  S; i++ )
    array[i] = i;

  // you don't need to free from stack
  
  printf("dynamic allocation of %zd bytes on stack the has been done\n", S * sizeof(char)); fflush(stdout);
  return;
}




int main(int argc, char **argv)
{
  unsigned int  S;
  struct rlimit stack_limits;
  
  getrlimit(RLIMIT_STACK, &stack_limits);
      

  // get the memory to be allocated in KB
  if(argc > 1)
    
    S = (unsigned int)(atoi(*(argv+1))  * 1024);
  
  else
    {
      S = stack_limits.rlim_cur;
      
      printf("\nyou did not give a memory amount to be allocated, "
	     "I'll use the current soft stack limit of %u\n(hard limit is set to %d)\n",
	     S, (int)stack_limits.rlim_max);
    }


  printf("\n");
  
  int MaxS = S;
  if ( MaxS < STACKSMASH )
    MaxS = STACKSMASH ;
  
  if( (long int)stack_limits.rlim_cur <= MaxS )
    {
      if( ( (long int)stack_limits.rlim_max < 0 ) ||
	  ( (long int)stack_limits.rlim_max > MaxS ) )
	// in this case you can enlarge the soft limit because either
	// the hard limit is not set or it is larger than the requested memory
	{
	  const struct rlimit mystack_limits = {MaxS + stack_limits.rlim_cur, stack_limits.rlim_max};
	  if ( setrlimit(RLIMIT_STACK, &mystack_limits) == 0 )
	    printf ( "\t --> the stack has been enlarged to %ld bytes\n\n", mystack_limits.rlim_cur );
	  else
	    printf ( "\t << for some reason I failed to enlarge the stack. Good luck and thanks for all the fish >> \n\n" );
	}
      else
	// the hard limit is set and it is smaller than the requested memory
	printf ( "\nI could not enlarge the stack enough because there is an hard limit set to %ld\n",
		 (long int)stack_limits.rlim_max );
    }
  else
    // there's enough room in the stack
    printf ( "\nIf you're lucky there's no need to enlarge the stack, in my opinion.. (%3.2f%% of occupancy requested)\n",
	     (float)MaxS * 100.0 / (long int)stack_limits.rlim_cur );


  
  // call func1, it dynamically allocates in the heap
  func1(MaxS);
  
  // call func2, it statically allocates in the stack
  func2();

  // call func3, it dynamically allocates in the stack
  func3(MaxS);

  // restore the original settings
  setrlimit(RLIMIT_STACK, &stack_limits);

  printf ( "\n\t --> the stack limits have been restored\n\n" );
  return 0;
}
