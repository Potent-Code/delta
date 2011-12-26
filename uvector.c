/* Vector functions
 * by Ryan Lucchese
 * Nov 7 2010 */

#include "uvector.h"

// prototypes for uvector functions
unsigned int* uvector_allocate(int n);
void print_uvector(uvector uvec);
int save_uvector(uvector uvec, const char *filename);
uvector load_uvector(const char *filename);
uvector mult_uvector(uvector a, uvector b);
uvector zero_uvector(int n);
uvector new_uvector(unsigned int (*element_function)(int, int),
		int n, int x);
void ucomponent_swap(uvector uvec, int i, int j);
void free_uvector(uvector uvec);

// allocate space for an n dimensional uvector
unsigned int* uvector_allocate(int n)
{
	unsigned int *a;

	// allocate space for uvector
	a = calloc((size_t)n,sizeof(*a));
	if(a == NULL)
	{
		perror("Error allocating memory");
		return NULL;
	}

	return a;
}

// Print a uvector
void print_uvector(uvector uvec)
{
	unsigned int i;

	for (i = 0; i < uvec->n; i++)
	{
		printf("%d\t",uvec->a[i]);
	}
	printf("\n");
}

// Write a uvector to hard disk
int save_uvector(uvector uvec, const char *filename)
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
	b = sizeof(*uvec)*fwrite(uvec,sizeof(*uvec),1,outfile);
	for (i = 0; i < uvec->n; i++)
	{
		b += sizeof(uvec->a)*fwrite(&uvec->a[i],
				sizeof(uvec->a),1,outfile);
	}

	// Insert an EOF	
	fputc(-1,outfile);
	
	// check to make sure all bytes were written
	// if not, errno should be set
	if (b != (sizeof(*uvec)+(sizeof(uvec->a)*uvec->n)))
	{
		perror("Error writing file");
		return 1;
	}

	fclose(outfile);
	//printf("Wrote %d bytes to %s\n",b,filename);
	return 0;
}

// Load a uvector from a file
uvector load_uvector(const char *filename)
{
	uvector uvec,uvec_info;
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
	
	// Read in the dimensions of the old uvector first
	// and then allocate space for where the rest of it goes
	if ((uvec_info = zero_uvector(1)) == NULL) // Only for dimensions
	{
		return NULL;
	}
	b = sizeof(*uvec_info)*fread(uvec_info,sizeof(*uvec_info),1,infile);

	// at this point, we should check to make sure the dimensions specified
	// in the file uvecch the size of the rest of the file. If not, it's very
	// likely that someone has modified them with intent to corrupt the heap
	stat(filename,&if_stat);
	if ((if_stat.st_size - sizeof(*uvec_info)-1) !=
			(sizeof(uvec_info->a)*uvec_info->n))
	{
		// If we don't catch this here and dimensions are wrong, fread()
		// will experience errors later trying to read parts of the file
		// that don't exist
		fprintf(stderr,"Error: Vector file has inconsistent dimensions\n");
		return NULL;
	}

	// This is where our uvector will go
	if ((uvec = zero_uvector(uvec_info->n)) == NULL)
	{
		return NULL;
	}

	// Set offsets
	uvec->x_offset = uvec_info->x_offset;
	
	// Read in each value of the uvector
	for (i = 0; i < uvec->n; i++)
	{
		b += sizeof(uvec->a)*fread(&uvec->a[i],sizeof(uvec->a),1,infile);
	}
	
	// check the number of bytes read against the number we expect
	// if perror() returns "Success" here, it's very likely that someone
	// has modified the uvector file with intent to corrupt the heap
	if (b != (sizeof(*uvec)+(sizeof(uvec->a)*uvec->n)))
	{
		perror("Error reading file");
		return NULL;
	}

	fclose(infile);
	//printf("Read %d bytes from %s\n",b,filename);
	return uvec;
}

// Set up a uvector structure
uvector zero_uvector(int n)
{
	uvector uvec;
	
	// aways check malloc, even though
	// we're only asking for a few bytes...
	if((uvec = (uvector)malloc(sizeof(*uvec))) == NULL)
	{
		perror("Error allocating memory");
		return NULL;
	}

	// Set offsets
	uvec->x_offset = 1;

	// Make sure dimensions are >=1
	if(n < 1)
	{
		fprintf(stderr,"Error: dimensions must be >=1\n");
		return NULL;
	}
	uvec->n = n;

	// allocate space for the actual uvector
	// errors are identified by uvector_allocate(),
	// so this is just to clean up
	if((uvec->a = uvector_allocate(n)) == NULL)
	{
		free(uvec);
		return NULL;
	}

	return uvec;
}

// (*element_function)() defines each element of the new uvector,
// x must be >=1
uvector new_uvector(unsigned int (*element_function)(int, int),
		int n, int x)
{
	uvector uvec;
	int i;

	if((uvec = zero_uvector(n)) == NULL)
	{
		return NULL;
	}

	if(x < 1)
	{
		fprintf(stderr,"Error: offset out of bounds\n");
		return NULL;
	}
	
	// set offset, it is 1 after allocation
	uvec->x_offset = x;

	// element-wise uvector definition loop
	for(i = x; i < n+x; i++)
	{
		uvec->a[i-x] = (*element_function)(i,n);
	}

	return uvec;
}

// swap a[i] <-> a[j]
void ucomponent_swap(uvector uvec, int i, int j)
{
	unsigned int tmp;
	tmp = uvec->a[j-1];
	uvec->a[j-1] = uvec->a[i-1];
	uvec->a[i-1] = tmp;
}

// Free a uvector and its struct
void free_uvector(uvector uvec)
{
	if(uvec->a != NULL)
	{
		free(uvec->a);
		uvec->a = NULL;
	}
	if(uvec != NULL)
	{
		free(uvec);
		uvec = NULL;
	}
}
