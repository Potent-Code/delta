/* Runge-Kutta method
 * 4th order
 * by Ryan Lucchese
 * Feb 1 2011 */

#ifndef RUNGE_KUTTA4_H
#define RUNGE_KUTTA4_H

#include "matrix.h"
#include "vector.h"
#include "vector_function.h"

extern matrix runge_kutta4(vector_function f, vector y0, float tmin, float tmax, float h);

#endif
