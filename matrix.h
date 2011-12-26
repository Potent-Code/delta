/* Matrix allocation
 * by Ryan Lucchese
 * Sep 22 2010 */

#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>

// Store dimensions and offsets with a matrix
typedef struct
{
	unsigned int n; // number of rows (height) of matrix
	unsigned int m; // number of columns (width) of matrix
	unsigned int x_offset; // coordinates of sub matrix 1 <= x_offset <= n
	unsigned int y_offset; // coordinates of sub matrix 1 <= y_offset <= m
	float **A;
} *matrix;

// prototypes for matrix functions
extern float ** matrix_allocate(int n, int m);
extern void print_matrix(matrix mat);
extern int save_matrix(matrix mat, const char *filename);
extern matrix load_matrix(const char *filename);
extern matrix mult_matrix(matrix A, matrix B);
extern matrix zero_matrix(unsigned int n, unsigned int m);
extern matrix new_matrix(float (*element_function)(int, int, int, int),
		int n, int m, int x, int y);
extern matrix identity_matrix(int n);
extern void row_swap(float **A, int row1, int row2);
extern void row_swap_partial(matrix mat, int row1, int row2, int min_col, int max_col);
extern void col_swap_partial(matrix mat, int col1, int col2, int min_row, int max_row);
extern void free_matrix(matrix mat);

#endif

