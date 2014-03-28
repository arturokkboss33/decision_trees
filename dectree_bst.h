/* =============================================================*/
/* --- DECISION TREES-BINARY TREE SEARCH CLASS HEADER   FILE ---*/
/* FILENAME: dectree_bst.cpp 
 *
 * DESCRIPTION: source file for the struct object for a node
 * in a binary search algorithm.
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
		Dectree_BST();
		dectree_node* root;
		void insert_node(dectree_node** , std::string, int, int); 
		bool search_node(dectree_node*, int);
		void postOrder(dectree_node*);

	private:
		dectree_node* create_node(std::string, int, int);
};


#endif
