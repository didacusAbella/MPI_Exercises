#ifndef MATRIX_H
#define MATRIX_H
#define OLD_M 0
#define NEW_M 1
/**
 * Remember:offset = i * columns + j or 
 * offset = i + rows * j. 
 * Now I can access to matrix element in a continous manner with matrix[offset] = matrix[i][j]
 **/ 
typedef struct matrix_t
{
  int rows, columns;
  int *oldMatrix, *newMatrix;
  int relRws, relRwe, relCls, relCle;
} matrix_t;

/*Allocate space for matrix*/
matrix_t *alloc_matrix(int rows, int columns, int rank);

/*Dealloc space for matrix*/
void dealloc_matrix(matrix_t *mt);

/*get cell value*/
int get_cell(matrix_t *mt, int row, int column, int type);

/*set cell value*/
void set_cell(matrix_t *mt, int row, int column, int value, int type);

int offset(int row, int column, matrix_t *mt);
#endif