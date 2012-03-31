/* Solve linear system by LU decomposition
 * by Ryan Lucchese
 * April 21, 2011 */

#ifndef LINEAR_SYSTEM_H
#define LINEAR_SYSTEM_H

#include "matrix.h"
#include "vector.h"
#include "float_cmp.h"

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

// a list of factored systems
extern struct factored_system *factorizations;
extern unsigned int n_factorizations; // maximum number of factorizations
extern unsigned int i_factorizations; // current number of factorizations

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
