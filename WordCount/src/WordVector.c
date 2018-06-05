#include "WordVector.h"
#include <stdlib.h>

vector_word * new_word_vector(int initial_size)
{
  vector_word *vector = malloc(sizeof(vector_word));
  vector->words = malloc(initial_size * sizeof(word_t));
  vector->size = initial_size;
  vector->used = 0;
  return vector;
}

void delete_word_vector(vector_word *vector)
{
  free(vector);
}

void add_word(vector_word *vector, word_t word)
{
  if(vector->used == vector->size)
  {
    vector->size *= 2;
    vector->words = (word_t *) realloc(vector->words, vector->size * sizeof(word_t));
  }
  vector->words[vector->used] = word;
  vector->used++;
}