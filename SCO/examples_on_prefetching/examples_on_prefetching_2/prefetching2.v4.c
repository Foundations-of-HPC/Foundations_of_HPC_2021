//
//
//
myFloat_sum  sum = 0;

for ( int r = 0; r < ITER; r++ )
  {
#define UNROLL 4
    int N_U = (N / UNROLL) * UNROLL;
    myFloat A[UNROLL+1];
      
    WIPE_CACHE;
    sum = 0;            

    myFloat *line = array + UNROLL;
    myFloat *stop = array + N_U;
    myFloat_sum register sum_tmp[UNROLL] = {0, 0, 0, 0};

    PAPI_START_CNTR;      
    double t0 = CPU_TIME;
      
    A [ 0 ] = array[0];
      
    for ( ; line < stop; line += UNROLL )
      /*
       * This for cycle tries to:
       *   - exploit 4 pipelines if available expressing
       *     4 independent summations
       *   - hide latancy of operations, postponing the
       *     final "collapse" summation out of the loop
       *   + same things than version v2
       */
      {
	A[ UNROLL ] = line[ UNROLL ];
	  
	A[      1 ] = line[      1 ];
	A[      2 ] = line[      2 ];
	A[      3 ] = line[      3 ];      

	sum_tmp[ 0 ] += A[ 0 ];                  // try to expose data parallelism
	sum_tmp[ 1 ] += A[ 1 ];                  // we now use 4 different accumulators
	sum_tmp[ 2 ] += A[ 2 ];		   // (the sum_tmp array) that are
	sum_tmp[ 3 ] += A[ 3 ];		   // independent from each other so to
	// use a vector op or as many pipelines
	// as possible
	  
	A[0] = A[UNROLL];
      }
      
    sum_tmp[ 0 ] += sum_tmp[ 1 ];
    sum_tmp[ 2 ] += sum_tmp[ 3 ];
    sum = sum_tmp[ 0 ] + sum_tmp[ 2 ];
      
    for ( int i = N_U; i < N; i++ )
      // account for final elements in case N % 4 > 0
      sum += array [ i ];

    double t = CPU_TIME - t0;
    PAPI_STOP_CNTR;
      
    avg  += t;
    stdv += t * t; 
  }
