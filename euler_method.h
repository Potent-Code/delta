/* Euler's method for solving ODE's
 * of form dy/dt = f(t,y), y(0) = y0
 * by Ryan Lucchese
 * Jan 31 2001 */

#ifndef EULER_METHOD_H
#define EULER_METHOD_H

#include "matrix.h"
#include "vector.h"
#include "vector_function.h"

extern matrix euler_method(vector_function f, vector y0, float tmin, float tmax, float h);

#endif
