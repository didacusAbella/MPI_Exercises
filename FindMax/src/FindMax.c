#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "FindMax.h"

int max(int list[], int size){
  int i = 0;
  int tmpMax = list[0];
  while(i < size){
    if(list[i] > tmpMax){
      tmpMax = list[i];
    }
    i++;
  }
  return tmpMax;
}

void generate_numbers(int list[], int size){
  srand(444);
  for(int i = 0; i < size; i++){
    list[i] = rand() % 50 + 1;
  }
}

int main(int argc, char **argv){
  MPI_Init(&argc, &argv);
  int rank, slots, block_size, remainder, size, tmpMax;
  MPI_Status status;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &slots);
  size = atoi(argv[1]);
  block_size = size / slots;
  remainder = size % slots;
  if(rank == 0){
    int *list = (int *) malloc(size * sizeof(int));
    int trueMax = 0;
    int offset = 0;
    generate_numbers(list, size);
    for(int i = 1; i < slots; i++){
      int tmp = (remainder != 0 && rank < remainder) ? block_size + 1 : block_size;
      MPI_Send(list + offset, tmp, MPI_INT, i, DEFAULT_TAG, MPI_COMM_WORLD);
      MPI_Recv(&tmpMax, 1, MPI_INT, i, DEFAULT_TAG, MPI_COMM_WORLD, &status);
      if(tmpMax > trueMax){
        trueMax = tmpMax;
      }
      offset += tmp;
    }
    free(list);
    printf("Max: %d\n", trueMax);
  }else {
    int tsize = (remainder != 0 && rank < remainder) ? block_size + 1 : block_size;
    int *tmpList = (int *) malloc(tsize * sizeof(int));
    MPI_Recv(tmpList, tsize, MPI_INT, 0, DEFAULT_TAG, MPI_COMM_WORLD, &status);
    tmpMax = max(tmpList, tsize);
    MPI_Send(&tmpMax, 1, MPI_INT, 0, DEFAULT_TAG, MPI_COMM_WORLD);
    free(tmpList);
  }
  MPI_Finalize();
  exit(EXIT_SUCCESS);
}
