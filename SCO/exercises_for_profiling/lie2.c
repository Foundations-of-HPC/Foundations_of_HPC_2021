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

void loop1(int n)
{
  int volatile i;  // 'volatile' qualifier tells the compiler not to optmize out i
  i = 0;
  while(i++ < n)
    ;
}

void loop2(int n)
{
  int volatile i;  // 'volatile' qualifier tells the compiler not to optmize out i
  i = 0;
  while(i++ < n)
    ;
}


void orchestrator(int n1, int n2) { loop1(n1); loop2(n2); }

void shot1(void) { orchestrator(100000, 100000000); }

void shot2(void) { orchestrator(100000000, 100000); }

int main(void)
{
  shot1();
  shot2();
  return 0;
}
