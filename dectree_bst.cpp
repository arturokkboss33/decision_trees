/*
 * =i====================================================================================
 *
 *       Filename:  bst.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/15/2014 07:14:38 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *i
 * =====================================================================================
 */

#include <iostream>
#include <sstream>

#include "dectree_bst.h"

Dectree_BST::Dectree_BST()
{
	root = NULL;
}

//insert is not the same as in a common binary balance tree
//the user states in which branch (left or right) the node is going to be inserted
void Dectree_BST::insert_node(dectree_node** rootPtr, std::string type, int attribute, int classification)
{
	//check if the tree is empty
	if(*rootPtr == NULL)
	{
		*rootPtr = create_node(type, attribute, classification);
	}
	/*
	else
	{
		if(branch == 0)
		{
			insert_node(&((*rootPtr)->f), node, branch);
		}
		else
		{
			insert_node(&((*rootPtr)->t), node, branch);
		}	
	}*/
}


bool Dectree_BST::search_node(dectree_node* root, int attribute_id)
{
	
	if(root != NULL)
	{
		if((root->type).compare("split"))
			if (attribute_id == root->attribute_id)
				return true;
		
		search_node(root->f, attribute_id);
		search_node(root->t, attribute_id);
				
	}
	else
		return false;

	return false; //added just to make clear that something is "return" for the compiler

}


void Dectree_BST::postOrder(dectree_node* ptr)
{
	if(ptr != NULL)
	{
		postOrder(ptr->f);
		postOrder(ptr->t);

		if(!((ptr->type).compare("terminal")))
			std::cout << "Terminal: " << ptr->output_id << " ";
		else
			std::cout << "Split: " << ptr->attribute_id << " ";
	}
}


dectree_node* Dectree_BST::create_node(std::string type, int attribute, int classification)
{
	dectree_node* new_node = new dectree_node();
	new_node->type = type;
	std::stringstream ss;

	if(!(type.compare("split")))
	{
		ss << attribute;
		new_node->attribute = ss.str();
		new_node->attribute_id = attribute;
		new_node->output = "none";
		new_node->output_id = -1;

		ss << "";
		ss.clear();

		std::cout << "Node split inserted" << std::endl;
	}
	else
	{
		
		ss << classification;
		new_node->attribute = "none";
		new_node->attribute_id = -1;
		new_node->output = ss.str();
		new_node->output_id = classification;

		ss << "";
		ss.clear();

		std::cout << "Node terminal inserted" << std::endl;
	}
	new_node->f = new_node->t = NULL;

	return new_node;
}

