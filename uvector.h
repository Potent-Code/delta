/* unsigned int vector allocation
 * by Ryan Lucchese
 * December 25 2011 */

#ifndef UVECTOR_H
#define UVECTOR_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>

// Store dimensions and offsets with a matrix
typedef struct
{
	unsigned int n; // number of rows (height) of matrix
	int x_offset; // coordinates of sub uvector 1 <= x_offset <= n
	unsigned int* a;
} *uvector;

// prototypes for uvector functions
extern unsigned int* uvector_allocate(int n);
extern void print_uvector(uvector uvec);
extern int save_uvector(uvector uvec, const char *filename);
extern uvector load_uvector(const char *filename);
extern uvector mult_uvector(uvector a, uvector b);
extern uvector zero_uvector(int n);
extern uvector new_uvector(unsigned int (*element_function)(int, int),
		int n, int x);
extern void ucomponent_swap(uvector uvec, int i, int j);
extern void free_uvector(uvector uvec);

#endif
