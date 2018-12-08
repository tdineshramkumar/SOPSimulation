/*
A simple graphics library for CSE 20211 by Douglas Thain
For course assignments, you should not change this file.
For complete documentation, see:
http://www.nd.edu/~dthain/courses/cse20211/fall2011/gfx
Version 3, 11/07/2012 - Now much faster at changing colors rapidly.
Version 2, 9/23/2011 - Fixes a bug that could result in jerky animation.
*/


#ifndef GFX_H
#define GFX_H

/* Open a new graphics window. */
void gfx_open( int width, int height, const char *title );

/* Gets the width and height of ith display screen if it exists*/
int gfx_width( int i = 0) ;
int gfx_height(int i = 0 ) ;
/* Draw a point at (x,y) */
void gfx_point( int x, int y );

/* Draw a line from (x1,y1) to (x2,y2) */
void gfx_line( int x1, int y1, int x2, int y2 ,int line_width = 1);

/* Draw a circle radius r with center at (cx,cy) */
void gfx_circle( int cx, int cy, int radius);

/* Draw a oval radius of width 'width' and height 'height' with upper-left corner of bounding rectangle at (x,y) */
void gfx_oval( int x, int y, int width, int height);

/* Draw a rectangle with width 'width' and height 'height' at (x,y) */
void gfx_rectangle( int x, int y, int width, int height);

/* Draw a circle radius r with center at (cx,cy) */
void gfx_circle_filled( int cx, int cy, int radius);

/* Draw a oval radius of width 'width' and height 'height' with upper-left corner of bounding rectangle at (x,y) */
void gfx_oval_filled( int x, int y, int width, int height);

/* Draw a rectangle with width 'width' and height 'height' at (x,y) */
void gfx_rectangle_filled( int x, int y, int width, int height);

/* Draw a string of length 'length' at (x,y) */
void gfx_string( int x, int y, char *str, int length);

/* Change the current drawing color. */
void gfx_color( int red, int green, int blue );

/* Clear the graphics window to the background color. */
void gfx_clear();

/* Change the current background color. */
void gfx_clear_color( int red, int green, int blue );

/* Wait for the user to press a key or mouse button. */
char gfx_wait();

/* Return the X and Y coordinates of the last event. */
int gfx_xpos();
int gfx_ypos();

/* Return the X and Y dimensions of the window. */
int gfx_xsize();
int gfx_ysize();

/* Check to see if an event is waiting. */
int gfx_event_waiting();

/* Flush all previous output to the window. */
void gfx_flush();

#endif

