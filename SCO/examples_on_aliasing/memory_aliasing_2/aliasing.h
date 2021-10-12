
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


#define ITER 30
#define N_def 10000000
#define TOP_FACTOR 3
#define TCPU_TIME (clock_gettime( id, &ts ), (double)ts.tv_sec +\
                   (double)ts.tv_nsec * 1e-9)

#define TYPE float

typedef void (*loopfunc)(int, const TYPE*, TYPE*, TYPE*);

void process_float_array           ( int, const TYPE*, TYPE*, TYPE *);
void process_float_array_noaliasing( int, const TYPE * restrict, TYPE* restrict, TYPE * restrict);
