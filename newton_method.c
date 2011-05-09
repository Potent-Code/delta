#include "newton_method.h"

float newton_method(float x0, polynomial f, polynomial fprime)
{
	int i=0;
	float nextx,lastx;

	lastx=x0;
	do
	{
		nextx = lastx - (*f)(lastx)/(*fprime)(lastx);
		lastx = nextx;
		i++;
	}while(fabs((*f)(lastx)) > FLT_EPSILON);

	return nextx;
}
