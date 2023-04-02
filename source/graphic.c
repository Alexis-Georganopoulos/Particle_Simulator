/* File name: graphic.c
 * 
 * Description: 
 * 	Contains functions used to change colors and line width as well as draw 
 * 	certain shapes (segment, arrow, rectangle, losange, circle, dotted circle) 
 * 	onto window. These functions are called for by the particle, black_hole and 
 * 	generator modules.
 * 
 * Date: 16.04.2019
 * Version: 2.03
 * 
 * Authors - SCIPER:
 * 	Alexis Philip George-Georganopoulos - 228168
 */

#include <stdio.h>
#include <math.h>
#include <GL/glu.h>
#include "graphic.h"

void graphic_draw_segment (float x1, float y1, float x2, float y2)
{ 
    glBegin (GL_LINES);
    glVertex2f (x1, y1);
    glVertex2f (x2, y2);
    glEnd();
}

void graphic_draw_arrow(float xc, float yc, float vx, float vy) 
{ 
	glBegin (GL_LINES); 
	
	double vitesse = sqrt(vx*vx + vy*vy); 
	double x = 15*vx/vitesse;
	double y = 15*vy/vitesse; 
	
	glVertex2f (xc, yc); 
	glVertex2f (xc + x, yc + y); 
	
	double theta = atan2(vy,vx); 
	double x1 = xc + 11*cos(theta - 133*M_PI/1800); 
	double y1 = yc + 11*sin(theta - 133*M_PI/1800); 
	
	glVertex2f (xc + x, yc + y); 
	glVertex2f (x1, y1);
	
	double x2 = xc + 11*cos(theta + 133*M_PI/1800); 
	double y2 = yc + 11*sin(theta + 133*M_PI/1800); 
	
	glVertex2f (xc + x, yc + y); 
	glVertex2f (x2, y2);
	
	glEnd (); 
}

void graphic_draw_rectangle(float xc, float yc, float width, float height,
							int filled)
{   
	if (filled == GRAPHIC_FILLED) glBegin (GL_POLYGON);
    else glBegin (GL_LINE_LOOP);

    glVertex2f (xc+width/2, yc+height/2);
    glVertex2f (xc-width/2, yc+height/2);
    glVertex2f (xc-width/2, yc-height/2);
    glVertex2f (xc+width/2, yc-height/2);

    glEnd ();
}

void graphic_draw_losange(float xc, float yc)
{
	glBegin(GL_POLYGON);
	
	glVertex2f (xc+2, yc);
	glVertex2f (xc, yc-2);
	glVertex2f (xc-2, yc);
	glVertex2f (xc, yc+2);
	
	glEnd ();
}

void graphic_draw_circle(float xc, float yc, float r, int filled)
{ 
	int i;
	float alpha;
	if(filled == GRAPHIC_FILLED) glBegin (GL_POLYGON);
	else glBegin (GL_LINE_LOOP);  
	for (i=0; i < PARTICLE_INTERVAL; i++)
	{
		alpha = i * 2. * M_PI / PARTICLE_INTERVAL;
		glVertex2f (xc+r*cos(alpha), yc+r*sin(alpha));
	}
	glEnd ();
}

void graphic_draw_dotted_circle(float xc, float yc, float r)
{
	int i;
	float alpha;
	for (i=0; i < ARC_INTERVAL; i++)
		{
			if(i%2 != 0)
			continue;
			glBegin (GL_LINES);
	
			alpha = i * 2. * M_PI / ARC_INTERVAL;
			glVertex2f (xc+r*cos(alpha), yc+r*sin(alpha));
			
			alpha = (i+1) * 2. * M_PI / ARC_INTERVAL;
			glVertex2f (xc+r*cos(alpha), yc+r*sin(alpha));
			
			glEnd ();
		}
}

void graphic_set_color3f(float r, float g, float b)
{
  glColor3f(r, g, b);
}

void graphic_set_color3fv(float color[3])
{
  glColor3fv(color);
}

void graphic_set_line_width(float width)
{
  glLineWidth(width);
}
