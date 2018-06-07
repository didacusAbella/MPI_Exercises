#include "LineVector.h"
#include <stdlib.h>

void new_line_vector(int initial_size, vector_line *vector)
{
  vector->lines = malloc(initial_size * sizeof(line_t));
  vector->size = initial_size;
  vector->used = 0;
}

void delete_line_vector(vector_line *vector)
{
  free(vector);
}

void add_line(vector_line *vector, line_t line)
{
  if(vector->used == vector->size)
  {
    vector->size *= 2;
    vector->lines = (line_t *) realloc(vector->lines, vector->size * sizeof(line_t));
  }
  vector->lines[vector->used] = line;
  vector->used++;
}