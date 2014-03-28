/* =============================================================*/
/* --- DECISION TREES-BINARY TREE SEARCH CLASS HEADER   FILE ---*/
/* FILENAME: dectree_bst.cpp 
 *
 * DESCRIPTION: header file for the struct object of a binary
 * search tree, modified for its use in a decision tree 
 * learning algorithm.
 *
 * VERSION: 1.0
 *
 * CREATED: 03/16/2013
 *
 * COMPILER: g++
 *
 * AUTHOR: ARTURO GOMEZ CHAVEZ
 * ALIAS: BOSSLEGEND33
 * 
 * ============================================================ */

#ifndef DECTREE_BST_H
#define DECTREE_BST_H

#include "dectree_node.h"

#include <string>

class Dectree_BST
{

	public:
		Dectree_BST(dectree_node* rootPtr = NULL);
		//get and set methods
		dectree_node* get_root();
		void set_root(dectree_node* rootPtr);
		//insert a node based and complete its fields
		//based on it's type: terminal or splitting
		void insert_node(dectree_node** rootPtr, std::string type, int attribute, int classification); 
		//search based on attribute id
		bool search_node(dectree_node* root, int attribute_id);
		//common methods to traverse a binary tree
		void inOrder(dectree_node* root);
		void postOrder(dectree_node* root);

	private:
		//member data
		dectree_node* root;
		//auxiliary method to insert a node
		dectree_node* create_node(std::string, int, int);
};


#endif
