#include <iostream>
#include <chrono> // for time based seed
#include <random> // random number generator
#include <vector>
#include <cmath>
#include <exception>
#include <iomanip>
#include "collaboration-curiosity.h"

// NOTE USE -std=c++11 while compiling

using namespace std;
// this function generates collaboration  probability matrix 
// of N*N dimension with given mean and standard deviation from normal distribution
// but limits each value from 0 to 1 
vector<vector<float> > randomcollab(int N,float mean,float std){
	if (N < 0){
		cerr << "Invalid matrix dimension" << endl ;
		terminate();	
	}
	// construct a psuedo-random generator engine from a time-based seed
 	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator (seed);
	// generate random values from normal distribution
  	normal_distribution<float> distribution (mean,std);
	
	vector <vector <float> > collab ;
	// set the sizes of collaboration matrix
	collab.resize(N);
	for (int i = 0 ;i < N;i ++) 
		collab[i].resize(N);

	// initialize the diagonal elements to 1
	for (int i=0 ; i < N ;i ++) 
		collab[i][i] = 1 ; // nodes are sure to share the information with themselves ..

	float tmp ; // stores the random value temporarily 
	for (int i = 0; i < N ; i++)
		for (int j= 0 ; j < i ; j++){
			// generate a random value from distribution
			tmp =  distribution(generator);
			// set its limit to between 0 and 1
			if (tmp < 0) tmp = 0;
			else if (tmp > 1) tmp = 1;
			// set it as the collaboration level
			collab[i][j] = collab[j][i] = tmp ;
		}
	return collab;
}
// Curiosity Matrix
// this function generates a random curiosity probability matrix of N dimension with
// given mean and standard deviation but limits each value from 0 to 1 
vector<float>  randomcuriosity(int N,float mean,float std){
	if (N < 0){
		cerr << "Invalid matrix dimension" << endl ;
		terminate();	
	}
	// construct a psuedo-random generator engine from a time-based seed
 	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator (seed);
	// generate random values from normal distribution
  	normal_distribution<float> distribution (mean,std);
	
	vector <float> curiosity;
	// set the sizes of curiosity matrix
	curiosity.resize(N);
	
	float tmp ; // stores the random value temporarily 
	for (int i = 0; i < N ; i++ ) {
		// generate a random value from distribution
		tmp =  distribution(generator);
		// set its limit to between 0 and 1
		if (tmp < 0) tmp = 0;
		else if (tmp > 1) tmp = 1;
		// set it as the curiosity levels
		curiosity[i] = tmp ;
	}
	return curiosity;
}
// this function prints the collaboration matrix
void printcollab ( vector <vector<float> > collab ){
	cout << "Collaboration values:" << endl ; 
	int numpoints = collab.size();
	cout << std::fixed << std::setprecision(4) ; // set the precision here
	cout << setw(9) << "Points ";	
	for (int i = 0 ; i < numpoints ; i ++){
		cout << setw(7) << i ;	
	}
	cout << endl ;
	cout << setw(9) << "";	
	for (int i = 0 ; i < numpoints ; i ++){
		cout << setw(7) << "------"  ;	
	}
	cout << endl ;
	for (int i= 0 ; i < numpoints ; i ++){
		cout << "Point" << setw(3) << i << ":" ;
		for (int j = 0 ; j < numpoints ; j ++ ){
			cout << setw(7) << collab[i][j]  ;
		}
		cout << endl ;
	}
}
// this function prints the Curiosity matrix
void printcuriosity ( vector<float> curiosity){
	cout << "Curiosity values:" << endl ; 
	int numpoints = curiosity.size();
	cout << std::fixed << std::setprecision(4) ; // set the precision here
	cout << setw(9) << "Points ";	
	for (int i = 0 ; i < numpoints ; i ++){
		cout << setw(7) << i ;	
	}
	cout << endl ;
	cout << setw(9) << "";	
	for (int i = 0 ; i < numpoints ; i ++){
		cout << setw(7) << "------"  ;	
	}
	cout << endl ;
	cout << setw(9) << "Curioisty";	
	for (int i= 0 ; i < numpoints ; i ++){
		cout << setw(7) << curiosity[i] ;
	}
	cout << endl ;
}
/*
int numpoints = distances.size();
	cout << std::fixed << std::setprecision(4) ; // set the precision here
	cout << setw(9) << "Points ";	
	for (int i = 0 ; i < numpoints ; i ++){
		cout << setw(7) << i ;	
	}
	cout << endl ;
	cout << setw(9) << "";	
	for (int i = 0 ; i < numpoints ; i ++){
		cout << setw(7) << "------"  ;	
	}
	cout << endl ;
	for (int i= 0 ; i < numpoints ; i ++){
		cout << "Point" << setw(3) << i << ":" ;
		for (int j = 0 ; j < numpoints ; j ++ ){
			cout << setw(7) << distances[i][j]  ;
		}
		cout << endl ;
	}

*/
/*
// this function prints the collaboration matrix
void printcollab ( vector <vector<float> > collab ){
	cout << "Collaboration values:" << endl ;
	vector <vector <float> >:: iterator itr1 ;
	vector <float> :: iterator itr2 ;
	cout << std::fixed ; // set precision to fixed value 
	cout << std::setprecision(3); // set number on decimal digits to display 	
	for ( int i = 0 ; i < collab.size() ; i ++) {

	}
	for ( itr1 = collab.begin() ; itr1 != collab.end() ; itr1++ ){
		for (itr2 = itr1->begin() ; itr2 != itr1->end() ; itr2++) {
			cout << " " <<  std::setw(7) 	// this sets the field width of next word to be printed
				<<*itr2 <<" " ; 	// and is left justified 
		}
		cout << endl ;
	}

}
// this function prints the Curiosity matrix
void printcuriosity ( vector<float> curiosity){
	vector <float> :: iterator itr ;
	cout << std::fixed ; // set precision to fixed value 
	cout << std::setprecision(3); // set number on decimal digits to display 
   	for (itr = curiosity.begin() ; itr != curiosity.end() ; itr ++) {
		cout << " " <<  std::setw(7) 	// this sets the field width of next word to be printed
			<< *itr <<" " ; 	// and is left justified 
	}
	cout << endl ;
}*/
/*
int main(){
	vector <vector <float> > collab = randomcollab(10,-0.5,1) ;
	vector <float> curiosity = randomcuriosity(10,0.5,0.01) ;
	cout << "COLLABORATION" << endl ;
	printcollab(collab);
	cout << endl << "CURIOSITY" << endl ;
	printcuriosity(curiosity);

	return 0;
}
*/
/*
int main(){

	vector<vector<float> > v = randomcollab(10,-0.5,1);
	vector <vector <float> >:: iterator itr1 ;
	vector <float> :: iterator itr2 ;
	cout << std::fixed ; // set precision to fixed value 
	cout << std::setprecision(3); // set number on decimal digits to display 
	for ( itr1 = v.begin() ; itr1 != v.end() ; itr1++ ){
		for (itr2 = itr1->begin() ; itr2 != itr1->end() ; itr2++) {
			cout << " " <<  std::setw(7) 	// this sets the field width of next word to be printed
											// and is left justified 
				<<*itr2 <<" " ;
		}
		cout << endl ;
	}

}
*/
