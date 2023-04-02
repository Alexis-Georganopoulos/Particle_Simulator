/* File name: black_hole.c
 * 
 * Description: 
 * 	Responsible for storing & testing the black hole data interpreted by sim.c. 
 * 	Upon detection of an error in the black hole data, an error message is 
 * 	shown, followed by the termination of the program. 
 *
 * Date: 11.05.2019
 * Version: 3.04
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

typedef struct Trou_Noir TROU_NOIR;
struct Trou_Noir
{
	VECTEUR pos; 			// position of black hole
	TROU_NOIR *next_bh;		// points to next black hole in list
};

static TROU_NOIR * access_black_hole(int indice);
static void memorize_blackhole(VECTEUR pos, TROU_NOIR * hole);
static TROU_NOIR * insert_link_bh(void);
static void delete_link_bh(int indice);
static void empty_list_bh(void);

static TROU_NOIR *head_bh = NULL;
static int nb_hole;

//------------------------------------------------------------------------------
// reads the number of black holes and memorizes this number in the varaible nb,
// previously declared in lecture()
void read_nb_hole(int * p_nbh, char * ligne)
{
	if(head_bh != NULL)
	empty_list_bh();
	
	if(1!=sscanf(ligne, "%d", p_nbh))
	{
		error_lect_nb_elements(ERR_TROU_N);
		exit(EXIT_FAILURE);
	}
	nb_hole = * p_nbh;
	int i;
	for (i = 0; i < nb_hole; i++) head_bh = insert_link_bh();
}

//------------------------------------------------------------------------------
// reads particle data and calls for functions to memorize it
void read_hole(FILE * fichier, int * counter)
{	
    int i;
	VECTEUR pos;
	char ligne[LIGNE];
	TROU_NOIR * current = head_bh;
	for(i=0; i<nb_hole; i++)
	{
		do{ 
			if(!fgets(ligne, LIGNE, fichier))
			{
				error_fichier_incomplet();
				exit(EXIT_FAILURE);
			}	
		}while((ligne[0]=='#')||(ligne[0]=='\n')||(ligne[0]=='\r'));
		if(2!=sscanf(ligne,"%lf %lf", &pos.x, &pos.y))
		{
			error_lecture_elements(ERR_TROU_N, ERR_PAS_ASSEZ);
			exit(EXIT_FAILURE);
		}
		memorize_blackhole(pos, current);
		if(current->next_bh != NULL) current = current->next_bh;
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
		error_lecture_elements(ERR_TROU_N, ERR_TROP);
		exit(EXIT_FAILURE);
	}
	else *counter = *counter + 1;
}

//------------------------------------------------------------------------------
// draws the black holes
void draw_black_holes(void)
{
	int i;
	TROU_NOIR *current = head_bh;
	for(i = 0; i < nb_hole && current; i++)
	{
		graphic_set_color3f(0.0, 1.0, 0.0);
		graphic_set_line_width(3);
		graphic_draw_dotted_circle(current -> pos.x, current -> pos.y, RBLACK);
		graphic_set_line_width(1);
		graphic_draw_losange(current -> pos.x, current -> pos.y);
		current = current->next_bh;
	}
}

//------------------------------------------------------------------------------
// transfers nb black holes to sim (which then transfers it to main)
void get_nb_bh(int *b)
{
	*b = nb_hole;
}

//------------------------------------------------------------------------------
// saves the black hole data onto the save file the user inputs
void save_data_bh(FILE *file)
{
	int i;
	TROU_NOIR * current = head_bh;
	fprintf(file, "\n%d\n", nb_hole);
	for(i = 0; i < nb_hole; i++)
	{
		current = access_black_hole(i);
		fprintf(file, "%lf %lf\n", current -> pos.x, current -> pos.y);
	}
	fputs("FIN_LISTE\n", file);
}

//------------------------------------------------------------------------------
// calculates the max and min positions for the black holes returns 0 if no bhs
int viewport_bh(float *x_min, float *x_max, float *y_min, float *y_max)
{
	int i;
	if (nb_hole == 0)
	return 0;
	TROU_NOIR * current = head_bh;
	*x_min = (float)(current -> pos.x);
	*x_max = (float)(current -> pos.x);
	*y_min = (float)(current -> pos.y);
	*y_max = (float)(current -> pos.y);
	for(i = 0; i < nb_hole && current; i++)
	{
		if((float)(current -> pos.x) < *x_min)
			*x_min = (float)(current -> pos.x);
			
		if((float)(current -> pos.x) > *x_max)
			*x_max = (float)(current -> pos.x);
			
		if((float)(current -> pos.y) < *y_min)
			*y_min = (float)(current -> pos.y);
			
		if((float)(current -> pos.y) > *y_max)
			*y_max = (float)(current -> pos.y);
		current = current-> next_bh;
	}
	return 1;
}

//------------------------------------------------------------------------------
// sends position of black hole of index indice to particle module
void black_hole_get_pos(int indice, double * bh_posx, double * bh_posy)
{
	TROU_NOIR * hole = access_black_hole(indice);
	*bh_posx = hole->pos.x;
	*bh_posy = hole->pos.y;
}

//------------------------------------------------------------------------------
// returns the distance from mouse to closest black hole
double black_hole_distance(double mouse_x, double mouse_y)
{
	if(nb_hole)
	{
		VECTEUR mouse;
		mouse.x = mouse_x;
		mouse.y = mouse_y;
		TROU_NOIR * black_hole = head_bh;
		int i;
		double min_distance = vector_distance(mouse, black_hole->pos);
		double distance = 0;
		for(i=0; i<nb_hole && black_hole; i++)
		{
			distance = vector_distance(mouse, black_hole->pos);
			if(distance < min_distance) min_distance = distance;
			black_hole = black_hole->next_bh;
		}
		return min_distance;
	}
	return -1.;
}

//------------------------------------------------------------------------------
// deletes black hole if one is selected when user presses 'd'
void black_hole_delete(double mouse_x, double mouse_y)
{
	VECTEUR mouse;
	mouse.x = mouse_x;
	mouse.y = mouse_y;
	TROU_NOIR * black_hole = head_bh;
	double min_distance = vector_distance(mouse, black_hole->pos);
	double distance;
	int to_delete = -1, i;
	for(i=0; i<nb_hole && black_hole; i++)
	{
		distance = vector_distance(mouse, black_hole->pos);
		if(distance < min_distance)
		{
			min_distance = distance;
			to_delete = i;
		}
		black_hole = black_hole->next_bh;
	}
	delete_link_bh(to_delete);
}

//------------------------------------------------------------------------------
// if the black hole is valid, this function memorizes it in a linked list
static void memorize_blackhole(VECTEUR pos, TROU_NOIR * hole)
{
	hole->pos.x = pos.x;
	hole->pos.y = pos.y;
}

//------------------------------------------------------------------------------
// adds a black hole to linked list at head
static TROU_NOIR * insert_link_bh(void)
{
	TROU_NOIR *bh = (TROU_NOIR*)malloc(sizeof(TROU_NOIR));
    if(bh == NULL)
    {
        printf("black hole: error in %s\n", __func__);
        exit(EXIT_FAILURE);
    }
	bh -> next_bh = head_bh;
	return bh;
}

//------------------------------------------------------------------------------
// deletes a black hole of a certain index from list
static void delete_link_bh(int indice)
{
	TROU_NOIR * current = head_bh;
	TROU_NOIR * previous = NULL;
	while(current!=NULL && indice>0)
	{
		previous = current;
		current = current->next_bh;
		indice--;
	}
	if(current != head_bh)
	{
		previous->next_bh = current->next_bh;
		free(current);
	}
	else
	{
		head_bh = current->next_bh;
		free(current);
	}
	nb_hole--;
}

//------------------------------------------------------------------------------
// empties entire blackhole list
static void empty_list_bh(void)
{
	while(nb_hole > 0) delete_link_bh(0);
}

//------------------------------------------------------------------------------
// access a black hole of a certain index from list
static TROU_NOIR * access_black_hole(int indice)
{
    int i;
    TROU_NOIR * current = head_bh;
    for(i=0; i<indice; i++)
    {
        if(current->next_bh != NULL) current = current->next_bh;
    }
    return current;
}
