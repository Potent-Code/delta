/* Vector functions
 * by Ryan Lucchese
 * Nov 7 2010 */

#include "vector.h"

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

// allocate space for an n dimensional vector
float * vector_allocate(int n)
{
	float *a;

	// allocate space for vector
	a = calloc((size_t)n,sizeof(*a));
	if(a == NULL)
	{
		perror("Error allocating memory");
		return NULL;
	}

	return a;
}

// Print a vector
void print_vector(vector vec)
{
	unsigned int i;

	for (i = 0; i < vec->n; i++)
	{
		printf("%.16E\t",vec->a[i]);
	}
	printf("\n");
}

// Write a vector to hard disk
int save_vector(vector vec, const char *filename)
{
	FILE *outfile;
	unsigned int i,b;

	// attempt to open file for writing
	if ((outfile = fopen(filename, "w")) == NULL)
	{
		perror("Error opening file");
		return 1;
	}

	// fwrite() may not be portable, but its fast and convenient
	// Not a problem as long as these files are opened
	// on the same architecture that created them
	b = sizeof(*vec)*fwrite(vec,sizeof(*vec),1,outfile);
	for (i = 0; i < vec->n; i++)
	{
		b += sizeof(vec->a)*fwrite(&vec->a[i],
				sizeof(vec->a),1,outfile);
	}

	// Insert an EOF	
	fputc(-1,outfile);
	
	// check to make sure all bytes were written
	// if not, errno should be set
	if (b != (sizeof(*vec)+(sizeof(vec->a)*vec->n)))
	{
		perror("Error writing file");
		return 1;
	}

	fclose(outfile);
	//printf("Wrote %d bytes to %s\n",b,filename);
	return 0;
}

// Load a vector from a file
vector load_vector(const char *filename)
{
	vector vec,vec_info;
	FILE *infile;
	unsigned int i,b;
	struct stat if_stat;

	b = 0;

	// attempt to open file for reading
	if ((infile = fopen(filename, "r+")) == NULL)
	{
		perror("Error opening file");
		return NULL;
	}
	
	// Read in the dimensions of the old vector first
	// and then allocate space for where the rest of it goes
	if ((vec_info = zero_vector(1)) == NULL) // Only for dimensions
	{
		return NULL;
	}
	b = sizeof(*vec_info)*fread(vec_info,sizeof(*vec_info),1,infile);

	// at this point, we should check to make sure the dimensions specified
	// in the file vecch the size of the rest of the file. If not, it's very
	// likely that someone has modified them with intent to corrupt the heap
	stat(filename,&if_stat);
	if ((if_stat.st_size - sizeof(*vec_info)-1) !=
			(sizeof(vec_info->a)*vec_info->n))
	{
		// If we don't catch this here and dimensions are wrong, fread()
		// will experience errors later trying to read parts of the file
		// that don't exist
		fprintf(stderr,"Error: Vector file has inconsistent dimensions\n");
		return NULL;
	}

	// This is where our vector will go
	if ((vec = zero_vector(vec_info->n)) == NULL)
	{
		return NULL;
	}

	// Set offsets
	vec->x_offset = vec_info->x_offset;
	
	// Read in each value of the vector
	for (i = 0; i < vec->n; i++)
	{
		b += sizeof(vec->a)*fread(&vec->a[i],sizeof(vec->a),1,infile);
	}
	
	// check the number of bytes read against the number we expect
	// if perror() returns "Success" here, it's very likely that someone
	// has modified the vector file with intent to corrupt the heap
	if (b != (sizeof(*vec)+(sizeof(vec->a)*vec->n)))
	{
		perror("Error reading file");
		return NULL;
	}

	fclose(infile);
	//printf("Read %d bytes from %s\n",b,filename);
	return vec;
}

// Set up a vector structure
vector zero_vector(int n)
{
	vector vec;
	
	// aways check malloc, even though
	// we're only asking for a few bytes...
	if((vec = (vector)malloc(sizeof(*vec))) == NULL)
	{
		perror("Error allocating memory");
		return NULL;
	}

	// Set offsets
	vec->x_offset = 1;

	// Make sure dimensions are >=1
	if(n < 1)
	{
		fprintf(stderr,"Error: dimensions must be >=1\n");
		return NULL;
	}
	vec->n = n;

	// allocate space for the actual vector
	// errors are identified by vector_allocate(),
	// so this is just to clean up
	if((vec->a = vector_allocate(n)) == NULL)
	{
		free(vec);
		return NULL;
	}

	return vec;
}

// (*element_function)() defines each element of the new vector,
// x must be >=1
vector new_vector(float (*element_function)(int, int),
		int n, int x)
{
	vector vec;
	int i;

	if((vec = zero_vector(n)) == NULL)
	{
		return NULL;
	}

	if(x < 1)
	{
		fprintf(stderr,"Error: offset out of bounds\n");
		return NULL;
	}
	
	// set offset, it is 1 after allocation
	vec->x_offset = x;

	// element-wise vector definition loop
	for(i = x; i < n+x; i++)
	{
		vec->a[i-x] = (*element_function)(i,n);
	}

	return vec;
}

// swap a[i] <-> a[j]
void component_swap(vector vec, int i, int j)
{
	float tmp;
	tmp = vec->a[j-1];
	vec->a[j-1] = vec->a[i-1];
	vec->a[i-1] = tmp;
}

// Free a vector and its struct
void free_vector(vector vec)
{
	if(vec->a != NULL)
	{
		free(vec->a);
		vec->a = NULL;
	}
	if(vec != NULL)
	{
		free(vec);
		vec = NULL;
	}
}
