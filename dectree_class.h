/* =============================================================*/
/* --- DECISION TREES- DECISION TREE CLASS HEADER FILE       ---*/
/* FILENAME: dectree_class.cpp 
 *
 * DESCRIPTION: source file for the struct object for a node
 * in a binary search algorithm.
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

#include "dectree_node.h"
#include "dectree_bst.h"

#include <string>
#include <vector>

class Dectree_class
{
	public:
		Dectree_class();
		void load_trainset(std::string);
		void set_hgoal();
		double get_hgoal();
		dectree_node* set_dectree();
		dectree_node* learn_dectree(std::vector< std::vector<char> >, std::vector< std::vector<char> >, std::vector<int>);
		std::vector<bool> test_cases(std::string);
		double get_per_error();	

		dectree_node* dbst;

		//auxiliary methods
		void print_trainset();

	private:
		std::vector< std::vector<char> > training_set;
		double hgoal; //goal entropy (how uncertain are we of the classification)
		std::vector<int> attributes;
		double hcompute(std::vector< std::vector<char> >);
		std::vector<int> max_gain_atr(std::vector<int>, std::vector< std::vector<char> >, double);
		int plurality(std::vector< std::vector<char> >);
		bool check_classif(std::vector< std::vector<char> >);
		double per_error;

};

#endif 
