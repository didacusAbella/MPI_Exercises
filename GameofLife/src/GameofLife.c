#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "GameofLife.h"

void start_simulation(matrix_t *mt, int steps, int rank)
{
  MPI_Status status;
  int sum;
  int toRead = mt->relCle;
  for(int n = 0; n < steps; n++)
  {
    for(int i = mt->relRws; i <= mt->relRwe; i++)
    {
      set_cell(mt, i, 0, get_cell(mt, i, mt->relCle, OLD_M), OLD_M);
      set_cell(mt, i, mt->relCle+1, get_cell(mt, i, 1, OLD_M), OLD_M);
    }
    if(rank == MASTER)
    {
      MPI_Send(mt->oldMatrix + offset(mt->relRwe, 0, mt), toRead, MPI_INT, 1, TAG, MPI_COMM_WORLD);
      MPI_Recv(mt->oldMatrix + offset(mt->relRwe + 1, 0, mt), toRead, MPI_INT, 1, TAG, MPI_COMM_WORLD, &status);
      MPI_Send(mt->oldMatrix + offset(1, 0, mt), toRead, MPI_INT, 1, TAG, MPI_COMM_WORLD);
      MPI_Recv(mt->oldMatrix + offset(0, 0, mt), toRead, MPI_INT, 1, TAG, MPI_COMM_WORLD, &status);

      /*Corners*/
      MPI_Send(mt->oldMatrix + offset(1, 1, mt), 1, MPI_INT, 1, TAG, MPI_COMM_WORLD);
      MPI_Recv(mt->oldMatrix + offset(0, 0, mt), 1, MPI_INT, 1, TAG, MPI_COMM_WORLD, &status);
      MPI_Send(mt->oldMatrix + offset(1, mt->relCle, mt), 1, MPI_INT, 1, TAG, MPI_COMM_WORLD);
      MPI_Recv(mt->oldMatrix + offset(0, mt->relCle, mt), 1, MPI_INT, 1, TAG, MPI_COMM_WORLD, &status);
    }
    else
    {
      MPI_Recv(mt->oldMatrix + offset(0, 0, mt), toRead, MPI_INT, MASTER, TAG, MPI_COMM_WORLD, &status);
      MPI_Send(mt->oldMatrix + offset(1, 0, mt), toRead, MPI_INT, MASTER, TAG, MPI_COMM_WORLD);
      MPI_Recv(mt->oldMatrix + offset(mt->relRwe, 0, mt), toRead, MPI_INT, MASTER, TAG, MPI_COMM_WORLD, &status);
      MPI_Send(mt->oldMatrix+ offset(mt->relRwe + 1, 0, mt), toRead, MPI_INT, MASTER, TAG, MPI_COMM_WORLD);
      /*Corners*/
      MPI_Recv(mt->oldMatrix + offset(mt->relRwe + 1, mt->relCle, mt), 1, MPI_INT, MASTER, TAG, MPI_COMM_WORLD, &status);
      MPI_Send(mt->oldMatrix + offset(mt->relRwe, mt->relCle, mt), 1, MPI_INT, MASTER, TAG, MPI_COMM_WORLD);
      MPI_Recv(mt->oldMatrix + offset(mt->relRwe + 1, 0, mt), 1, MPI_INT, MASTER, TAG, MPI_COMM_WORLD, &status);
      MPI_Send(mt->oldMatrix + offset(mt->relRwe, 1, mt), 1, MPI_INT, MASTER, TAG, MPI_COMM_WORLD);
    }
    for(int i = mt->relRws; i <= mt->relRwe; i++)
    {
      for(int j = mt->relCls; j <= mt->relCle; j++)
      {
        int it = i+1;
        int ib = i-1;
        int jl = j-1;
        int jr = j+1;
        sum =  get_cell(mt, i, it, OLD_M) + get_cell(mt, i, ib, OLD_M) + get_cell(mt, i, jl, OLD_M) + get_cell(mt, i, jr, OLD_M)
          + get_cell(mt, it, jl, OLD_M) + get_cell(mt, it, jr, OLD_M) + get_cell(mt, ib, jr, OLD_M) + get_cell(mt, ib, jl, OLD_M);
        switch(sum)
        {
          case 3:
          set_cell(mt, i, j, 1, NEW_M);
          break;
          case 2:
          set_cell(mt, i, j, get_cell(mt, i, j, OLD_M), NEW_M);
          break;
          default:
          set_cell(mt, i, j, 0, NEW_M);
        }
      }
    }
    for(int i = mt->relRws; i <= mt->relRwe; i++)
    {
      for(int j = mt->relCls; j <= mt->relCle; j++)
      {
        set_cell(mt, i, j, get_cell(mt, i, j, NEW_M), OLD_M);
      }
    }
  }
}

int cells_alive(matrix_t *mt)
{
  int sum = 0;
  for(int i = mt->relRws; i <= mt->relRwe; i++)
  {
    for(int j = mt->relCls; j <= mt->relCle; j++)
    {
      sum += get_cell(mt, i, j, NEW_M);
    }
  }
  return sum;
}

int main(int argc, char **argv)
{
  if(argc != 4)
  {
    perror("Usage: <rows> <columns> <steps>\n");
    exit(EXIT_FAILURE);
  }
  int rank, slots;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &slots);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int rows = atoi(argv[1]);
  int columns = atoi(argv[2]);
  int steps = atoi(argv[3]);
  int tsum = 0;
  matrix_t *mt = alloc_matrix(rows+4, columns, rank);
  start_simulation(mt, steps, rank);
  tsum = cells_alive(mt);
  if(rank == MASTER)
  {
    int psum = tsum;
    int rsum = 0;
    MPI_Recv(&rsum, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD, &status);
    psum += rsum;
    printf("Celle vive %d\n", psum);
  }
  else 
  {
    MPI_Send(&tsum, 1, MPI_INT, MASTER, TAG, MPI_COMM_WORLD);
  }
  dealloc_matrix(mt);
  MPI_Finalize();
  exit(EXIT_SUCCESS);
}