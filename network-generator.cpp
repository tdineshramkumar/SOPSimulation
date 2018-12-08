#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>
#include <vector>
#include <utility>
#include <cmath>
#include <exception>
#include "network-generator.h"
//#define _DEBUG
using namespace std;

// this function checks if given graph is connected
bool isconnected(vector<vector<int> > graph ){
	int vertices = graph.size(); // get the number of vertices
	bool connected[vertices] ;  // this will contain the connected component of node 0
	for (int i=0 ; i < vertices ; i++) 
		connected[i] = false ;
	connected[0] = true ;// check connectivity of all nodes from node 0
	int i ; 
	for (int j=0;j < vertices ; j ++){
		if (connected[j] == true) {
			i = j ;
			for (int k = 0 ; k < vertices ; k++) {
				if ( connected[k] == false 
						&& k != j
						&& graph[j][k] > 0
						&& graph[k][j] > 0 ){
					connected[k] = true ;
					if ( k < i ) i = k ;
				}
			}
			j = i ;
		}
	}		
	// check if graph is connected
	for (int i = 0 ; i < vertices ; i++)
		if (connected[i] == false)
			return false;
	return true;
}

// this function returns random points from 2D space with values between 'MINVALUE' and 'MAXVALUE'
// in each dimension . Points are chosen from uniform distribution ...
vector<pair<float,float> > getrandompoints(int numpoints){

	vector<pair<float,float> > points ;
 	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator (seed); // use time as seed to initialize random number generator
    std::uniform_real_distribution<float> distribution(MINVALUE,MAXVALUE); // pick from uniform distribution for points
	for (int i = 0 ;i < numpoints ; i++){
		float x = distribution(generator);
		float y = distribution(generator);
		points.push_back(make_pair(x,y)); 
	}
	return points;
}


// this function displays the points in the plane in aesthetically pleasing way
void printpoints(vector<pair<float,float> > points){
	vector<pair<float,float> >::iterator itr = points.begin();
	int index = 0 ;
	cout << "Points: " << endl ;
	cout << std::fixed << std::setprecision(3);
	while( itr!= points.end()){
		cout << "Point " << setw(3) << index ; 
		cout << std::setw(6) << itr->first ;
		cout << std::setw(6) << itr->second ;
		cout << endl ;
		itr++;
		index++;
	}
}

// this function returns the distance between two points in space
float getdistance(pair<float,float> point1,pair<float,float> point2) {
	float distance = sqrt( pow(point1.first - point2.first,2) + pow(point1.second - point2.second,2) ) ; 
	return distance ;
}

// this function returns the distance matrix between all points in space
vector<vector<float> > getdistances(vector<pair<float,float> > points){
	int numpoints = points.size(); 
	vector<vector<float> > distance ;
	for (int i = 0 ; i < numpoints ; i ++){
		vector<float> tmp ;
		for (int j=0; j < numpoints ; j ++){
			tmp.push_back(getdistance(points[i],points[j])) ;
		}
		distance.push_back(tmp) ;
	}
	return distance ;
}

// this function prints the distance matrix 
void printdistances( vector<vector<float> > distances ){
	int numpoints = distances.size();
	cout << std::fixed << std::setprecision(4) ; // set the precision here
	cout << setw(9) << "Points ";	
	for (int i = 0 ; i < numpoints ; i ++){
		cout << setw(7) << i ;	
	}
	cout << endl ;
	cout << setw(9) << "";	
	for (int i = 0 ; i < numpoints ; i ++){
		cout << setw(7) << "------"  ;	
	}
	cout << endl ;
	for (int i= 0 ; i < numpoints ; i ++){
		cout << "Point" << setw(3) << i << ":" ;
		for (int j = 0 ; j < numpoints ; j ++ ){
			cout << setw(7) << distances[i][j]  ;
		}
		cout << endl ;
	}

}

// this function finds the average distance between all points in spaces
float averagedistance(vector<vector<float> > distances){
	float avgdist = 0 ;
	int numpoints = distances.size();
	for (int i = 0 ; i < numpoints ; i ++){
		float tmp = 0 ;
		for (int j = 0 ; j < numpoints ; j ++){
			tmp = tmp + distances[i][j] ;
		}
		avgdist = avgdist + tmp/numpoints ;
	}
	return avgdist/numpoints;
}

// this function returns the edge matrix (connectivity matrix) based on 'threshold_distance' distance
// edge is established when distance between pair of nodes is less than 'threshold_distance'
vector<vector<int> > getedgematrix(vector<vector<float> > distances , float threshold_distance){
	vector<vector<int> >  edge ;
	int numpoints = distances.size();
	edge.resize(numpoints);
	for (int i = 0 ; i < numpoints ; i++)
		edge[i].resize(numpoints) ;
	for (int i = 0 ; i < numpoints ; i++){
		for (int j = 0 ; j < numpoints ; j ++){
			if ( distances[i][j] > threshold_distance) // when distance between nodes are greater that threshold 
				edge[i][j] = 0 ; // no edge established
			else 
				edge[i][j] = 1; // else edge established
		}
	}
	return edge ;
}
// this function prints the given graph into console
void printgraph(vector< vector<int> > graph){
	// display the graph
	int size = graph.size();
	vector <vector <int> >:: iterator itr1 ;
	vector <int> :: iterator itr2 ;
	cout << std::setw(8) << "NODES" << std::setw(2) << "";
	for (int i = 0 ; i < size ; i ++){
		cout << std::setw(4) << i ;		
	}	
	cout << endl ;
	cout << std::setw(10) << " " ;
	for (int i = 0 ; i < size ; i ++){
		cout << std::setw(4) << "----" ;		
	}
	cout << endl ;	
	int i = 0 ;

	for ( itr1 = graph.begin() ; itr1 != graph.end() ; itr1++ ,i++){
		cout << std::setw(5) << "NODE" << std::setw(3) << i << std::setw(2) << ":";
		for (itr2 = itr1->begin() ; itr2 != itr1->end() ; itr2++) {
			cout << std::setw(4) << *itr2  ;
		}
		cout << endl ;
	}
}

// this function returns a random graph 
// threshold lies between 0 and 1 
// it tries to generate graph with given threshold 10 times
// if it fails it terminates
// defualt repeat = 10
pair<vector<pair<float,float> > ,vector<vector<int> > > randomgraph(int numpoints,float threshold,int repeat){
	
	for (int i = 0 ; i < repeat ; i ++){	
		
		vector<pair<float,float> > points = getrandompoints(numpoints);	
		vector<vector<float> > distances = getdistances(points);
		float avgdst = averagedistance(distances);
		vector<vector<int> > graph = getedgematrix(distances ,avgdst*threshold) ;
		if (isconnected(graph)) 
			return make_pair(points,graph);
	}
	
	cerr << "Unable to generate the graph with " << numpoints << " at " << threshold << " threshold even after " << repeat << "attempts " << endl;
	terminate();
}
/*

vector<vector<int> > randomgraph(int numpoints,float threshold,int repeat = 10){
	for (int i = 0 ; i < repeat ; i ++){
		
		#ifdef _DEBUG
		cout << "ITERATION :" << i << endl;
		cout << "NUPOINTS:" << numpoints << endl ;
		#endif		
		vector<pair<float,float> > points = getrandompoints(numpoints);	
		#ifdef _DEBUG
		printpoints(points);
		#endif
		vector<vector<float> > distances = getdistances(points);
		#ifdef _DEBUG
		printdistances(distances) ;
		#endif
		float avgdst = averagedistance(distances);
		#ifdef _DEBUG
		cout << "avg distance " <<  avgdst << endl ; 
		#endif
		vector<vector<int> > graph = getedgematrix(distances ,avgdst*threshold) ;
		#ifdef _DEBUG
		printgraph(graph);
		#endif
		if (isconnected(graph)) return graph ;
		#ifdef _DEBUG
		else cout << "No Connected" << endl ;
		#endif
	}
	cerr << "Unable to generate the graph with " << numpoints << " at " << threshold << " threshold even after " << repeat << "attempts " << endl;
	terminate();
}
int main(){
	//	vector<vector<int> > graph = randomrealgraph(10,1);
	//cout << "Final Graph" << endl;
	//printgraph(graph);
	int numpoints = 10;
	vector<pair<float,float> > points = getrandompoints(numpoints);
	printpoints(points);
	initgraph();
	shownodes(points);
	printpoints(points);
	vector<vector<float> > distances = getdistances(points);
	printdistances(distances) ;
	float avgdist =	averagedistance(distances) ;
	cout << "avg distance " <<  avgdist << endl ; 
	vector<vector<int> > graph = getedgematrix(distances,avgdist);
	printgraph(graph);
	showedges(graph);
	waitgraph();
	return 0 ;
}


int main(){
	int numpoints = 10;
	vector<pair<float,float> > points = getrandompoints(numpoints);
	printpoints(points);
	vector<vector<float> > distances = getdistances(points);
	printdistances(distances) ;
	float avgdist =	averagedistance(distances) ;
	cout << "avg distance " <<  avgdist << endl ; 
	vector<vector<int> > graph = getedgematrix(distances,avgdist);
	printgraph(graph);
	vector<vector<int> > graph1 = randomrealgraph(10,1);
	return 0;
}
*/
