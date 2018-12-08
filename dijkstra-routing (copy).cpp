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

vector<bool> nonodevisited (int numnodes){
	vector<bool> visited ;
	visited.resize(numnodes);
	for (int i = 0 ; i < numnodes ; i ++)
		visited[i] = false ; 
	return visited ;
}

vector<int> getadjacentnotvisitednodes(vector<vector<int> > &edges,vector<bool> visited){
	int numnodes = visited.size(); // vector visited is passed by value
	vector<int> adjacentlist ;
	for (int i =0 ; i < numnodes ; i ++){
		// check if node is visited and check its adjacent nodes also visited else add them to adjacentlist
		if ( visited[i] ) {
			// assuming symmetric adjacent matrix
			for (int j = 0 ; j < numnodes ; j ++ ){
				if ( edges[i][j] > 0 && edges[j][i] > 0  && !visited[j] ) // if these is an edge and the node is not visited
					adjacentlist.push_back(j) , visited[i] = true ; // to make sure same edge not added twice
			}
		}
	}
	return adjacentlist ;
}

vector <pair<int,float> > getperformanceamongadjacentnodes(vector<vector<int> > &keys,
	 vector<int> &adjacentlist,vector<vector<float> > &collab,
	 vector<float> &curiosity,int src,int dst,int whichkey)	{ // here which key contains the key currently considered in 'keys'
	// Assuming some amount of adjacent nodes exist ...
	vector< pair<int,float> > performance ;
	int numadjnodes = adjacentlist.size(); // get number of adjacent nodes
	for (int i = 0 ; i < numadjnodes ; i ++){
		int x = keys[  adjacentlist[i] ][whichkey]  ; // sinces keys are passed by reference
		keys[ adjacentlist[i]  ][whichkey] = 1 ; // Assume that key is given to that adjacent node find performance
		performance.push_back(make_pair(  adjacentlist[i] ,nonodebreakskeys(keys,collab,curiosity,src,dst))) ;
		keys[adjacentlist[i]][whichkey] = x ; // Restore the adjacent keys
	}
	return performance;
}

int getnextbestchoice( vector<pair<int,float> >  &choices) {
	// assuming there is atleast one choice
	if ( choices.size() <= 0){
		cerr << "DIJKSTRAS-ROUTING: No choices Exist" << endl ;
		terminate();
	}
	int best = choices[0].first; // get the first node
	float performance = choices[0].second;
	for ( int i = 1 ; i < choices.size() ; i ++){
		if ( performance < choices[i].second ) // if current best is not the best
			performance = choices[i].second, best = choices[i].first ; // update with nodes with best node
	}
	for (int i =  0 ; i < choices.size() ; i ++)
		cout << "Choice "<< i << ": " << choices[i].first << " performance :" << choices[i].second  << endl ;
	cout << "Next Best Choice:" << best << " performance: " << performance << endl ;
	return best ;
}
bool containsdestination(vector<int> adjacentlist, int dst){
	bool contains = false ;
	for ( int i = 0 ; i < adjacentlist.size() ; i ++)
		if ( adjacentlist[i] == dst )
			contains = true , cout << "Found Destination" << endl ;
	return contains ;
}

vector<int> nextbestpath( vector<vector<int> > edges,vector<vector<int> > keys,
	vector<vector<float> > collab,vector<float> curiosity,int src,int dst,int whichkey) {
	vector<int> path ;
	int numnodes = keys.size(); // get num nodes
	vector<bool> visited = nonodevisited (numnodes); // set all nodes to not visited
	visited[src] = true ; // set source to visited
	path.push_back(src); // push source to the path
	while ( !visited[dst] ) {  
		vector<int> adjacentlist = getadjacentnotvisitednodes(edges,visited);
		if ( containsdestination(adjacentlist,dst) ) {
			path.push_back(dst) ;
			visited[dst] = true;
			break ;
		}
		vector <pair<int,float> > choices =  
			getperformanceamongadjacentnodes(keys,adjacentlist,collab,curiosity,src,dst,whichkey);
		int nextchoice = getnextbestchoice(choices);
		visited[nextchoice] = true ;
		path.push_back(nextchoice);
	}
	return path ;
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
		bool containsdestination(){
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
					if ( path1.getPathLength() <= path2.getPathLength() )
						return false ; // path1 on top
					else
						return true ; // path2 on top

			}
};
 


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
	float threshold = 0.9 ; // set threshold on connectivity to neighbourhood 

	pair<vector<pair<float,float> > ,vector<vector<int> > >  graph = randomgraph( numnodes,threshold,repeat);
	vector<pair<float,float> >  points = graph.first ; // get cooridinates of points
	vector<vector<int> > edges = graph.second ; // get the edge matrix of points
	printpoints(points);
	printgraph(edges);
	drawnodes(points,true);
	drawedges(edges); // draw edge 
	// set collaboration parameters
	float meanCollaboration = 0.3 ; float stdCollaboration = 0.2 ;
	vector<vector<float> > collab = randomcollab(numnodes,meanCollaboration,stdCollaboration);
	// set curiosity parameters
	float meanCuriosity = 0.5 ; float stdCuriosity = 0.2 ;
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
	for (int i = 0 ; i < 5 ; i++){
		cout << "KEY ITERATION: " << i << endl ; 
		pair<vector<int>,vector<vector<int> > > nop = nextoptimalpath1( adjlist, keys ,  collab, curiosity,src,dst);
		cout << "BEST PATH PERFORMANCE::" << endl ;
		printapath(nop.first);
		keys = nop.second ;
		printkeydistribution(keys);

	}
/*
	int numkeys = 5 ;
	int keyscount = 1 ;
	for (int i=0; i <numnodes; i++){ 
		keys[i].resize(keyscount);
		for (int j=0; j< keyscount; j++){
			keys[i][j] = 0; // initially set 0 ie (dont have that key) 
		}
	}

	int whichkey = 0 ;
	vector<int> nbp = nextbestpath(edges,keys,collab,curiosity,src,dst,whichkey);
	printapath(nbp);
	printkeydistribution(keys);
	*/
	waitgraph();// press q to quit and r to redraw
	return 0;
}

