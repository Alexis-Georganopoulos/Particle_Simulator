/* File name: generator.h
 * 
 * Description: 
 * 	Responsible for exporting the functions required by sim.c in order to begin 
 *  interpreting generator data. 
 * 
 * Date: 11.05.2015
 * Version 3.06
 * 
 * Authors - SCIPER:
 * 	Nawel Naas - 245996
 * 	Alexis Philip George-Georganopoulos - 228168
 */

#ifndef GENERATOR_H
#define GENERATOR_H

//reads the generator data, and then calls for functions to test and memorize it
void read_gen(FILE * fichier, int * counter);

// reads the number of generators and memorizes this number in the varaible nb,
// previously declared in lecture()
void read_nb_gen(int * p_nbp, char * ligne);

// draws generators
void draw_generators(void);

// saves generator data in the save file inputed by user
void save_data_g(FILE *file);

// calculates the max and min positions for the generators, returns 0 if no gens
int viewport_g(float *x_min, float *x_max, float *y_min, float *y_max);

// transfers nb generators to sim (which then transfers it to main)
void get_nb_g(int *a);

// sends info of a generator of index indice to particle module
void generator_get_info(int indice, double *rayon, double *posx, double *posy, 
						double *vpix, double *vpiy);

// returns distance from mouse to closest generator
double generator_distance(double mouse_x, double mouse_y);

// deletes a generator if a generator is selected when user presses 'd'
void generator_delete(double mouse_x, double mouse_y);

#endif
