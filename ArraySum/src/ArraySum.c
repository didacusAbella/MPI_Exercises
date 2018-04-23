#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "ArraySum.h"

int sum(int list[], int size){
  int sum = 0;
  for(int i=0; i < size; i++){
    sum += list[i];
  }
  return sum;
}

void generate_numbers(int list[], int size){
  srand(444);
  for(int i=0; i < size; i++){
    list[i] = rand() % 10 + 1;
  }
}

int main(int argc, char **argv){
  MPI_Init(&argc, &argv);
  if(argc != 2){
    if(atoi(argv[1]) <= 0){
      printf("Can't create zero o negative list\n");
      exit(EXIT_FAILURE);
    }
    printf("Usage <size>\n");
    exit(EXIT_FAILURE);
  }
  int block_size, slots, tsize, remainder, rank, size;
  MPI_Status status;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &slots);
  size = atoi(argv[1]);
  block_size = size / slots;
  remainder = size % slots;
  if(rank == MASTER){
    int totalSum = 0;
    int sumReceived;
    int *list = (int*) malloc(size * sizeof(int));
    printf("Generate numbers of size %d\n", size);
    generate_numbers(list, size);
    int offset = 0;
    for(int i = 1; i < slots; i++){
      tsize = (remainder != 0 && i <= remainder) ? block_size + 1 : block_size;
      MPI_Send(list + offset, tsize, MPI_INT, i, TAG, MPI_COMM_WORLD);
      sumReceived = 0;
      MPI_Recv(&sumReceived, 1, MPI_INT, i, TAG, MPI_COMM_WORLD, &status);
      totalSum += sumReceived;
      offset += tsize;
    }
    printf("Somma totale %d\n", totalSum);
    free(list);
  }else {
    tsize = (remainder != 0 && rank <= remainder) ? block_size + 1 : block_size;
    int *msg = (int*) malloc(tsize * sizeof(int)); 
    MPI_Recv(msg, tsize, MPI_INT, MASTER, TAG, MPI_COMM_WORLD, &status);
    int psum = sum(msg, tsize);
    MPI_Send(&psum, 1, MPI_INT, MASTER, TAG, MPI_COMM_WORLD);
    free(msg);
  }
  MPI_Finalize();
  exit(EXIT_SUCCESS);
}
