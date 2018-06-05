#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#define LOG_ON 1
#include "Logger.h"
#include <unistd.h>
#include "MapReduce.h"
#include "WordCount.h"

void send_lines(line_array *array, int rank, int slots)
{
  MPI_Datatype MPI_Line;
  int block_length[3] = {1024, 1, 1};
  MPI_Datatype types[3] = {MPI_CHAR, MPI_UNSIGNED_LONG, MPI_UNSIGNED_LONG};
  MPI_Aint offsets[3];
  offsets[0] = offsetof(line_t, file_name);
  offsets[1] = offsetof(line_t, start);
  offsets[2] = offsetof(line_t, offset);
  MPI_Type_create_struct(3, block_length, offsets, types, &MPI_Line);
  MPI_Type_commit(&MPI_Line);
  int *displacement, *slots_elements;
  int offset = 0;
  int size = array->used / (slots - 1);
  int remains = array->used % (slots - 1);
  int d = (remains == 0) ? size + 1 : size;
  line_t *recv_lines = malloc(d * sizeof(line_t));
  displacement = malloc(slots * sizeof(int));
  slots_elements = malloc(slots * sizeof(int));
  /*Master not read anything*/
  *(slots_elements + 0) = 0;
  *(displacement + 0) = 0;
  //Fill displacement
  for (int i = 1; i < slots; i++)
  {
    int t_size = (remains != 0 && i <= remains) ? size + 1 : size;
    *(slots_elements + i) = t_size;
    *(displacement + i) = offset;
    offset += t_size;
  }
  MPI_Scatterv(array->list, slots_elements, displacement, MPI_Line, recv_lines, d, MPI_Line, 0, MPI_COMM_WORLD);
  if(rank > 0)
  {
    for(int i = 0; i < slots_elements[rank]; i++)
    {
      info("Rank:%d, File:%s, Inizio:%lu, Fine:%lu\n", rank, (recv_lines+i)->file_name, (recv_lines+i)->start, (recv_lines+i)->offset);
    }
  }
  MPI_Type_free(&MPI_Line);
  free(slots_elements);
  free(displacement);
}

int main(int argc, char **argv)
{
  int slots, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &slots);
  line_array *array = line_splitter(argv[1], slots);
  send_lines(array, rank, slots);
  MPI_Finalize();
  exit(EXIT_SUCCESS);
}
