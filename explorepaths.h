#ifndef _EXPLORE_PATHS
#define _EXPLORE_PATHS
#include <vector>

using namespace std;

// this function finds all paths in the graph from source
// to desination using Breadth First Search ...
vector<vector<int> > findpaths( vector<vector<pair<int,int> > > adjlist ,int src, int dst );
// this function prints the found paths in the graph
void printpaths(vector<vector<int> > &paths);
bool isNotVisited( int x,vector<int> &path);
#endif