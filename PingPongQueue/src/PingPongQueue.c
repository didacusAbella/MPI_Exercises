#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "PingPongQueue.h"

void loginfo(int from, int to, int value){
  printf("---Ping from %d to %d--- Value:%d\n", from, to, value);
}

int main(int argc, char **argv){
  MPI_Init(&argc, &argv);
  int ball, slots, rank;
  MPI_Status status;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &slots);
  if(rank == MASTER){
    ball = 0;
    loginfo(rank, 1, ball);
		MPI_Send(&ball, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD);
		MPI_Recv(&ball, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD, &status);
		printf("Valore finale di ball:%d\n", ball);
  }else if (rank == 1){
    MPI_Recv(&ball, 1, MPI_INT, MASTER, TAG, MPI_COMM_WORLD, &status);
		ball++;
		loginfo(1, rank+1, ball);
		MPI_Send(&ball, 1, MPI_INT, rank +1, TAG, MPI_COMM_WORLD);
		MPI_Recv(&ball, 1, MPI_INT, slots-1, TAG, MPI_COMM_WORLD, &status);
		ball++;
		loginfo(1, MASTER, ball);
		MPI_Send(&ball, 1, MPI_INT, MASTER, TAG, MPI_COMM_WORLD);
  }else if(rank == (slots - 1)){
    MPI_Recv(&ball, 1, MPI_INT, rank - 1, TAG, MPI_COMM_WORLD, &status);
		ball++;
		loginfo(slots, 1, ball);
		MPI_Send(&ball, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD);
  } else {
    MPI_Recv(&ball, 1, MPI_INT, rank - 1, TAG, MPI_COMM_WORLD, &status);
		ball++;
		loginfo(rank, rank+1, ball);
		MPI_Send(&ball, 1, MPI_INT, rank + 1, TAG, MPI_COMM_WORLD);
  }
  MPI_Finalize();
  exit(EXIT_SUCCESS);
}
