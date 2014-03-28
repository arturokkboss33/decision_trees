/*
 * =====================================================================================
 *
 *       Filename:  dectree_test.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/18/2014 01:28:12 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "dectree_class.h"
#include "dectree_bst.h"

#include <iostream>


int main ( int argc, char *argv[] )
{
	Dectree_class dectree;
	double hgoal = 0.;
	Dectree_BST dbst;
	std::vector<bool> results;
	std::string train_dir = "TrainSets/";
	std::string test_dir = "TestSets/";
	std::string train_ext = ".train";
	std::string test_ext = ".test";
	std::string train_file = train_dir+argv[1]+train_ext;	
	std::string test_file = test_dir+argv[1]+test_ext;	
	
	//Load training set
	dectree.load_trainset(train_file);
	dectree.print_trainset();
	//Compute entropy of overall classification
	dectree.set_hgoal();
	hgoal = dectree.get_hgoal();
	std::cout << "Goal entropy: " << hgoal << std::endl;
	//Build decision tree from the examples	
	dbst.root = dectree.set_dectree();
	dbst.postOrder(dbst.root);
	
	std::cout << std::endl;

	//results
	results = dectree.test_cases(test_file);
	std::cout << "\% error: " << dectree.get_per_error() << std::endl;
	for(std::vector<bool>::iterator it = results.begin(); it != results.end(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	return 0;
}				/* ----------  end of function main  ---------- */
