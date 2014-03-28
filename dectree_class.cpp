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
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>

Dectree_class::Dectree_class()
{
	hgoal = 0.;
	per_error = 0.;
	dbst = NULL;
}

void Dectree_class::load_trainset(std::string filename)
{

	//Variables for parsing the data file
	std::string line;
	std::stringstream parse;
	int ssize = 100; //establish a buffer size to store attribute values, which for binary classification string are no bigger than 1
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

			parse << ""; //safety measure to erase previous contents
			parse.clear(); //clear flags to be able to read from it again

			training_set.push_back(tmpcase);
			tmpcase.clear(); 
		}
	}

	//set a vector of ints identifying each attribute
	tmpcase = training_set.at(0);
	int j = 0;
	for(std::vector<int>::size_type i = 1; i != tmpcase.size(); i++)
	{
		attributes.push_back(++j);
	}


}


double Dectree_class::hcompute(std::vector< std::vector<char> > ex)
{
	int pos_count = 0;
	int neg_count = 0;
	double p_pos = 0;
	double entropy = 0.;
	std::vector<char> tmp;

	for(std::vector< std::vector<char> >::iterator it = ex.begin(); it != ex.end(); ++it)
	{
		tmp = *it;
		//std::cout << tmp.at(0) << " ";
		if(tmp.at(0) == '1') //make notation
			pos_count++;
		else
			neg_count++;
	}
	std::cout << std::endl;
	tmp.clear();
	std::cout << "Positive and negative counts" << std::endl;
	std::cout << "* " << pos_count << " * " << neg_count << std::endl;
	p_pos = (double)pos_count/(pos_count+neg_count);
	if(p_pos == 0 || p_pos == 1)
		entropy = 0;
	else
		entropy = -1*(p_pos*log2(p_pos)+(1-p_pos)*log2(1-p_pos));

	return entropy;		
}

void Dectree_class::set_hgoal()
{
	hgoal = hcompute(training_set);
}

double Dectree_class::get_hgoal()
{
	return hgoal;
}

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


dectree_node* Dectree_class::set_dectree()
{
	dbst = learn_dectree(training_set,training_set, attributes);
	return dbst;
}

dectree_node* Dectree_class::learn_dectree(std::vector< std::vector<char> > p_ex, std::vector< std::vector<char> > ex, std::vector<int> atr)
{
	double h_curr = 0.;
	std::vector<int> best_atr;
	std::vector< std::vector<char> > new_ex1, new_ex2;
	std::vector<char> tmp_ex;
	Dectree_BST dectree;
	
	//check the base cases to get of the recursion		
	if(ex.empty())
	{
		std::cout << "Case 1" << std::endl;
		dectree.insert_node(&(dectree.root), "terminal", -1, plurality(p_ex));
		return dectree.root;
	}
	else if(check_classif(ex))
	{
		std::cout << "Case 2" << std::endl;
		std::vector< std::vector<char> >::size_type i = 0;
		if(ex[i].at(0)=='0')
			dectree.insert_node(&(dectree.root), "terminal", -1,0);
		else
			dectree.insert_node(&(dectree.root), "terminal", -1,1);
		return dectree.root;

	}
	else if(atr.empty())
	{
		std::cout << "Case 3" << std::endl;
		dectree.insert_node(&(dectree.root), "terminal", -1, plurality(ex));
		return dectree.root;

	}

	//find the atrribute with the highest information gain
	h_curr = hcompute(ex);
	std::cout << "Current entropy: " << h_curr << std::endl;	
	best_atr = max_gain_atr(atr, ex, h_curr);
	std::cout << "Best attribute: " << best_atr.at(0) << std::endl;

	//create a tree with root test with best attribute
	//Dectree_BST dectree;
	dectree.insert_node(&(dectree.root),"split", best_atr.at(0), -1);	
	std::cout << "Tree was splitted" << std::endl;
	//iterate through the attribute classes and call this fcn recursively
	//select the appropriate subset of examples	
	for(std::vector< std::vector<char> >::iterator it = ex.begin(); it != ex.end(); it++)
	{
		tmp_ex = *it;
		if(tmp_ex.at(best_atr.at(0))=='0')
			new_ex1.push_back(tmp_ex);
		else
			new_ex2.push_back(tmp_ex);
	}

	std::cout << "Examples subset created" << std::endl;	

	atr.erase(atr.begin()+best_atr.at(1));
	(dectree.root)->f = learn_dectree(ex,new_ex1,atr);	
	(dectree.root)->t = learn_dectree(ex,new_ex2,atr);

	std::cout << "Learning done" << std::endl;

	return dectree.root;	
}

std::vector<int> Dectree_class::max_gain_atr(std::vector<int> atr, std::vector< std::vector<char> > ex, double h_bef_split)
{

	//one vector of frequencies for each class of the attribute
	//convenient to genralize for any number of classes
	std::vector<int> atr_class1(2,0);
	std::vector<int> atr_class2(2,0);
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
	

	for(std::vector<int>::iterator it_atr = atr.begin(); it_atr != atr.end(); ++it_atr)
	{	
		idx_atr = *it_atr;
		std::cout << "++ " << idx_atr << std::endl;
		
		for(std::vector< std::vector<char> >::iterator it = ex.begin(); it != ex.end(); ++it)
		{
			tmp = *it;
			//std::cout << "xx " << tmp.size() << std::endl;
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
		atr_info_gain = h_bef_split - (h1+h2);

		std::cout << "h1: " << h1 << std::endl;
		std::cout << "h2: " << h2 << std::endl;
		std::cout << "n1: " << atr_class1.at(0) << std::endl;
		std::cout << "p1: " << atr_class1.at(1) << std::endl;
		std::cout << "n2: " << atr_class2.at(0) << std::endl;
		std::cout << "p2: " << atr_class2.at(1) << std::endl;
		std::cout << "xx " << atr_info_gain << std::endl;

		if(atr_info_gain > max_gain)
		{
			best_atr = idx_atr;
			best_atr_pos = count;
			max_gain = atr_info_gain;
		}
		count++;

		atr_class1.assign(2,0);
		atr_class2.assign(2,0);
	}

	best_atr_info.push_back(best_atr);
	best_atr_info.push_back(best_atr_pos);
	return best_atr_info;
}

int Dectree_class::plurality(std::vector< std::vector<char> > ex)
{
	std::vector<char> tmp;
	int pos_count = 0;
	int neg_count = 0;

	for(std::vector< std::vector<char> >::iterator it = ex.begin(); it != ex.end(); ++it)
	{
		tmp = *it;
		
		if(tmp.at(0)=='0')
			neg_count++;
		else
			pos_count++;
	}

	if(pos_count > neg_count)
		return 1;
	else if(pos_count < neg_count)
		return 0;
	else
	{
		srand(time(NULL));
		return (rand()%2+1);
	}
		
}

bool Dectree_class::check_classif(std::vector< std::vector<char> > ex)
{
	std::vector<char> tmp;
	int pos_count = 0;
	int neg_count = 0;

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

std::vector<bool> Dectree_class::test_cases(std::string test_file)
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

	std::ifstream dataset_file(test_file.c_str(), std::ios::in);

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
			std::cout << "***Case***" << std::endl;
			//for(std::vector<char>::iterator it = tmpcase.begin(); it != tmpcase.end(); ++it)
			//{ 	std::cout << *it << " " << std::endl;}
			//std::cout << std::endl;


			while(tmp_node != NULL && !(tmpcase.empty()))
			{

				if(!(tmp_node->type.compare("terminal")))
				{
					parse << tmp_node->output_id;	
					std::cout << "decide" << std::endl;
					std::cout << "++" << tmp_node->output_id << std::endl;
					std::cout << "++" << tmpcase.at(0)<< std::endl;
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
						std::cout << "go left" << std::endl;
						tmp_node = tmp_node->f;
					}
					else
					{
						std::cout << "go right" << std::endl;
						tmp_node = tmp_node->t;
					}
				}
			}

			//training_set.push_back(tmpcase);
			tmpcase.clear(); 
		}
	}

	per_error = (double)bad_classif / (good_classif+bad_classif);
	return results;
}

double Dectree_class::get_per_error()
{
	return per_error;
}
