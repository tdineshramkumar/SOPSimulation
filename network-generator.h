#ifndef _NETWORK_GENERATOR
#define _NETWORK_GENERATOR
#include <vector>
#include <utility>
using namespace std ;

const float MAXVALUE = 10.0f ;
const float MINVALUE = 0.0f ;
// this function returns random points from 2D space with values between 'MINVALUE' and 'MAXVALUE'
// in each dimension . Points are chosen from uniform distribution ...
vector<pair<float,float> > getrandompoints(int numpoints);
// this function returns the distance between two points in space
float getdistance(pair<float,float> point1,pair<float,float> point2);
// this function returns the distance matrix between all points in space
vector<vector<float> > getdistances(vector<pair<float,float> > points);
// this function finds the average distance between all points in spaces
float averagedistance(vector<vector<float> > distances);
// this function returns the edge matrix (connectivity matrix) based on 'threshold' distance
// edge is established when distance between pair of nodes is less than 'threshold'
vector<vector<int> > getedgematrix(vector<vector<float> > distances , float threshold_distance);
// this function returns a random graph 
// threshold lies between 0 and 1 
// it tries to generate graph with given threshold 10 times
// if it fails it terminates
pair<vector<pair<float,float> > ,vector<vector<int> > >  randomgraph(int numpoints,float threshold =1 ,int repeat  = 10 );


// DISPLAY FUNCTIONS
void printpoints(vector<pair<float,float> > points);
void printdistances( vector<vector<float> > distances);
void printgraph(vector< vector<int> > graph);
#endif