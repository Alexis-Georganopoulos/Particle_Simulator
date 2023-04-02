/* File name: sim.h
 * 
 * Description: 
 * 	Responsible for exporting the functions required by main.cpp in order to 
 *  begin interpreting input data.
 * 
 * Date: 29.03.2019
 * Version: 3.04
 * 
 * Authors - SCIPER:
 * 	Alexis Philip George-Georganopoulos - 228168
 */
 
#ifndef SIM_H
#define SIM_H

enum MODE {ERROR, FORCE, INTEGRATION, GRAPHIC, SIMULATION, NOT_VALID};

// redirects user input in file to other functions that test its validity and
// memorize the data
void lecture(int mode, const char* open_file);

// writes save file user inputs
void ecriture(const char *save_name);

// draws everything
void redraw_all(void);

// compares all postion data to obtain final viewport values
void get_viewport(float *x_min, float *x_max, float *y_min, float *y_max);

// gets the nb of all elements and sends them to main
void get_nb(int *a, int *b, int *c);

// updates simulation with a time DELTA_T
void sim_update(void);

// selects a particle is user presses mouse while mouse is on top of particle
void sim_select(double mouse_x, double mouse_y);

// releases particle when user stops pressing mouse
void sim_release(void);

// deletes particle if particle is selected, otherwise deletes closest entity
void sim_delete(double mouse_x, double mouse_y);

#endif
