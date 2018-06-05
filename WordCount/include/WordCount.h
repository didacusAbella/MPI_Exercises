#ifndef WORDCOUNT_H
#define WORDCOUNT_H
#define MASTER 0
#include "LineVector.h"
vector_line * send_lines(vector_line *arr, int rank, int tasks, MPI_Datatype dt);
#endif