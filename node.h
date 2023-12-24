//
// node.h
// Created by Khoa Nguyen on 12/22/23.

#ifndef _node_h_
#define _node_h_

#include <string>
#include <vector>
#include "outcome.h"

using namespace std;

class node {
public:
	// Constructors
	node();
	node(vector<outcome> input_outcome_list, string input_outcome_name, int input_turn);
	node(node *input_node);
	
	//destructor
	~node();
	
	vector<outcome> outcome_list; //outcome list
	bool has_child_node;

	
	//methods
	string get_outcome_name(); //name getter
	void add_child_node(node* input_node); //add a child node to the node
	void set_p(float input_p);
	float get_p();
	vector<node*>* get_child_node_vector();
	int get_turn();
	
	
private:
	int turn;
	float p; //probability at node
	string outcome_name; //name of the outcome at the node
	vector<node*>* child_node_vector; //pointer to vector of poiters of the child nodes
};

#endif
