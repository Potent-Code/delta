/* Vectors of functions
 * by Ryan Lucchese
 * Nov 7 2010 */

#include "vector_function.h"

// allocate space for an n dimensional vector function
func * vecfunc_allocate(int n)
{
	func * f;

	// allocate space for function pointers
	f = calloc(n,sizeof(*f));
	if(f == NULL)
	{
		perror("Error allocating memory");
		return NULL;
	}

	return f;
}

vector_function new_vecfunc(int n, func f)
{
	vector_function vf;
	int i;

	if((vf = malloc(sizeof(*vf))) == NULL)
	{
		perror("Error allocating memory");
		return NULL;
	}

	// Make sure dimensions are >=1
	if(n < 1)
	{
		fprintf(stderr, "Error: dimensions must be >=1\n");
		return NULL;
	}
	vf->n = n;

	// allocate space for the actual vector function
	// errors are identified by vecfunc_allocate(),
	// so this is just to clean up
	if((vf->f = vecfunc_allocate(n)) == NULL)
	{
		free(vf);
		return NULL;
	}

	// set up default pointer for vector components
	for(i = 0; i < n; i++)
	{
		vf->f[i] = f;
	}

	return vf;
}

// Free a vector function
void free_vecfunc(vector_function vf)
{
	free(vf->f);
	free(vf);
}
