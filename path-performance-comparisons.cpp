/*
	this function in this library has good metric ... But i donno how to utilize it ... 
	THis function tells how one path performance in comparison to other paths .. also
	how one path influences other paths ....
*/

#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
#include "path-performance-comparisons.h"
using namespace std ;

// this function returns the probability that nodes in one path dont 
// share information/shares with another path
// and probability that nodes in a path dont break the secret
// send paths to be included for comparisions
// assuming curiosity and collaborations are compatible
// paths also compatible
// assuming collaborations matrix is symmetric
vector<vector<float> > getdisjoincy( vector<vector<int> >  paths, 
		vector<vector<float> > collab,
		vector<float> curiosity){
	// if i != j
	// probability that no node in path j would
	// break the code given that path i has the
	// the code .. ie .. tendency not to share
	// if i == j
	// probability that no nodes in path i would
	// the code given the code
	int pathcount = paths.size();
	vector<vector<float> > disjoincy ;
	// size the vector for optimal performance
	disjoincy.resize(pathcount);
	for (int i = 0 ; i < pathcount;i++) {
		disjoincy[i].resize(pathcount);
	}
	// setting the probability none in path
	// will break 
	// this depends on curiosity of nodes along the path
	for (int i = 0 ; i < pathcount; i++) {
		disjoincy[i][i] = 1 ;
		int pathlength = paths[i].size();
		int start = paths[i].front(); // contains the source node
		int destination = paths[i].back(); // contains the destination node
		// traverse along the path except source and destination
		for (int j = 1 ; j < pathlength -1 ; j++ ){
			// probability that nodes along the path (except source and destination)
			// will not break the code 
			disjoincy[i][i] = disjoincy[i][i] * ( 1 - curiosity[ paths[i][j] ] ) ; 
		}
	}
	
	//traverse each of the path one by one
	//probability that no along one path 
	//does not share information with nodes in the other path
	//this matrix is symmetric
	for (int i = 0 ; i < pathcount ; i++ ){
		// not to repeat calculations ?
		for (int j = 0 ; j < i ; j ++) {
			disjoincy[i][j] = 1;
			disjoincy[j][i] = 1;
			// calculate that probability that NO node in path i
			// will share info with nodes in path j
			int pathlength_i = paths[i].size() ; // get the length of first path
			int pathlength_j = paths[j].size() ; // get the length of second path
			// dont visit the source and destination nodes
			for (int m = 1 ; m < pathlength_i - 1 ; m++ ){
				for (int n = 1 ; n < pathlength_j - 1 ; n++) {
					// may be later used in case of asymmetry ?
					//cout << i << " " << j << " " <<  disjoincy[i][j] << ":" << collab[paths[i][m]][paths[j][n]] << endl;
					disjoincy[i][j] = disjoincy[i][j] * (1 - collab[paths[i][m]][paths[j][n]]) ; 
					disjoincy[j][i] = disjoincy[j][i] * (1 - collab[paths[j][n]][paths[i][m]]) ; 
				}
			}
		
		}
	}
	return disjoincy ;
}

bool bkcompare(pair<float,int> value1, pair<float,int> value2){
	return value1.first < value2.first ; // ascending order
}
// this function returns 'count' most optimal paths..
vector<vector<int> > getoptimalpaths( vector<vector<int> > paths,vector<vector<float> > collab,
		vector<float> curiosity, int count){
	vector<vector<float> > disjoincy = getdisjoincy( paths,collab,curiosity);
	vector<pair<float,int> > bk ; // a measure that some on will break the key in a path...  
	bk.resize(paths.size()) ;
	for (int i = 0 ; i < paths.size() ; i ++){
		bk[i].second = i ;
		bk[i].first = 0 ;
		for ( int j = 0 ;  j < paths.size() ; j ++ ) {
			if ( j == i) 
				bk[i].first += (1 - disjoincy[i][i]) ; // probability that some one will be curious to see the key
			else  
				// probability that some nodes in one path share with nodes in another path and that they will break 
				bk[i].first += (1 - disjoincy[i][j])*( 1- disjoincy[j][j]) ; 
		}
	}
	sort (bk.begin(), bk.end(), bkcompare);
	bk.erase(bk.begin()+count,bk.end());
	vector<vector<int> > newpath ;
	for (int i = 0 ; i < bk.size() ; i++) {
		newpath.push_back(paths[ bk[i].second]) ;
	} 
	return newpath;
}
/*
int main(){
	// set precision of outputs
	cout << setprecision(3) ;
	return 0;
}
*/
