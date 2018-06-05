#ifndef MAPREDUCE_H
#define MAPREDUCE_H
#include "LineVector.h"
#include "WordVector.h"

vector_line * line_splitter(const char *dir_path);
vector_word * map(vector_line *lines, int tasks, int rank);
#endif