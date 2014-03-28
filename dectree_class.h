/* =============================================================*/
/* --- DECISION TREES- DECISION TREE CLASS HEADER FILE       ---*/
/* FILENAME: dectree_class.cpp 
 *
 * DESCRIPTION: header file for the struct object which implements
 * a decision tree learning algorithm.
 *
 * VERSION: 1.0
 *
 * CREATED: 03/18/2013
 *
 * COMPILER: g++
 *
 * AUTHOR: ARTURO GOMEZ CHAVEZ
 * ALIAS: BOSSLEGEND33
 * 
 * ============================================================ */


#ifndef DECTREE_CLASS_H
#define DECTREE_CLASS_H

#include "dectree_bst.h"

#include <string>
#include <vector>

class Dectree_class
{
	public:
		Dectree_class();
		//---get and set methods---
		void set_hgoal(); //entropy of the training set
		double get_hgoal();
		void set_dectree(); //learn decision tree
		dectree_node* get_dectree();
		double get_per_error();	

		//---auxiliary methods---
		void load_trainset(std::string filename); //save training set in a container
		void print_trainset();//print training set only for debugging
		std::vector<bool> test_cases(std::string filename); //test cases 

	private:
		//---data members---
		dectree_node* dbst; //decision tree root
		std::vector< std::vector<char> > training_set; //container for the training set file
		double hgoal; //goal entropy (how uncertain are we of the classification)
		std::vector<int> attributes;
		double per_error; //percentage of erros made in the test case
		
		//---auxiliary functions---	
		void set_attributes(); //create a list of attributes ids
		dectree_node* learn_dectree(std::vector< std::vector<char> >, std::vector< std::vector<char> >, std::vector<int>);//learning decision tree algorithm
		double hcompute(std::vector< std::vector<char> >);//compute entropy of examples' sets
		std::vector<int> max_gain_atr(std::vector<int>, std::vector< std::vector<char> >, double);//see which attribute has more information gain
		int plurality(std::vector< std::vector<char> >);//majority count/vote
		bool check_classif(std::vector< std::vector<char> >);//check if all the examples have the same classification
};

#endif 
