/* File name: vector.c 
 * 
 * Description: 
 * 	Contains functions used to calculate norm of a vector and distance between 2
 * 	vectors.
 * 
 * Date: 16.04.2019
 * Version: 2.03
 * 
 * Authors - SCIPER:
 * 	Alexis Philip George-Georganopoulos - 228168
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"

//------------------------------------------------------------------------------
// calculates norm of vector
double vector_norm(VECTEUR v)
{
    return sqrt(v.x*v.x + v.y*v.y);
}

//------------------------------------------------------------------------------
// calculates distance between 2 vectors
double vector_distance(VECTEUR v1, VECTEUR v2)
{
    return sqrt((v2.x-v1.x)*(v2.x-v1.x) + (v2.y-v1.y)*(v2.y-v1.y));
}


