#ifndef WORD_H
#define WORD_H
#include <mpi.h>
#define MAX_WORD_CHARS 100
typedef struct
{
  char text[MAX_WORD_CHARS];
  int frequency;
} word_t;

/**
 * Allocation plus initialization
 **/
void word_ctor(word_t **self, char *word);

/**
 * Deallocation plus reset
 **/
void word_dtor(word_t *self);

extern const int word_length[2];

extern const MPI_Datatype word_types[2];

extern const MPI_Aint word_addresses[2];

#endif