#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "WordCount.h"
#include "MapReduce.h"

int main(int argc, char **argv){
  int rank, tasks;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &tasks);
  /************* Create custom datatypes *****/
  MPI_Datatype MPI_Word;
  MPI_Datatype MPI_Line;
  MPI_Type_create_struct(3, line_length, line_addresses, line_types, &MPI_Line);
  MPI_Type_create_struct(2, word_length, word_addresses, word_types, &MPI_Word);
  MPI_Type_commit(&MPI_Line);
  MPI_Type_commit(&MPI_Word);
  /*******************************************/
  double start_time, end_time;
  task_t *master_task, *slave_task;
  if(rank == MASTER)
  {
    start_time = MPI_Wtime();
    task_ctor(&master_task, 10, 10);
    line_splitter(argv[1], &master_task);
  }
  send_lines(&master_task, rank, tasks, MPI_Line, &slave_task);
  if(rank > MASTER)
  {
    map(&slave_task);
    reduce(&slave_task);
  }
  receive_words(&slave_task, rank, tasks, MPI_Word, &master_task);
  if(rank == MASTER)
  {
    reduce(&master_task);
    print_words(master_task);
    task_dtor(master_task);
    end_time = MPI_Wtime();
    double tot = end_time - start_time;
    printf("Tempo: %g\n", tot);
  }
  else
  {
    task_dtor(slave_task);
  }
  MPI_Type_free(&MPI_Line);
  MPI_Type_free(&MPI_Word);
  MPI_Finalize();
  exit(EXIT_SUCCESS);
}

void send_lines(task_t **send_buffer, int rank, int tasks, MPI_Datatype MPI_Line, task_t **receive_buffer)
{
  int rcv_size;
  int tasks_elements[tasks];
  int displacements[tasks];
  if(rank == MASTER)
  {
    int size = (*send_buffer)->lines_used / (tasks - 1);
    int remains = (*send_buffer)->lines_used % (tasks - 1);
    int offset = 0;
    tasks_elements[0] = 0;
    displacements[0] = 0;
    for(int i = 1; i < tasks; i++)
    {
      int true_size = (remains != 0 && i <= remains) ? size + 1 : size;
      tasks_elements[i] = true_size;
      displacements[i] = offset;
      offset += true_size;
    }
  }
  MPI_Scatter(tasks_elements, 1, MPI_INT, &rcv_size, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
  task_ctor(receive_buffer, rcv_size, rcv_size);
  (*receive_buffer)->lines_used = rcv_size;
  MPI_Scatterv((*send_buffer)->task_lines, tasks_elements, displacements, MPI_Line, (*receive_buffer)->task_lines, rcv_size, MPI_Line, MASTER, MPI_COMM_WORLD);
}

void receive_words(task_t **send_buffer, int rank, int tasks, MPI_Datatype MPI_Word, task_t **receive_buffer)
{
  int receive_size[tasks];
  int displacement[tasks];
  int offset = 0;
  int size = (rank == MASTER) ? 0 : (*send_buffer)->words_used;
  MPI_Gather(&size, 1, MPI_INT, receive_size, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
  if(rank == MASTER)
  {
    displacement[0] = 0;
    for(int i = 1; i < tasks; i++)
    {
      displacement[i] = offset;
      offset += receive_size[i];
    }
    (*receive_buffer)->task_words = realloc((*receive_buffer)->task_words, offset * sizeof(word_t));
    (*receive_buffer)->word_size = offset;
    (*receive_buffer)->words_used = offset;
  }
  MPI_Gatherv((*send_buffer)->task_words, size, MPI_Word, (*receive_buffer)->task_words, receive_size, displacement, MPI_Word, MASTER, MPI_COMM_WORLD);
}