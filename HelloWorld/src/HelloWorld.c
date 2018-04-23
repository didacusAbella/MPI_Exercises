#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include "HelloWorld.h"

void printMessage(char message[], int processId){
  sprintf(message, "Hello From MPI %d\n", processId);
}

int main(int argc, char **argv){
  int myrank, p, dest, source, i;
  int tag = 0;
  char message[100];
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  if(myrank != 0){
    printMessage(message, myrank);
    dest = 0;
    for(i=0; i < 100000; i++) ;
    MPI_Send(message, strlen(message)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
  }else {
    printf("Hello from process 0: Num processes %d\n", p);
    for(source = 1; source < p; source++){
      MPI_Recv(message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
      printf("%s", message);
    }
  }
  MPI_Finalize();
  return (EXIT_SUCCESS);
}
