#include "runge_kutta4.h"

matrix runge_kutta4(vector_function f, vector y0,
		float tmin, float tmax, float h)
{
	matrix Y;
	int n=(int)((tmax-tmin)/h);
	int i,j;
	float k1,k2,k3,k4;

	if(f->n != y0->n)
	{
		fprintf(stderr,"System has inconsistent dimensions\n");
		return NULL;
	}

	// initialization	
	if((Y=zero_matrix(n, f->n+1)) == NULL)
	{
		return NULL;
	}

	// time slices
	Y->A[0][0] = tmin;
	for(i=1; i < Y->n; i++)
	{
		Y->A[i][0] = Y->A[i-1][0]+h;
	}

	// initial conditions
	for(j=1; j < Y->m; j++)
	{
		Y->A[0][j] = y0->a[j-1];
	}

	// solution matrix
	for(i=0; i < n-1; i++)
	{
		for(j=1; j < Y->m; j++)
		{
			Y->A[i+1][j] = Y->A[i][j] + h*(*f->f[j-1])(Y->A[i], Y->m);
		}
	}

	return Y;
}
