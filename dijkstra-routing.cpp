#include <vector>
#include <utility>
#include <iostream>
#include <iomanip>
#include <exception>
#include <queue>          // std::priority_queue
#include "performance-measures.h"
#include "collaboration-curiosity.h"
#include "network-generator.h"
#include "network-visualisation.h"
#include "adjacency-list.h"
#include "explorepaths.h"

using namespace std;

bool containsdestination(vector<int> adjacentlist, int dst){
	bool contains = false ;
	for ( int i = 0 ; i < adjacentlist.size() ; i ++)
		if ( adjacentlist[i] == dst )
			contains = true , cout << "Found Destination" << endl ;
	return contains ;
}

void printapath(vector<int> path){
	cout << "Path : " ;
	for (int j = 0; j < path.size() ; j++ ){
		cout << setw(3) << path[j] << " " ;
	}
	cout << endl ;
}
// we create a path class
// this is used with 'ComparePaths' comparator class and priority_queue for comparing performances of paths
class Path{
		vector<int> path; 
		vector<vector<int> > keys;
		vector<vector<float> > collab ;
		vector<float> curiosity ;
		int src ;
		int dst ;
	public:
		Path(vector<int> _path, vector<vector<int> > _keys_previous, 
			vector<vector<float> > _collab,vector<float> _curiosity,int _src,int _dst )
			// _keys_previous contains key allocation before _path was chosen for giving the next key
		{
			path = _path ;
			keys = _keys_previous ;
			int numnodes = keys.size();
			for (int i = 0 ; i < numnodes ; i ++){
				keys[i].push_back(0) ;// since a new key is given increase the number of keys and 
									  // keys not allocated to any one	
			}
			// allocate the new key to those in the path
			int pathlength = path.size() ;
			int numkeys = keys[0].size();
			for (int i = 0 ; i < pathlength ; i ++){
				keys[ path[i] ][numkeys-1] = 1; // allocate keys to those in the path
			}
			collab = _collab ;
			curiosity = _curiosity ;
			dst = _dst ;
			src = _src ;
		}
		float getPerformance() const{
			return nonodebreakskeys(keys,collab,curiosity,src,dst);
		}
		int getPathLength() const{
			return path.size();
		}
		vector<int> getPath() const{
			return path;
		}
		bool containsdestination() const{
			bool contains = false ;
			for ( int i = 0 ; i < path.size() ; i ++)
				if ( path[i] == dst )
					contains = true  ;//, cout << "Found Destination" << endl ;
			return contains ;
		}
		vector<vector<int> > getKeys() const{
			return keys;
		}

};
class ComparePaths{
		public:
			// if returns true then goes to bottom -> poped last
			// i want max performance metrics to be poped out first
			// so return true if performance of first is lesser than other
			// so that higher performance path is poped out first
			// if same performance between paths ..
			// return true if path length of first is greater than next second one
			// so that paths with smaller ones returned 
			bool operator() ( const Path &path1, const Path &path2){
				// now to compare the performance of path 
				if ( path1.getPerformance() > path2.getPerformance() )
					return false ; // path1 on top
				else if ( path1.getPerformance() < path2.getPerformance() )
					return true ; // path2 on top
				else
					if ( path1.containsdestination() )
						return false; // path1 on top
					else if ( path2.containsdestination() )
						return true; // path2 on top
					else if ( path1.getPathLength() <= path2.getPathLength() )
						return false ; // path1 on top
					else
						return true ; // path2 on top

			}
};
 /*
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
 */
 // this function checks if destination is reachable if traverse along a path or not 
bool isdestinationreachable( vector<vector<pair<int,int> > > adjlist , vector<int> path, int dst) {
	// do a BFS and check if destination reachable or not ...
	queue<vector<int> > q;
	vector<int> pathtmp; // this is the temporary path
	int pathlength = path.size(); // get size of current path
	if ( pathlength == 0 ) // if path length is zero assuming connected graph return true
		return true;

	int lastnode = path[pathlength-1] ; // this returns the last node in the path ... hoping that it leads towards the destination
	
	pathtmp.push_back(lastnode); // push the last node check all paths from here
	q.push(pathtmp); // now look at all paths from the last node and check if destination reachable
	
	while ( !q.empty()) {
		pathtmp = q.front(); // get the first element from the queue
		q.pop();
		// get the last element of path
		int last = pathtmp[pathtmp.size() -1] ;
		// if the last vertex is the desired destination
		// then store it in the paths vector
		if (last == dst)
			return true;

		// traverse to all the nodes connected to current vertex 
		// and push new path to queue if it is not path or original path or current tmp path
		for (int i=0 ; i < adjlist[last].size(); i++){
			if ( isNotVisited(adjlist[last][i].second, pathtmp)  && isNotVisited(adjlist[last][i].second, path) ){
				vector<int> newpath(pathtmp);
				newpath.push_back(adjlist[last][i].second);
				q.push(newpath);
			}
		}
	}
	return false;
}

pair<vector<int>,vector<vector<int> > > nextoptimalpath1(	vector<vector<pair<int,int> > > adjlist, // this function needs the adjacent paths as well
	vector<vector<int> > keys_previous , // this contains the previous key distribution 
	vector<vector<float> > collab, vector<float> curiosity,int src,int dst){
	// create a priority_queue which stores the paths from source sorted based on path performance
	// if destination is a member of the path then your done
    priority_queue <Path, vector<Path>, ComparePaths> pq;
    vector<int> path ;
    vector<vector<int> > keys ;
    path.push_back(src); // make a path with source at the start
    pq.push(Path(path, keys_previous, collab, curiosity, src, dst ));
    while(!pq.empty()) {
    	path = pq.top().getPath(); // get the path with best performance
    	keys = pq.top().getKeys(); // get the key distribution corresponding to the best path
    	
    	if ( containsdestination (path,dst) ) {
    		cout << "BEST PATH PERFORMANCE" << pq.top().getPerformance() << endl << endl << endl;
    		cout << "CURRENT PATH:" ;
    		printapath(path);
    		cout << "LIST OF ALL OTHER PATHS" << endl ;
    		// we can check the contents of the pq
    		priority_queue <Path, vector<Path>, ComparePaths> pqtmp (pq);
    		while(!pqtmp.empty()){
    			vector<int> pathtmp = pqtmp.top().getPath();
    			vector<vector<int> > keystmp = pqtmp.top().getKeys();
				printapath(pathtmp);
				//printkeydistribution(keystmp);
				cout << "Path performance: " << pqtmp.top().getPerformance() << endl;
				pqtmp.pop();

    		}


    		return make_pair(path,keys);
    	}

    	pq.pop(); // pop it out
    	
    	int last = path[path.size()-1]; // get the last node
    	// traverse to all the nodes connected to current vertex 
		// and push new path to queue
		for (int i=0 ; i < adjlist[last].size(); i++){
			if (isNotVisited(adjlist[last][i].second, path)){
				vector<int> newpath(path); // construct a new path 
				newpath.push_back(adjlist[last][i].second); // add the adjacent node
				pq.push(Path(newpath, keys_previous, collab, curiosity, src, dst ));
			}
		}

    }
    cerr << "DIJKSTRAS-ROUTING: nextoptimalpath1: NO PATH TO DESTINATION"  << endl;
    terminate();
   // return path; // no use of this statement
}

/// THESE FUNCTION SHALL ASSIST GUI CHANGES
void converttotypededge(vector<vector<int> > &edges , vector<int> path,int type){
	int numnodes = path.size();
	for (int i = 0 ; i < numnodes -1 ; i ++){
		edges[ path[i] ][ path[i+1] ] = edges[ path[i+1] ][ path[i] ] = type ;
	}
	drawedges(edges);
}
void convertbacktonormaledge(vector<vector<int> > &edges , vector<int> path){
	int numnodes = path.size();
	for (int i = 0 ; i < numnodes -1 ; i ++){
		edges[ path[i] ][ path[i+1] ] = edges[ path[i+1] ][ path[i] ]= DEFAULT_EDGE ;
	}
	drawedges(edges);
}
void converttodecisionedge(vector<vector<int> > &edges, int node1, int node2){
	//edges[node1][node2] = DECISION_EDGE ;
	//drawedges(edges);
	drawedge(node1, node2, DECISION_EDGE);
}
void convertalldecisiontonormaledges(vector<vector<int> > &edges){
	for (int i = 0 ; i < edges.size() ; i ++)
		for (int j = 0 ; j < edges[i].size() ; j ++)
			if ( edges[i][j] == DECISION_EDGE ) 
				edges[i][j] = DEFAULT_EDGE ;
	drawedges(edges);
}
void convertalltonormaledge(vector<vector<int> > &edges ){
	for (int i = 0 ; i < edges.size() ; i ++)
		for (int j = 0 ; j < edges[i].size() ; j ++)
			if ( edges[i][j] > 0 ) 
				edges[i][j] = DEFAULT_EDGE ;
	drawedges(edges);
}
//// THIS IS FUNCTION WITH GUI CHANGE
// this function also considers if destination is reachable if current choice is made
//pair<vector<int>,vector<vector<int> > > 
Path nextoptimalpath1gui(	vector<vector<pair<int,int> > > adjlist, // this function needs the adjacent paths as well
	vector<vector<int> > edges , // note this function also needs edges to take care of drawing 
	vector<vector<int> > keys_previous , // this contains the previous key distribution 
	vector<vector<float> > collab, vector<float> curiosity,int src,int dst){
	// create a priority_queue which stores the paths from source sorted based on path performance
	// if destination is a member of the path then your done
    priority_queue <Path, vector<Path>, ComparePaths> pq;
    vector<int> path ;
    vector<vector<int> > keys ;
    path.push_back(src); // make a path with source at the start
    pq.push(Path(path, keys_previous, collab, curiosity, src, dst ));
    convertalltonormaledge(edges);
    bool gosteps = true ;
    if ( gosteps ) if ( waitandreturn() == 'i') gosteps = false ;
    while(!pq.empty()) {
    	path = pq.top().getPath(); // get the path with best performance
    	keys = pq.top().getKeys(); // get the key distribution corresponding to the best path
    	// this was added to the end .. REMOVE IF NOT NEEDED OR WORKING
    	setPerformance(pq.top().getPerformance());
    	// create a temporary prioirity queue just to get display paths
    	priority_queue <Path, vector<Path>, ComparePaths> pqtmp (pq);
    	while(!pqtmp.empty()){
			vector<int> pathtmp = pqtmp.top().getPath();
			//vector<vector<int> > keystmp = pqtmp.top().getKeys();
			converttotypededge(edges ,pathtmp, EXPLORATION_EDGE);
			pqtmp.pop();
    	}
    	//converttotypededge(vector<vector<int> > &edges , vector<int> path,int type)
    	if ( containsdestination (path,dst) ) { // if destination found
    		converttotypededge(edges ,path, PATH_EDGE);
    		waitandreturn();
    		
			cout << "BEST PATH PERFORMANCE" << pq.top().getPerformance() << endl << endl << endl;
    		cout << "CURRENT PATH:" ;
    		printapath(path);
    		cout << "LIST OF ALL OTHER PATHS" << endl ;
    		// we can check the contents of the pq
    		priority_queue <Path, vector<Path>, ComparePaths> pqtmp1 (pq);
    		while(!pqtmp1.empty()){
    			vector<int> pathtmp = pqtmp1.top().getPath();
    			vector<vector<int> > keystmp = pqtmp1.top().getKeys();
				printapath(pathtmp);
				//printkeydistribution(keystmp);
				cout << "Path performance: " << pqtmp1.top().getPerformance() << endl;
				pqtmp1.pop();

    		}
    		



    		//return make_pair(path,keys); MODIFIED HERE
    		return pq.top();
    	}

    	pq.pop(); // pop it out
    	
    	int last = path[path.size()-1]; // get the last node
    	// traverse to all the nodes connected to current vertex 
		// and push new path to queue
		for (int i=0 ; i < adjlist[last].size(); i++){
			if (isNotVisited(adjlist[last][i].second, path)){
				vector<int> newpath(path); // construct a new path 
				newpath.push_back(adjlist[last][i].second); // add the adjacent node
				// color as decision edges ....
				converttodecisionedge(edges,last,adjlist[last][i].second);
				// check if destination is reachable before pushing
				// REMOVE THIS IF NOT WORKING
				if ( isdestinationreachable(adjlist,newpath,dst) )
					pq.push(Path(newpath, keys_previous, collab, curiosity, src, dst ));
			}
		}
    	if ( gosteps ) if ( waitandreturn() == 'i') gosteps = false ;
    	convertalldecisiontonormaledges(edges);

    }
    cerr << "DIJKSTRAS-ROUTING: nextoptimalpath1: NO PATH TO DESTINATION"  << endl;
    terminate();
}


 // priority queue ->> true element goes to bottom 
// false element is popped first and is a queue
// false element on top 
// same performace then first in popped first ...


int main() {	
		// initialize graph with required size, radius of nodes and maximum coordinate values of points
	initgraph(500,20,10);
	// set the number of points in the graph
	int numnodes = 10 ;
	// set the number of times to try to get a random graph before failing
	int repeat = 20 ;
	float threshold = 1 ; // set threshold on connectivity to neighbourhood 

	pair<vector<pair<float,float> > ,vector<vector<int> > >  graph = randomgraph( numnodes,threshold,repeat);
	vector<pair<float,float> >  points = graph.first ; // get cooridinates of points
	vector<vector<int> > edges = graph.second ; // get the edge matrix of points
	printpoints(points);
	printgraph(edges);
	drawnodes(points,true);
	drawedges(edges); // draw edge 
	// set collaboration parameters
	float meanCollaboration = 0.5 ; float stdCollaboration = 0.2 ;
	vector<vector<float> > collab = randomcollab(numnodes,meanCollaboration,stdCollaboration);
	// set curiosity parameters
	float meanCuriosity = 0.6 ; float stdCuriosity = 0.3 ;
	vector<float> curiosity = randomcuriosity(numnodes,meanCuriosity,stdCuriosity);
	// print the collaboration and curiosity levels ...
	printcollab ( collab );
	printcuriosity ( curiosity );
	//get adjacency list 
	vector <vector <pair <int,int> > >  adjlist = getadjlist( edges);
		int src = 0 ; 
	int dst = numnodes - 1;
	vector<vector<int> > keys ; // keys[<node>][<key>]
	keys.resize(numnodes) ; // Number of nodes -> first dimension
	vector<Path> optimalPaths ;
	// for (int i = 0 ; i < 5 ; i++){
	// 	cout << "KEY ITERATION: " << i << endl ; 
	// 	setCurrentKey(i + 1);
	// 	//pair<vector<int>,vector<vector<int> > > nop = nextoptimalpath1( adjlist, keys ,  collab, curiosity,src,dst);
	// 	//pair<vector<int>,vector<vector<int> > > nop = nextoptimalpath1gui(	 adjlist, edges, keys ,collab,curiosity,src, dst);
	// 	Path nop = nextoptimalpath1gui(	 adjlist, edges, keys ,collab,curiosity,src, dst);
	// 	optimalPaths.push_back(nop);
	// 	cout << "BEST PATH PERFORMANCE::" << endl ;
	// 	printapath(nop.getPath());
	// 	keys = nop.getKeys() ;
	// 	printkeydistribution(nop.getKeys());

	// }
	// Alternative to previous code 
	int iteration = 0 ;
	float performanceThreshold = 0.9 ;
	int maxIterations = 10 ;
	float currPerformance = 0 ;
	while( currPerformance < performanceThreshold && iteration < maxIterations ){
		cout << "KEY ITERATION: " << iteration << endl ; 
		setCurrentKey(iteration + 1);
		//pair<vector<int>,vector<vector<int> > > nop = nextoptimalpath1( adjlist, keys ,  collab, curiosity,src,dst);
		//pair<vector<int>,vector<vector<int> > > nop = nextoptimalpath1gui(	 adjlist, edges, keys ,collab,curiosity,src, dst);
		Path nop = nextoptimalpath1gui(	 adjlist, edges, keys ,collab,curiosity,src, dst);
		optimalPaths.push_back(nop);
		cout << "BEST PATH PERFORMANCE::" << endl ;
		printapath(nop.getPath());
		keys = nop.getKeys() ;
		printkeydistribution(nop.getKeys());
		iteration++ ;
		currPerformance = nop.getPerformance();
	}
	cout <<endl << endl <<" OPTIMAL PATHS: " << endl ;
	for (int i = 0 ; i < optimalPaths.size() ; i ++){
		cout << "KEY " << i << " : " << endl ;
		printapath(optimalPaths[i].getPath());
		cout << "Performance :" << optimalPaths[i].getPerformance() << endl ;
	}
	waitgraph();// press q to quit and r to redraw
	return 0;
}

