#include "outcome.h"
#include "node.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<outcome> readfile(string input_filename, int* N);
node* make_probability_tree(vector<outcome> outcome_list, int N);
void make_child_node(node* parent_node, int turn, int N);
void terminal_outcome_probabilities(node* parent_node, string current_seq,float carrying_p);
void outcome_probailities_at(node* parent_node, string current_seq, float carrying_p, int n, int N, int count);

int main(int argc, char *argv[]) {
	const string QUIT = "q";
	const string TERMINAL_OUTCOMES = "t";
	const string OUTCOMES_AT_N = "n";
	int a = 0;
	int *N = &a;
	vector<outcome> outcome_list = readfile(argv[1], N);
	
	if(outcome_list.size() == 0){
		cout << "No outcome_list made. Program terminated";
	}else{
		cout << *N << endl;
		for(int i = 0; i < outcome_list.size(); i++){
			cout << outcome_list.at(i).get_name() << " " << outcome_list.at(i).get_n() << endl;
		}
		cout << endl;
		cout << "Outcome_list made!" << endl;
		
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
				outcome_probailities_at(starting_state, "", 1.0, n, *N, 0);
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

vector<outcome> readfile(string input_filename, int* N){
	
	vector<outcome> outcome_list;
	ifstream input_file;
	input_file.open(input_filename);
	
	if (input_file.fail()){
		cerr << "ERROR: Unable to open file" << endl;
		exit(EXIT_FAILURE);
	}
	
	input_file >> *N ; 
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

node* make_probability_tree(vector<outcome> outcome_list, int N){
	cout << "make_probability_tree called"<< endl;
	int turn = 0;
	node* starting_state = new node(outcome_list, "", turn); // Dynamic allocation
	make_child_node(starting_state, turn, N); // Pass by reference
	cout << "done making probability tree!" << endl;
	return starting_state;
}

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

void outcome_probailities_at(node* parent_node, string current_seq, float carrying_p, int n, int N, int count){
	if(n > N){
		cout << "ERROR: n cannot be larger than N!" << endl;
	}else{
		carrying_p*=parent_node->get_p();
		current_seq+=parent_node->get_outcome_name() + " ";
		
		if(count < n){
			if(parent_node->has_child_node == true){
				for(int i = 0; i < parent_node->get_child_node_vector()->size(); i++){
					outcome_probailities_at(parent_node->get_child_node_vector()->at(i),current_seq, carrying_p, n, N, count+1);
				}
			}else{
				cout << current_seq << " " << carrying_p << endl;
			}
		}else{
			cout << current_seq << " " << carrying_p << endl;
		}
	}
}