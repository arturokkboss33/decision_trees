/*
 * =====================================================================================
 *
 *       Filename:  dectree_class.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/18/2014 08:50:55 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "dectree_class.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <math.h>
#include <stdlib.h>
#include <time.h>

//can make a initializer list for a default constructor, but this
//gives the possibility to the client to change these default
//values
Dectree_class::Dectree_class()
{
	hgoal = 0.;
	per_error = 0.;
	dbst = NULL;
}

//compute the overall entropy of the training set
void Dectree_class::set_hgoal()
{
	hgoal = hcompute(training_set);
}
double Dectree_class::get_hgoal()
{return hgoal;}

//learn the decision tree based on the training example
void Dectree_class::set_dectree()
{
	dbst = learn_dectree(training_set,training_set, attributes);
}
dectree_node* Dectree_class::get_dectree()
{ return dbst;}

double Dectree_class::get_per_error()
{
	return per_error;
}

//read training data set and store it in a vector container
void Dectree_class::load_trainset(std::string filename)
{

	//Variables for parsing the data file
	std::string line;
	std::stringstream parse;
	int ssize = 100; //establish a buffer size to store attribute values,
			 //which for binary classification string are no bigger than 1
	char c[ssize];
	char delimiter = ',';

	//Variables to store the values in the data file
	std::vector<char> tmpcase;

	std::ifstream dataset_file(filename.c_str(), std::ios::in);

	if(!dataset_file)
	{
		std::cerr << "Cannot load training set file" << std::endl;
	}
	else
	{
		while( (getline(dataset_file, line))!= NULL )
		{
			parse << line;

			while( parse.getline(c,ssize,delimiter) )
				tmpcase.push_back(*c);

			parse.str(""); //safety measure to erase previous contents
			parse.clear(); //clear flags to be able to read from it again

			training_set.push_back(tmpcase);
			tmpcase.clear(); 
		}
	}


}

//auxiliary function to see if the training data set was parsed correctly
void Dectree_class::print_trainset()
{

	std::vector<char> tmp;

	for(std::vector< std::vector<char> >::iterator it = training_set.begin(); it != training_set.end(); ++it)
	{
		tmp = *it;

		for(std::vector<char>::iterator it2 = tmp.begin(); it2 != tmp.end(); ++it2)
		{
			std::cout << *it2 << " ";
		}
		std::cout << std::endl;
	}

	tmp.clear();	
}

//this method should be called after training the decision tree
//reads a file with case examples and evaluate them through the
//decision tree just learned. Then compares the predicted classification
//with the real one to compute a percentage error
//It returns a vector of booleans 
std::vector<bool> Dectree_class::test_cases(std::string filename)
{	

	//Variables for parsing the data file
	std::string line;
	std::stringstream parse;
	int ssize = 100; //establish a buffer size to store attribute values, which for binary classification string are no bigger than 1
	char c[ssize];
	char delimiter = ',';

	//Variables to store the values in the data file
	std::vector<char> tmpcase;

	//Variables to traverse the decision tree and store results
	dectree_node* tmp_node;
	std::vector<bool> results;
	int good_classif = 0;
	int bad_classif = 0; 

	std::ifstream dataset_file(filename.c_str(), std::ios::in);

	if(!dataset_file)
	{
		std::cerr << "Cannot load test file" << std::endl;
	}
	else
	{
		while( (getline(dataset_file, line))!= NULL )
		{
			parse << line;

			while( parse.getline(c,ssize,delimiter) )
				tmpcase.push_back(*c);

			parse.str(""); //safety measure to erase previous contents
			parse.clear(); //clear flags to be able to read from it again

			//traverse the decision tree and evaluate the parsed case
			tmp_node = dbst;
			//std::cout << "***Case***" << std::endl;


			while(tmp_node != NULL && !(tmpcase.empty()))
			{

				if(!(tmp_node->type.compare("terminal")))
				{
					parse << tmp_node->output_id;	
					
					/*				
					std::cout << "decide" << std::endl;
					std::cout << "++" << tmp_node->output_id << std::endl;
					std::cout << "++" << tmpcase.at(0)<< std::endl;
					*/

					if(*(parse.str().c_str())==tmpcase.at(0))
					{
						results.push_back(true);
						good_classif++;
					}
					else
					{
						results.push_back(false);
						bad_classif++;
					}
					
					parse.str(""); 
					parse.clear();
					tmp_node = NULL;
				}
				else
				{
					if(tmpcase.at(tmp_node->attribute_id)=='0')
					{
						//std::cout << "go left" << std::endl;
						tmp_node = tmp_node->f;
					}
					else
					{
						//std::cout << "go right" << std::endl;
						tmp_node = tmp_node->t;
					}
				}
			}

			tmpcase.clear(); 
		}
	}

	per_error = (double)bad_classif / (good_classif+bad_classif);
	return results;
}

//set a vector of ints identifying each attribute
void Dectree_class::set_attributes()
{
	std::vector<char> tmpcase;
	tmpcase = training_set.at(0);
	int j = 0;
	for(std::vector<int>::size_type i = 1; i != tmpcase.size(); i++)
	{
		attributes.push_back(++j);
	}
}


//algorithm to learn a decision tree
//it returns the root of the decision tree (or one of its subtrees in the recursion) 
//inputs: parent examples, current examples, list of remaining attributes
dectree_node* Dectree_class::learn_dectree(std::vector< std::vector<char> > p_ex, std::vector< std::vector<char> > ex, std::vector<int> atr)
{
	//auxiliary variables
	double h_curr = 0.;
	std::vector<int> best_atr;
	std::vector< std::vector<char> > new_ex1, new_ex2;
	std::vector<char> tmp_ex;
	Dectree_BST dectree; //decision tree structure
	dectree_node* dectree_root = dectree.get_root();

	//check the base cases to get out of the recursion
	//if there are no more examples		
	if(ex.empty())
	{
		//std::cout << "Case 1" << std::endl;
		dectree.insert_node(&dectree_root, "terminal", -1, plurality(p_ex));
		return dectree_root;
	}
	//if all examples have the same classification
	else if(check_classif(ex))
	{
		//std::cout << "Case 2" << std::endl;
		std::vector< std::vector<char> >::size_type i = 0;
		if(ex[i].at(0)=='0')
			dectree.insert_node(&dectree_root, "terminal", -1,0);
		else
			dectree.insert_node(&dectree_root, "terminal", -1,1);
		return dectree_root;

	}
	//if there are no more attributes 
	else if(atr.empty())
	{
		//std::cout << "Case 3" << std::endl;
		dectree.insert_node(&dectree_root, "terminal", -1, plurality(ex));
		return dectree_root;

	}

	//find the atrribute with the highest information gain
	h_curr = hcompute(ex);
	//std::cout << "Current entropy: " << h_curr << std::endl;	
	best_atr = max_gain_atr(atr, ex, h_curr);
	//std::cout << "Best attribute: " << best_atr.at(0) << std::endl;

	//create a tree with the best attribute as root
	dectree.insert_node(&dectree_root,"split", best_atr.at(0), -1);	
	//std::cout << "Tree was splitted" << std::endl;
	
	//iterate through the attribute classes and call this fcn recursively
	//select the appropriate subset of examples according to the attribute	
	for(std::vector< std::vector<char> >::iterator it = ex.begin(); it != ex.end(); it++)
	{
		tmp_ex = *it;
		if(tmp_ex.at(best_atr.at(0))=='0')
			new_ex1.push_back(tmp_ex);
		else
			new_ex2.push_back(tmp_ex);
	}

	//std::cout << "Examples subset created" << std::endl;	

	//erase the attribute used and call the fcm recursively
	//for each of the classes of the attribute and the set
	//of examples created
	atr.erase(atr.begin()+best_atr.at(1));
	(dectree.get_root())->f = learn_dectree(ex,new_ex1,atr);	
	(dectree.get_root())->t = learn_dectree(ex,new_ex2,atr);

	//std::cout << "Learning done" << std::endl;

	return dectree_root;	
}

//compute total entropy based on current examples
//return entropy and inputs: current examples
double Dectree_class::hcompute(std::vector< std::vector<char> > ex)
{
	int pos_count = 0;
	int neg_count = 0;
	double p_pos = 0;
	double entropy = 0.;
	std::vector<char> tmp;

	//count number of positive and negative examples
	for(std::vector< std::vector<char> >::iterator it = ex.begin(); it != ex.end(); ++it)
	{
		tmp = *it;
		if(tmp.at(0) == '1') 
			pos_count++;
		else
			neg_count++;
	}
	tmp.clear();

	/* 
	//Debugging print functions, uncomment if necessary
	std::cout << std::endl;
	std::cout << "Positive and negative counts" << std::endl;
	std::cout << "* " << pos_count << " * " << neg_count << std::endl;
	*/

	//compute entropy
	p_pos = (double)pos_count/(pos_count+neg_count);
	if(p_pos == 0 || p_pos == 1)
		entropy = 0;
	else
		entropy = -1*(p_pos*log2(p_pos)+(1-p_pos)*log2(1-p_pos));

	return entropy;		
}

//return a 2-tuple indicating the best atr id and the its position of the list of all attributes (this position keeps changing as the list change size) 
//inputs: list of remaining attriutes, current examples, entropy of the ancestor node
std::vector<int> Dectree_class::max_gain_atr(std::vector<int> atr, std::vector< std::vector<char> > ex, double h_bef_split)
{

	//one vector of frequencies for each class of the attribute
	//convenient to genralize for any number of classes
	std::vector<int> atr_class1(2,0);
	std::vector<int> atr_class2(2,0);

	//auxiliary variables to make computations step by step
	double h1 = 0.;
	double h2 = 0.;
	double p_pos = 0.;
	double atr_info_gain = 0.;
	std::vector<char> tmp;

	//variables to decide which atr to choose
	double max_gain = -1.; //info gain lies between 0-h(goal)
	int best_atr = 0;
	int idx_atr = 0;
	int best_atr_pos = 0;
	int count = 0;
	std::vector<int> best_atr_info;
	
	//iterate through all attributes
	for(std::vector<int>::iterator it_atr = atr.begin(); it_atr != atr.end(); ++it_atr)
	{	
		idx_atr = *it_atr;
		//std::cout << "++ " << idx_atr << std::endl;
		
		//iterate through all examples
		for(std::vector< std::vector<char> >::iterator it = ex.begin(); it != ex.end(); ++it)
		{
			tmp = *it;
			//std::cout << "xx " << tmp.size() << std::endl;
			//count according to the atr class and the 
			//classification of the case it belongs
			if(tmp.at(0) == '0')
			{
				if(tmp.at(idx_atr) == '0')
					atr_class1.at(0) += 1;
				else
					atr_class2.at(0) += 1;
			} 
			else
			{
				if(tmp.at(idx_atr) == '0')
					atr_class1.at(1) += 1;
				else
					atr_class2.at(1) += 1;
			}
		}
		tmp.clear();

		//compoute entropy and information gain
		//be careful to consider the limit values of the math fcns
		p_pos = (double)atr_class1.at(1)/(atr_class1.at(0)+atr_class1.at(1));
		
		if(p_pos == 0 || p_pos == 1)
			h1 = 0;
		else
		{
			h1 = -1*(p_pos*log2(p_pos)+(1-p_pos)*log2(1-p_pos));
			h1 *= (double)(atr_class1.at(0)+atr_class1.at(1))/ex.size();
		}
		p_pos = (double)atr_class2.at(1)/(atr_class2.at(0)+atr_class2.at(1));
		std::cout << "ñññ " << p_pos << std::endl;
		if(p_pos == 0 || p_pos == 1)
			h2 = 0;
		else
		{
			h2 = -1*(p_pos*log2(p_pos)+(1-p_pos)*log2(1-p_pos));
			std::cout << "ñññ " << h2 << std::endl;
			h2 *= (double)(atr_class2.at(0)+atr_class2.at(1))/ex.size();
		}
		//info gain
		atr_info_gain = h_bef_split - (h1+h2);

		/* 
		//Debugging print functions, uncomment if necessary
		std::cout << "h1: " << h1 << std::endl;
		std::cout << "h2: " << h2 << std::endl;
		std::cout << "n1: " << atr_class1.at(0) << std::endl;
		std::cout << "p1: " << atr_class1.at(1) << std::endl;
		std::cout << "n2: " << atr_class2.at(0) << std::endl;
		std::cout << "p2: " << atr_class2.at(1) << std::endl;
		std::cout << "xx " << atr_info_gain << std::endl;
		*/

		//Decide if it is the best attribute
		if(atr_info_gain > max_gain)
		{
			best_atr = idx_atr;
			best_atr_pos = count;
			max_gain = atr_info_gain;
		}
		count++;
		
		//Reset the counters
		atr_class1.assign(2,0);
		atr_class2.assign(2,0);
	}

	//return the best atr and its position in the list
	best_atr_info.push_back(best_atr);
	best_atr_info.push_back(best_atr_pos);
	return best_atr_info;
}

//Acoording to the basic cases of the learning algorithm
//To decide a classification output it's necessary to do a majority vote
//according to the remaining training examples
int Dectree_class::plurality(std::vector< std::vector<char> > ex)
{
	std::vector<char> tmp;
	int pos_count = 0;
	int neg_count = 0;

	//do the vote counting
	for(std::vector< std::vector<char> >::iterator it = ex.begin(); it != ex.end(); ++it)
	{
		tmp = *it;
		
		if(tmp.at(0)=='0')
			neg_count++;
		else
			pos_count++;
	}

	//return the classification with more number of votes
	if(pos_count > neg_count)
		return 1;
	else if(pos_count < neg_count)
		return 0;
	//if it's a tie, decide randomly
	else
	{
		srand(time(NULL));
		return (rand()%2+1);
	}
		
}

//check if the remaining examples have the same classification
//if true, there is no necessity in continue the leaning process
//through the respective branch
bool Dectree_class::check_classif(std::vector< std::vector<char> > ex)
{
	std::vector<char> tmp;
	int pos_count = 0;
	int neg_count = 0;

	//count votes
	for(std::vector< std::vector<char> >::iterator it = ex.begin(); it != ex.end(); ++it)
	{
		tmp = *it;
		
		if(tmp.at(0)=='0')
			neg_count++;
		else
			pos_count++;
	}

	if(neg_count == 0 || pos_count == 0)
		return true;
	else
		return false;
}
