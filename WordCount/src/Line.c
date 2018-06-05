#include <stdlib.h>
#include <string.h>
#include "Line.h"

line_t * new_line(unsigned long start, unsigned long offset, char *fname)
{
  line_t *line = malloc(sizeof(line_t));
  strcpy(line->file_name, fname);
  line->start = start;
  line->offset = offset;
  return line;
}

void destroy_line(line_t *line)
{
  free(line->file_name);
  free(line);
}

line_array * new_line_array(int initial_size)
{
  line_array *array = (line_array *) malloc(sizeof(line_array));
  array->list = (line_t*) malloc(initial_size * sizeof(line_t));
  array->size = initial_size;
  array->used = 0;
  return array;
}

void destroy_line_array(line_array *array)
{
  free(array->list);
  array->list = NULL;
  array->used = array->size = 0;
  free(array);
  array = NULL;
}

void push_line(line_array *array, line_t element)
{
  if(array->used == array->size)
  {
    array->size *= 2;
    array->list = (line_t *) realloc(array->list, array->size * sizeof(line_t));
  }
  array->list[array->used] = element;
  array->used++;
}