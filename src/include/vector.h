/* Vector allocation
 * by Ryan Lucchese
 * Nov 7 2010 */

#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>

// Store dimensions and offsets with a matrix
typedef struct
{
	unsigned int n; // number of rows (height) of matrix
	unsigned int offset; // coordinates of sub vector 1 <= offset <= n
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

#endif
