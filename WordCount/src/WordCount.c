#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "WordCount.h"
#include "MapReduce.h"

int main(int argc, char **argv){
  int rank, tasks;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &tasks);
  MPI_Datatype MPI_Line;
  MPI_Datatype MPI_Word;
  create_mpi_line(&MPI_Line);
  create_mpi_word(&MPI_Word);
  vector_line *arr;
  vector_line *lines;
  arr = line_splitter(argv[1]);
  lines = send_lines(arr, rank, tasks, MPI_Line);
  if(rank >  0)
  {
    vector_word *vmap = map(lines);
    vector_word *vreduce = reduce(vmap);
    int index = 0;
    while(index < vreduce->used)
    {
      printf("%s -- %d\n", vreduce->words[index].word, vreduce->words[index].frequency);
      index++;
    }
    delete_line_vector(lines);
    delete_word_vector(vmap);
    //delete_word_vector(vreduce);
  }
  MPI_Type_free(&MPI_Line);
  MPI_Type_free(&MPI_Word);
  MPI_Finalize();
  exit(EXIT_SUCCESS);
}

vector_line * send_lines(vector_line *arr, int rank, int tasks, MPI_Datatype dt)
{
  int *displacements, *tasks_elements;
  int offset = 0;
  int task_size = arr->used / (tasks - 1);
  int remains = arr->used % (tasks - 1);
  displacements = malloc(tasks * sizeof(int));
  tasks_elements = malloc(tasks * sizeof(int));
  *(tasks_elements + 0) = 0;
  *(displacements + 0) = 0;
  for(int i = 1; i < tasks; i++)
  {
    int t_size = (remains != 0 && i <= remains) ? task_size + 1 : task_size;
    *(tasks_elements + i) = t_size;
    *(displacements + i) = offset;
    offset += t_size;
  }
  line_t *receive_lines = malloc(tasks_elements[rank] * sizeof(line_t)); 
  MPI_Scatterv(arr->lines, tasks_elements, displacements, dt, receive_lines, tasks_elements[rank], dt, 0, MPI_COMM_WORLD);
  vector_line *vl = new_line_vector(tasks_elements[rank]);
  vl->lines = receive_lines;
  vl->used = tasks_elements[rank];
  vl->size = tasks_elements[rank];
  free(displacements);
  free(tasks_elements);
  return vl;
}