#include "float_cmp.h"

int float_cmp(float a, float b, int n);

// compare a and b to an accuracy of n*FLT_EPSILON
int float_cmp(float a, float b, int n)
{
	if ((a > (b - (n*FLT_EPSILON))) && (a < (b + (n*FLT_EPSILON))))
	{
		return 1;
	}
	return 0;
}
