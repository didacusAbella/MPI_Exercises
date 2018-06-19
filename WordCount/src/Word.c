#include <string.h>
#include <stdlib.h>
#include "Word.h"

void word_ctor(word_t **self, char *word)
{
  (*self) = (word_t *) malloc(sizeof(word_t));
  strcpy((*self)->text, word);
  (*self)->frequency = 1;
}
void word_dtor(word_t *self)
{
  if(self)
  {
    free(self);
  }
}

int word_length[2] =  {100, 1};

MPI_Datatype word_types[2] = {MPI_CHAR, MPI_INT};

MPI_Aint word_addresses[2] = { offsetof(word_t, text), offsetof(word_t, frequency) };