//
// node.cpp
// Created by Khoa Nguyen on 12/22/23.

#include "node.h"
#include <string>
#include "outcome.h"
#include <iostream>
#include <fstream>
#include <vector>

//constructors
node::node() {
	turn = 0;
	outcome_name = "";
	p = 1.0;
	child_node_vector = new vector<node*>();
	has_child_node = false;
}

node::node(vector<outcome> input_outcome_list, string input_outcome_name, int input_turn) {
	outcome_list = input_outcome_list;
	outcome_name = input_outcome_name;
	turn = input_turn;
	child_node_vector = new vector<node*>(); // Initialize child_node_vector in this constructor
	has_child_node = false;
	p = 1.0;
}

node::node(node* input_node) { //copy constructor
	outcome_list = input_node->outcome_list;
	outcome_name = input_node->outcome_name;
	turn = input_node->turn;
	child_node_vector = new vector<node*>(); // Initialize child_node_vector in this constructor
	has_child_node = false;
	p = 1.0;
}

node::~node() {
	// Properly delete child nodes if they were dynamically allocated
	for (node* child : *child_node_vector) {
		delete child;
	}
	delete child_node_vector; // delete the vector itself
}

void node::set_p(float input_p){ //set probability at node
	p = input_p;
}
float node::get_p(){ //get the probability at node
	return p;
}

string node::get_outcome_name(){ //get the outcome name at node
	return outcome_name;
}

vector<node*>* node::get_child_node_vector(){ //return the vector that holds the pointers to the child nodes of this node
	return child_node_vector;
}

void node::add_child_node(node* input_node) {
	if (child_node_vector == nullptr) {
		cout << "child_node_vector is null!" << endl;
		return; // Return or handle this error condition appropriately
	}
	child_node_vector->push_back(input_node);
	if (input_node == nullptr) {
		cout << "input_node is null!" << endl;
		return; // Return or handle this error condition appropriately
	}
}
 int node::get_turn(){ //return the turn of the node
	return turn;
}
