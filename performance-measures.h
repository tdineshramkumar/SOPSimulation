/*
	Note: this library needs to include visualization features as well ....
	Include them later ... show dynamically how nodes explore paths ....
*/
#ifndef _PERFORMANCE_MEASURES
#define _PERFORMANCE_MEASURES

#include <vector>
using namespace std;

#define ALLOC_ONLY_PATHS 1
#define ALLOC_NO_PATHS_AS_WELL 2

// this function returns default allocation
vector<int> getdefaultalloc(int numkeys);
// this function randomly allocate keys to paths
vector<int> randomallocation(int numkeys,int numpaths,int method=ALLOC_ONLY_PATHS);
// this function prints allocation of keys to paths ...
void printalloc(vector<int> alloc);
// this function returns the key distribution among
// nodes ie which node has which key based on allocation
// of keys to different paths
// input: 	current allocation .ie. which key given to which path
//			-1 to indicate current key NOT given
//			size of alloc equal number of keys
// OUTPUT : Rows correspond to each NODE 
//			Columns correspond to whether that given node has a key or not
vector<vector<int> > keydistribution(vector<int> alloc, vector<vector<int> > paths,int numnodes);
// this function prints the key distribution specified
// assuming valid distribution
void printkeydistribution(vector< vector<int> > keys);


// this function returns number of keys wiht each node
vector<int> getkeyscount (vector<int> alloc, vector<vector<int> > paths,int numnodes);
// this function prints number of keys with each node ....
void printnumkeys(vector<int> alloc, vector<vector<int> > paths,int numnodes);

// ACTUAL PARAMETERS THAT NODE WILL BREAK THE KEY
// WHAT ARE THE PHYSICAL INTERPRETATIONS OF THIS PROBABILITIES
// NOTE CURIOSITY ALSO TAKEN CARE OFF ....
// this function returns probability that 'node' will break given 'key' under given key distribution
// if assuming no collaboration with source and destination 
// 'keys' contain the current key distribution 
float nodebreakakey(int node, int key,vector<vector<int> >  keys,vector<vector<float> > collab, vector<float> curiosity,int src,int dst) ;
// this function returns probability that 'node' will break all the keys
float nodebreakkeys(int node,vector<vector<int> >  keys,vector<vector<float> > collab, vector<float> curiosity,int src,int dst);
// this function returns the probability that none of the nodes will break the key
float nonodebreakskeys(vector<vector<int> >  keys,vector<vector<float> > collab, vector<float> curiosity,int src,int dst);
// this function returns next optimal path under given situation
// maximize the probability that none will break the code
// alloc index contain which key is allocated
int nextoptimalpath(vector<int> curralloc,vector<vector<int > > paths,vector<vector<float> > collab, vector<float> curiosity,int src,int dst,int allocindex);
// this allocates via greedy search
vector<int> optimalallocation(vector<vector<int > > paths,vector<vector<float> > collab, vector<float> curiosity,int src,int dst,int numkeys);


#endif