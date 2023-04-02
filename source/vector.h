/* File name: vector.h 
 * 
 * Description: 
 * 	Exports functions used to calculate norm of a vector and distance between 2
 * 	vectors.
 * 
 * Date: 16.04.2015
 * Version: 2.03
 * 
 * Authors - SCIPER:
 * 	Nawel Naas - 245996
 * 	Alexis Philip George-Georganopoulos - 228168
 */
 
#ifndef VECTEUR_H
#define VECTEUR_H

typedef struct Vecteur
{
	double x;
	double y;
}VECTEUR;

// calculates vector norm
double vector_norm(VECTEUR v);

// calculates distance between 2 vectors
double vector_distance(VECTEUR v1, VECTEUR v2);

#endif
