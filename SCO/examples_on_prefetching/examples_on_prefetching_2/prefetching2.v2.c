

//
//
// sum the array with induced prefetching, V
//

myFloat_sum register sum;

for ( int r = 0; r < ITER; r++ )
  {
#define UNROLL 4
    int N_U = (N / UNROLL) * UNROLL;
    myFloat_sum register ssum0 = 0;
    myFloat_sum register ssum1 = 0;

    WIPE_CACHE;
    sum = 0;

    myFloat *line = array;
    myFloat *stop = array + N_U;

    PAPI_START_CNTR;
    double t0 = CPU_TIME;

    for ( ; line < stop; line += UNROLL )
      /*
       * This for cycle tries to:
       *   - be faster, because the loop counter is
       *     a pointer: this way when you reference
       *     the i-th element, you are directly
       *     referencing memory with no 
       *     pointer arithmetic.
       *   + same things than version v1
       */
      {
	sum   += ssum0 + ssum1;
	ssum0  = line[ 0 ] + line[ 1 ];
	ssum1  = line[ 2 ] + line[ 3 ];
      }
    for ( int i = N_U; i < N; i++ )
      // account for final elements in case N % 4 > 0
      sum += array[i];
      
    double t1 = CPU_TIME;
    PAPI_STOP_CNTR;
      
    double t = t1 - t0;
    avg  += t;
    stdv += t * t; 
  }
