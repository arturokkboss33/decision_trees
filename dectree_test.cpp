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
//#include "dectree_bst.h"

#include <iostream>


int main ( int argc, char *argv[] )
{
	Dectree_class dectree;
	Dectree_BST dbst;
	double hgoal = 0.;
	std::vector<bool> results; //save list of results
	//variables to parse the console input and search te trainig and test files
	std::string train_dir = "TrainSets/";
	std::string test_dir = "TestSets/";
	std::string train_ext = ".train";
	std::string test_ext = ".test";
	std::string train_file = train_dir+argv[1]+train_ext;	
	std::string test_file = test_dir+argv[1]+test_ext;	
	
	//Load training set
	dectree.load_trainset(train_file);
	//dectree.print_trainset();
	//Compute entropy of overall classification
	dectree.set_hgoal();
	hgoal = dectree.get_hgoal();
	//std::cout << "\nGoal entropy: " << hgoal << std::endl;
	//Build decision tree from the examples	
	dectree.set_dectree();
	dbst.set_root(dectree.get_dectree());
	//traverse the dectree inOrder and postOrder to define its structure
	std::cout<< "\ninOrder traversal: " << std::endl;
	dbst.inOrder(dectree.get_dectree());
	std::cout << std::endl;
	std::cout<< "\npostOrder traversal: " << std::endl;
	dbst.postOrder(dectree.get_dectree());
	
	std::cout << std::endl;

	//print results
	results = dectree.test_cases(test_file);
	std::cout << "\n \% error: " << dectree.get_per_error() << std::endl;
	/*//for debugging
	std::cout << "\nTruth table for every test case" << std::endl;
	for(std::vector<bool>::iterator it = results.begin(); it != results.end(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	*/

	return 0;
}				/* ----------  end of function main  ---------- */
