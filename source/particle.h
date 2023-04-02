/* File name: particle.h
 * 
 * Description: 
 * 	Responsible for exporting the functions required by sim.c in order to begin 
 *  interpreting particle data. 
 *  
 * Date: 11.05.2015
 * Version: 3.03
 * 
 * Authors - SCIPER:
 * 	Nawel Naas - 245996
 * 	Alexis Philip George-Georganopoulos - 228168
 */

#ifndef PARTICLE_H
#define PARTICLE_H

// reads particle data, and then calls for functions to test and memorize it
void read_particle(FILE * fichier, int * counter);

// reads the number of particles and memorizes this number in the varaible nb,
// previously declared in lecture()
void read_nb_part(int * p_nbp, char * ligne);

// calculates the interaction force between the first two particles and prints 
// it in the terminal
void particule_force_rendu1(void);

// calculates the force acting on the first particle due to the interaction with
// all the others and prints its horizontal and vertical components. it then
// prints its speed and position after a time DELTA_T
void particule_integration_rendu2(void);

// updates all particle data
void particle_update(void);

// I wonder what this does?
void draw_particles(void);

// saves particle data on savefile user inputs
void save_data_p(FILE *file);

// calculates min and max positions for particles
int viewport_p(float *x_min, float *x_max, float *y_min, float *y_max);

// transfers nb particle to sim module (which then transfers it to main)
void get_nb_p(int *c);

// generates a particle on average once every 200 DELTA_Ts for each generator
void particle_generate(int gen_indice);

// selects particle user presses on
int particle_select(double mouse_x, double mouse_y);

// when user stops selecting particle, particle is released
void particle_release(void);

// if particle is selected when user presses 'd', this function deletes particle
void particle_user_delete(void);

#endif
