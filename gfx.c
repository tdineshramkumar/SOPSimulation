/*
A simple graphics library for CSE 20211 by Douglas Thain
For complete documentation, see:
http://www.nd.edu/~dthain/courses/cse20211/fall2011/gfx
Version 3, 11/07/2012 - Now much faster at changing colors rapidly.
Version 2, 9/23/2011 - Fixes a bug that could result in jerky animation.
Also Modified By Me....
*/

#include <X11/Xlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "gfx.h"

/*
gfx_open creates several X11 objects, and stores them in globals
for use by the other functions in the library.
*/

static Display *gfx_display=0;
static Window  gfx_window;
static GC      gfx_gc;
static Colormap gfx_colormap;
static int      gfx_fast_color_mode = 0;

/* These values are saved by gfx_wait then retrieved later by gfx_xpos and gfx_ypos. */

static int saved_xpos = 0;
static int saved_ypos = 0;

/* Open a new graphics window. */

void gfx_open( int width, int height, const char *title )
{
	gfx_display = XOpenDisplay(0);
	if(!gfx_display) {
		fprintf(stderr,"gfx_open: unable to open the graphics window.\n");
		exit(1);
	}

	Visual *visual = DefaultVisual(gfx_display,0);
	if(visual && visual->c_class==TrueColor) {
		gfx_fast_color_mode = 1;
	} else {
		gfx_fast_color_mode = 0;
	}

	int blackColor = BlackPixel(gfx_display, DefaultScreen(gfx_display));
	int whiteColor = WhitePixel(gfx_display, DefaultScreen(gfx_display));

	gfx_window = XCreateSimpleWindow(gfx_display, DefaultRootWindow(gfx_display), 0, 0, width, height, 0, blackColor, blackColor);

	XSetWindowAttributes attr;
	attr.backing_store = Always;

	XChangeWindowAttributes(gfx_display,gfx_window,CWBackingStore,&attr);

	XStoreName(gfx_display,gfx_window,title);

	XSelectInput(gfx_display, gfx_window, StructureNotifyMask|KeyPressMask|ButtonPressMask);

	XMapWindow(gfx_display,gfx_window);

	gfx_gc = XCreateGC(gfx_display, gfx_window, 0, 0);

	gfx_colormap = DefaultColormap(gfx_display,0);

	XSetForeground(gfx_display, gfx_gc, whiteColor);

	// Wait for the MapNotify event

	for(;;) {
		XEvent e;
		XNextEvent(gfx_display, &e);
		if (e.type == MapNotify)
			break;
	}
}
/* Gets the width and height of ith display screen */

int gfx_width( int i){
	Screen *screen ;
	int count_screens = ScreenCount(gfx_display);
	if ( i < count_screens ) {	
		screen = ScreenOfDisplay(gfx_display,i);
		return screen->width;
	}
	else return -1;
}
int gfx_height(int i){
	Screen *screen ;
	int count_screens = ScreenCount(gfx_display);
	if ( i < count_screens ) {	
		screen = ScreenOfDisplay(gfx_display,i);
		return screen->height;
	}
	else return -1;
}
/* Draw a single point at (x,y) */

void gfx_point( int x, int y )
{
	XDrawPoint(gfx_display,gfx_window,gfx_gc,x,y);
}

/* Draw a line from (x1,y1) to (x2,y2) */

void gfx_line( int x1, int y1, int x2, int y2,int line_width)
{ /* define the style of lines that will be drawn using this GC. */
  	XSetLineAttributes(gfx_display, gfx_gc,
                     line_width, LineSolid, CapNotLast, JoinRound);
	XDrawLine(gfx_display,gfx_window,gfx_gc,x1,y1,x2,y2);
}


/* Draw a circle radius r with  center at (cx,cy) */
void gfx_circle( int cx, int cy, int radius){
	int x = cx - radius;
	int y = cy - radius;
	XDrawArc(gfx_display,gfx_window,gfx_gc,x,y,2*radius,2*radius,0,360*64);
}

/* Draw a oval radius of width 'width' and height 'height' with upper-left corner of bounding rectangle at (x,y) */
void gfx_oval( int x, int y, int width, int height){
	XDrawArc(gfx_display,gfx_window,gfx_gc,x,y,width,height,0,360*64);
}

/* Draw a rectangle with width 'width' and height 'height' at (x,y) */
void gfx_rectangle( int x, int y, int width, int height){
	XDrawRectangle(gfx_display,gfx_window,gfx_gc,x,y,width,height);
}

/* Draw a circle radius r with  center at (cx,cy) */
void gfx_circle_filled( int cx, int cy, int radius){
	int x = cx - radius;
	int y = cy - radius;
	XFillArc(gfx_display,gfx_window,gfx_gc,x,y,2*radius,2*radius,0,360*64);
}

/* Draw a oval radius of width 'width' and height 'height' with upper-left corner of bounding rectangle at (x,y) */
void gfx_oval_filled( int x, int y, int width, int height){
	XFillArc(gfx_display,gfx_window,gfx_gc,x,y,width,height,0,360*64);
}

/* Draw a rectangle with width 'width' and height 'height' at (x,y) */
void gfx_rectangle_filled( int x, int y, int width, int height){
	XFillRectangle(gfx_display,gfx_window,gfx_gc,x,y,width,height);
}

/* Draw a string of length 'length' at (x,y) */
void gfx_string( int x, int y, char *str, int length){
	XDrawString(gfx_display,gfx_window,gfx_gc,x,y,str,length);
}

/* Change the current drawing color. */

void gfx_color( int r, int g, int b )
{
	XColor color;

	if(gfx_fast_color_mode) {
		/* If this is a truecolor display, we can just pick the color directly. */
		color.pixel = ((b&0xff) | ((g&0xff)<<8) | ((r&0xff)<<16) );
	} else {
		/* Otherwise, we have to allocate it from the colormap of the display. */
		color.pixel = 0;
		color.red = r<<8;
		color.green = g<<8;
		color.blue = b<<8;
		XAllocColor(gfx_display,gfx_colormap,&color);
	}

	XSetForeground(gfx_display, gfx_gc, color.pixel);
}

/* Clear the graphics window to the background color. */

void gfx_clear()
{
	XClearWindow(gfx_display,gfx_window);
}

/* Change the current background color. */

void gfx_clear_color( int r, int g, int b )
{
	XColor color;
	color.pixel = 0;
	color.red = r<<8;
	color.green = g<<8;
	color.blue = b<<8;
	XAllocColor(gfx_display,gfx_colormap,&color);

	XSetWindowAttributes attr;
	attr.background_pixel = color.pixel;
	XChangeWindowAttributes(gfx_display,gfx_window,CWBackPixel,&attr);
}

int gfx_event_waiting()
{
       XEvent event;

       gfx_flush();

       while (1) {
               if(XCheckMaskEvent(gfx_display,-1,&event)) {
                       if(event.type==KeyPress) {
                               XPutBackEvent(gfx_display,&event);
                               return 1;
                       } else if (event.type==ButtonPress) {
                               XPutBackEvent(gfx_display,&event);
                               return 1;
                       } else {
                               return 0;
                       }
               } else {
                       return 0;
               }
       }
}

/* Wait for the user to press a key or mouse button. */

char gfx_wait()
{
	XEvent event;

	gfx_flush();

	while(1) {
		XNextEvent(gfx_display,&event);

		if(event.type==KeyPress) {
			saved_xpos = event.xkey.x;
			saved_ypos = event.xkey.y;
			return XLookupKeysym(&event.xkey,0);
		} else if(event.type==ButtonPress) {
			saved_xpos = event.xkey.x;
			saved_ypos = event.xkey.y;
			return event.xbutton.button;
		}
	}
}

/* Return the X and Y coordinates of the last event. */

int gfx_xpos()
{
	return saved_xpos;
}

int gfx_ypos()
{
	return saved_ypos;
}

/* Flush all previous output to the window. */

void gfx_flush()
{
	XFlush(gfx_display);
}


