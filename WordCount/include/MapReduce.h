#ifndef MAPREDUCE_H
#define MAPREDUCE_H
#define TOKENIZER " \r\n,?!.;"
#include "LineVector.h"
#include "WordVector.h"

vector_line * line_splitter(const char *dir_path);
vector_word * map(vector_line *vector);
vector_word * reduce(vector_word *vector);
#endif