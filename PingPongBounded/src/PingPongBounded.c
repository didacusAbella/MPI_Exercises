#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "PingPongBounded.h"

void master_work(int limit, int slots)
{
  int value = 0;
  unsigned short terminate = 0;
  MPI_Status status;
  printf("Initial value %d\n", value);
  while (1)
  {
    if (value < limit)
    {
      for (int i = 1; i < slots; i++)
      {
        MPI_Send(&terminate, 1, MPI_UNSIGNED_SHORT, i, DEFAULT_TAG, MPI_COMM_WORLD);
      }
      MPI_Send(&value, 1, MPI_INT, 1, DEFAULT_TAG, MPI_COMM_WORLD);
      MPI_Recv(&value, 1, MPI_INT, slots - 1, DEFAULT_TAG, MPI_COMM_WORLD, &status);
    }
    else
    {
      terminate = 1;
      for (int i = 1; i < slots; i++)
      {
        MPI_Send(&terminate, 1, MPI_UNSIGNED_SHORT, i, DEFAULT_TAG, MPI_COMM_WORLD);
      }
      break;
    }
  }
  printf("Final Value:%d\n", value);
  printf("Exiting master\n");
}
void slave_work(int rank, int slots)
{
  int value;
  unsigned short terminate;
  MPI_Status status;
  while (1)
  {
    MPI_Recv(&terminate, 1, MPI_INT, MASTER, DEFAULT_TAG, MPI_COMM_WORLD, &status);
    if (terminate == 1)
    {
      break;
    }
    else
    {
      if(rank == (slots - 1))
      {
        MPI_Recv(&value, 1, MPI_INT, rank - 1, DEFAULT_TAG, MPI_COMM_WORLD, &status);
        value++;
        MPI_Send(&value, 1, MPI_INT, MASTER, DEFAULT_TAG, MPI_COMM_WORLD);
      }
      else
      {
        MPI_Recv(&value, 1, MPI_INT, rank - 1, DEFAULT_TAG, MPI_COMM_WORLD, &status);
        value++;
        MPI_Send(&value, 1, MPI_INT, rank + 1, DEFAULT_TAG, MPI_COMM_WORLD);
      }
    }
  }
  printf("Exit process rank %d\n", rank);
}

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);
  int rank, slots;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &slots);
  if (rank == MASTER)
  {
    master_work(atoi(argv[1]), slots);
  }
  else
  {
    slave_work(rank, slots);
  }
  MPI_Finalize();
  exit(EXIT_SUCCESS);
}
