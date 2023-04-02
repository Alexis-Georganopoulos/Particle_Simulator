/* File name: sim.c
 * 
 * Description: 
 * 	Responsible for interpreting user input and calling the appropriate module 
 * 	to proccess the data(error/generator/particle/black_hole) depending on the 
 *  type of execution mode(Simulation, Graphic, etc...).
 * 	When an error is detected, the execution of the program stops. 
 * 
 * Date: 14.05.2019
 * Version: 3.04
 * 
 * Authors - SCIPER:
 * 	Alexis Philip George-Georganopoulos - 228168
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constantes.h"
#include "error.h"
#include "generator.h"
#include "black_hole.h"
#include "particle.h"

#define NB_ELEMENTS 3

enum STATE {GENERATOR, BLACK_HOLE, PARTICLE};
enum MODE {ERROR, FORCE, INTEGRATION, GRAPHIC, SIMULATION, NOT_VALID};

static void decodage(enum STATE * state, FILE * fichier);
static void decodage_nb(enum STATE state, int * p_nb, char * ligne);
static float compare_2 (float a, float b, int type);

static int particle_selected;

//------------------------------------------------------------------------------
// redirects user input in file to other functions that test its validity and
// memorize the data
void lecture(enum MODE mode, const char* open_file)
{
	int nb;
	enum STATE state = GENERATOR;
	char ligne[LIGNE];
	FILE * fichier = NULL;
	
	char quick_fix[15] = "load_files/";//TODO more robust folder management
	if((fichier = fopen(strcat(quick_fix,open_file), "r")) != NULL) 
	{
		while(fgets(ligne, LIGNE, fichier))
		{
			if((ligne[0]=='#')||(ligne[0]=='\n')||(ligne[0]=='\r')) 
				continue;
			decodage_nb(state, &nb, ligne);
			decodage(&state, fichier);
		}
	}
	else 
	{
		error_fichier_inexistant();
		exit(EXIT_FAILURE);
	}
	if((int) state != NB_ELEMENTS)
	{
		error_fichier_incomplet();
		exit(EXIT_FAILURE);
	} 
	fclose(fichier);
		
	if(mode == ERROR)
	{
		error_success();
		exit(EXIT_SUCCESS);
	}
	else if(mode == FORCE) 
	{
		particule_force_rendu1();
		exit(EXIT_SUCCESS);
	}
	else if(mode == INTEGRATION) 
	{
		particule_integration_rendu2();
		exit(EXIT_SUCCESS);
	}
	else if(mode == GRAPHIC);
	else if(mode == SIMULATION);
	else error_msg("mode de test inutilisé"); 
}

//------------------------------------------------------------------------------
// draws everything
void redraw_all(void)
{
	draw_particles();
	draw_black_holes();
	draw_generators();
}

//------------------------------------------------------------------------------
// updates what is shown in window when "step" button is pressed, or when
// simultion is running
void sim_update(void)
{
	particle_update();
	int nbg, i;
	get_nb_g(&nbg);
	for(i=0; i<nbg; i++)
	{
		// probability is a number between 1 and 200 (inclusive), with each 
		// number equally likely to occur.
		int probability = (int) (200. * rand() / (RAND_MAX + 1.)) + 1;
		// a particle is generated on average once ever 200 DELTA_Ts
		if(probability<=1) particle_generate(i);
	}
}

//------------------------------------------------------------------------------
// calls for a function that selects a particle (prevents the selected particle 
// from moving) when a person clicks on the window
void sim_select(double mouse_x, double mouse_y)
{
	
	particle_selected = particle_select(mouse_x, mouse_y);
}

//------------------------------------------------------------------------------
// calls for a function that allows selected particle to move when user stops
// pressing on glut window
void sim_release(void)
{
	particle_release();
}

//------------------------------------------------------------------------------
// deletes particle if particle is selected, otherwise deletes closest 
// black hole or generator
void sim_delete(double mouse_x, double mouse_y)
{
	if(particle_selected) particle_user_delete();
	else
	{
		// if there are no bh's or g's: bh or g_distance = -1
		double bh_distance = black_hole_distance(mouse_x, mouse_y);
		double g_distance  = generator_distance(mouse_x, mouse_y);
		if(((g_distance < bh_distance) || bh_distance < 0) && g_distance>=0) 
			generator_delete(mouse_x, mouse_y);
		else if(((bh_distance <= g_distance)||g_distance < 0) && bh_distance>=0) 
			black_hole_delete(mouse_x, mouse_y);
	}
}

//------------------------------------------------------------------------------
// gets the nb of all elements and sends them to main
void get_nb(int *a, int *b, int *c)
{
	get_nb_g(a);
	get_nb_bh(b);
	get_nb_p(c);
}

//------------------------------------------------------------------------------
// writes save file user inputs
void ecriture(const char *save_name)
{
	FILE *save_file = fopen(save_name, "w");
	
	save_data_g(save_file);
	save_data_bh(save_file);
	save_data_p(save_file);
	
	fclose(save_file);
}

//------------------------------------------------------------------------------
// compares all postion data to obtain final viewport values
void get_viewport(float *x_min, float *x_max, float *y_min, float *y_max)
{
	float x_min1, x_min2, x_min3, x_max1, x_max2, x_max3, y_min1, y_min2, y_min3,
		  y_max1, y_max2, y_max3;
	int a = viewport_g(&x_min1, &x_max1, &y_min1, &y_max1);
	int b = viewport_bh(&x_min2, &x_max2, &y_min2, &y_max2);
	int c = viewport_p(&x_min3, &x_max3, &y_min3, &y_max3);
	
	switch(a+b+c)
	{
		case 3 :
			*x_min = compare_2(x_min1, compare_2(x_min2, x_min3, -1), -1)-RMAX;
			*x_max = compare_2(x_max1, compare_2(x_max2, x_max3, 1), 1) + RMAX;
			*y_min = compare_2(y_min1, compare_2(y_min2, y_min3, -1), -1)-RMAX;
			*y_max = compare_2(y_max1, compare_2(y_max2, y_max3, 1), 1) + RMAX;
			break;	
		case 2 :
			if(a+b == 2)
			{
				*x_min = compare_2(x_min1, x_min2, -1)- RMAX;
				*x_max = compare_2(x_max1, x_max2, 1) + RMAX;
				*y_min = compare_2(y_min1, y_min2, -1)- RMAX;
				*y_max = compare_2(y_max1, y_max2, 1) + RMAX;
			}
			else if(b+c == 2)
			{
				*x_min = compare_2(x_min2, x_min3, -1)- RMAX;
				*x_max = compare_2(x_max2, x_max3, 1) + RMAX;
				*y_min = compare_2(y_min2, y_min3, -1)- RMAX;
				*y_max = compare_2(y_max2, y_max3, 1) + RMAX;
			}
			else
			{
				*x_min = compare_2(x_min1, x_min3, -1)- RMAX;
				*x_max = compare_2(x_max1, x_max3, 1) + RMAX;
				*y_min = compare_2(y_min1, y_min3, -1)- RMAX;
				*y_max = compare_2(y_max1, y_max3, 1) + RMAX;
			}
			break;
		case 1 :
			if(a)
			{
				*x_min = x_min1 - RMAX; 	*x_max = x_max1 + RMAX;
				*y_min = y_min1 - RMAX; 	*y_max = y_max1 + RMAX;
			}
			else if(b)
			{
				*x_min = x_min2 - RMAX; 	*x_max = x_max2 + RMAX;
				*y_min = y_min2 - RMAX;		*y_max = y_max2 + RMAX;
			}
			else
			{
				*x_min = x_min3 - RMAX;		*x_max = x_max3 + RMAX;
				*y_min = y_min3 - RMAX;		*y_max = y_max3 + RMAX;
			}
			break;
		case 0 :  break;
		default : 
			error_msg("in: get_viewport\n");
			exit(EXIT_FAILURE);
	}	
}

//------------------------------------------------------------------------------
// depending on the state of the reading, this reads the number of generators,
// black holes or particles.
static void decodage_nb(enum STATE state, int * p_nb, char * ligne)
{
	switch(state)
	{
		case GENERATOR:
			read_nb_gen(p_nb, ligne);
			break;
		
		case BLACK_HOLE:
			read_nb_hole(p_nb, ligne);
			break;
		
		case PARTICLE:
			read_nb_part(p_nb, ligne);
			break;
	}
}

//------------------------------------------------------------------------------
// depending on the state of reading this calls for functions that test and
// memorize the data in their appropriate modules
static void decodage(enum STATE * state, FILE * fichier)
{
	switch(*state)
	{	
		case GENERATOR:
			read_gen(fichier, (int *) state);	
			break;
		
		case BLACK_HOLE:
			read_hole(fichier, (int *) state); 
			break;
		
		case PARTICLE:
			read_particle(fichier, (int *) state);	
			break;
	}
}

//------------------------------------------------------------------------------
// compares 2 values and gives min or max upon demand
static float compare_2 (float a, float b, int type)
{
	switch(type)
	{
		case -1 :
			if(a < b) return a;
			else return b;
		break;
		
		case 1 :
			if(a < b) return b;
			else return a;
		break;
		
		default : 
			error_msg("in: compare_2");
			exit(EXIT_FAILURE);
	}
}
