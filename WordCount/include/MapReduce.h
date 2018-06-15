#ifndef MAPREDUCE_H
#define MAPREDUCE_H
#include "Task.h"
#define TOKENIZER " \r\n,?!.;"
#define MAX_LINE_BUFFER 1024
void line_splitter(const char *dir_path, task_t **master_task);
void map(task_t **slave_task);
void reduce(task_t **slave_task);
#endif