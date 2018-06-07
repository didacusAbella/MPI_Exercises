#ifndef MAPREDUCE_H
#define MAPREDUCE_H
#define TOKENIZER " \r\n,?!.;"
#include "LineVector.h"
#include "WordVector.h"

void line_splitter(const char *dir_path, vector_line *all_lines);
void map(vector_line *vector, vector_word *mvector);
void reduce(vector_word *vector, vector_word *cvector);
#endif