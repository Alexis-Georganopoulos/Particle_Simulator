/* File name: black_hole.h
 * 
 * Description: 
 * 	Responsible for exporting the functions required by sim.c in order to begin 
 *  interpreting black hole data. 
 *
 * Date: 11.05.2019
 * Version: 3.01
 * 
 * Authors - SCIPER:
 * 	Alexis Philip George-Georganopoulos - 228168
 */

#ifndef BLACK_HOLE_H
#define BLACK_HOLE_H

// reads particle data and calls for functions to memorize it
void read_hole(FILE * fichier, int * counter);

// reads the number of black holes and memorizes this number in the varaible nb,
// previously declared in lecture()
void read_nb_hole(int * p_nbp, char * ligne);

// draws the black holes
void draw_black_holes(void);

// saves the black hole data onto the save file the user inputs
void save_data_bh(FILE *file);

// calculates the max and min positions for the black holes 
int viewport_bh(float *x_min, float *x_max, float *y_min, float *y_max);

// transfers nb black holes to sim (which then transfers it to main)
void get_nb_bh(int *b);

// sends position of black hole of index indice to particle module
void black_hole_get_pos(int indice, double *bh_posx, double *bh_posy);

// returns the distance from mouse to closest black hole
double black_hole_distance(double mouse_x, double mouse_y);

// deletes black hole if one is selected when user presses 'd'
void black_hole_delete(double mouse_x, double mouse_y);

#endif
