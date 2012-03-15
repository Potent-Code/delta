/* Matrix functions
 * by Ryan Lucchese
 * Sep 22 2010 */

#include "matrix.h"

// prototypes for matrix functions
float ** matrix_allocate(int n, int m);
void print_matrix(matrix mat);
int save_matrix(matrix mat, const char *filename);
matrix load_matrix(const char *filename);
matrix mult_matrix(matrix A, matrix B);
void matrix_product(matrix A, matrix B);
matrix zero_matrix(unsigned int n, unsigned int m);
matrix new_matrix(float (*element_function)(int, int, int, int),
		int n, int m, int x, int y);
matrix identity_matrix(int n);
void row_swap(float **A, int row1, int row2);
void row_swap_partial(matrix mat, int row1, int row2, int min_col, int max_col);
void col_swap_partial(matrix mat, int col1, int col2, int min_row, int max_row);
void free_matrix(matrix mat);

// Allocate space for an n x m matrix
float ** matrix_allocate(int n, int m)
{
	int i;
	float **A;

	// Allocate space for rows so we can access A[i]
	A = calloc(n,sizeof(*A));
	if(A == NULL)
	{
		perror("Error allocating memory");
		return NULL;
	}

	// Allocate space for the entire matrix
	A[0] = calloc(n*m,sizeof(*A));
	if(A[0] == NULL)
	{
		perror("Error allocating memory");
		return NULL;
	}

	// Set up the pointers for the columns
	for(i = 1; i < n; i++)
	{
		A[i] = A[0] + i*m;
	}
	return A;
}

// Print a matrix
void print_matrix(matrix mat)
{
	unsigned int i,j;

	for (i = 0; i < mat->n; i++)
	{
		for (j = 0; j < mat->m; j++)
		{
			printf("%.16E\t",mat->A[i][j]);
		}
		printf("\n");
	}
}

// Write a matrix to hard disk
int save_matrix(matrix mat, const char *filename)
{
	FILE *outfile;
	unsigned int i,j,b;

	// Attempt to open file for writing
	if ((outfile = fopen(filename, "w")) == NULL)
	{
		perror("Error opening file");
		return 1;
	}

	// fwrite() may not be portable, but its fast and convenient
	// Not a problem as long as these files are opened
	// on the same architecture that created them
	b = sizeof(*mat)*fwrite(mat,sizeof(*mat),1,outfile);
	for (i = 0; i < mat->n; i++)
	{
		for(j = 0; j < mat->m; j++)
		{
			b += sizeof(mat->A)*fwrite(
					&mat->A[i][j],
					sizeof(mat->A),
					1,
					outfile);
		}
	}

	// Insert an EOF	
	fputc(-1,outfile);
	
	// Check to make sure all bytes were written
	// if not, errno should be set
	if (b != (sizeof(*mat)+(sizeof(mat->A)*mat->n*mat->m)))
	{
		perror("Error writing file");
		return 1;
	}

	fclose(outfile);
	//printf("Wrote %d bytes to %s\n",b,filename);
	return 0;
}

// Load a matrix from a file
matrix load_matrix(const char *filename)
{
	matrix mat,mat_info;
	FILE *infile;
	unsigned int i,j,b;
	struct stat if_stat;

	b = 0;

	// Attempt to open file for reading
	if ((infile = fopen(filename, "r+")) == NULL)
	{
		perror("Error opening file");
		return NULL;
	}
	
	// Read in the dimensions of the old matrix first
	// and then allocate space for where the rest of it goes
	if ((mat_info = zero_matrix(1,1)) == NULL) // Only for dimensions
	{
		return NULL;
	}
	b = sizeof(*mat_info)*fread(mat_info,sizeof(*mat_info),1,infile);

	// At this point, we should check to make sure the dimensions specified
	// in the file match the size of the rest of the file. If not, it's very
	// likely that someone has modified them with intent to corrupt the heap
	stat(filename,&if_stat);
	if ((if_stat.st_size - sizeof(*mat_info)-1) !=
			(sizeof(mat_info->A)*mat_info->n*mat_info->m))
	{
		// If we don't catch this here and dimensions are wrong, fread()
		// will experience errors later trying to read parts of the file
		// that don't exist
		fprintf(stderr,"Error: Matrix file has inconsistent dimensions\n");
		return NULL;
	}

	// This is where our matrix will go
	if ((mat = zero_matrix(mat_info->n,mat_info->m)) == NULL)
	{
		return NULL;
	}

	// Set offsets
	mat->x_offset = mat_info->x_offset;
	mat->y_offset = mat_info->y_offset;
	
	// Read in each value of the matrix
	for (i = 0; i < mat->n; i++)
	{
		for(j = 0; j < mat->m; j++)
		{
			b += sizeof(mat->A)*fread(
					&mat->A[i][j],
					sizeof(mat->A),
					1,
					infile);
		}
	}
	
	// Check the number of bytes read against the number we expect
	// if perror() returns "Success" here, it's very likely that someone
	// has modified the matrix file with intent to corrupt the heap
	if (b != (sizeof(*mat)+(sizeof(mat->A)*mat->n*mat->m)))
	{
		perror("Error reading file");
		return NULL;
	}

	fclose(infile);
	//printf("Read %d bytes from %s\n",b,filename);
	return mat;
}

// Return matrix C = AB
matrix mult_matrix(matrix A, matrix B)
{
	matrix C;
	unsigned int i,j,k;
	float sum;

	// Make sure we can multiply these matrices
	if(A->m != B->n)
	{
		fprintf(stderr,"Matrices are dimensionally incompatible.");
		return NULL;
	}
	
	// Set up space for our matrix product
	if((C = zero_matrix(A->n, B->m)) == NULL)
	{
		return NULL;
	}

	// Set offsets for sub-matrix C
	C->x_offset = A->x_offset;
	C->y_offset = B->y_offset;

	// This is the O(k*n*m) algorithm for matrix multiplication
	for(i = 0; i < A->n; i++)
	{
		for(j = 0; j < B->m; j++)
		{
			sum = 0.;
			for(k = 0; k < A->n; k++)
			{
				sum += A->A[i][k] * B->A[k][j];
			}
			C->A[i][j] = sum;
		}
	}

	return C;

}

// A = AB
void matrix_product(matrix A, matrix B)
{
	float product[A->n][B->m]; // temporary space for our product
	float sum;
	unsigned int i,j,k;

	// check dimension
	if (A->m != B->n)
	{
		fprintf(stderr, "Matrices are dimensionally incompatible.");
		return;
	}

	// multiply matrices
	for (i = 0; i < A->n; i++)
	{
		for (j = 0; j < B->m; j++)
		{
			sum = 0.0;
			for (k = 0; k < A->n; k++)
			{
				sum += A->A[i][k] * B->A[k][j];
			}
			product[i][j] = sum;
		}
	}

	// set A = product
	for (i = 0; i < A->n; i++)
	{
		for (j = 0; j < B->m; j++)
		{
			A->A[i][j] = product[i][j];
		}
	}
}

// Set up a matrix structure
matrix zero_matrix(unsigned int n, unsigned int m)
{
	matrix mat;
	
	if((mat = malloc(sizeof(*mat))) == NULL)
	{
		perror("Error allocating memory");
		return NULL;
	}

	// Set offsets
	mat->x_offset = 1;
	mat->y_offset = 1;

	// Make sure dimensions are >=1
	if(n < 1 || m < 1)
	{
		fprintf(stderr,"Error: dimensions must be >=1\n");
		return NULL;
	}
	mat->n = n;
	mat->m = m;

	// Allocate space for the actual matrix
	// errors are identified by matrix_allocate(),
	// so this is just to clean up
	if((mat->A = matrix_allocate(n,m)) == NULL)
	{
		free(mat);
		return NULL;
	}

	return mat;
}

// (*element_function)() defines each element of the new matrix,
// and may depend on the values of i, j, n, and m
// int n,m are the desired width,height dimensions and must be >= 1
// int x,y are the row,column offsets for computing a sub-matrix
// x,y must be >=1
matrix new_matrix(float (*element_function)(int, int, int, int),
		int n, int m, int x, int y)
{
	matrix mat;
	int i,j;

	if((mat = zero_matrix(n,m)) == NULL)
	{
		return NULL;
	}

	if(x < 1 || y < 1)
	{
		fprintf(stderr,"Error: offset out of bounds\n");
		return NULL;
	}
	// set offsets, they are 1 after allocation
	// width and height were set during allocation
	mat->x_offset = x;
	mat->y_offset = y;

	// element-wise matrix definition loop
	for (i = x; i < n+x; i++)
	{
		for(j = y; j < m+y; j++)
		{
			mat->A[i-x][j-y] = (*element_function)(i,j,n,m);
		}
	}

	return mat;
}

// This may be useful for something
matrix identity_matrix(int n)
{
	matrix I;
	int j;

	if((I = zero_matrix(n,n)) == NULL)
	{
		return NULL;
	}

	for(j = 0; j < n; j++)
	{
		I->A[j][j] = 1.;
	}

	return I;
}

// swap rows
void row_swap(float **A, int row1, int row2)
{
	float *temp;
	temp = A[row2];
	A[row2] = A[row1];
	A[row1] = temp;
}

// partial row swap
void row_swap_partial(matrix mat, int row1, int row2, int min_col, int max_col)
{
	float tmp;
	int i;
	
	for(i=min_col; i <= max_col; i++)
	{
		tmp = mat->A[row2-1][i-1];
		mat->A[row2-1][i-1] = mat->A[row1-1][i-1];
		mat->A[row1-1][i-1] = tmp;
	}
}

// partial column swap
void col_swap_partial(matrix mat, int col1, int col2, int min_row, int max_row)
{
	float tmp;
	int i;

	for(i=min_row; i <= max_row; i++)
	{
		tmp = mat->A[i-1][col2-1];
		mat->A[i-1][col2-1] = mat->A[i-1][col1-1];
		mat->A[i-1][col1-1] = tmp;
	}
}

// Free a matrix and its struct
void free_matrix(matrix mat)
{
	if(mat->A != NULL)
	{
		free(mat->A[0]);
		free(mat->A);
		mat->A = NULL;
	}
	if(mat != NULL)
	{
		free(mat);
		mat = NULL;
	}
}

