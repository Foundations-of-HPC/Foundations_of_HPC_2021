
//
//
//
typedef double v4d __attribute__ ((vector_size (4*sizeof(double))));
typedef union {
  v4d   V;
  double v[4];
}v4d_u;

double sum;

for ( int r = 0; r < ITER; r++ )
  {
    int N_4 = N / 4;
    v4d register ssum = {0.0};
    v4d prefetch;
    v4d value;
    v4d *line = (v4d*)&array[    0 ];
    v4d *stop = line + N_4*4;
	
    WIPE_CACHE;
    sum = 0;

    PAPI_START_CNTR;
    double t0 = CPU_TIME;
      
    prefetch = line[ 0 ];
    do
      {
	value     = prefetch;
	prefetch  = line[ 2 ];
	ssum     += value + line[ 0 ];
      } while ( (line+=2) < stop );

    for ( int i = N_4*4; i < N; i++ )
      sum += array[ i ];

    v4d_u ssum_u;
    ssum_u.V = ssum;
    sum = ssum_u.v[0] + ssum_u.v[1] +ssum_u.v[2] + ssum_u.v[3];

    double t = CPU_TIME - t0;
    PAPI_STOP_CNTR;

    avg  += t;
    stdv += t * t; 
  }
