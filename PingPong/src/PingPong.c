#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "PingPong.h"

void ping(int *ball){
  printf("Rispondo %d\n", ++(*ball));
}

int main(int argc, char **argv){
  int myrank, numtasks, ball;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  ball = 10;
  if(myrank == 0){
    printf("Ping: %d\n", ball);
    MPI_Send(&ball, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    MPI_Recv(&ball, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
    printf("Pong: %d\n", ball);
  }else if(myrank == 1) {
    MPI_Recv(&ball, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    ping(&ball);
    MPI_Send(&ball, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  MPI_Finalize();
  exit(EXIT_SUCCESS);
}
