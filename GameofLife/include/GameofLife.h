#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H
#define MASTER 0
#define TAG 0
#include "Matrix.h"
/*start simulation*/
void start_simulation(matrix_t *mt, int steps, int rank);
/*count cell alive*/
int cells_alive(matrix_t *mt);
#endif