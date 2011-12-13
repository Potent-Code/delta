/* header for math library
 * by Ryan Lucchese
 * May 12 2011 */

#ifndef MATHLIB_H
#define MATHLIB_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <math.h>
#include <float.h>

// floating point comparison
extern int float_cmp(float a, float b, int n);

// vector stuff
// Store dimensions and offsets with a matrix
typedef struct
{
	int n; // number of rows (height) of matrix
	int x_offset; // coordinates of sub vector 1 <= x_offset <= n
	float *a;
} *vector;

// prototypes for vector functions
extern float * vector_allocate(int n);
extern void print_vector(vector vec);
extern int save_vector(vector vec, const char *filename);
extern vector load_vector(const char *filename);
extern vector mult_vector(vector a, vector b);
extern vector zero_vector(int n);
extern vector new_vector(float (*element_function)(int, int),
		int n, int x);
extern void component_swap(vector vec, int i, int j);
extern void free_vector(vector vec);

// vector functions
// form of function ptr f(t,y)
typedef float(*func)(float *, int);

// Store dimensions and offsets with a matrix
typedef struct
{
	int n; // number of rows (height) of matrix
	func * f; // array of function pointers
} *vector_function;

// prototypes for vector functions
extern func * vecfunc_allocate(int n);
extern vector_function new_vecfunc(int n, func f);
extern void free_vecfunc(vector_function vf);

// matrix stuff
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
extern float ** matrix_allocate(int n, int m);
extern void print_matrix(matrix mat);
extern int save_matrix(matrix mat, const char *filename);
extern matrix load_matrix(const char *filename);
extern matrix mult_matrix(matrix A, matrix B);
extern matrix zero_matrix(int n, int m);
extern matrix new_matrix(float (*element_function)(int, int, int, int),
		int n, int m, int x, int y);
extern matrix identity_matrix(int n);
extern void row_swap(float **A, int row1, int row2);
extern void row_swap_partial(matrix mat, int row1, int row2, int min_col, int max_col);
extern void col_swap_partial(matrix mat, int col1, int col2, int min_row, int max_row);
extern void free_matrix(matrix mat);

// numerical root finding
typedef float(*polynomial)(float); 

extern float newton_method(float x0, polynomial f, polynomial fprime);

// Euler method for ODEs
extern matrix euler_method(vector_function f, vector y0, float tmin, float tmax, float h);

// Runge-Kutta method for ODEs 4th order
extern matrix runge_kutta4(vector_function f, vector y0, float tmin, float tmax, float h);

// LU factorization of linear systems
// a list of things relevant to an LU factorization Ax=LUx=b
struct factored_system
{
	// the linear system
	matrix system;
	// its LU factorization in 1 matrix
	matrix factors;
	// pivoting for x
	vector x_permutation;
	// pivoting for b
	vector b_permutation;
	// this value is the diagonals of L (usually 1.)
	float alpha;
};

// directly solve lower triangular system Lx=b
extern void forward_substitution(matrix L, vector x, vector b);
// directly solve upper triangular system Ux=b
extern void backward_substitution(matrix U, vector x, vector b);
// direct solution of LUx=b with factorizations index f
extern void lu_solve(int f, vector x, vector b);
// generate an LU factorization, return factorizations index
extern int lu_factor(matrix A);
// general method to solve a linear system Ax=b
extern void linear_solve(matrix A, vector x, vector b);
// cleanup
extern void free_factor(int factor);
extern void free_all_factors(void);

#endif

