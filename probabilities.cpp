#include "outcome.h"
#include "node.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<outcome> readfile(string input_filename, int* N);
node* make_probability_tree(vector<outcome> outcome_list, int N);
void make_child_node(node* parent_node, int turn, int N);
void terminal_outcome_probabilities(node* parent_node, string current_seq, float carrying_p);
void outcome_probabilities_at(node* parent_node, string current_seq, float carrying_p, int n, int N, int count);
bool compare_outcome_list(vector<outcome> outcome_list_1, vector<outcome> outcome_list_2, vector<outcome> og_list);
void composition_probabilites_at_n(node* parent_node, vector<outcome> composition_list, vector<outcome> og_list, int* count, float carrying_p, float* total_p, string current_seq, int max_turn);

int main(int argc, char *argv[]) {
	const string QUIT = "q";
	const string TERMINAL_OUTCOMES = "t";
	const string OUTCOMES_AT_N = "n";
	const string COMPOSITION_PROBABILITIES_AT_N = "c";
	int a = 0;
	int *N = &a;
	vector<outcome> outcome_list = readfile(argv[1], N);
	
	if(outcome_list.size() == 0){
		cout << "No outcome_list made. Program terminated";
	}else{
		cout << "Outcome_list made!" << endl;
		cout << "Original state: " << endl;
		cout <<"N: "<< *N << endl;
		for(int i = 0; i < outcome_list.size(); i++){
			cout << outcome_list.at(i).get_name() << " " << outcome_list.at(i).get_n() << endl;
		}
		cout << endl;
		
		node* starting_state = make_probability_tree(outcome_list, *N);
		
		//query loop
		string query_str;
		cout << "Query: ";
		cin >> query_str;
		cout << endl;
		while(query_str != QUIT){
			if(query_str == TERMINAL_OUTCOMES){
				terminal_outcome_probabilities(starting_state,"", 1.0);
			}else if(query_str == OUTCOMES_AT_N){
				int n;
				cout << "input turn at which you want the outcome probabilities calculated: ";
				cin >> n;
				cout << endl;
				outcome_probabilities_at(starting_state, "", 1.0, n, *N, 0);
			}else if(query_str == COMPOSITION_PROBABILITIES_AT_N){
				string comp_list_file;
				int max_turn;
				int b = 0;
				int* comp_N = &b;
				cout << "Path to composition list file: ";
				cin >> comp_list_file; 
				cout << endl;
				
				vector<outcome> comp_list = readfile(comp_list_file, comp_N);
				cout << "comp_list: " << endl;
				cout <<"comp_N: "<< *comp_N << endl;
				for(int i = 0; i < comp_list.size(); i++){
					cout << comp_list.at(i).get_name() << " " << comp_list.at(i).get_n() << endl;
				}
				cout << endl;

				cout << "Input max turn: ";
				cin >> max_turn;
				int c = 0;
				float d = 0.0;
				float* total_p = &d;
				int* count = &c;
				composition_probabilites_at_n(starting_state, comp_list, outcome_list,count, 1.0, total_p, "", max_turn);
				cout << "Count: " << *count << endl;
				cout << "Total probability for composition: " << *total_p << endl;
				
			}else{
				cout << "query not recognized!" << endl;
			}
			
			cout << "Query: ";
			cin >> query_str;
		}
		
		//thank you messages!
		cout << "Thank you for using my probabilities software! Have a nice day." << endl;
		cout << "From KHOA T. NGUYEN" << endl;
	}
}

//readfile
//description: reads input outcome list file and make an outcome vector
//input: string that is the path to the file, pointer to total "ball" count
//output: a vector that holds the outcomes 
vector<outcome> readfile(string input_filename, int* N){
	
	vector<outcome> outcome_list;
	ifstream input_file;
	input_file.open(input_filename);
	
	if (input_file.fail()){
		cerr << "ERROR: Unable to open file" << endl;
		exit(EXIT_FAILURE);
	}
	
	input_file >> *N; 
	int total_n_count = 0;
	string outcome_name;
	int input_n;
	
	while(input_file >> outcome_name >> input_n){
		total_n_count += input_n;
		if(total_n_count > *N){
			cout << "ERROR: total n surpassed N" << endl;
			cout << "total n is " << total_n_count << " ,while N is " << *N << endl;
			return outcome_list;
		}
		outcome_list.push_back(outcome(outcome_name, input_n));
	}
	input_file.close();
	return outcome_list;
}

//make_probability_tree
//description: takes in an outcome vector and builds a probability tree
//input: vector that holds the original outcome list, int that is the total "ball" count
//output: a pointer to the original node
node* make_probability_tree(vector<outcome> outcome_list, int N){
	cout << "make_probability_tree called"<< endl;
	int turn = 0;
	node* starting_state = new node(outcome_list, "", turn); // Dynamic allocation
	make_child_node(starting_state, turn, N); // Pass by reference
	cout << "done making probability tree!" << endl;
	return starting_state;
}

//make_child_node
//description: make child nodes for an input node. Used to recursively build a probability tree
//input: pointer to a parent node, integer turn that makes sure no more than N steps are in each 
//sequence of the tree
//output: none
void make_child_node(node* parent_node, int turn, int N) {
	if (turn < N) {
		for (int i = 0; i < parent_node->outcome_list.size(); i++) {
			if (parent_node->outcome_list.at(i).get_n() > 0) {
				vector<outcome> temp_list = parent_node->outcome_list;
				temp_list.at(i).decrease_n();
				node* child_node = new node(temp_list, parent_node->outcome_list.at(i).get_name(), turn + 1);
				child_node->set_p(float(parent_node->outcome_list.at(i).get_n()) / (N - turn));
				//cout << child_node->get_outcome_name() << " " << child_node->get_p() << endl;
				parent_node->add_child_node(child_node);
				if(parent_node->has_child_node == false){
					parent_node->has_child_node = true;
				}
			}
		}
		//cout << "Made it to " << turn + 1 << " turn" << endl;
		//cout << endl;
		turn++;
		for (node* child : *parent_node->get_child_node_vector()) {
			make_child_node(child, turn, N);
			//cout << endl;
		}
	}
}

//terminal_outcome_probabilities
//description: recursively prints out all the possible sequences to the probability tree and the 
//associated probability
//input: pointer to a parent node, string that is the running sequence, float that is the carrying 
//probability all the way from the root node
//output: none
void terminal_outcome_probabilities(node* parent_node, string current_seq, float carrying_p){
	
	carrying_p*=parent_node->get_p();
	current_seq+=parent_node->get_outcome_name() + " ";

	if(parent_node->has_child_node == true){
		for(int i = 0; i < parent_node->get_child_node_vector()->size(); i++){
			terminal_outcome_probabilities(parent_node->get_child_node_vector()->at(i),current_seq, carrying_p);
		}
	}else{
		cout << current_seq << " " << carrying_p << endl;
	}
}

//outcome_probability_at
//description: recursively prints out all the possible sequences till a certain turn
//input: as shown in function declaration
//output: void
void outcome_probabilities_at(node* parent_node, string current_seq, float carrying_p, int n, int N, int count){
	if(n > N){
		cout << "ERROR: n cannot be larger than N!" << endl;
	}else{
		carrying_p*=parent_node->get_p();
		current_seq+=parent_node->get_outcome_name() + " ";
		
		if(count < n){
			if(parent_node->has_child_node == true){
				for(int i = 0; i < parent_node->get_child_node_vector()->size(); i++){
					outcome_probabilities_at(parent_node->get_child_node_vector()->at(i),current_seq, carrying_p, n, N, count+1);
				}
			}else{
				cout << current_seq << " " << carrying_p << endl;
			}
		}else{
			cout << current_seq << " " << carrying_p << endl;
		}
	}
}

//compare_outcome_list
//description: determine if a node satisfies a certain outcome composition. 
//input: outcome list 1, 2 and the original one
//output: boolean
bool compare_outcome_list(vector<outcome> outcome_list_1, vector<outcome> outcome_list_2, vector<outcome> og_list) {
	int size_list_1 = outcome_list_1.size();
	int size_list_2 = outcome_list_2.size();
	
	if (size_list_1 != size_list_2) {
		return false; // If sizes don't match, lists are different
	}
	
	// Track whether an element from outcome_list_1 has been found in outcome_list_2
	bool found;
	
	for (int i = 0; i < size_list_1; ++i) {
		found = false; // Reset found flag for each element in outcome_list_1
		for (int j = 0; j < size_list_2; ++j) {
			if (outcome_list_1[i].get_name() == outcome_list_2[j].get_name()) {
				if (outcome_list_1[i].get_n() > og_list[i].get_n() - outcome_list_2[j].get_n()) {
					return false; // If names match but n values don't, lists are different
				}
				found = true; // Mark as found if names and n values match
				break;
			}
		}
		if (!found) {
			return false; // If an element from outcome_list_1 isn't found in outcome_list_2, lists are different
		}
	}
	
	return true; // Lists are the same
}

//composition_probabilities_at_n
//description: recursively search for sequences that satisfy a certain composition requirement. 
//input: as shown in function contract
//output: void
void composition_probabilites_at_n(node* parent_node, vector<outcome> composition_list, vector<outcome> og_list, int* count, float carrying_p, float* total_p, string current_seq, int max_turn){
	if(parent_node->get_turn() > max_turn){
		return;
	}
	current_seq += parent_node->get_outcome_name() + " ";
	carrying_p *=parent_node->get_p();
	if(compare_outcome_list(parent_node->outcome_list, composition_list, og_list)){
		cout << current_seq << carrying_p << endl;
		(*total_p)+=carrying_p;
		(*count)++;
	}else{
		for(int i = 0; i < parent_node->get_child_node_vector()->size(); i++){
			composition_probabilites_at_n(parent_node->get_child_node_vector()->at(i), composition_list, og_list, count, carrying_p, total_p, current_seq, max_turn);
		}
	}
}
	