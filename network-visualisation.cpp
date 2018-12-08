
/*
	this is a graphics library to help visualize graphs
*/

#include <iostream>
#include <iomanip>
#include <vector>
#include <utility>
#include <exception>
#include <string>
#include <sstream> // stringstream
#include "gfx.h"
#include "network-visualisation.h"
#include "unistd.h"
using namespace std;

int xsize ; // this contains the width of display
int ysize ; // this contains the height of display
int width ; // this contains the actual width(or height) which is used for drawing
int xoffset ;  // this contains the x offset so that image drawn in required region
int yoffset ;  // this contains the y offset so that image drawn in required region
int scale ; // this contains the scale for mapping from points space (node space) to graphic space
int radius ; // this contains the radius of nodes used for drawing
int numnodes ; // this contains the number of nodes in the network
bool prevshownodenumbers = true ;
vector<pair<float,float> > networkpoints; // this vector stores location of points in space
vector<vector<int> > connectivity ; // this stores the edges between nodes


// this function converts points for given coordinates into points in the graph
int getX( float x ) {
	return (int) (x * scale + xoffset) ;
}

int getY( float  y ){
	return (int) (y * scale + yoffset) ;
}

// this was added late ...
float performance = 0 ;
int curkey = 0 ;
bool issetPerformance = false ;
bool issetCurKey = false;
void setCurrentKey(int _key){
	issetCurKey = true;
	curkey = _key ;
}
void setPerformance(float _performance){
	issetPerformance = true ;
	performance = _performance ;
}
void showCurKey(){
		if ( issetCurKey){
			int x =(int) getX(0) ;
			int y =(int) getY(0) ;
			string str = "KEY: "+std::to_string(curkey) ;
			gfx_string(x,y,(char *)str.c_str(),str.size()) ;
		}
}
void showPerformance(){
		if ( issetPerformance){
			int x =(int) getX(0) ;
			int y =(int) getY(10) ;
			stringstream stream;
			stream << fixed << setprecision(4) << performance;
			string str = "PERFORMANCE: "+stream.str();
			gfx_string(x,y,(char *)str.c_str(),str.size()) ;
		}
}
// s contains the size of required display
// r contains the radius required in that display
// max contains the maximum value (rounded to nearest int) of coordinates of points (both x and y)
// this function opens a window and ensures that graph is displayed in square symmetric space ..
void initgraph(int s, int r, int max){
	// Note : 1/10 of display area used for offset
	xoffset = (int) 0.1 * s ; 
	yoffset = (int) 0.1 * s ;
	// get the size after applying offset on either side
	xsize = s + xoffset * 2;
	ysize = s + yoffset * 2;
	// store the radius of the node
	radius = r ;
	// obtain the scaling parameter
	scale = s/max ;
	// open new window of specified size
	gfx_open(xsize,ysize,"Node Network");
	//cout << "size of window height:" << gfx_height() << " width:" << gfx_width() << endl;
	// get actual window sizes ... 
	xsize = gfx_width() ; 
	ysize = gfx_height() ;
	// to display points in a graph in a square region ..
	// determing which axis is longer .. accordingly manipulate offsets and scale ...
	if ( xsize > ysize ){
		yoffset = ysize / 12 ;
		width = ysize * 10 / 12 ; // this is the new width of the graph drawing area
		xoffset = (xsize - width)/ 2 ;
	}
	else {
		// if ysize > xsize
		xoffset =  xsize / 12 ;
		width = xsize * 10 /12  ;
		yoffset = ( ysize - width ) / 2 ;
	}
	scale = width / max ; // set the new scale
	radius = r * width / s ; // also scale the radius accordingly
	gfx_clear_color(0,0,0); // set background color here
	gfx_clear(); // clear the background 
}


// this function displays the nodes given by their location 'points' in space
// this function assumes source and destination points
// as the first and the last points in the list of points 
bool drawslowly = false ; 	// this is used to slowly view points as their being placed ..
							// used for the purpose of debugging
void drawnodes(vector<pair<float,float> > &points,bool shownodenumbers){
	gfx_clear();
	prevshownodenumbers = shownodenumbers ;
	numnodes = points.size();
	networkpoints = points ;
	int src = 0 , dst = numnodes-1; 
	string nodenum; 
	gfx_color(250,0,0); // set the color of nodes
	for (int i = 0 ; i < numnodes ; i ++){

		int x =(int) getX(points[i].first) ;
		int y =(int) getY(points[i].second) ;
		if (i == src )
			gfx_color(0,255,0); // source is in green color
		else if (i == dst)
			gfx_color(255,0,0); // destination is red in color
		else 
			gfx_color(0,0,255); // rest are blue in color
		gfx_circle_filled(x,y,radius) ;
		if (shownodenumbers){
			gfx_color(255,255,255);
			nodenum = std::to_string(i);
			gfx_string(x,y,(char *)nodenum.c_str(),nodenum.size()) ;
		}
		// just for debuggin purpose
		if ( drawslowly) {
			 gfx_flush();sleep(1) ;
		}
	}
	// to make sure that the source and destination are seen .. sometimes get overlapped
	for (int i = 0 ; i < numnodes ; i ++){
		if ( i == src) {
			int x =(int) getX(points[i].first) ;
			int y =(int) getY(points[i].second) ;
			gfx_color(0,255,0); // source is in green color
			gfx_circle_filled(x,y,radius) ;
			if (shownodenumbers){
				gfx_color(255,255,255);
				nodenum = std::to_string(i);
				gfx_string(x,y,(char *)nodenum.c_str(),nodenum.size()) ;
			}
		}
		if ( i == dst) {
			int x =(int) getX(points[i].first) ;
			int y =(int) getY(points[i].second) ;
			gfx_color(255,0,0); // source is in red color
			gfx_circle_filled(x,y,radius) ;
			if (shownodenumbers){
				gfx_color(255,255,255);
				nodenum = std::to_string(i);
				gfx_string(x,y,(char *)nodenum.c_str(),nodenum.size()) ;
			}
		}
	}
	showPerformance();
	showCurKey();
	gfx_flush(); // update the display
}

// this functions shows the draws between points ...
void drawedges(vector<vector<int> > &edges){
	drawnodes(networkpoints,prevshownodenumbers);
	numnodes = edges.size();
	connectivity = edges;
	gfx_color(255,255,0);
	for ( int i = 0 ; i < numnodes ; i ++) {
		for (int j = 0 ; j < i ; j ++) {
			drawedge(i, j, edges[i][j]);
			/*
			if (edges[i][j] == 1 ){
				gfx_line(
						getX(networkpoints[i].first),
						getY(networkpoints[i].second),
						getX(networkpoints[j].first),
						getY(networkpoints[j].second) 
				);
			}*/
		}
	}
	gfx_flush();
}


// this function draws an edge between points and of given type
//void drawedge(int point1, int point2, int type=DEFAULT_EDGE) -- > default value
void drawedge(int point1, int point2, int type){
	switch(type){
		case DEFAULT_EDGE: // defualt edge
			gfx_color(255,255,0);
			break;
		case PATH_EDGE: // path edge
			gfx_color(0,255,125);
			break;
		case EXPLORATION_EDGE: // exploration edge
			gfx_color(125,0,255);
			break;
		case DECISION_EDGE: // decide later
			gfx_color(255,255,255);
			break;
		default:
			gfx_color(125,125,125);
	}
	if (type != NO_EDGE){
		if ( type == PATH_EDGE)
			gfx_line(
				getX(networkpoints[point1].first),
				getY(networkpoints[point1].second),
				getX(networkpoints[point2].first),
				getY(networkpoints[point2].second),
				4
		);
		else if ( type == EXPLORATION_EDGE ||type == DECISION_EDGE)
			gfx_line(
				getX(networkpoints[point1].first),
				getY(networkpoints[point1].second),
				getX(networkpoints[point2].first),
				getY(networkpoints[point2].second),
				2
		);
		else
		gfx_line(
				getX(networkpoints[point1].first),
				getY(networkpoints[point1].second),
				getX(networkpoints[point2].first),
				getY(networkpoints[point2].second) 
		);
		gfx_flush();
	}
}
/*
// this function also draws edges .. but also considers type information while drawing
void drawedgeswithtype(vector<pair<pair<int,int>,int> >  &edge_type){
	drawnodes(networkpoints,prevshownodenumbers);
	int numedges = edge_type.size();
	for (int i = 0 ; i < numedges ; i ++) {
		pair<pair<int,int>,int> edge = edge_type[i];
		pair<int,int> point = edge.first;
		drawedge(point.first,point.second,edge.second); // x,y,type
	}
}
*/
// this function must be called only after drawedge to restore the edges 
// not to be called before initializing edges
void restoreedges(){
	drawedges(connectivity);
}

// this function clears all the edges
void clearedges(){
	drawnodes(networkpoints,prevshownodenumbers) ;
}

// this function makes that graph to be displayed till 'q' is pressed ..
void waitgraph(){
	char c ;
	while(1) {
		// Wait for the user to press a character.
		c = gfx_wait();
		// Quit if it is the letter q.
		if(c=='q') break;
		if(c=='n') break; // next same as quit
		else if (c == 'r') restoreedges();
		else if ( c== 'c') clearedges() ;
		else if ( c == 's') drawslowly = !drawslowly ;
	}
}
char waitandreturn(){
	char c ;
	while(1){
		c = gfx_wait();
		// if n or i or f return it
		// n : next i : iterate f : finish
		if ( c == 'n' ||  c == 'i' || c == 'f') 
			return c;
		else if ( c == 'q') // if user pressed 'q' terminate
		{	
			cerr << "USER WANTED TO TERMINATE" << endl ;
			terminate();
		}
	}
}

