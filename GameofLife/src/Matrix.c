#include "Matrix.h"
#include <stdlib.h>

matrix_t *alloc_matrix(int rows, int columns, int rank)
{
  matrix_t *mt = malloc(sizeof(matrix_t));
  mt->rows = rows;
  mt->columns = columns;
  mt->relRws = (rank * (mt->rows / 2)) + 1;
  mt->relRwe = (rank == 0) ? (mt->rows / 2) - 2 : mt->rows - 2;
  mt->relCls = 1;
  mt->relCle = mt->columns - 2;
  int trueSize = (mt->rows) * (mt->columns);
  mt->oldMatrix = (int *)malloc(trueSize*sizeof(int));
  mt->newMatrix = (int *)malloc(trueSize*sizeof(int));
  float randomValue;
  for(int i = mt->relRws; i <= mt->relRwe; i++)
  {
    for(int j = mt->relCls; j <= mt->relCle; j++)
    {
      randomValue = rand() / ((float) RAND_MAX + 1);
      if(randomValue < 0.5)
        set_cell(mt, i, j, 0, OLD_M);
      else
        set_cell(mt, i, j, 1, OLD_M);
    }
  }
  return mt;
}

/*Dealloc space for matrix*/
void dealloc_matrix(matrix_t *mt)
{
  free(mt->oldMatrix);
  free(mt->newMatrix);
  free(mt);
}


/*get cell value*/
int get_cell(matrix_t *mt, int row, int column, int type)
{
  int off = offset(row, column, mt);
  if(type == OLD_M)
    return mt->oldMatrix[off];
  else
    return mt->newMatrix[off];
}

/*set cell value*/
void set_cell(matrix_t *mt, int row, int column, int value, int type)
{
  int off = offset(row, column, mt);
  if(type == OLD_M)
    mt->oldMatrix[off] = value;
  else
    mt->newMatrix[off] = value;
}

int offset(int row, int column, matrix_t *mt)
{
  return (row * mt->columns) + column;
}