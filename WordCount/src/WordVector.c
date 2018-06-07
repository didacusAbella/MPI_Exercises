#include "WordVector.h"
#include <stdlib.h>
#include <string.h>

void  new_word_vector(int initial_size, vector_word *vector)
{
  vector->words = malloc(initial_size * sizeof(word_t));
  vector->size = initial_size;
  vector->used = 0;
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

int contains_word(vector_word *vector, char *word)
{
  for(int i = 0; i < vector->used; i++)
  {
    if(strcmp(vector->words[i].word, word) == 0){
      return i;
    }
  }
  return -1;
}