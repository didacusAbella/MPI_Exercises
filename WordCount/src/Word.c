#include "Word.h"
#include <stdlib.h>
#include <string.h>

word_t * new_word(char *word)
{
  word_t *nword = malloc(sizeof(word_t));
  strcpy(nword->word, word);
  nword->frequency = 1;
  return nword;
}

void delete_word(word_t *word)
{
  free(word->word);
  free(word);
}

void create_mpi_word(MPI_Datatype *wdt)
{
  int block_length[2] = {100, 1};
  MPI_Datatype types[2] = {MPI_CHAR, MPI_INT};
  MPI_Aint offsets[2];
  offsets[0] = offsetof(word_t, word);
  offsets[1] = offsetof(word_t, frequency);
  MPI_Type_create_struct(2, block_length, offsets, types, wdt);
  MPI_Type_commit(wdt);
}