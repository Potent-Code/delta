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
	int n; // number of rows (height) of matrix
	int x_offset; // coordinates of sub vector 1 <= x_offset <= n
	float *a;
} *vector;

// prototypes for vector functions
float * vector_allocate(int n);
void print_vector(vector vec);
int save_vector(vector vec, const char *filename);
vector load_vector(const char *filename);
vector mult_vector(vector a, vector b);
vector zero_vector(int n);
vector new_vector(float (*element_function)(int, int),
		int n, int x);
void component_swap(vector vec, int i, int j);
void free_vector(vector vec);

#endif
