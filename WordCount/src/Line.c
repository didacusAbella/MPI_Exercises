#include "Line.h"
#include <stdlib.h>
#include <string.h>

line_t * new_line(char *file, unsigned long start, unsigned long offset)
{
  line_t *line = malloc(sizeof(line_t));
  strcpy(line->file, file);
  line->start = start;
  line->end = offset;
  return line;
}

void delete_line(line_t *line)
{
  free(line->file);
  free(line);
}

void create_mpi_line(MPI_Datatype *ldt)
{
  int block_length[3] = {1024, 1, 1};
  MPI_Datatype types[3] = {MPI_CHAR, MPI_UNSIGNED_LONG, MPI_UNSIGNED_LONG};
  MPI_Aint offsets[3];
  offsets[0] = offsetof(line_t, file);
  offsets[1] = offsetof(line_t, start);
  offsets[2] = offsetof(line_t, end);
  MPI_Type_create_struct(3, block_length, offsets, types, ldt);
  MPI_Type_commit(ldt);
}