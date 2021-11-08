#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[])        
{
#define MSGLEN 2048
  int ITAG_A = 100,ITAG_B = 200; 
  int irank, i, idest, isrc, istag, iretag;
  float rmsg1[MSGLEN];
  float rmsg2[MSGLEN];
  MPI_Status recv_status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &irank);  

  for (i = 1; i <= MSGLEN; i++)
    {
      rmsg1[i] = 100;
      rmsg2[i] = -100;
    }
  if ( irank == 0 )
    { 
      idest  = 1;
      isrc   = 1;
      istag  = ITAG_A;
      iretag = ITAG_B;
    }
  else if ( irank == 1 )
    {
      idest  = 0;
      isrc   = 0;
      istag  = ITAG_B;
      iretag = ITAG_A;
    }

  printf("Task %d has sent the message\n", irank);
  MPI_Ssend(&rmsg1, MSGLEN, MPI_FLOAT, idest, istag, MPI_COMM_WORLD); 
  MPI_Recv(&rmsg2, MSGLEN, MPI_FLOAT, isrc, iretag, MPI_COMM_WORLD, &recv_status);
  printf("Task %d has received the message\n", irank);
  MPI_Finalize();
}
