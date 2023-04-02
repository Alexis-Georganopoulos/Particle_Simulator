/* File name: graphic.h
 * 
 * Description: 
 * 	Transfers graphic.c functions to other modules
 * 
 * Date: 16.04.2019
 * Version: 4.20
 * 
 * Authors - SCIPER:
 * 	Alexis Philip George-Georganopoulos - 228168
 */

#ifndef GRAPHIC_H
#define GRAPHIC_H

#define GRAPHIC_EMPTY  0
#define GRAPHIC_FILLED 1
#define ARC_INTERVAL 120
#define PARTICLE_INTERVAL 24

// draws segment
void graphic_draw_segment(float x1,float y1, float x2, float y2); 

// draws arrow                     
void graphic_draw_arrow (float x1, float y1, float vx, float vy);

// draws rectangle
void graphic_draw_rectangle(float xc, float yc, float width, float height,
							int filled);
							
// draws losange (diamond)
void graphic_draw_losange(float xc, float yc);

// draws circle
void graphic_draw_circle(float xc, float yc, float r, int filled);

// you get the point..
void graphic_draw_dotted_circle (float xc, float yc, float r);

// sets a color given r,g,b values
void graphic_set_color3f(float r, float g, float b);

// sets a color given an array of size 3 (usually called the name of a color)
void graphic_set_color3fv(float color[3]);

// sets a line width
void graphic_set_line_width(float width);

#endif
