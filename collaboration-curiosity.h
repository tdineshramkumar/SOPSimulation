#ifndef _COLLAB_CURIOSITY
#define _COLLAB_CURIOSITY
#include <vector>
using namespace std;
// this function generates collaboration  probability matrix 
// of N*N dimension with given mean and standard deviation from normal distribution
// but limits each value from 0 to 1 
vector<vector<float> > randomcollab(int N,float mean,float std);

// Curiosity Matrix
// this function generates a random curiosity probability matrix of N dimension with
// given mean and standard deviation but limits each value from 0 to 1 
vector<float>  randomcuriosity(int N,float mean,float std);

// this function prints the collaboration matrix
void printcollab ( vector <vector<float> > collab );

// this function prints the Curiosity matrix
void printcuriosity ( vector<float> curiosity);
#endif