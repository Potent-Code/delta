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
	int n; // number of rows (height) of matrix
	int m; // number of columns (width) of matrix
	int x_offset; // coordinates of sub matrix 1 <= x_offset <= n
	int y_offset; // coordinates of sub matrix 1 <= y_offset <= m
	float **A;
} *matrix;

// prototypes for matrix functions
float ** matrix_allocate(int n, int m);
void print_matrix(matrix mat);
int save_matrix(matrix mat, const char *filename);
matrix load_matrix(const char *filename);
matrix mult_matrix(matrix A, matrix B);
matrix zero_matrix(int n, int m);
matrix new_matrix(float (*element_function)(int, int, int, int),
		int n, int m, int x, int y);
matrix identity_matrix(int n);
void row_swap(float **A, int row1, int row2);
void row_swap_partial(matrix mat, int row1, int row2, int min_col, int max_col);
void col_swap_partial(matrix mat, int col1, int col2, int min_row, int max_row);
void free_matrix(matrix mat);

#endif
