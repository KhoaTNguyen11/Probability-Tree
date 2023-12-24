#include "outcome.h"
#include <iostream>
#include <string>

using namespace std;
//constructors
outcome::outcome(){
	name = "";
	n = 0;
}

outcome::outcome(string input_name, int input_n){
	name = input_name;
	n = input_n;
}
//decreases the outcome count of this outcome
void outcome::decrease_n(){
	n--;
}
//get the outcome count of this outcome
int outcome::get_n(){
	return n;
}
//get the name of this outcome
string outcome::get_name(){
	return name;
}