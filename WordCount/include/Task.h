#ifndef TASK_H
#define TASK_H
#include "Line.h"
#include "Word.h"

typedef struct
{
  line_t *task_lines;
  word_t *task_words;
  int line_size;
  int word_size;
  int lines_used;
  int words_used;
}task_t;

void task_ctor(task_t **self, int l_size, int w_size);
void task_dtor(task_t *self);
void add_line(task_t *self, line_t *line);
void add_word(task_t *self, word_t *word);
void print_lines(task_t *self);
void print_words(task_t *self);
#endif