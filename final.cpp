#include "network-visualisation.h"
#include "network-generator.h"
#include "adjacency-list.h"
#include "explorepaths.h"
#include "collaboration-curiosity.h"
#include "performance-measures.h"
#include "path-performance-comparisons.h"
#include <vector>
#include <iostream>
#include <utility>
#include <iomanip>
#include <cmath>
using namespace std;

int main(){
	// initialize graph with required size, radius of nodes and maximum coordinate values of points
	initgraph(500,20,10);
	// set the number of points in the graph
	int numpoints = 15 ;
	// set the number of times to try to get a random graph before failing
	int repeat = 20 ;
	float threshold = 0.7 ; // set threshold on connectivity to neighbourhood 
	// generate a random graph ...
	pair<vector<pair<float,float> > ,vector<vector<int> > >  graph = randomgraph( numpoints,threshold,repeat);
	vector<pair<float,float> >  points = graph.first ; // get cooridinates of points
	vector<vector<int> > edges = graph.second ; // get the edge matrix of points
	printpoints(points);
	printgraph(edges);
	// set 'true' if you want to display node numbers	
	drawnodes(points,true);
	drawedges(edges); // draw edge 
	vector<vector<pair<int,int> > > adjlist = getadjlist(edges); // get the adjacency lists
	int src = 0, dst = numpoints - 1 ; // set the source and destination points
	vector<vector<int> > paths = findpaths( adjlist , src, dst ) ; 
	printpaths(paths);

	// set collaboration parameters
	float meanCollaboration = 0.1 ; float stdCollaboration = 0.2 ;
	vector<vector<float> > collab = randomcollab(numpoints,meanCollaboration,stdCollaboration);
	// set curiosity parameters
	float meanCuriosity = 0.1 ; float stdCuriosity = 0.2 ;
	vector<float> curiosity = randomcuriosity(numpoints,meanCuriosity,stdCuriosity);
	// print the collaboration and curiosity levels ...
	printcollab ( collab );
	printcuriosity ( curiosity );


	// set limit to number of paths ...
	//int maxnumpaths = (int) numpoints * log( numpoints );
    //if ( paths.size() > maxnumpaths ) 
    //	paths.erase(paths.begin()+maxnumpaths, paths.end()) ;
	// set collaboration parameters
	//paths = getoptimalpaths(paths,collab,curiosity, maxnumpaths);
	
	
	// set number of keys
	int numkeys = 5 ;
	vector<int> alloc ; // = getdefaultalloc(1);
	//printalloc(alloc);
	//printnumkeys(alloc,paths,numpoints);
	// this contains edges with type information as well 
	vector<pair<pair<int,int>,int> >  edge_type ; 
	vector<float> performance ;
	performance.resize(numkeys); 
	// for each key distribute it ....
	for (int allocindex = 0 ; allocindex < numkeys ; allocindex ++) {
		alloc.push_back(-1);
		int nop = nextoptimalpath(alloc,paths,collab,curiosity,src,dst,allocindex);
		alloc[allocindex] = nop;
		vector<vector<int> > keys = keydistribution(alloc,paths,numpoints) ;
		performance[allocindex] = nonodebreakskeys(keys,collab,curiosity,src,dst);
	}
	//drawedgeswithtype(vector<pair<pair<int,int>,int> >  &edge_type);
	printalloc(alloc);
	for (int i = 0 ; i < numkeys ; i ++) 
		cout << "Performance with " << i << " Keys: " << performance[i] << endl ;

	waitgraph();// press q to quit and r to redraw
	return 0;
}