/* File name: generator.c
 * 
 * Description: 
 * 	Responsible for storing & testing the generator data interpreted by sim.c. 
 * 	Upon detection of an error in the generator data, an error message is shown, 
 * 	followed by the termination of the program. 
 * 
 * Date: 11.05.2019
 * Version: 3.03
 * 
 * Authors - SCIPER:
 * 	Alexis Philip George-Georganopoulos - 228168
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "vector.h"
#include "constantes.h"
#include "error.h"
#include "graphic.h"

typedef struct Generateur GENERATEUR;
struct Generateur
{
	double rgen;			// radius of particle it generates, in [RMIN, RMAX]
	VECTEUR pos;			// position of generator
	VECTEUR vpi;			// initial speed < MAX_VITESSE of particle it gens
    GENERATEUR * next_g;    // points to next particle in list 
};

static void test_generator(int indice, double rgen, VECTEUR vpi);
static void memorize_generator(double rgen, VECTEUR pos, VECTEUR vpi, 
							   GENERATEUR * generator);
static GENERATEUR * insert_link_g(void);
static void delete_link_g(int indice);
static void empty_list_g(void);
static GENERATEUR * access_generator(int indice);

static GENERATEUR * head_g = NULL;
static int nb_gen;

//--­---------------------------------------------------------------------------
// reads the number of generators and memorizes this number in the varaible nb,
// previously declared in lecture()
void read_nb_gen(int * p_nbg, char * ligne)
{
	if(head_g != NULL)
	empty_list_g();
	
    int i;
	if(1!=sscanf(ligne, "%d", p_nbg))
	{
		error_lect_nb_elements(ERR_GENERAT);
		exit(EXIT_FAILURE);
	}
	
	nb_gen = *p_nbg;
	
    for (i = 0; i < nb_gen; i++) head_g = insert_link_g();
}	

//------------------------------------------------------------------------------
//reads the generator data, and then calls for functions to test and memorize it
void read_gen(FILE * fichier, int * counter)
{	
	int i; 
	double rgen;
	VECTEUR vpi, pos;
	char ligne[LIGNE];
    GENERATEUR * current = head_g;
	for(i=0; i<nb_gen; i++)
	{
		do{ 
			if(!fgets(ligne, LIGNE, fichier))
			{
				error_fichier_incomplet();
				exit(EXIT_FAILURE);
			}
		}while((ligne[0]=='#')||(ligne[0]=='\n')||(ligne[0]=='\r'));
		if(5!=sscanf(ligne,"%lf%lf%lf%lf%lf",&rgen,&pos.x,&pos.y,&vpi.x,&vpi.y))
		{
			error_lecture_elements(ERR_GENERAT, ERR_PAS_ASSEZ);
			exit(EXIT_FAILURE);
		}
		test_generator(i, rgen, vpi);
		memorize_generator(rgen, pos, vpi, current);	
        if(current->next_g != NULL) current = current->next_g;
	}
	do{ 
		if(!fgets(ligne, LIGNE, fichier))
		{
			error_fichier_incomplet();
			exit(EXIT_FAILURE);
		}
	}while((ligne[0]=='#')||(ligne[0]=='\n')||(ligne[0]=='\r'));
	if(strstr(ligne, "FIN_LISTE") == NULL)
	{
		error_lecture_elements(ERR_GENERAT, ERR_TROP);
		exit(EXIT_FAILURE);
	}
	else *counter = *counter + 1;
}

//------------------------------------------------------------------------------
// draws generators
void draw_generators(void)
{
	int i;
	double vitesse;
	GENERATEUR *current = head_g;
	for(i = 0; i < nb_gen && current; i++)
	{
		graphic_set_color3f(0.0, 0.0, 1.0);
		graphic_draw_circle(current->pos.x, current->pos.y, 2, 
							GRAPHIC_FILLED);
		graphic_set_line_width(3);
		vitesse = vector_norm(current->vpi);
		if(vitesse >= EPSILON_ZERO) 
			graphic_draw_arrow(current->pos.x, current->pos.y, 
							   current->vpi.x, current->vpi.y);
		graphic_set_line_width(1);
		current = current->next_g;
	}
}

//------------------------------------------------------------------------------
// transfers nb generators to sim (which then transfers it to main)
void get_nb_g(int *a)
{
	*a = nb_gen;
}

//------------------------------------------------------------------------------
// saves generator data in the save file inputed by user
void save_data_g(FILE *file)
{
	int i;
	GENERATEUR *current = head_g;
	fprintf(file, "\n%d\n", nb_gen);
	for(i = 0; i < nb_gen; i++)
	{
		current = access_generator(i);
		fprintf(file, "%lf %lf %lf %lf %lf\n", current -> rgen, 
											   current -> pos.x, 
											   current -> pos.y, 
											   current -> vpi.x, 
											   current -> vpi.y);
	}
	fputs("FIN_LISTE\n", file);
}

//------------------------------------------------------------------------------
// calculates the max and min positions for the generators returns 0 if no gens
int viewport_g(float *x_min, float *x_max, float *y_min, float *y_max)
{
	int i;
	if(nb_gen == 0)
	return 0;
	GENERATEUR * current = head_g;
	*x_min = (float)(current -> pos.x);
	*x_max = (float)(current -> pos.x);
	*y_min = (float)(current -> pos.y);
	*y_max = (float)(current -> pos.y);
	for(i = 0; i < nb_gen && current; i++)
	{
		if((float)(current -> pos.x) < *x_min)
			*x_min = (float)(current -> pos.x);
			
		if((float)(current -> pos.x) > *x_max)
			*x_max = (float)(current -> pos.x);
			
		if((float)(current -> pos.y) < *y_min)
			*y_min = (float)(current -> pos.y);
			
		if((float)(current -> pos.y) > *y_max)
			*y_max = (float)(current -> pos.y);
		current = current->next_g;
	}
	return 1;
}

//------------------------------------------------------------------------------
// sends info of a generator of index indice to particle module
void generator_get_info(int indice, double *rayon, double *posx, double *posy, 
						double *vpix, double *vpiy)
{
	GENERATEUR * generator = access_generator(indice);
	*rayon = generator->rgen;
	*posx = generator->pos.x;
	*posy = generator->pos.y;
	*vpix = generator->vpi.x;
	*vpiy = generator->vpi.y;
}	

//------------------------------------------------------------------------------
// returns distance from mouse to closest generator
double generator_distance(double mouse_x, double mouse_y)
{	
	if(nb_gen)
	{
		VECTEUR mouse;
		mouse.x = mouse_x;
		mouse.y = mouse_y;
		GENERATEUR * generator = head_g;
		int i;
		double min_distance = vector_distance(mouse, generator->pos);
		double distance;
		for(i=0; i<nb_gen && generator; i++)
		{
			distance = vector_distance(mouse, generator->pos);
			if(distance < min_distance) min_distance = distance;
			generator = generator->next_g;
		}
		return min_distance;
	}
	return -1.;
}

//------------------------------------------------------------------------------
// deletes a generator if a generator is selected when user presses 'd'
void generator_delete(double mouse_x, double mouse_y)
{
	VECTEUR mouse;
	mouse.x = mouse_x;
	mouse.y = mouse_y;
	GENERATEUR * generator = head_g;
	double min_distance = vector_distance(mouse, generator->pos);
	double distance;
	int to_delete = -1, i;
	for(i=0; i<nb_gen && generator; i++)
	{
		distance = vector_distance(mouse, generator->pos);
		if(distance < min_distance)
		{
			min_distance = distance;
			to_delete = i;
		}
		generator = generator->next_g;
	}
	delete_link_g(to_delete);
}

//------------------------------------------------------------------------------
// tests generator data for validity
static void test_generator(int indice, double rgen, VECTEUR vpi)
{
	if(rgen < RMIN || rgen > RMAX)
	{
		error_rayon_partic(ERR_GENERAT, indice);
		exit(EXIT_FAILURE);
	}

	double vitesse = vector_norm(vpi);
	if(vitesse>MAX_VITESSE)
	{
		error_vitesse_partic(ERR_GENERAT, indice);
		exit(EXIT_FAILURE);
	}
}

//------------------------------------------------------------------------------
// if the generator data is valid, it is memorized in a linked list
static void memorize_generator(double rgen, VECTEUR pos, VECTEUR vpi, 
                               GENERATEUR * generator)
{
	generator->rgen = rgen;
	generator->pos.x = pos.x;
	generator->pos.y = pos.y;
	generator->vpi.x= vpi.x;
	generator->vpi.y= vpi.y;
}

//------------------------------------------------------------------------------
// adds a generator to linked list
static GENERATEUR * insert_link_g(void)
{
    GENERATEUR *g = (GENERATEUR *)malloc(sizeof(GENERATEUR));
    if(g == NULL)
    {
        printf("black hole: error in %s\n", __func__);
        exit(EXIT_FAILURE);
    }
	g -> next_g = head_g;
	return g;
}

//------------------------------------------------------------------------------
// deletes a generator of a certain index from list
static void delete_link_g(int indice)
{
	GENERATEUR * current = head_g;
	GENERATEUR * previous = NULL;
	while(current!=NULL && indice>0)
	{
		previous = current;
		current = current->next_g;
		indice--;
	}
	if(current != head_g)
	{
		previous->next_g = current->next_g;
		free(current);
	}
	else
	{
		head_g = current->next_g;
		free(current);
	}
	nb_gen--;
}

//------------------------------------------------------------------------------
// empties entire generator list
static void empty_list_g(void)
{
	while(nb_gen > 0) delete_link_g(0);
}

//------------------------------------------------------------------------------
// access a generator of a certain index from list
static GENERATEUR * access_generator(int indice)
{
    int i;
    GENERATEUR * current = head_g;
    for(i=0; i<indice; i++)
    {
        if(current->next_g != NULL) current = current->next_g;
    }
    return current;
}

