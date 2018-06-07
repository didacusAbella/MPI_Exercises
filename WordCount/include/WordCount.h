#ifndef WORDCOUNT_H
#define WORDCOUNT_H
#define MASTER 0
#include "LineVector.h"
#include "WordVector.h"
vector_line * send_lines(vector_line *arr, int rank, int tasks, MPI_Datatype dt);
vector_word * receive_words(vector_word *arr, MPI_Datatype dt, int tasks, int rank);
#endif