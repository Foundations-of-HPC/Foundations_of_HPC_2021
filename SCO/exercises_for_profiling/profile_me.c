/*
 * This file is part of the exercises for the Lectures on 
 *   "Foundations of High Performance Computing"
 * given at 
 *   Master in HPC and 
 *   Master in Data Science and Scientific Computing
 * @ SISSA, ICTP and University of Trieste
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


#include <stdlib.h>
#include <stdio.h>

/* --=={ function prototypes declaration }==--
   ........................................... */

void loop       ( int );
void loop_caller( int );
unsigned long long int factorial  ( int );

/* -- END of funtion declarations  ======== -- */


/*       --=={ function definition }==--
   ........................................... */

unsigned long long int factorial (int n )
/*
  This function calculates the factorial of n
  how large can ben n to get a meaningful result ?
 */
{
  if ( n & 0xFFFFFFFE )
    {
      unsigned long long int f = factorial( n - 1 );
      return n * f;
    }
  return 1;
}



void loop(int n)
/*
  this function does not do much, actually
 */
{
  int volatile i;  // does not optimize out
  i = 0;
  while(i++ < n)
    ;
  
  unsigned long long int f;
  for ( i = 0; i < 100000; i++ )
    f = factorial( 20 );
  printf ( "factorial is %llu\n", f );
}



/* 
   that is definitely  a phony function
 */
void loop_caller(int n) { loop(n); }

/* -- END of function definitions  ======== -- */



/*             --=={ M A I N }==--
   ........................................... */

int main( void )
{
  loop_caller(10000000);
  return 0;
  
}
