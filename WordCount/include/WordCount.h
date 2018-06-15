#ifndef WORDCOUNT_H
#define WORDCOUNT_H
#define MASTER 0
#include "Task.h"
void send_lines(task_t **send_buffer, int rank, int tasks, MPI_Datatype MPI_Line, task_t **receive_buffer);
void receive_words(task_t **send_buffer, int rank, int tasks, MPI_Datatype MPI_Word, task_t **receive_buffer);
#endif