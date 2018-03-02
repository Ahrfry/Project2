#ifndef MATRIX_H_
#define MATRIX_H_
#include <stdio.h>

#define M_SIZE 4

typedef struct _matrix_struct_t{
	int A[M_SIZE][M_SIZE];
	int B[M_SIZE][M_SIZE];
	int C[M_SIZE][M_SIZE];
}matrix_t;

void matrix_init(int M[M_SIZE][M_SIZE]);
void matrix_mult(matrix_t *m_struct);
void print_matrix(int M[M_SIZE][M_SIZE]);
#endif // QUEUE_H_INCLUDED
