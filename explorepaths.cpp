#include <iostream>
#include <queue>
#include <vector>
#include <utility>
#include <exception>
#include <iomanip>

#include "explorepaths.h"

using namespace std;


// Algorithm from GeeksForGeeks
// this function prints the found paths in the graph
void printpaths(vector<vector<int> > &paths){
	int numpaths = paths.size();
	for (int i = 0 ; i < numpaths; i++){
		vector<int> path = paths[i] ;
		cout << "Path " << std::setw(5) << i << ": " ;
		for (int j = 0; j < path.size() ; j++ ){
			cout << setw(3) << path[j] << " " ;
		}
		cout << endl ;
	}
}

// this function checks if current vertex is 
// already present in the path
bool isNotVisited( int x,vector<int> &path){
	int size = path.size();
	for (int i = 0 ; i < size ; i++)
		if (path[i] == x)
			return false;
	return true;
}
// Breadth First Search ...
// this function finds all paths in the graph from source
// to desination
// adjlist pair -> edge
vector<vector<int> > findpaths( 
		vector<vector<pair<int,int> > > adjlist ,
		int src, int dst ){
	// create a queue which store tmp
	// the paths
	queue<vector<int> > q;
	// this stores the final set of paths
	vector<vector<int> > paths ;
	//path vector to store the current path
	vector<int> path;
	path.push_back(src);
	q.push(path);
	while (!q.empty()) {
		path = q.front();
		q.pop();
		// get the last element of path
		int last = path[path.size() -1] ;
		// if the last vertex is the desired destination
		// then store it in the paths vector
		if (last == dst)
			paths.push_back(path);

		// traverse to all the nodes connected to current vertex 
		// and push new path to queue
		for (int i=0 ; i < adjlist[last].size(); i++){
			if (isNotVisited(adjlist[last][i].second, path)){
				vector<int> newpath(path);
				newpath.push_back(adjlist[last][i].second);
				q.push(newpath);
			}
		}
	}
	return paths;
}

