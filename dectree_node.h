/* =============================================================*/
/* --- DECISION TREES - BINARY TREE NODE  HEADER FILE        ---*/
/* FILENAME: dectree_node.h 
 *
 * DESCRIPTION: header file for the struct object for a node
 * in a binary search tree for decision tree usage. 
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

//guard header file against multiple inclusions
#ifndef DECTREE_NODE_H
#define DECTREE_NODE_H

#include <vector>
#include <string>

struct dectree_node 
{
	std::string type; 	//terminal (states a classification), 
				//split node (it only states an attribute and splits the tree)
	std::string attribute; 	//if a label is needed to identify the attribute
	int attribute_id; 	//attribute number
	std::string output; 	//label for a classification
	int output_id; 		//classification id
	dectree_node* f; //stands for false case
	dectree_node* t; //stands for true case
};


#endif

