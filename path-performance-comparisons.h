#ifndef _PATH_PERFORMANCE
#define _PATH_PERFORMANCE

/*
	this function in this library has good metric ... But i donno how to utilize it ... 
	THis function tells how one path performance in comparison to other paths .. also
	how one path influences other paths ....
*/
#include <utility>
#include <vector>
using namespace std;
// this function returns the probability that nodes in one path dont 
// share information/shares with another path
// and probability that nodes in a path dont break the secret
// send paths to be included for comparisions
// assuming curiosity and collaborations are compatible
// paths also compatible
// assuming collaborations matrix is symmetric
vector <vector<float> > getdisjoincy( vector<vector<int> >  paths, 
		vector<vector<float> > collab,
		vector<float> curiosity);
// this function returns 'count' most optimal paths..
vector<vector<int> > getoptimalpaths( vector<vector<int> > paths,vector<vector<float> > collab,
		vector<float> curiosity, int count);
#endif