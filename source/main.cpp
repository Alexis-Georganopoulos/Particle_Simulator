/* File name: main.cpp
 * 
 * Description: 
 * 	Responsible for redirecting user input (argv) and making it usable for the 
 *  reading module (sim.h). 
 * 	Error mode tests the validity of input data.
 * 	Force mode prints the mutual force between two particles.
 * 
 * Date: 14.05.2019
 * Version: 3.05
 * 
 * Authors - SCIPER:
 * 	Alexis Philip George-Georganopoulos - 228168
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include <GL/glui.h>
extern "C" 
{
	#include "sim.h"
	#include "constantes.h"
	#include "error.h"
}

namespace
{ 
	enum MODE mode;
	
    GLfloat aspect_ratio = 1.f; 
    int main_window, nb_g = 0, nb_bh = 0, nb_p = 0;
    float width, height;
    float gauche= -RMAX, droite = RMAX, bas= -RMAX, haut= RMAX;
    
    GLUI *glui;
    GLUI_Panel *file, *simulation, *information;
    GLUI_Button *load, *save, *start, *step, *quit;
    
    GLUI_EditText *load_file, *save_file, 
				  *nb_particule, *nb_generateur, *nb_trou_noir;
    const char *start_b = "Start"; 
    
    int running = 0, flag = 0, selected = 0;
    double ortho_x, ortho_y;
}

#define BUTTON1_ID 11
#define BUTTON2_ID 12
#define BUTTON3_ID 13
#define BUTTON4_ID 14

//------------------------------------------------------------------------------
enum MODE determine_mode(char * mode)
{
	if(strncmp(mode, "Error", sizeof("Error")) == 0) 
		return ERROR;
	else if(strncmp(mode, "Force", sizeof("Force")) == 0) 
		return FORCE;
	else if(strncmp(mode, "Integration", sizeof("Integration")) == 0)
		return INTEGRATION;
	else if(strncmp(mode, "Graphic", sizeof("Graphic")) == 0) 
		return GRAPHIC;
	else if(strncmp(mode, "Simulation", sizeof("Simulation")) == 0)
		return SIMULATION;
	else return NOT_VALID;
}
//------------------------------------------------------------------------------
// control call back; a switch for each GLUI callback 
void control_cb( int control )
{
    switch(control)
    {
		case BUTTON1_ID :
			if(running) break;
			lecture(SIMULATION, load_file -> get_text());
			get_viewport(&gauche, &droite, &bas, &haut);
			
			float ratio; // allows for non streched layout on screen
			ratio =(float) (droite-gauche)/(haut-bas);
			if(ratio > 1)
			{   
				bas  *= ratio;
				haut *= ratio;
			}
			if(ratio < 1)
			{   
				gauche /= ratio;
				droite /= ratio;
			}
			
			flag = 1;
			glutPostRedisplay();
			
			break;
		case BUTTON2_ID :
			ecriture(save_file -> get_text());	
			break;
			
		case BUTTON3_ID :
			if(!running)
			{
				start -> set_name("Stop");
				running = 1;
			}
			else
			{
				start -> set_name("Start");
				running = 0;
			}
			break;
			
		case BUTTON4_ID :
			if(running)
			{
				running = 0;
				start -> set_name("Start");
			}
			if(mode == GRAPHIC) printf("sim_update\n");
			if(mode == SIMULATION)
			{
				sim_update();
				glutPostRedisplay();
			}
			break;
			
		default :
			printf("INVALID COMMAND\nPROGRAM TERMINATION\n");
			exit(EXIT_FAILURE);
			break;
	}
}

//------------------------------------------------------------------------------
// draws everything in window
void affichage(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
	if (aspect_ratio <= 1.)
       glOrtho(gauche, droite, bas/aspect_ratio, haut/aspect_ratio, -1.0, 1.0);
    else 
       glOrtho(gauche*aspect_ratio, droite*aspect_ratio, bas, haut, -1.0, 1.0);   
    if(flag)
    {
		redraw_all();
		nb_generateur-> set_int_val(nb_g);
		nb_trou_noir -> set_int_val(nb_bh);
		nb_particule -> set_int_val(nb_p);
	}
	glutSwapBuffers();
}  

//------------------------------------------------------------------------------
// keeps things in proportion on screen
void reshape(int w, int h)
{ 
    glViewport(0, 0, w, h);        
    aspect_ratio = (GLfloat) w / (GLfloat) h;
    width = (float) w;
    height = (float) h; 
} 

//------------------------------------------------------------------------------
// updates execution while user is not interacting
void idle(void)
{
	if(glutGetWindow() != main_window) glutSetWindow(main_window);
	get_nb(&nb_g, &nb_bh, &nb_p);
	nb_generateur -> set_int_val(nb_g);
	nb_trou_noir -> set_int_val(nb_bh);
	nb_particule -> set_int_val(nb_p);
	if(running && mode == GRAPHIC) printf("sim_update\n");
	if(running && mode == SIMULATION) sim_update();
	glutPostRedisplay();
}

//------------------------------------------------------------------------------
// mouse call back. selects element
void mouse(int button, int state, int x, int y)
{
	selected = 0;
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		selected = 1;
		if(aspect_ratio >= 1.f)
		{
			ortho_x = (x*(droite-gauche)/width + gauche) * aspect_ratio;
			ortho_y = haut - y*(haut-bas)/height;
		}
		if(aspect_ratio < 1.f)
		{
			ortho_x = x*(droite-gauche)/width + gauche;
			ortho_y = (haut - y*(haut-bas)/height) / aspect_ratio;
		}
		if(mode == GRAPHIC)	printf("sim_select\n");
		if(mode == SIMULATION) sim_select(ortho_x, ortho_y);
	}
	else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if(mode == GRAPHIC) printf("sim_release\n");
		if(mode == SIMULATION) sim_release();
		selected = 0;
	}
}

//------------------------------------------------------------------------------
// keyboard call back. deletes selected element
void keyboard(unsigned char key, int x, int y)
{
	if(selected && key == 'd')
	{ 
		if(mode == GRAPHIC)	printf("sim_delete\n");
		if(mode == SIMULATION) sim_delete(ortho_x, ortho_y);
	}
	selected = 0;
}

//------------------------------------------------------------------------------
// creates GUI
void create_glui(int argc, char * argv[])
{
	glui = GLUI_Master.create_glui("MICROCOSMOS", 0, 467, 0);
    
    file = glui -> add_panel("File", GLUI_PANEL_EMBOSSED);
    load_file = glui -> add_edittext_to_panel(file, "Filename");
    if(argc == 3)
    load_file -> set_text(argv[2]);
    
    load = glui -> add_button_to_panel(file, "Load", BUTTON1_ID, control_cb);
    save_file = glui -> add_edittext_to_panel(file, "Filename");
							
    save_file -> set_text("Save.txt");
    save = glui -> add_button_to_panel(file, "Save", BUTTON2_ID, control_cb);
    
    simulation = glui -> add_panel("Simulation", GLUI_PANEL_EMBOSSED);
    start = glui -> add_button_to_panel(simulation, start_b, BUTTON3_ID, 
										control_cb);
    step = glui -> add_button_to_panel(simulation, "Step", BUTTON4_ID, 
										control_cb);
    information = glui -> add_panel("Information", GLUI_PANEL_EMBOSSED);
	
	nb_particule = glui -> add_edittext_to_panel(information, "Nb Particule");		
	nb_generateur = glui -> add_edittext_to_panel(information, "Nb Generateur");
	nb_trou_noir = glui -> add_edittext_to_panel(information, "Nb Trou Noir");
    
    quit = glui -> add_button("Exit", EXIT_SUCCESS, (GLUI_Update_CB)exit);
    
    glui -> set_main_gfx_window(main_window);
}

//------------------------------------------------------------------------------
// program mainframe (see inside for more details)
int main(int argc, char * argv[])
{
	if(argc == 1) mode = SIMULATION;
	if((argc != 1) && (argc != 3))
	{
		error_msg((char *) "Usage: './sim.x mode_test fichier' ou './sim.x'\n");
		exit(EXIT_FAILURE);
	}
	if(argc == 3)
	{
		mode = determine_mode(argv[1]);
		lecture((int) mode, argv[2]);
		flag = 1;
	}
	
	if(flag)
	{
		get_viewport(&gauche, &droite, &bas, &haut);
		float ratio;
		ratio =(float) (droite-gauche)/(haut-bas);
		if(ratio > 1)	
		{   
			bas  *= ratio;
			haut *= ratio;
		}
		if(ratio < 1)
		{   
			gauche /= ratio;
			droite /= ratio;
		}	
		get_nb(&nb_g, &nb_bh, &nb_p);
	}
	
	// responsible for GLUT window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
    glutInitWindowSize(400, 600);
    main_window = glutCreateWindow("MICROCOSMOS");
    glClearColor(1.0, 1.0, 1.0, 0.0);
    
    glutDisplayFunc(affichage);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    
    // responsible for GUI
	create_glui(argc, argv);
   
    glutMainLoop();	
	return EXIT_SUCCESS;
}
