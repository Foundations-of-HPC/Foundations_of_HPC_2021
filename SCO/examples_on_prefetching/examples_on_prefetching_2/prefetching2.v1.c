

  // - V1
  //   sum the array with unrolling, without prefetching
  //
  myFloat_sum sum;
  
  for ( int r = 0; r < ITER; r++ )
    {
#define UNROLL 4      
      int N_U = (N / UNROLL) * UNROLL;
      myFloat_sum register sumA = 0;
      myFloat_sum register sumB = 0;
      WIPE_CACHE;
      sum = 0;

      PAPI_START_CNTR;
      double t0 = CPU_TIME;

      for ( int i = 0; i < N_U; i += UNROLL )
	/*
	 * This for cycle tries to:
	 *   - populate 2 pipelines, explicitly expressing
	 *     2 separate summations
	 *   - hide (a bit) the latency of the sums, postponing
	 *     the accumulation of sumA and sumB on sum
	 *   - induce a vectorization
	 */
	{
	  double sumC = array[ i+4 ];
	  sum  += sumA + sumB;                     // <--+ here sumA and sumB will be (almost) available
	  sumA  = array[ i   ] + array[ i+1 ];	   //    |
	  sumB  = array[ i+2 ] + array[ i+3 ];	   // ---+ sumA and sumB "travel" to the
						   //      next iteration; loop-related
						   //      opearations reduce the latency
						   //      of fp instructions, so that
						   //      the wait for them will be smaller
						   //      if not none at all
						   //
						   //      using -march=native you may force
						   //      the compiler to notice that 4
						   //      entries of array can be treated
						   //      simultaneously ( -> attempt of
						   //      inducing vectoriazion )
	}
      sum += sumA + sumB;
      
      for ( int i = N_U; i < N; i++ )
	// account for final elements in case N % 4 > 0
	sum += array[ i ];
      
      double t1 = CPU_TIME;
      PAPI_STOP_CNTR;
      
      double t = t1 - t0;
      avg  += t;
      stdv += t * t; 
    }
