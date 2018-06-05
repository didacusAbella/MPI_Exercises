#ifndef LINE_H
#define LINE_H

/**
 * ADT per rappresentare la linea di un file
 * Una linea è composta da path del file, offset di inizio e fine
 **/
typedef struct line_t
{
  unsigned long start, offset;
  char file_name[1024];
} line_t;

/**
 * Alloca e crea una nuova linea
 **/
line_t * new_line(unsigned long start, unsigned long offset, char *fname);

/**
 * Libera spazio per una linea
 **/
void destroy_line(line_t *line);

/**
 * ADT Array dinamico di line_t
 **/
typedef struct line_array
{
  line_t *list;
  int size;
  int used;
} line_array;

/**
 * Alloca spazio per un nuovo array
 **/
line_array * new_line_array(int initial_size);

/**
 * Libera spazio per un array
 **/
void destroy_line_array(line_array *array);

/**
 * Aggiunge un elemento all'array
 **/
void push_line(line_array *array, line_t element);

#endif /* LINE_H */