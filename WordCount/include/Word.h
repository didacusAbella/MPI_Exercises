#ifndef WORD_H
#define WORD_H
#define MAX_WORD 100
#include <mpi.h>
typedef struct 
{
  char word[MAX_WORD];
  int frequency;
} word_t;

word_t * new_word(char *word);

void delete_word(word_t *word);

void create_mpi_word(MPI_Datatype *wdt);
#endif