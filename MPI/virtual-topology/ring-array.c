// a simple implementation of the ring exercises
// taken from Bill Gropp's tutorial and slightly adapted
#include <stdio.h>
#include "mpi.h"

int main( argc, argv )
int argc;
char **argv;
{
    int rank, value, size;
    MPI_Status status;

    MPI_Init( &argc, &argv );
    value=20;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    if (rank == 0) {
            
	    MPI_Send( &value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD );
	}
	else {
	    MPI_Recv( &value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, 
		      &status );
	    if (rank < size - 1) 
		MPI_Send( &value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD );
	}
	printf( "Process %d got %d\n", rank, value );

    MPI_Finalize( );
    return 0;
}
