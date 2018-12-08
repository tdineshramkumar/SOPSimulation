#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <exception>
#include <utility>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include "performance-measures.h"

using namespace std;

// this function returns default allocation
// ie no key is given to any path
vector<int> getdefaultalloc(int numkeys){
	vector<int> defaultalloc;
	defaultalloc.resize(numkeys);
	for (int i = 0 ; i < numkeys; i ++)
		defaultalloc[i] = -1;
	return defaultalloc ;
}

// this function randomly allocate keys to paths
// method is used to decide whether to allocate all keys to paths or not ..
// that is some keys unallocated ...
vector<int> randomallocation(int numkeys,int numpaths,int method){
	cout << "Random Allocation: " ;
	vector<int> alloc ;
	srand(time(NULL)) ;
	alloc.resize(numkeys);
	for (int i=0; i< numkeys; i++){
		if (method == ALLOC_NO_PATHS_AS_WELL)
			alloc[i] = ( rand() % (numpaths + 1) ) -1 ; // if you want some keys not be allocated
		else if (method == ALLOC_ONLY_PATHS)
			alloc[i] = rand() % numpaths ; // if you want all keys allocated to paths randomly 
		cout << i << ":" <<alloc[i] << " " ;
	}
	cout << endl ;
	return alloc;
}

// this function prints allocation of keys to paths ...
void printalloc(vector<int> alloc){
	int numkeys = alloc.size();
	cout << "Current Allocation : " <<endl;
	for (int i = 0 ;i < numkeys ; i ++) {
		cout << "KEY" <<setw(4) << i << ":" << "PATH" << setw(4) << alloc[i] << endl ; 
	}
}

// this function returns the key distribution among
// nodes ie which node has which key based on allocation
// of keys to different paths
// input: 	current allocation .ie. which key given to which path
//			-1 to indicate current key NOT given
//			size of alloc equal number of keys
// OUTPUT : Rows correspond to each NODE 
//			Columns correspond to whether that given node has a key or not
vector<vector<int> > keydistribution(vector<int> alloc, vector<vector<int> > paths,int numnodes){
	int keyscount = alloc.size();
	vector<vector<int> > keys ; // keys[<node>][<key>]
	keys.resize(numnodes) ; // Number of nodes -> first dimension
	for (int i=0; i <numnodes; i++){ 
		keys[i].resize(keyscount);
		for (int j=0; j< keyscount; j++){
			keys[i][j] = 0; // initially set 0 ie (dont have that key) 
		}
	}
	for (int i=0; i <keyscount; i++) {// for every key
		if ( alloc[i] != -1) { // that is allocated to a path
			vector<int> currpath = paths[alloc[i]] ; // get that path
			int pathcount = currpath.size(); // get its size
			for (int j=0; j <pathcount; j++){ // for nodes along the path
				keys[currpath[j]][i] = 1 ; // set curresponding key to 1 (received)
			}
		}
	}
	return keys;
}
vector<int> getkeyscount (vector<int> alloc, vector<vector<int> > paths,int numnodes){
	
	int numkeys = alloc.size();
	vector<int> keyscount ; 
	keyscount.resize(numnodes);
	vector<vector<int> > keys = keydistribution(alloc, paths, numnodes);
	
	for (int i=0; i <numnodes; i++){ 
		keyscount[i] = 0 ;
		for (int j = 0; j < numkeys; j++) {
			keyscount[i] += keys[i][j];
		}
	}
	return keyscount ;

}
// this function prints number of keys with each node ...
void printnumkeys(vector<int> alloc, vector<vector<int> > paths,int numnodes){
	
	cout << "Number of Keys with Each Node:" << endl ;
	int numkeys = alloc.size();
	
	vector<int> keyscount ; // keys[<node>][<key>]
	keyscount.resize(numnodes) ; // Number of nodes -> first dimension

	cout << setw(9) << "Nodes ";	
	for (int i = 0 ; i < numnodes ; i ++){
		cout << setw(7) << i ;	
	}
	
	cout << endl ;
	cout << setw(9) << "";	
	for (int i = 0 ; i < numnodes ; i ++){
		cout << setw(7) << "------"  ;	
	}
	cout << endl ;
	
	cout << setw(9) << "Keys ";	
	vector<vector<int> > keys = keydistribution(alloc, paths, numnodes);
	for (int i=0; i <numnodes; i++){ 
		keyscount[i] = 0 ;
		for (int j = 0; j < numkeys; j++) {
			keyscount[i] += keys[i][j];
		}
		cout << setw(7) << keyscount[i] ;
	}
	cout << endl ;
}
// this function prints the key distribution specified
// assuming valid distribution
void printkeydistribution(vector< vector<int> > keys){
	// display the graph
	vector <vector <int> >:: iterator itr1 ;
	vector <int> :: iterator itr2 ;
	int numkeys = keys[0].size();
	cout << std::setw(8) << "KEYS" << std::setw(2) << "";
	for (int i = 0 ; i < numkeys ; i ++){
		cout << std::setw(4) << i ;		
	}
	cout << endl ;
	cout << std::setw(10) << " " ;
	for (int i = 0 ; i < numkeys ; i ++){
		cout << std::setw(4) << "----" ;		
	}
	cout << endl ;	
	int i = 0 ;
	for (itr1 = keys.begin() ; itr1 != keys.end() ; i++, itr1++ ){
		cout << std::setw(5) << "NODE" << std::setw(3) << i << std::setw(2) << ":";
 		for (itr2 = itr1->begin() ; itr2 != itr1->end() ; itr2++) {
			cout << std::setw(4) << *itr2 ;
		}
		cout << endl ;
	}
}


// WHAT ARE THE PHYSICAL INTERPRETATIONS OF THIS PROBABILITIES
// NOTE CURIOSITY ALSO TAKEN CARE OFF ....
// this function returns probability that 'node' will break given 'key' under given key distribution
// if assuming no collaboration with source and destination 
// 'keys' contain the current key distribution 
// don't call this function on source and destination of a path ... i will yield it can't break a key ...
float nodebreakakey(int node, int key,vector<vector<int> >  keys,vector<vector<float> > collab, vector<float> curiosity,int src,int dst) {
	float p = 0 , np = 1;
	int numnodes = keys.size();
	int numkeys = keys[0].size();
	//cout << "NODE :" << node << " KEY:" << key << endl ;
	//cout << "INITIAL NOT PROP BREAK KEY:" << np << endl ;
	// first we ll compute probability that 'node' wont break 'key'
	// that is no one collaborates
	for (int i=0; i<numnodes ; i++){
		// multiply each time with probability node 'i' wont collaborate with 'node'
		// assuming he has key
		if (keys[i][key] && i!= src && i!= dst) // assuming src and dst dont share their keys
			np = np * ( 1 - collab[node][i] ) ;
		//cout << "NODE:" << i << " HAS KEY:"<<keys[i][key]  << " COLLAB:" << collab[node][i] << " PROP NOT BREAK KEY:" << np << endl ;
	}
	p = 1- np; 
	//cout << "PROP BREAK KEY:" << p << endl ;
	return curiosity[node] * p; // probability that node will break a key with given curiosity
}

// this function returns probability that 'node' will break all the keys
// don't call this function with node as 'src' or 'dst' i will give unexpected answer ... 'this it wont break the secret ' ---> Good assumption though 
// if u consider that it wont break not the key but that system security 
float nodebreakkeys(int node,vector<vector<int> >  keys,vector<vector<float> > collab, vector<float> curiosity,int src,int dst) {
	//cout << "NODE:" << node << " TRIES TO BREAK KEYS" ;
	float sp = 1 ; // product of probabilities that 'node' will break keys
	int numkeys = keys[0].size();
	//cout << "INITIAL SP: " << sp << endl ;
	for (int key =0; key < numkeys; key++){
		sp = sp * nodebreakakey(node,key,keys,collab,curiosity,src,dst); // add the probabilities that 'node' will break the code 'key'
		//Horrible choice
		//sp = sp + nodebreakakey(node,key,keys,collab,curiosity,src,dst); // add the probabilities that 'node' will break the code 'key'
		//cout << "KEY:" << key <<" SP:" << sp << endl ;	
	}
	// Most horrible choice
	//float p = sp/numkeys ; // average probability of breaking all keys
	//cout << "FINAL P:" << p << endl ;
	return sp ;
}

// this function returns the probability that none of the nodes will break the key
float nonodebreakskeys(vector<vector<int> >  keys,vector<vector<float> > collab, vector<float> curiosity,int src,int dst) {
	float np = 1;
	int numnodes = keys.size() ;
	for ( int node=0 ; node < numnodes ; node++){
		if (node != src || node!= dst ) // dont consider src and dst for this probabilites
			np = np * (1 - nodebreakkeys(node,keys,collab,curiosity,src,dst)) ; // probability that 'node' wont break any key
	}
	return np ;
}


// this function returns next optimal path under given situation
// maximize the probability that none will break the code
// alloc index contain which key is allocated
int nextoptimalpath(vector<int> curralloc,vector<vector<int > > paths,vector<vector<float> > collab, vector<float> curiosity,int src,int dst,int allocindex) {
	int numnodes = collab.size() ;
	vector <float> performance ;
	int numpaths = paths.size() ;
	performance.resize(numpaths);
	cout << setprecision(5) ;
	cout << "Performance Choices on allocation of key " << allocindex << ":" << endl ;

	for (int i=0 ; i < numpaths ; i++){

		int prev = curralloc[allocindex] ;
		curralloc[allocindex] = i ;
		vector<vector<int> > keys = keydistribution(curralloc,paths,numnodes) ;
		performance[i] = nonodebreakskeys(keys,collab,curiosity,src,dst);

		cout << "advantage along path" << i << " : " << setw(7) << performance[i] << endl ;
		curralloc[allocindex] = prev ;
 		//printkeydistribution(keys);
	}
	int nop = distance(performance.begin(),max_element(performance.begin(),performance.end())) ;
	cout << " Optimal Choice for next path is " << nop << endl;
	return nop;
}
// this allocates via greedy search
vector<int> optimalallocation(vector<vector<int > > paths,vector<vector<float> > collab, vector<float> curiosity,int src,int dst,int numkeys) {
	vector<int> alloc = getdefaultalloc(numkeys) ;
	for (int i = 0 ; i < numkeys ; i++){
		int nop = nextoptimalpath(alloc,paths,collab,curiosity,src,dst,i);
		alloc[i] = nop;
	}
	return alloc ;
}

/*

// this function returns the probability
// that given node will break the code
// under given the key distribution
vector<float> breakprobabilities(vector<vector<int> >keys,vector<vector<float> > collab,vector<float> curiosity){
	vector<float> bp ;	
	int numnodes = keys.size() ;// Number of nodes;
	int numkeys = keys[0].size(); // number of keys
	bp.resize(numnodes) ;
	for (int i = 0 ; i < numnodes ; i++){
		// for each node
		vector <float> bpk ; // breaking probabilities of different keys of node 'i'
		bpk.resize(numkeys);
		for (int j = 0; j < numkeys; j++){
			//for each key
			//what is probability that that person 
			//will break that key

			float prob = 1; // probability that i th node wont get key j

			for (int k=0; k < numnodes; k++) { // from each node
				// what is probability that he will get the key from him
				float pk = collab[i][k]; // probability that i and k will collaborate
				prob = prob * (1 - pk * keys[k][j]) ; // probability that 'k' wont give key 'j' to 'i' iff 'k' has key 'j'
			}
			bpk[j] = 1 - prob ; // what is probability that node i will break key j
		}
		float sp = 0 ; // sum of probabilities
		for (int z =0 ; z < numkeys; z++){
			sp = sp + bpk[z] ;
		}
		bp[i] = sp/numkeys ; // probability that i will break all keys
	}
	return bp;
}

// probability that no one will break the code given 
// individual probabilities of breaking the code 
float noOneBreaking(vector<float> indBreakCode){
	int count = indBreakCode.size() ;
	float finalProb = 1 ;
	for (int i = 0; i < count ; i ++) 
		finalProb = finalProb * (1 - indBreakCode[i]) ;
	return finalProb;
}

*/

