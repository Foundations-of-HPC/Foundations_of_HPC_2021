
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

#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>
#define __USE_GNU
#include <sched.h>


// defines and type defs ::........................
// ................................................

#define MEMORY_SIZE_DEFAULT (1024 * 1024 *1024)

#define NTYPES 3
#define INT    0
#define FLOAT  1
#define DOUBLE 2

typedef struct
{
  // write-access timings
  uint64_t wr[NTYPES][17];
  
  // read-access timings
  uint64_t rd[NTYPES][17];

} timings_st;

typedef unsigned long long ULL;
typedef unsigned int       uint;


// vars  ::........................................
// ................................................


char         *scratch, *bay;
timings_st    timings;
unsigned int  MEMORY_SIZE;
const char   *labels[NTYPES] = {"integer access..", "float access..", "double access.."};


// text  ::........................................
// ................................................


// -------------------------------------------------------------

uint64_t test_write(char *memory, int mode)
{
  volatile unsigned int *intpage         = (int*)memory;
  volatile float        *floatpage       = (float*)memory;
  volatile double       *doublepage      = (double*)memory;
  unsigned int           int_template    = 0xFA5D52CA;
  float                  float_template  = 1.23456789;
  double                 double_template = 1.23456789;

  unsigned int limit;
  size_t       tck_start, tck_stop;

  if(mode == 0)
    // perform integer access
    {

      limit = (MEMORY_SIZE - 15) / sizeof(int);
      
      tck_start = read_cyc_counter();
      for( uint i = 0; i < limit-1; i++ )
	{
#ifdef RUNNING
	  *(intpage + i) = int_template+i;
#else
	  *intpage = int_template;	 
#endif
	}
      tck_stop = read_cyc_counter();      
      
    }

  else if(mode == 1)
  // perform float access
    {

      limit = (MEMORY_SIZE - 15) / sizeof(float);
      
      tck_start = read_cyc_counter();
      for( uint i = 0; i < limit-1; i++ )
	{
#ifdef RUNNING
	  *(floatpage + i) = float_template + i;
#else
	  *floatpage = float_template;
#endif
	}

      tck_stop = read_cyc_counter();      
      
    }

  else if(mode == 2)
  // perform double access
    {

      limit = (MEMORY_SIZE - 15) / sizeof(double);
      
      tck_start = read_cyc_counter();
      for( uint i = 0; i < limit-1; i++ )
	{
#ifdef RUNNING
	  *(doublepage + i) = double_template + i;
#else
	  *doublepage = double_template;
#endif
	}
      tck_stop = read_cyc_counter();      
      
    }


  return tck_stop - tck_start;
}



// -------------------------------------------------------------

uint64_t test_read(char *memory, int mode)
{
  volatile unsigned int *intpage         = (int*)memory;
  volatile float        *floatpage       = (float*)memory;
  volatile double       *doublepage      = (double*)memory;
  unsigned int           int_template;
  float                  float_template;
  double                 double_template;

  unsigned int limit;

  size_t       tck_start, tck_stop;
  
  if(mode == 0)
    // perform integer access
    {

      limit = (MEMORY_SIZE - 15) / sizeof(int);
      
      tck_start = read_cyc_counter();
      for( uint i = 0; i < limit-1; i++)
	{
#ifdef RUNNING
	  int_template = *(intpage + i) + i;
#else
	  int_template = *intpage;
#endif
	}
      tck_stop = read_cyc_counter();      
      
    }

  else if(mode == 1)
    // perform float access
    {

      limit = (MEMORY_SIZE - 15) / sizeof(float);
      
      tck_start = read_cyc_counter();
      for( uint i = 0; i < limit-1; i++)
	{
#ifdef RUNNING
	  float_template = *(floatpage + i) + i;
#else
	  float_template = *floatpage;
#endif
	}
      tck_stop = read_cyc_counter();      
      
    }

  else
  // perform double access
    {

      limit = (MEMORY_SIZE - 15) / sizeof(double);
      
      tck_start = read_cyc_counter();
      for( uint i = 0; i < limit-1; i++)
	{
#ifdef RUNNING
	  double_template = *(doublepage + i) + i;
#else
	  double_template = *doublepage;
#endif
	}
      tck_stop = read_cyc_counter();      
      
    }


  return tck_stop - tck_start;
}



// -------------------------------------------------------------

void print_results(FILE *file)
{
  double ref;

  if(file == stdout)
    printf("WRITE ------\n");
  
  ref = (double)timings.wr[INT][0];
  fprintf(file, "integer ref time: %g\n", ref);
  for( int i = 0; i <= 16; i++)
    fprintf(file, "offset %2d int    %12llu - %5.3g\n", i, (ULL)timings.wr[INT][i], timings.wr[INT][i] / ref);
  fprintf(file, "\n\n");
  ref = (double)timings.wr[FLOAT][0];
  fprintf(file, "float ref time: %g\n", ref);
  for( int i = 0; i <= 16; i++)
    fprintf(file, "offset %2d float    %12llu - %5.3g\n", i, (ULL)timings.wr[FLOAT][i], timings.wr[FLOAT][i] / ref);
  fprintf(file, "\n\n");
  ref = (double)timings.wr[DOUBLE][0];
  fprintf(file, "double ref time: %g\n", ref);
  for( int i = 0; i <= 16; i++)
    fprintf(file, "offset %2d double %12llu - %5.3g\n", i, (ULL)timings.wr[DOUBLE][i], timings.wr[DOUBLE][i] / ref);

  fprintf(file, "\n\n");

  if(file == stdout)
    printf("READ ------\n");

  ref = (double)timings.rd[INT][0];
  fprintf(file, "integer ref time: %g\n", ref);
  for( int i = 0; i <= 16; i++)
    fprintf(file, "offset %2d int    %12llu - %5.3g\n", i, (ULL)timings.rd[INT][i], timings.rd[INT][i] / ref);
  fprintf(file, "\n\n");
  
  ref = (double)timings.rd[FLOAT][0];
  fprintf(file, "float ref time: %g\n", ref);
  for( int i = 0; i <= 16; i++)
    fprintf(file, "offset %2d float    %12llu - %5.3g\n", i, (ULL)timings.rd[FLOAT][i], timings.rd[FLOAT][i] / ref);
  fprintf(file, "\n\n");
  
  ref = (double)timings.rd[DOUBLE][0];
  fprintf(file, "double ref time: %g\n", ref);
  for( int i = 0; i <= 16; i++)
    fprintf(file, "offset %2d double %12llu - %5.3g\n", i, (ULL)timings.rd[DOUBLE][i], timings.rd[DOUBLE][i] / ref);

  return;
}






// -------------------------------------------------------------

int main(int argc, char **argv)
{
#ifdef __USE_GNU
  int        bind = 0;
  int        current_cpu;
  cpu_set_t  set;
  size_t     size = sizeof(cpu_set_t);
#endif

  int        offset, max_align, mem_size;
  char      *offset_scratch;

  MEMORY_SIZE = MEMORY_SIZE_DEFAULT + 4096;  // 1GB
  
  if(argc > 1)
    {
      if ( (offset = atoi( *(argv + 1) ) ) > 16 )
	offset = 16;
      
      if ( argc > 2 )
	{
	  // MEMORY_SIZE is given as argument in units of GB
	  MEMORY_SIZE = (unsigned int)(atof( *(argv + 2) ) * MEMORY_SIZE_DEFAULT) ;
	  MEMORY_SIZE = MEMORY_SIZE + 4096 - MEMORY_SIZE % 32;

	  if(argc > 3)
	    bind = atoi( *(argv + 2) );
	}
    }
  

  init_tsc_counter();
  
#ifdef __USE_GNU
  if(bind)
    {
      // set affinity on core 0
      current_cpu = 0;
      CPU_ZERO(&set);
      CPU_SET(current_cpu, &set);
      sched_setaffinity(0, sizeof(cpu_set_t), &set);
    }
#endif

  MEMORY_SIZE += offset;
  MEMORY_SIZE += 16;
  bay         = (char*)calloc(MEMORY_SIZE, 1);
  
  for(max_align = 2; max_align < 1024; max_align <<= 1)
    if( (unsigned long long )bay % max_align )
      break;
  printf("allocated memory memory is aligned to %d bytes\n", max_align);

  scratch = bay + offset;

  // ==================================================
  printf("----> starting write access test..\n\n");

  for(int k = 0; k < 3; k++)
    {
      printf("%s\n", labels[k]);
      for( int j = 0; j <= 16; j++)
	{
	  // offset by j bytes
	  printf("%2d byte offset.. ", j); fflush(stdout);
	  if( !(j % 8) )
	    printf("\n\t");

	  timings.wr[k][j] = test_write(scratch + 4*j + j, k);
	  
#ifdef __USE_GNU
	  if(bind)
	    {
	      // flip affinity between cpu 0 and 1
	      current_cpu = !current_cpu;
	      CPU_ZERO(&set);
	      CPU_SET(current_cpu, &set);
	      sched_setaffinity(0, sizeof(cpu_set_t), &set);	  
	    }
#endif
	  free( bay );

	  MEMORY_SIZE += 16;
	  bay          = (char*)calloc(MEMORY_SIZE + 4096, 1);
	  scratch      = bay + offset;
	}	
      printf("\n");
    }
      
  // ==================================================
  printf("----> starting read access test..\n\n");

  for(int k = 0; k < 3; k++)
    {
      printf("%s\n", labels[k]);
      for(int j = 0; j <= 16; j++)
	{
	  // offset by j bytes
	  printf("%2d byte offset.. ", j); fflush(stdout);
	  if( !(j % 8) )
	    printf("\n\t");
	  
	  timings.rd[k][j] = test_read(scratch + 4*j + j, k);
	  
#ifdef __USE_GNU
	  if(bind)
	    {
	      // flip affinity between cpu 0 and 1
	      current_cpu = !current_cpu;
	      CPU_ZERO(&set);
	      CPU_SET(current_cpu, &set);
	      sched_setaffinity(0, sizeof(cpu_set_t), &set);	  
	    }
#endif
	  free(bay);

	  MEMORY_SIZE += 16;
	  bay          = (char*)calloc(MEMORY_SIZE + 4096, 1);
	  scratch      = bay + offset;	  
	}	
      printf("\n");
    }
      

  // ==================================================
  printf("\nresults:\n");
  print_results(stdout);
  

  FILE *file;
  char  filename[200], buffer[100];

  sprintf(filename, "%s", "memory_access.");

  if(bind)
    strcat(filename, "bind.");
  sprintf(buffer, "%dM.dat", (int)(MEMORY_SIZE));
  strcat(filename, buffer);
  
  file = fopen(filename, "w");

  print_results(file);
  fclose(file);
  
  
  free( bay );
  
  return 0;
}
