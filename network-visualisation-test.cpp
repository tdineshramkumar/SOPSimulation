#include "network-visualisation.h"
#include "network-generator.h"
#include <vector>
#include <iostream>
#include <utility>
#include <iomanip>
using namespace std;

int main(){
	initgraph(500,20,10);
	int numpoints = 10 ;
	//vector<pair<float,float> > points = getrandompoints(numpoints);
	//drawnodes(points);
	//vector<vector<float> > distances = getdistances(points);
	//float avg = averagedistance(distances);
	//float threshold = 0.5 * avg ;
	//vector<vector<int> > edges = getedgematrix(distances ,threshold);
	float threshold = 0.8 ;
	pair<vector<pair<float,float> > ,vector<vector<int> > >  graph = randomgraph( numpoints,threshold,20);
	vector<pair<float,float> >  points = graph.first ;
	vector<vector<int> > edges = graph.second ;
	//printpoints(points);
	//printdistances( vector<vector<float> > distances);
	//printgraph(edges);
	drawnodes(points,false);
	drawedges(edges);
	drawedge(0,9,EXPLORATION_EDGE);
	waitgraph();// press q to quit and r to redraw
	return 0;
}
