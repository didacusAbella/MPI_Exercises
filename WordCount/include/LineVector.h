#ifndef LINEVECTOR_H
#define LINEVECTOR_H
#include "Line.h"
typedef struct {
  int used;
  int size;
  line_t *lines;
} vector_line;

void new_line_vector(int initial_size, vector_line *vector);

void delete_line_vector(vector_line *vector);

void add_line(vector_line *vector, line_t line);

#endif