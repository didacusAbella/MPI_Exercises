#ifndef LINE_H
#define LINE_H
#include <mpi.h>
#define MPI_LINE_OFFSETS { offsetof(line_t, file), offsetof(line_t, start), offsetof(line_t, end) }
#define PATH_LENGTH 255
typedef struct
{
  char file[PATH_LENGTH];
  unsigned long start;
  unsigned long end;
}line_t;


/**
 * Allocation plus initialization
 **/
void line_ctor(line_t **self, char *path, unsigned long start, unsigned long end);

/**
 * Deallocation + reset
 **/
void line_dtor(line_t *self);

extern const int line_length[3];

extern const MPI_Datatype line_types[3];

extern const MPI_Aint line_addresses[3];

#endif