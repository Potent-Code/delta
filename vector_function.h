/* Vector function allocation
 * by Ryan Lucchese
 * Jan 31 2011 */

#ifndef VECTOR_FUNCTION_H
#define VECTOR_FUNCTION_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>

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

#endif
