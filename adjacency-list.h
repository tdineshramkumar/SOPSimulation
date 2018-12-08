#ifndef _ADJACENCY_LIST_REPR
#define _ADJACENCY_LIST_REPR
#include <vector>
#include <utility>

using namespace std;
// this function returns the adjacency representation of lists ...
vector <vector <pair <int,int> > >  getadjlist( vector <vector <int> > graph );
// this function prints the adjacency list representation of matrix
void printadjlist(  vector <vector <pair <int,int> > > adjlist );
#endif
