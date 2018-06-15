#include "Task.h"
#include <stdlib.h>
#include <stdio.h>

void task_ctor(task_t **self, int l_size, int w_size)
{
  (*self) = (task_t *) malloc(sizeof(task_t));
  (*self)->line_size = l_size;
  (*self)->word_size = w_size;
  (*self)->lines_used = 0;
  (*self)->words_used = 0;
  (*self)->task_lines = (line_t *) malloc(l_size * sizeof(line_t));
  (*self)->task_words = (word_t *) malloc(w_size * sizeof(word_t));
}
void task_dtor(task_t *self)
{
  if(self)
  {
    free(self->task_lines);
    free(self->task_words);
    free(self);
  }
}

void add_line(task_t *self, line_t *line)
{
  if(self->lines_used == self->line_size)
  {
    self->line_size *= 2;
    self->task_lines = realloc(self->task_lines, self->line_size * sizeof(line_t));
  }
  self->task_lines[self->lines_used] = *line;
  self->lines_used++;
}
void add_word(task_t *self, word_t *word)
{
  if(self->words_used == self->word_size)
  {
    self->word_size *= 2;
    self->task_words = realloc(self->task_words, self->word_size * sizeof(word_t));
  }
  self->task_words[self->words_used] = *word;
  self->words_used++;
}

void print_lines(task_t *self)
{
  for(int i = 0; i < self->lines_used; i++)
  {
    printf("File: %s, Inizio: %lu, Fine: %lu\n", self->task_lines[i].file, self->task_lines[i].start, self->task_lines[i].end);
  }
}

void print_words(task_t *self)
{
  for(int i = 0; i < self->words_used; i++)
  {
    printf("Parola: %s --> %d\n", self->task_words[i].text, self->task_words[i].frequency);
  }
}