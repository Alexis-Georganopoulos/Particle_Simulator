/* File name: particle.c
 * 
 * Description: 
 * 	Responsible for storing & testing the particle data interpreted by sim.c. 
 * 	Also responsible for calculating the forces on a particle and updating each 
 * 	particle's data. 
 *  Upon detection of an error in the particle data, an error message is shown, 
 *  followed by the termination of the program. 
 * 
 * Date: 11.05.2015
 * Version: 3.03
 * 
 * Authors - SCIPER:
 * 	Nawel Naas - 245996
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
#include "black_hole.h"
#include "generator.h"

typedef struct Particule PARTICULE;
struct Particule
{
	double rayon;			// radius of particle, in [RMIN,RMAX]
	VECTEUR pos;			// position of particle after update
	VECTEUR prev_pos;		// position of particle before update
	VECTEUR vit;			// speed of particle, norm < MAX_VITESSE
	PARTICULE *next_p;		// points to next particle in list
};

static void test_particle(int indice, double rayon, VECTEUR pos, VECTEUR vit);
static void memorize_particle(double rayon, VECTEUR pos, VECTEUR vit, 
                              PARTICULE *current);
static double force2part(VECTEUR pos1, VECTEUR pos2, double ray1, double ray2);
static double min(double num1, double num2);
static VECTEUR force_integration(PARTICULE * particle0);
static VECTEUR calc_acceleration(double rayon, VECTEUR force);
static VECTEUR new_velocity(PARTICULE particle, VECTEUR acceleration);
static VECTEUR new_position(PARTICULE particle, VECTEUR velocity);
static PARTICULE * insert_link_p(void);
static void delete_link_p(int indice);
static void empty_list_p(void);
static PARTICULE * access_particle(int i);
static double force_black_hole(double distance);
static VECTEUR force_total(int indice, PARTICULE * particle);

static PARTICULE * head_p = NULL;
static PARTICULE * no_update = NULL;
static int nb_part;

//------------------------------------------------------------------------------
// reads the number of particles and memorizes this number in the varaible nb,
// previously declared in lecture()
void read_nb_part(int * p_nbp, char * ligne)
{
	if(head_p != NULL)
	empty_list_p();
	if(1!=sscanf(ligne, "%d", p_nbp))
	{
		error_lect_nb_elements(ERR_PARTIC);
		exit(EXIT_FAILURE);
	}
	nb_part = *p_nbp;
	int i;
	for (i = 0; i < nb_part; i++) head_p = insert_link_p();
}

//------------------------------------------------------------------------------
// reads particle data, and then calls for functions to test and memorize it
void read_particle(FILE * fichier, int * counter)
{
	int i;
	double rayon;
	VECTEUR pos, vit;
	char ligne[LIGNE];
	
	PARTICULE * current = head_p;
	
	for(i=0; i<nb_part; i++)
	{
		do{ 
			if(!fgets(ligne, LIGNE, fichier))
			{
				error_fichier_incomplet();
				exit(EXIT_FAILURE);
			}
		}while((ligne[0]=='#')||(ligne[0]=='\n')||(ligne[0]=='\r'));
		if(5!=sscanf(ligne,"%lf%lf%lf%lf%lf",&rayon,&pos.x,&pos.y,&vit.x,&vit.y))
		{
			error_lecture_elements(ERR_PARTIC, ERR_PAS_ASSEZ);
			exit(EXIT_FAILURE);
		}
		test_particle(i, rayon, pos, vit);
		memorize_particle(rayon, pos, vit, current);
		if(current -> next_p != NULL) current = current -> next_p;
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
		error_lecture_elements(ERR_PARTIC, ERR_TROP);
		exit(EXIT_FAILURE);
	}
	else *counter = *counter + 1;
}

//------------------------------------------------------------------------------
// calculates the interaction force between the first two particles and prints 
// it in the terminal
void particule_force_rendu1(void)
{
	PARTICULE * particle = head_p;
	double force_mag = force2part(particle->pos, particle->next_p->pos, 
								  particle->rayon, particle->next_p->rayon);
	printf("%8.3f\n", force_mag);
}

//------------------------------------------------------------------------------
// calculates the force acting on the first particle due to the interaction with
// all the others and prints its horizontal and vertical components. it then
// prints its speed and position after a time DELTA_T
void particule_integration_rendu2(void)
{
	if(nb_part)
	{
		int i;
		PARTICULE * current = head_p;
		for(i=0; i<nb_part && current; i++)
		{
			current->prev_pos = current->pos;
			current = current->next_p;
		}
			
		PARTICULE * particle0 = access_particle(0);
		
		VECTEUR force = force_integration(particle0); 
		printf("%8.3f %8.3f\n", force.x, force.y);

		VECTEUR acceleration, velocity, position;
		acceleration = calc_acceleration(particle0->rayon, force);
		velocity = new_velocity(*particle0, acceleration);
		position = new_position(*particle0, velocity);
	
		printf("%7.3f %7.3f %9.4f %9.4f\n", velocity.x, velocity.y, 
											position.x, position.y);
	}
}

//------------------------------------------------------------------------------
// draws particles
void draw_particles(void)
{
	int i;
	double vitesse;
	float red, green, blue;
	PARTICULE *current = head_p;
	for(i = 0; i < nb_part && current; i++)
	{
		vitesse = vector_norm(current -> vit);
		red = (float)(vitesse / MAX_VITESSE);
		green = (float)(0.2 * vitesse / MAX_VITESSE);
		blue = (float)(0.2 * vitesse / MAX_VITESSE);
		graphic_set_color3f(red, green, blue);
		graphic_set_line_width(3);
		graphic_draw_circle(current->pos.x, current->pos.y, 
							current->rayon, GRAPHIC_EMPTY);
		graphic_set_line_width(1);
		current = current->next_p;
	}
}

//------------------------------------------------------------------------------
// transfers nb particle to sim module (which then transfers it to main)
void get_nb_p(int *c)
{
	*c = nb_part;
}

//------------------------------------------------------------------------------
// saves particle data on savefile user inputs
void save_data_p(FILE *file)
{
	int i;
	PARTICULE *current = head_p;
	fprintf(file, "\n%d\n", nb_part);
	for(i = 0; i < nb_part && current; i++)
	{
		fprintf(file, "%lf %lf %lf %.10lf %.10lf\n",current -> rayon, 
													current -> pos.x, 
													current -> pos.y, 
													current -> vit.x, 
													current -> vit.y);
		current = current->next_p;
	}
	fputs("FIN_LISTE\n", file);
}

//------------------------------------------------------------------------------
// calculates min and max positions for particles
int viewport_p(float *x_min, float *x_max, float *y_min, float *y_max)
{
	int i;
	if(nb_part == 0) return 0;
	PARTICULE * current = head_p;
	*x_min = (float)(current -> pos.x);
	*x_max = (float)(current -> pos.x);
	*y_min = (float)(current -> pos.y);
	*y_max = (float)(current -> pos.y);
	for(i = 0; i < nb_part && current; i++)
	{	
		if((float)(current -> pos.x) < *x_min)
			*x_min = (float)(current -> pos.x);
			
		if((float)(current -> pos.x) > *x_max)
			*x_max = (float)(current -> pos.x);
			
		if((float)(current -> pos.y) < *y_min)
			*y_min = (float)(current -> pos.y);

		if((float)(current -> pos.y) > *y_max)
			*y_max = (float)(current -> pos.y);
		current = current->next_p;
	}
	return 1;
}

//------------------------------------------------------------------------------
// updates all particle data
void particle_update(void)
{
	PARTICULE * current = head_p;
	int i;
		
	VECTEUR force, acceleration, velocity, position;
	
	while(current)
	{
		current->prev_pos = current->pos;
		current = current->next_p;
	}
	
	current = head_p;			
	for(i=0; i<nb_part && current; i++)
	{
		if(no_update)
		{	
			if(no_update->next_p == current->next_p) 
			{
				current = current->next_p;
				continue;
			}
		}
		force = force_total(i, current);
		acceleration = calc_acceleration(current->rayon, force);
		velocity = new_velocity(*current, acceleration);
		position = new_position(*current, velocity);
		
		current->vit = velocity;
		current->pos = position;
		
		current = current->next_p;
	}
}

//------------------------------------------------------------------------------
// selects particle user presses on, returns 1 if selection occurs, 0 otherwise
int particle_select(double mouse_x, double mouse_y)
{
	int not_to_update;
	VECTEUR mouse;
	mouse.x = mouse_x;
	mouse.y = mouse_y;
	PARTICULE * particle = head_p;
	int i, counter = 0, selected[nb_part];
	double distance, selected_distance[nb_part];
	for(i=0; i<nb_part && particle; i++)
	{
		distance = vector_distance(mouse, particle->pos);
		if(distance < particle->rayon)
		{
			selected[counter] = i;
			selected_distance[counter] = distance;
			counter++;
		}
		particle = particle->next_p;
	}
	if(counter == 1)
	{
		not_to_update = selected[0];
		no_update = access_particle(not_to_update);
		return 1;
	}
	else if(counter > 1)
	{
		double min_distance = selected_distance[0];
		int to_select = selected[0];
		for(i=1; i<counter; i++)
		{
			if(selected_distance[i] < min_distance) 
			{	
				min_distance = selected_distance[i];
				to_select = selected[i];
			}
		}
		not_to_update = to_select;
		no_update = access_particle(not_to_update);
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
// when user stops selecting particle, particle is released
void particle_release(void)
{
	no_update = NULL;
}

//------------------------------------------------------------------------------
// if particle is selected when user presses 'd', this function deletes particle
void particle_user_delete(void)
{
	int index_to_delete=0;
	PARTICULE * current = head_p;
	if(no_update)
	{	
		while(no_update->next_p != current->next_p)
		{
			index_to_delete++;
			current=current->next_p;
		}
		delete_link_p(index_to_delete);
	}
	no_update = NULL;
}

//------------------------------------------------------------------------------
// generates a particle on average once every 200 DELTA_Ts for each generator
void particle_generate(int gen_indice)
{
	double rayon, distance;
	VECTEUR pos, vpi;
	generator_get_info(gen_indice, &rayon, &pos.x, &pos.y, &vpi.x, &vpi.y);
	int i, counter = 0;
	PARTICULE *check = head_p;
	for(i=0; i<nb_part && check; i++)
	{
		distance = vector_distance(pos, check->pos);
		if(distance > check->rayon) counter++;
		check = check->next_p;
	}
	if(counter == nb_part)
	{
		PARTICULE *new_particle = insert_link_p();
		nb_part++;
		new_particle->rayon = rayon;
		new_particle->pos = pos;
		new_particle->vit = vpi;
	}
}

//------------------------------------------------------------------------------
// tests the validity of the particle data
static void test_particle(int indice, double rayon, VECTEUR pos, VECTEUR vit)
{
	if(rayon < RMIN || rayon > RMAX)
	{
		error_rayon_partic(ERR_PARTIC, indice);
		exit(EXIT_FAILURE);
	}
	
	double vitesse = vector_norm(vit);
	if(vitesse > MAX_VITESSE)
	{
		error_vitesse_partic(ERR_PARTIC, indice);
		exit(EXIT_FAILURE);
	}
}

//------------------------------------------------------------------------------
// if the particle data is valid, this memorizes the data in a linked list
static void memorize_particle(double rayon, VECTEUR pos, VECTEUR vit, 
                              PARTICULE *particle)
{	
	particle -> rayon = rayon;
	particle -> pos.x = pos.x;
	particle -> pos.y = pos.y;
	particle -> vit.x = vit.x;
	particle -> vit.y = vit.y;
}

//------------------------------------------------------------------------------
// calculates the force between any 2 particles
static double force2part(VECTEUR pos1, VECTEUR pos2, double ray1, double ray2)
{
	double distance, seuil_d, force_var;
	
	distance = vector_distance(pos1, pos2);
	seuil_d = ray1 + ray2 + min(ray1, ray2);
	force_var = distance/seuil_d;
	
	if     (force_var>=0&& force_var<=1)	return -MAX_REP*(force_var-1);
	else if(force_var>1 && force_var<=2)	return  MAX_ATTR*(force_var-1);
	else if(force_var>2 && force_var<=3)	return -MAX_ATTR*(force_var-3);
	else if(force_var>3)					return  0;
	else
	{
		error_msg("distance or seuil_d are (somehow) negative\n");
		exit(EXIT_FAILURE);
	}
}

//------------------------------------------------------------------------------
// returns the smallest number among two inputs
static double min(double num1, double num2)
{
	if(num1<num2) return num1;
	else return num2;
}

//------------------------------------------------------------------------------
// calculates the total x and y components of the particleinteraction force
// acting on a certain particle 
static VECTEUR force_integration(PARTICULE * particle0)
{
	int j;
    PARTICULE * particle1 = head_p;
	VECTEUR force = {0., 0.}, unit_direction;
	double force_mag, distance;
	for(j=0; j<nb_part && particle1; j++)
	{
		if(particle1 == particle0)
		{
			particle1 = particle1->next_p;
			continue;
		}
		force_mag = force2part(particle0->prev_pos, particle1->prev_pos, 
							   particle0->rayon, 	particle1->rayon);
		distance = vector_distance(particle0->prev_pos, particle1->prev_pos);
		if(distance >= EPSILON_ZERO)
		{
			unit_direction.x=(particle1->prev_pos.x-particle0->prev_pos.x)
							 /distance;
			unit_direction.y=(particle1->prev_pos.y-particle0->prev_pos.y)
							 /distance;
		}
		else
		{
			unit_direction.x = 0.;
			unit_direction.y = 1.;
		}
		force.x += force_mag * unit_direction.x;
		force.y += force_mag * unit_direction.y;
		particle1 = particle1->next_p;
	}
    return force;
}

//------------------------------------------------------------------------------
// finds the force between a particle and a black hole given the distance
// between them
static double force_black_hole(double distance)
{
	if(distance>RBLACK) return FBLACK_MIN;
	else 				return (double) -FBLACK/RBLACK * distance + FBLACK 
								+ FBLACK_MIN;
}

//------------------------------------------------------------------------------
// calculates acceleration of particle given its force and radius
static VECTEUR calc_acceleration(double rayon, VECTEUR force)
{
    VECTEUR acceleration;
    double mass = KMASSE * (rayon)*(rayon);
	acceleration.x = force.x/mass;
	acceleration.y = force.y/mass;
    return acceleration; 
}

//------------------------------------------------------------------------------
// calculates the new velocity of a particle after a force acts on it for a time 
// DELTA_T
static VECTEUR new_velocity(PARTICULE particle, VECTEUR acceleration)
{
	VECTEUR velocity;
	
	velocity.x = particle.vit.x;
	velocity.y = particle.vit.y;
	
	velocity.x += acceleration.x * DELTA_T;
	velocity.y += acceleration.y * DELTA_T;
	
	double magnitude = vector_norm(velocity);
	
	if(magnitude > MAX_VITESSE)
	{
		double coefficient = (double) (MAX_VITESSE-EPSILON_ZERO) / magnitude;
		velocity.x *= coefficient;
		velocity.y *= coefficient;
	}
	return velocity;
}

//------------------------------------------------------------------------------
// calculates the new position of a particle after a time DELTA_T
static VECTEUR new_position(PARTICULE particle, VECTEUR velocity)
{
	VECTEUR position;
	
	position.x = particle.pos.x;
	position.y = particle.pos.y;
	
	position.x += velocity.x * DELTA_T;
	position.y += velocity.y * DELTA_T;
	
	return position;
}		

//------------------------------------------------------------------------------
// adds link to chain / linked list
static PARTICULE * insert_link_p(void)
{
	PARTICULE *p = (PARTICULE*)malloc(sizeof(PARTICULE));
	if(p == NULL)
	{
		printf("particle: error in %s\n", __func__);
		exit(EXIT_FAILURE);
	}
	p -> next_p = head_p;
	head_p = p;
	return p;
}

//------------------------------------------------------------------------------
// deletes a particle of a certain index
static void delete_link_p(int indice)
{
	PARTICULE * current = head_p;
	PARTICULE * previous = NULL;
	while(current!=NULL && indice>0)
	{
		previous = current;
		current = current->next_p;
		indice--;
	}
	if(previous && current && current != head_p)
	{
		previous->next_p = current->next_p;
		free(current);
	}
	else if(current)
	{
		head_p = current->next_p;
		free(current);
	}
	nb_part--;
}

//------------------------------------------------------------------------------
// empties entire particle list
static void empty_list_p(void)
{
	while(nb_part > 0) delete_link_p(0);
}

//------------------------------------------------------------------------------
// access a particle of a certain index
static PARTICULE * access_particle(int indice)
{
    int i;
    PARTICULE * current = head_p;
    for(i=0; i<indice; i++)
    {
        if(current->next_p) current = current->next_p;
    }
    return current;
}

//------------------------------------------------------------------------------
// calculates the total force acting on one particle (including the force acting
// on a particle due to black hole attraction)
static VECTEUR force_total(int indice, PARTICULE * particle)
{
	VECTEUR force, black_hole_pos, unit_direction;
	double distance, force_mag;
	int nb_black_hole, j;
	force = force_integration(particle);
	
	get_nb_bh(&nb_black_hole);

	for(j=0; j<nb_black_hole; j++)
	{
		black_hole_get_pos(j, &black_hole_pos.x, &black_hole_pos.y);
		distance = vector_distance(black_hole_pos, particle->prev_pos);
		
		force_mag = force_black_hole(distance);
		if(distance >= EPSILON_ZERO)
		{
			unit_direction.x = (black_hole_pos.x-particle->prev_pos.x)/distance;
			unit_direction.y = (black_hole_pos.y-particle->prev_pos.y)/distance;
		
			force.x += force_mag*unit_direction.x;
			force.y += force_mag*unit_direction.y;
		}
		if(((distance < particle->rayon) && nb_part) || distance < EPSILON_ZERO)
		{
			delete_link_p(indice);
			force.x = 0;	force.y = 0;
			break;
		}
	}	
	return force;
}
