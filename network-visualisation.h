#ifndef _NETWORK_VISUALIZATION
#define _NETWORK_VISUALIZATION

#include <vector>
#include <utility>
using namespace std;

#define NO_EDGE 0
#define DEFAULT_EDGE 1
#define PATH_EDGE 2
#define EXPLORATION_EDGE 3
#define DECISION_EDGE 4


// this function intializes a square window of given size 's' 
// and sets the radius of nodes to 'r'
// max value contains the maximum value of dimensions going to specify coordinates of points
void initgraph(int s=500, int r=10, int max = 10);

// this function displays the nodes given by their location 'points' in space
// this function assumes source and destination points
// as the first and the last points in the list of points 
void drawnodes(vector<pair<float,float> > &points,bool shownodenumbers=true);

// this functions shows the draws between points ...
void drawedges(vector<vector<int> > &edges);

// this function draws an edge between points and of given type
void drawedge(int point1, int point2, int type=NO_EDGE);

// this function also draws edges .. but also considers type information while drawing
//void drawedgeswithtype(vector<pair<pair<int,int>,int> >  &edge_type);

// this function must be called only after drawedges
// to restore the edges set using drawedges ..
// not to be called before initializing edges
void restoreedges();

// this function clears all the edges
void clearedges();

// this function makes that graph to be displayed till 'q' is pressed ..
void waitgraph();
// this function waits and returns few of the keys if entered 
char waitandreturn();
// this functions to set performance values and key count to display
void setCurrentKey(int _key);
void setPerformance(float _performance);
#endif