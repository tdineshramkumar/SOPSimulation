#include <iostream>
#include <vector>
#include <utility>
#include <exception>
#include "adjacency-list.h"

using namespace std;


// this function  returns the adjacency lists representation of a given matrix
vector <vector <pair <int,int> > > 
getadjlist( vector <vector <int> > graph )
{
	int N = graph.size() ;
	for (int i = 0 ; i < N ; i++ )
		for (int j = 0; j < i; j ++)
			if (graph[i][j] != graph[j][i]){
				clog << "Invalid matrix" << endl;
				terminate();
			}
	
	vector <vector< pair<int,int> > > adjlist;
	adjlist.resize(N);
	for (int i= 0 ; i < N; i++){
		for (int j = 0 ; j < N; j++){
			if (i != j && graph[i][j] == 1) {
				adjlist[i].push_back(make_pair(i,j)) ;
			}
		}
	}
	return adjlist;
}

// this function prints the adjacency list representation of matrix
void printadjlist( 
		vector <vector <pair <int,int> > > adjlist 
)
{	int N = adjlist.size();
	vector< pair<int,int> >:: iterator itr;
	for ( int i = 0 ; i < N ; i ++) {
		for(itr = adjlist[i].begin(); 
				itr != adjlist[i].end() ; itr++){
			cout << itr->first << "-" << itr->second << endl;
		}
	}
}
