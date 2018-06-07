#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "WordCount.h"
#include "MapReduce.h"

int main(int argc, char **argv)
{
  int rank, tasks;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &tasks);
  MPI_Datatype MPI_Line;
  MPI_Datatype MPI_Word;
  create_mpi_line(&MPI_Line);
  create_mpi_word(&MPI_Word);
  vector_line *all_lines;
  vector_line *tasks_lines;
  vector_word *mvector;
  vector_word cvector;
  vector_word *rc_words;
  vector_word rvector;
  if(rank == MASTER)
  {
    all_lines = (vector_line *) malloc(sizeof(vector_line));
    line_splitter(argv[1], all_lines);
  }
  tasks_lines = send_lines(all_lines, rank, tasks, MPI_Line);
  if(rank > MASTER)
  {
    mvector = (vector_word *) malloc(sizeof(vector_word));
    map(tasks_lines, mvector);
    delete_line_vector(tasks_lines);
    reduce(mvector, &cvector);
    delete_word_vector(mvector);
  }
  rc_words = receive_words(&cvector, MPI_Word, tasks, rank);
  if(rank == MASTER)
  {
    delete_line_vector(all_lines);
    reduce(rc_words, &rvector);
    for(int i = 0; i < rvector.used; i++)
    {
      printf("%s - %d\n", rvector.words[i].word, rvector.words[i].frequency);
    }
    delete_word_vector(rc_words);
  }
  MPI_Type_free(&MPI_Line);
  MPI_Type_free(&MPI_Word);
  MPI_Finalize();
  exit(EXIT_SUCCESS);
}

vector_line *send_lines(vector_line *arr, int rank, int tasks, MPI_Datatype dt)
{
  int rcv_lines;
  int *task_elements;
  int *displacement;
  if(rank == MASTER)
  {
    int offset = 0;
    int task_size = arr->used / (tasks - 1);
    int remains = arr->used % (tasks - 1);
    task_elements = malloc(tasks*sizeof(int));
    displacement = malloc(tasks*sizeof(int));
    *(task_elements + 0) = 0;
    *(displacement + 0) = 0;
    for(int i = 1; i < tasks; i++)
    {
      int t_size = (remains != 0 && i <= remains) ? task_size + 1 : task_size;
      *(task_elements + i) = t_size;
      *(displacement + i) = offset;
      offset += t_size;
    }
  }
  MPI_Scatter(task_elements, 1, MPI_INT, &rcv_lines, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
  line_t *buff_lines = malloc(rcv_lines *sizeof(line_t));
  MPI_Scatterv(arr->lines, task_elements, displacement, dt, buff_lines, rcv_lines, dt, MASTER, MPI_COMM_WORLD);
  vector_line *vl = malloc(sizeof(vector_line));
  vl->size = rcv_lines;
  vl->used = rcv_lines;
  vl->lines = buff_lines;
  if(rank == MASTER)
  {
    free(displacement);
    free(task_elements);
  }
  return vl;
}

vector_word *receive_words(vector_word *arr, MPI_Datatype dt, int tasks, int rank)
{
  int *receive_size;
  int *displacement;
  word_t *receive_ws;
  int offset = 0;
  int size = (rank == MASTER) ? 0 : arr->used;
  if(rank == MASTER)
  {
    receive_size = malloc(tasks*sizeof(int));
    displacement = malloc(tasks*sizeof(int));
  }
  MPI_Gather(&size, 1, MPI_INT, receive_size, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
  if(rank == MASTER)
  {
    *(displacement + 0) = 0;
    for(int i = 1; i < tasks; i++)
    {
      *(displacement + i) = offset;
      offset += *(receive_size + i);
    }
    receive_ws = (word_t *) malloc(offset *sizeof(word_t));
  }
  MPI_Gatherv(arr->words, size, dt, receive_ws, receive_size, displacement, dt, MASTER, MPI_COMM_WORLD);
  vector_word *vw = malloc(sizeof(vector_word));
  vw->used = offset;
  vw->size = offset;
  vw->words = receive_ws;
  if(rank == MASTER)
  {
    free(receive_size);
    free(displacement);
  }
  return vw;
}