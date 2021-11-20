#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>
#include <gsl/gsl_rng.h>

typedef long long int map_index;
typedef int map_point[2];             // NOTE: "int" use limits the plane to a side length of 2^31 points

#define max(x,y) ((x) > (y) ? (x) : (y))

#define x_ 0
#define y_ 1
#define BL 0  // bottom-left
#define TR 1  // top-right

#define dprintf(LEVEL, ...) if(verbose_level > (LEVEL)) fprintf(stderr, __VA_ARGS__);

int verbose_level = 2;

map_index get_map                 ( map_point );
uint      random_seed             ( );
int       cmp_map_points          ( const void *, const void *);
int       generate_seeds_subregion( map_point, map_point, uint, uint **, uint,  map_index * );
int       generate_seeds          ( int, int, map_point [4][2], uint seed, uint **, map_index * );
void      transpose_subregion     ( int, uint, map_point *, map_point * );
int       get_plane_subregions    ( uint, map_point, map_point, map_point [4][2] );



map_index get_map(map_point P)
// returns the ordinal number of a given point along the spiral
{
  map_index l, d;
  uint      mx, my, c;
  
  mx = abs(P[x_]);
  my = abs(P[y_]);
  l = 2 * max(mx, my);

  c = (P[y_] > P[x_]) + (P[x_] > 0)*(P[x_] == P[y_]);
  
  d = (c) ? l * 3 + P[x_] + P[y_] : l - P[x_] - P[y_];

  return (l-1)*(l-1) + d;
}


uint random_seed()
// written by R. G. Brown, Duke Univ.
// http://www.sourceware.org/ml/gsl-discuss/2004-q1/msg00071.html
// Generate a random seed from the system
// Called if the user does not provide a seed
{

 unsigned int seed;
 struct timeval tv;
 FILE *devrandom;

 if ((devrandom = fopen("/dev/random","r")) == NULL) {
   gettimeofday(&tv,0);
   seed = tv.tv_sec + tv.tv_usec;
 } else {
   fread(&seed,sizeof(seed),1,devrandom);
   fclose(devrandom);
 }

 return(seed);
}


map_index *map;

int cmp_map_points(const void *A, const void *B)
// comparison function to be called by qsort
// performs indexed-sorting
// this version works if map (that is the indexed
//  array) is in its scope
{
  map_index a = map[*(uint*)A];
  map_index b = map[*(uint*)B];

  if(a < b)
    return -1;
  if(a > b)
    return 1;
  return 0;
}



int generate_seeds_subregion(map_point bottom_left, map_point top_right, uint seed, uint **seed_plane, uint offset, map_index *value)
// this function manages the generation of seeds for a single subregion
// - bottom_left, top_right are the corners of the subregion
// - seed is the initial seed for the random number generator
// - seed_plane is a pointer to the pointer that will host the seeds
// - offset is the offset to be accounted while filling the memory pointed by seed_plane
// - xstride is the xlength of the whole memory matrix pointed by seed_plane, it's used to fill it correctly
// - value is a returning value in case of error. Basically it handles the case in which some allocation fails, and
//   it will return the amount of bytes that were requested.
// we assume that the seed plane is stored in memory startign from bottom_left  

{
  map_index N;
  map_point p;
  uint xlength, ylength;
  uint *idxs;

  // calculate the region's dimensions
  xlength = top_right[x_] - bottom_left[x_];
  ylength = top_right[y_] - bottom_left[y_];
  N = xlength * ylength;

  printf("from (%d , %d) to (%d, %d), %llu points\n", bottom_left[x_], bottom_left[y_], top_right[x_], top_right[y_], N);
  
  // allocate temporary memory for the random seeds
  if ( (map = calloc( sizeof(map_index), (size_t)N)) == NULL )
    {
      if(value != NULL)
	*value = N * sizeof(map_index);
      return 1;
    }

  // allocate temporary memory for the indexes that will be used to
  // sort the points along the spiral
  if ( (idxs = calloc( sizeof(uint), (size_t)N)) == NULL )
    {
      if(value != NULL)
	*value = N * sizeof(uint);      
      return 2;
    }
  
  // initialize indexes
  // for the following loop check whether:
  // - leave unroll to the compiler
  // - or explicitly use simd instruction
  for ( int i = 0; i < N; i++)
    idxs[i] = i;

  dprintf(1, "\t\t-> getting the map..");
  // get the map value from spiral for each point in the region
  for (int j = 0; j < ylength; j++ )
    {
      p[y_] = bottom_left[y_]+j;
      
      for ( int i = 0; i < xlength; i++ )
	{ 
	  p[x_] = bottom_left[x_]+i;
	  map[j*xlength + i] = get_map(p);
	}
    }
  dprintf(1, "\n"); fflush(stdout);

  dprintf(1, "\t\t-> sorting %lu spiral points..", (size_t)N);
  // index-sort the map
  qsort(idxs, (size_t)N, sizeof(uint), cmp_map_points);
  dprintf(1, "\n"); fflush(stdout);
  
  // generate random numbers following the spiral traversal order

  // set the random number generator
  gsl_rng *R;
  
  R = gsl_rng_alloc(gsl_rng_mt19937);
  gsl_rng_set(R, seed);

  // actually generates number
  long long unsigned int rnd_counter = 0;
  long long unsigned int Ncnt        = 0;
  dprintf(1, "\t\t-> generating random seeds..");
  for ( int i = 0; i < N; i++ )
    {
      if(rnd_counter < map[idxs[i]]-1)
  	for(; rnd_counter < map[idxs[i]]-1; rnd_counter++)
  	  gsl_rng_get(R);

      map[idxs[i]] = (map_index)gsl_rng_get(R);
      Ncnt++;
      rnd_counter++;
    }
  dprintf(1, "(%llu randoms for %llu points)\n", rnd_counter, Ncnt); fflush(stdout);
    
  gsl_rng_free(R);  // releases generator
  
  free(idxs); // releases indexes

  // allocate actual memory for the random seeds, if needed
  // The difference with map is that we do not need to store 8-bytes
  // integers, while we needed it to store in the map array the
  // spiral ordered points
  /* if(*seed_plane == NULL) */
  /*   { */
  /*     if((*seed_plane = calloc( sizeof(uint), (size_t)N)) == NULL) */
  /* 	{ */
  /* 	  if(value != NULL) */
  /* 	    *value = N * sizeof(uint); */
  /* 	  return 3; */
  /* 	} */
  /*     offset = 0; */
  /*   } */

  dprintf(1, "\t\t-> copying seeds..");
  //copy the seeds from map into seed_plane, as uint instead of as long long

  for(uint k = 0, j = 0; j < N; j++, k++)
    {
      if((k > 0) && (j % xlength == 0))
	k += offset;
      (*seed_plane)[k] = (uint)map[j];
    }
  dprintf(1, "\n\t\tdone\n\n"); fflush(stdout);
  
  free(map);
  return 0;  
}


int generate_seeds(int N, int Nregions, map_point Regions[4][2], uint seed, uint **seed_plane, map_index *value)
// this function manages the generation of seed for all the subregions
// - N is the side of the whole plane
// - Nregions is the number of subregions
// - Regions is the array of (bottom_left, top_right) coordinates for the subregions
// - seed is the initial seed for the random number generator
// - seed_plane is a pointer to the pointer that will host the seeds
// - value is a returning value in case of error. Basically it handles the case in which some allocation fails, and
//   it will return the amount of bytes that were requested.
  
{
  map_point transposed_subregion[2];
  map_index Npoints;
  uint xstride, offset, start, *start_pointer;

  // calculate the total length of the whole region along x
  xstride = Regions[Nregions-1][TR][x_] - Regions[0][BL][x_];
  
  // calculate how many points will be generated
  Npoints = xstride * (Regions[Nregions-1][TR][y_] - Regions[0][BL][y_]);

  // allocate memory for all the seeds
  if((*seed_plane = calloc( sizeof(uint), (size_t)Npoints)) == NULL)
    {
      if(value != NULL)
	*value = Npoints * sizeof(uint);
      return 3;
    }

  // generate seeds in each subregion
  for ( int i = 0; i < Nregions; i++ )
    {
      transposed_subregion[0][x_] = Regions[i][0][x_];
      transposed_subregion[0][y_] = Regions[i][0][y_];
      transposed_subregion[1][x_] = Regions[i][1][x_];
      transposed_subregion[1][y_] = Regions[i][1][y_];

      // transpose subregion in spiral's coordinates
      transpose_subregion(0, N, &transposed_subregion[0], &transposed_subregion[1]);

      if(i & 1) // index is odd -> subregion is on the left part
	offset = Regions[0][TR][x_] - Regions[0][BL][x_]; // offset the insertion on the left by the right part
      else
	offset = Regions[1][TR][x_] - Regions[1][BL][x_];

      start = 0;
      if(i & 2) // index is >= 2 -> subregion is on the higher part
	start = xstride * (Regions[0][TR][y_] - Regions[0][BL][y_]); // offset the starting point by the vertical extent of lower part

      if(i & 1) // index is odd -> subregion is on the right part
	start += Regions[0][TR][x_] - Regions[0][BL][x_]; // further offset the starting point by the horizontal extent of the right part

      start_pointer = &(*seed_plane)[start];
      generate_seeds_subregion(transposed_subregion[0], transposed_subregion[1], seed, &start_pointer, offset, value);
    }

  return 0;
}

void transpose_subregion(int mode, uint N, map_point *bottom_left, map_point *top_right)
// by construction single regions are supposed to lie within quadrants
{
  uint N_2 = N/2;

  if(mode == 0)
    // from plane to spiral
    {     
      if((*bottom_left)[x_] >= N_2)
	{
	  (*bottom_left)[x_] = (*bottom_left)[x_] - N;
	  (*top_right)[x_] = (*top_right)[x_] - N;
	}
      
      if((*bottom_left)[y_] >= N_2)
	{
	  (*bottom_left)[y_] = (*bottom_left)[y_] - N;
	  (*top_right)[y_] = (*top_right)[y_] - N;
	}
    }
  else
    // from spiral back to plane
    {
      if((*bottom_left)[x_] < 0)
	{
	  (*bottom_left)[x_] = N + (*bottom_left)[x_];
	  (*top_right)[x_] = N + (*top_right)[x_];
	}
      
      if((*bottom_left)[y_] < 0)
	{
	  (*bottom_left)[y_] = N + (*bottom_left)[y_];
	  (*top_right)[y_] = N + (*top_right)[y_];
	}
    }
  
  return;
}


int get_plane_subregions(uint N, map_point bottom_left, map_point top_right, map_point Regions[4][2])
{
#define NCOORDS 2
  int  c[NCOORDS] = {0, 1};
  uint N_2 = N/2;
  int  Nregions = 1;

  
  for ( int i = 0; i < NCOORDS; i++ )
    {
      Regions[0][BL][i] = bottom_left[i];
      if(bottom_left[i] > N)
	return -1;
    }
  for ( int i = 0; i < NCOORDS; i++ )
    {
      Regions[0][TR][i] = top_right[i];
      if(top_right[i] > N)
	return -1;
    }

  for ( int k = 0; k < NCOORDS; k++ )
    {
      int m = 1;
      for ( int R = 0; R < Nregions; R++ )
	if(Regions[R][BL][c[k]] < N_2 &&
	   Regions[R][TR][c[k]] > N_2)
	  {
	    int j = Nregions + R;

	    for ( int i = 0; i < NCOORDS; i++ )
	      Regions[j][BL][c[i]] = Regions[R][BL][c[i]];
	    Regions[j][BL][c[k]] = N_2;
	    
	    for ( int i = 0; i < NCOORDS; i++ )
	      Regions[j][TR][c[i]] = Regions[R][TR][c[i]];
	    Regions[R][TR][c[k]] = N_2;


	    m = 2;
	  }

      Nregions *= m;
    }
  

  return Nregions;  
}



int main(int argc, char **argv)
{
  uint N, *SEEDS, seed;
  map_point bottom_left, top_right;
  map_point subregions[4][2];
  map_index error;

  if ( argc < 5 )
    {
      printf ( "at least 5 arguments must be provided\n" );
      exit ( 1 );
    }
  
  N               = atoi(*(argv+1));
  bottom_left[x_] = (uint)atoi(*(argv+2));
  bottom_left[y_] = (uint)atoi(*(argv+3));
  top_right[x_]   = (uint)atoi(*(argv+4));
  top_right[y_]   = (uint)atoi(*(argv+5));
  if(argc > 6)
    seed = (uint)atoi(*(argv+6));
  else
    {
      seed = random_seed();
      printf("using seed: %u\n", seed);
    }

  int Nr = get_plane_subregions(N, bottom_left, top_right, subregions);

  dprintf(0, "%d subregion%c: \n\n", Nr, (Nr>1)?'s':' ');
  
  for ( int i = 0; i < Nr; i++ )
    {
      dprintf(0, "\t %d: bottom left : %d, %d\n", i, subregions[i][BL][0], subregions[i][BL][1]);
      dprintf(0, "\t %d: top right : %d, %d\n", i, subregions[i][TR][0], subregions[i][TR][1]);
      dprintf(1, "\t which becomes:\n");

      // transpose to check that transposition is ok
      transpose_subregion(0, N, &subregions[i][BL], &subregions[i][TR]);
      dprintf(1, "\t\t bottom left : %d, %d\n", subregions[i][BL][0], subregions[i][BL][1]);
      dprintf(1,"\t\t top right : %d, %d\n", subregions[i][TR][0], subregions[i][TR][1]);

      // transpose back, because transposition will be made by the seeds generating routins
      transpose_subregion(1, N, &subregions[i][BL], &subregions[i][TR]);

      dprintf(0, "\n");
    }

  for ( int j = 0; j < 100; j++ )
    {
      int i = generate_seeds(N, Nr, subregions, seed, &SEEDS, &error);
      
      if(i > 0)
	printf("some problem occured!\n");
    }


  free(SEEDS);
  return 0;
}



// ===================================================================================
//  Unused code snippets
// ===================================================================================

/* map_index get_map_n(map_point P, uint n) */
/* // the same than get_map, but coordinates of the points */
/* // are given not on the spiral's reference plane but on */
/* // the seed plane */
/* // not used in the present code, actually */
/* { */
/*   map_index l,d; */
/*   uint mx, my, c; */
  
/*   P[x_] -= (n - 1) / 2; */
/*   P[y_] -= n / 2; */
  
/*   mx = abs(P[x_]); */
/*   my = abs(P[y_]); */
/*   l = 2 * max(mx, my); */

/*   c = (P[y_] > P[x_]) + (P[x_] > 0)*(P[x_] == P[y_]); */

/*   d = (c) ? l * 3 + P[x_] + P[y_] : l - P[x_] - P[y_]; */
  
/*   return (l - 1)*(l-1) + d; */
/* } */


// the following function would require #define _GNU_SOURCE
//  at the beginning of file, since it requires the usage
//  of non-standard qsort_r()

/* int cmp_map_points(const void *A, const void *B, void *plane) */
/* // comparison function to be called by qsort */
/* // performs indexed-sorting */
/* // this version works if map (that is the indexed */
/* //  array) is passed as third argument: it needs to be */
/* // called in a _GNU_SOURCE code. */
/* { */
/*   map_index *P = (map_index*)plane; */
/*   map_index a = P[*(uint*)A]; */
/*   map_index b = P[*(uint*)B]; */

/*   if(a < b) */
/*     return -1; */
/*   if( a > b) */
/*     return 1; */
/*   return 0; */
/* } */
