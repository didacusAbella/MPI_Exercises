#ifndef WORDVECTOR_H
#define WORDVECTOR_H
#include "Word.h"
typedef struct
{
  int size;
  int used;
  word_t *words;
} vector_word;

void new_word_vector(int initial_size, vector_word *vector);

void delete_word_vector(vector_word *vector);

void add_word(vector_word *vector, word_t line);

int contains_word(vector_word *vector, char *word);
#endif