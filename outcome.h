#ifndef OUTCOME_H
#define OUTCOME_H

using namespace std;

#include <string>

class outcome{
public: 
	//constructors
	outcome();
	outcome(string input_name, int input_n);
	
	void decrease_n();
	int get_n();
	string get_name();
	
private:
	string name;
	int n; //number of times the outcome falls into 
};

#endif