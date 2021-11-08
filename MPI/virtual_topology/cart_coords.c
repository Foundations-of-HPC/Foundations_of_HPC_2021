// a simple example to show mpi_cart_coord and mpi_cart_rank
// stolen somewhere on the net and commented 
#include<mpi.h>
#include<stdio.h> 
#define TRUE 1
#define FALSE 0
/* Run with 12 processes */
int main(int argc, char *argv[]) 
{
 int rank;
 MPI_Comm vu;
 int dim[2],period[2],reorder;
 int coord[2],id;
 MPI_Init(&argc, &argv);
 MPI_Comm_rank(MPI_COMM_WORLD,&rank);
 dim[0]=4; dim[1]=3;
 period[0]=TRUE; period[1]=FALSE;
 reorder=TRUE;
 // create the topology 
 MPI_Cart_create(MPI_COMM_WORLD,2,dim,period,reorder,&vu);
 // identify coordinates of rank 5 
 if(rank==5){
    MPI_Cart_coords(vu,rank,2,coord);
    printf("P:%d My coordinates are %d %d\n",rank,coord[0],coord[1]);
    }
 if(rank==0) {
   coord[0]=3; coord[1]=1;
   MPI_Cart_rank(vu,coord,&id);
   printf("The processor at position (%d, %d) has rank %d\n",coord[0],coord[1],id);
  }
 MPI_Finalize();
}
