#ifndef LINE_H
#define LINE_H
#define MAX_LINE 1024
#include <mpi.h>

typedef struct 
{
  char file[MAX_LINE];
  unsigned long start;
  unsigned long end;
}line_t;

line_t * new_line(char *file, unsigned long start, unsigned long offset);

void delete_line(line_t *line);

void create_mpi_line(MPI_Datatype *ldt);
#endif /*LINE_H*/