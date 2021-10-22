//
//
// sum the array with induced prefetching, I
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
    myFloat_sum register sumA = 0;
    myFloat_sum register sumB = 0;

    PAPI_START_CNTR;      
    double t0 = CPU_TIME;
      
    A [ 0 ] = array[0];
      
    for ( ; line < stop; line += UNROLL )
      /*
       * This for cycle tries to:
       *   - hide memory latency by inducing pre-fetching; in fact,
       *     the instruction
       *         A [ UNROLL ] = line[UNROLL]
       *     "touches" the memory 4 bytes ahead, trying to populate
       *     the cache for the next iteration.
       *     while data travels from main memory to the cache, A
       *     is populated with data that are already in the cache.
       *   + same things than version v2
       */
      {
	A[ UNROLL ] = line[ UNROLL ];    // We access the UNROLL-th (5th in this case)
	// entry, counting from the memory position
	// pointed by line*
	// If it wasn't in the cachem we force it -
	// and the following data - to be loaded in
	  
	A[      1 ] = line[      1 ];    // Here we access data that are already in the
	A[      2 ] = line[      2 ];    // cache because they were in the same line
	A[      3 ] = line[      3 ];    // than the element accessed in the previous
	// iteration

	sum  += sumA + sumB;             // here we try to hide the latency of fp sum
	sumA  = A[ 0 ] + A[ 1 ];	   // operation: sumA and sumB have been calculated
	sumB  = A[ 2 ] + A[ 3 ];	   // in the previous iteration
	  
	A[0] = A[UNROLL];   		   // now the UNROLL-th element is available
					   // (or much more "closer" than before)
      }
      
    sum += sumA + sumB;
      
    for ( int i = N_U; i < N; i++ )
      // account for final elements in case N % 4 > 0
      sum += array [ i ];

    double t = CPU_TIME - t0;
    PAPI_STOP_CNTR;
      
    avg  += t;
    stdv += t * t; 
  }
