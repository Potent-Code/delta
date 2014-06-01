/* Solve linear systems by LU decomposition
 * by Ryan Lucchese
 * April 21 2010 */

#include "linear_system.h"

struct factored_system *factorizations;
unsigned int n_factorizations; // maximum number of factorizations
unsigned int i_factorizations; // current number of factorizations

// for creating a permutation vector of indices 1<=a<=n
float index_element(int a, int b);
// directly solve lower triangular system Lx=b
void forward_substitution(matrix L, vector x, vector b);
// directly solve upper triangular system Ux=b
void backward_substitution(matrix U, vector x, vector b);
// direct solution of LUx=b with factorizations index f
void lu_solve(int f, vector x, vector b);
// generate an LU factorization, return factorizations index
int lu_factor(matrix A);
// general method to solve a linear system Ax=b
void linear_solve(matrix A, vector x, vector b);
// cleanup
void free_factor(int factor);

float index_element(int a, int b)
{
	(void)b;

	return a;
}

// direct solution of lower triangular system Lx=b
void forward_substitution(matrix L, vector x, vector b)
{
	unsigned int i,k;
	float sum;

	if (L->n != x->n || L->n != b->n)
	{
		fprintf(stderr,"System is dimensionally inconsistent\n");
		return;
	}

	for (i=1; i <= x->n; i++)
	{
		sum=0.;
		for(k=1; k <= i-1; k++)
		{
			sum += L->A[i-1][k-1]*x->a[k-1];
		}
		x->a[i-1] = (b->a[i-1] - sum)/L->A[i-1][i-1];
	}
}

// direct solution of upper triangular system Ux=b
void backward_substitution(matrix U, vector x, vector b)
{
	unsigned int i,k;
	float sum;

	if (U->n != x->n || U->n != b->n)
	{
		fprintf(stderr,"System is dimensionally inconsistent\n");
		return;
	}

	for (i=x->n; i >= 1; i--)
	{
		sum=0.;
		for (k=i+1; k <= x->n; k++)
		{
			sum += U->A[i-1][k-1]*x->a[k-1];
		}
		x->a[i-1] = (b->a[i-1]-sum)/U->A[i-1][i-1];
	}
}

// direct solution of decomposed system LUx=b
// f is the number of the factor in the factor list
void lu_solve(int f, vector x, vector b)
{
	unsigned int i,k;
	float sum;
	int bp,xp; // b and x index permutation
	vector z; // intermediate solution

	if (factorizations[f].factors->n != x->n || factorizations[f].factors->n != b->n)
	{
		fprintf(stderr,"System is dimensionally inconsistent\n");
		return;
	}

	// initialize intermediate solution vector
	if ((z = zero_vector(b->n)) == NULL)
	{
		return;
	}

	// forward substitution of lower triangular portion Lz=b
	for (i=1; i <= x->n; i++)
	{
		sum=0.;
		for (k=1; k <= i-1; k++)
		{
			sum += factorizations[f].factors->A[i-1][k-1]*z->a[k-1];
		}
		bp = (int)(factorizations[f].b_permutation->a[i-1])-1;
		z->a[i-1] = (b->a[bp] - sum)/factorizations[f].alpha;
	}

	// backward substitution of upper triangular portion Ux=z
	for (i=x->n; i >= 1; i--)
	{
		sum=0.;
		for (k=i+1; k <= x->n; k++)
		{
			xp=(int)(factorizations[f].x_permutation->a[k-1])-1;
			sum += factorizations[f].factors->A[i-1][k-1]*x->a[xp];
		}
		xp=(int)(factorizations[f].x_permutation->a[i-1])-1;
		x->a[xp] = (z->a[i-1]-sum)/factorizations[f].factors->A[i-1][i-1];
	}

	free_vector(z);
}

// factor matrix A into lower and upper triangular parts LU
// if a factorization of A has already been done, it will be re-factored in place
int lu_factor(matrix A)
{
	unsigned int i,j,k; // iterators
	int factor=-1; // if this is positive we are refactoring
	float r,sum,sum2; // temporary sums
	float alpha=1.; // diag(L)
	float beta; // diag(U)
	matrix LU; // an LU factorization
	vector xi; // x permutations
	vector bi; // b permutations
	vector max; // row maximums

	// initialize the list of factorizations
	if (factorizations == NULL)
	{
		n_factorizations=10;
		i_factorizations=0;
		if ((factorizations = malloc(sizeof(*factorizations)*n_factorizations)) == NULL)
		{
			fprintf(stderr,"Couldn't initialize factorizations list\n");
			return -1;
		}
	}
	else
	{
		// search for an existing factorization of this matrix
		for (i=0; i < i_factorizations; i++)
		{
			if (factorizations[i].system == A)
			{
				factor = i;
				break;
			}
		}
	}

	if (factor >= 0)
	{
		// set up pointers so to re-factor system
		LU = factorizations[factor].factors;
		xi = factorizations[factor].x_permutation;
		bi = factorizations[factor].b_permutation;
		alpha = factorizations[factor].alpha;

		// reset things back to their original state
		for (i=0; i < LU->n; i++)
		{
			for (j=0; j < LU->m; j++)
			{
				LU->A[i][j]=0.;
			}
			xi->a[i] = (float)(i+1);
			bi->a[i] = (float)(i+1);
		}
			
	}
	else
	{
		// initialize a new factors matrix
		// to save space, we save both L and U in the same matrix
		// except for the diagonal the nonzero entries are disjoint sets
		// so we keep the diagonal of U in LU and alpha in the factorizations list
		if ((LU = zero_matrix(A->n,A->n)) == NULL)
		{
			return -1;
		}
		if ((xi = new_vector(&index_element, A->n, 1)) == NULL)
		{
			free_matrix(LU);
			return -1;
		}
		if ((bi = new_vector(&index_element, A->n, 1)) == NULL)
		{
			free_matrix(LU);
			free_vector(xi);
			return -1;
		}
	}

	// initialize vector for row maximums
	if ((max = zero_vector(A->n)) == NULL)
	{
		if (factor < 0)
		{
			free_matrix(LU);
			free_vector(xi);
			free_vector(bi);
		}
		else
		{
			free_factor(factor);
		}
		return -1;
	}
	
	// find the maximums of each row of A
	for (i=0; i < A->n; i++)
	{
		for (j=0; j < A->n; j++)
		{
			if (A->A[i][j] > max->a[i])
			{
				max->a[i] = A->A[i][j];
			}
		}
		if (max->a[i] == 0.)
		{
			// we need least squares method
			fprintf(stderr,"No unique solution\n");
			if (factor < 0)
			{
				free_matrix(LU);
				free_vector(xi);
				free_vector(bi);
				free_vector(max);
			}
			else
			{
				free_factor(factor);
			}
			return -1;
		}
	}

	/* iteration for total scaled pivoting and LU factorization */
	for (i=1; i <= A->n; i++)
	{
		r=0.;
		// find scaled maximum of ith column
		for (j=0; j < A->n; j++)
		{
			if (r < A->A[j][i-1]/max->a[j])
			{
				r = A->A[j][i-1]/max->a[j];
			}
		}
		
		// total scaled pivoting scheme
		for (j=i; j < A->n+1; j++)
		{
			for (k=i; k < A->n+1; k++)
			{
				if (float_cmp(A->A[j-1][k-1]/max->a[j-1],r,1))
				{
					if (i < j)
					{
						row_swap_partial(A, i, j, i, A->n);
						component_swap(bi, i, j);
						row_swap_partial(LU, i, j, 1, i-1);
						component_swap(max, i, j);
					}
					if (i < k)
					{
						col_swap_partial(A, i, k, i, A->n);
						component_swap(xi, i, k);
						col_swap_partial(LU, i, k, 1, i-1);
					}
				}
			}
		}

		// Find diagonal LU[i][i] = beta
		sum=0.;
		for (j=1; j <= i-1; j++)
		{
			sum+=LU->A[i-1][j-1]*LU->A[j-1][i-1];
		}
		beta = A->A[i-1][i-1] - sum;

		// matrix A is singular if this is true
		if (float_cmp(A->A[i-1][i-1],sum,i))
		{
			// need least squares
			fprintf(stderr,"No unique solution\n");
			if (factor < 0)
			{
				free_matrix(LU);
				free_vector(xi);
				free_vector(bi);
				free_vector(max);
			}
			else
			{
				free_factor(factor);
			}
			return -1;
		}

		// set diagonal components
		LU->A[i-1][i-1] = beta;

		// find off diagonal components of L and U
		if (i < A->n)
		{
			for (j=i+1; j <= A->n; j++)
			{
				sum=0.;
				sum2=0.;
				for (k=1; k <= i-1; k++)
				{
					sum += LU->A[j-1][k-1]*LU->A[k-1][i-1];
					sum2 += LU->A[i-1][k-1]*LU->A[k-1][j-1];
				}
				
				LU->A[j-1][i-1] = (A->A[j-1][i-1] - sum)/LU->A[i-1][i-1];
				LU->A[i-1][j-1] = (A->A[i-1][j-1] - sum2)/alpha;
			}
		}
	}
	free_vector(max);

	// new factorization
	if (factor < 0)
	{
		// save these pointers in the factorizations list
		factorizations[i_factorizations].system = A;
		factorizations[i_factorizations].factors = LU;
		factorizations[i_factorizations].x_permutation = xi;
		factorizations[i_factorizations].b_permutation = bi;
		factorizations[i_factorizations].alpha = alpha;
		i_factorizations++;
	}

	// return the factorizations index we just set
	return i_factorizations-1;
}

// solve linear system Ax=b
void linear_solve(matrix A, vector x, vector b)
{
	unsigned int i;
	int factor;
	
	if (A->n != A->m)
	{
		fprintf(stderr,"Matrix is not square\n");
		return;
	}

	// this means we have not found a factor yet
	factor=-1;

	// search for an existing factorization of matrix A
	// this only compares the pointer! if A has changed
	// since the last factorization, it must be refactored
	// by calling lu_factor(A) manually
	if (factorizations != NULL)
	{
		for (i=0; i < n_factorizations; i++)
		{
			if (factorizations[i].system == A)
			{
				factor=i;
				break;
			}
		}
	}

	// if A has not been factored yet now is the time
	if (factor < 0)
	{
		if ((factor = lu_factor(A)) < 0)
		{
			return;
		}
	}

	// find the solution of LUx=b
	lu_solve(factor,x,b);
}

// free a factor from the factor list
void free_factor(int factor)
{
	if (factorizations[factor].factors != NULL)
	{
		free_matrix(factorizations[factor].factors);
	}
	if (factorizations[factor].x_permutation != NULL)
	{
		free_vector(factorizations[factor].x_permutation);
	}
	if (factorizations[factor].b_permutation != NULL)
	{
		free_vector(factorizations[factor].b_permutation);
	}
	factorizations[factor].system=NULL;
}

// clean up the factor list
void free_all_factors(void)
{
	unsigned int i;

	// free all factors
	for (i=0; i < i_factorizations; i++)
	{
		free_factor(i);
	}
	// free the factorization list itself
	free(factorizations);
	factorizations=NULL;
}
