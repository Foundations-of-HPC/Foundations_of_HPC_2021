// a simple example to use MPI_Cart_shift
// stolen somewhere and just cleaned and commented
#include<mpi.h>
#include<stdio.h>
#define TRUE 1
#define FALSE 0
int main(int argc, char *argv[]) {
  int rank;
  MPI_Comm vu;
  int dim[2],period[2],reorder;
  int up,down,right,left;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  // define all the elements of the virtual topology
  dim[0]=4; dim[1]=3;
  period[0]=TRUE; period[1]=TRUE;
  reorder=TRUE;
  MPI_Cart_create(MPI_COMM_WORLD,2,dim,period,reorder,&vu);
  // randomly choosen process
  //if(rank==9){
   MPI_Cart_shift(vu,0,1,&left,&right);
   MPI_Cart_shift(vu,1,1,&up,&down);
   printf("P:%d My neighbors are right: %d down:%d left:%d up:%d\n",rank,right,down,left,up);
  //  }
  MPI_Finalize();
}
