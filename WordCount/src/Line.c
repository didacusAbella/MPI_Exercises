#include <string.h>
#include <stdlib.h>
#include <mpi.h>
#include "Line.h"

void line_ctor(line_t **self, char *path, unsigned long start, unsigned long end)
{
  *self = NULL;
  *self = (line_t *) malloc(sizeof(line_t));
  strcpy((*self)->file, path);
  (*self)->start = start;
  (*self)->end = end;
}

void line_dtor(line_t *self)
{
  if(self)
  {
    free(self);
  }
}

int line_length[3] = {255, 1, 1};

MPI_Datatype line_types[3] = { MPI_CHAR, MPI_UNSIGNED_LONG, MPI_UNSIGNED_LONG };

MPI_Aint line_addresses[3] = { offsetof(line_t, file), offsetof(line_t, start), offsetof(line_t, end) };