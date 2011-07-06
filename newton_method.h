/* Newton's method
 * for finding roots f(x)=0
 * by Ryan Lucchese
 * Feb 1 2011 */

#ifndef NEWTON_METHOD_H
#define NEWTON_METHOD_H

#include <math.h>
#include <float.h>

typedef float(*polynomial)(float); 

extern float newton_method(float x0, polynomial f, polynomial fprime);

#endif
