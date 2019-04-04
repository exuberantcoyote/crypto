// This is the definition file for the huffman coding tree ADT
// The classes include: HUFF_NODE, LEAF_NODE, IN_NODE, and HUFF_TREE

// header files

#include <stdio.h>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>
#include "hufftree.h"
using namespace std;


//-------------------------------------------------------------------------------------------
// LEAF_NODE class member functions


//-------------------------------------------------------------------------------------------
// LEAF_NODE constructor
//
LEAF_NODE::LEAF_NODE(int new_freq, unsigned char new_value)
{
	freq = new_freq;
	value = new_value;
} // end LEAF_NODE constructor
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// freq_is() returns the frequency of the LEAF_NODE
//
int LEAF_NODE::freq_is()
{	
	return freq;
} // end freq_is()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// value_is() returns the binary value of the LEAF_NODE
//
unsigned char LEAF_NODE::value_is()
{	
	return value;
} // end value_is()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// is_leaf() returns true for LEAF_NODE
//
bool LEAF_NODE::is_leaf()
{
	return true;
} // end is_leaf()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// left_is() returns the value of the left child (NULL)
//
HUFF_NODE* LEAF_NODE::left_is() const
{
	return NULL;
} // end left_is()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// right_is() returns the value of the right child (NULL)
//
HUFF_NODE* LEAF_NODE::right_is() const
{
	return NULL;
} // end right_is()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// edit_freq() sets the frequency value to new_freq
//
void LEAF_NODE::edit_freq(int new_freq)
{
	freq = new_freq;
} // end edit_freq()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// edit_value() sets the binary value to new_value
//
void LEAF_NODE::edit_value(unsigned char new_value)
{
	value = new_value;
} // end edit_value()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// edit_left() does nothing for LEAF_NODE
//
void LEAF_NODE::edit_left(HUFF_NODE* new_left)
{
	return;	 
} // end edit_left()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// edit_right() does nothing for LEAF_NODE
//
void LEAF_NODE::edit_right(HUFF_NODE* new_right)
{
	return;	
} // end edit_left()
//-------------------------------------------------------------------------------------------
// end LEAF_NODE class member functions
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
// IN_NODE class member functions


//-------------------------------------------------------------------------------------------
// IN_NODE constructor
//
IN_NODE::IN_NODE(HUFF_NODE* new_left, HUFF_NODE* new_right)
{
	right = new_right;
	left = new_left;
	
	// calculate total frequency of the internal node
	freq = right->freq_is() + left->freq_is();

} // end IN_NODE constructor
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// IN_NODE constructor
//
IN_NODE::IN_NODE(HUFF_NODE* new_left, HUFF_NODE* new_right, int new_freq)
{
	right = new_right;
	left = new_left;
	
	// calculate total frequency of the internal node
	freq = new_freq;

} // end IN_NODE constructor
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
// freq_is() returns the frequency of the IN_NODE
//
int IN_NODE::freq_is()
{	
	return freq;
} // end freq_is()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// value_is() does not do anything for IN_NODE
//
unsigned char IN_NODE::value_is()
{	
	return 0;
} // end value_is()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// is_leaf() returns false for IN_NODE
//
bool IN_NODE::is_leaf()
{
	return false;
} // end is_leaf()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// left_is() returns the value of the left child
//
HUFF_NODE* IN_NODE::left_is() const
{
	return left;
} // end left_is()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// right_is() returns the value of the right child
//
HUFF_NODE* IN_NODE::right_is() const
{
	return right;
} // end right_is()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// edit_freq() sets the value of freq to new_freq
//
void IN_NODE::edit_freq(int new_freq)
{
	freq = new_freq;
} // end edit_freq()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// edit_left() sets the value of the left child to new_left
//
void IN_NODE::edit_left(HUFF_NODE* new_left)
{
	left = new_left;
} // end edit_left()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// edit_right() sets the value of the right child to new_right
//
void IN_NODE::edit_right(HUFF_NODE* new_right)
{
	right = new_right;
} // end edit_left()
//-------------------------------------------------------------------------------------------
// end IN_NODE class member functions
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
// HUFF_TREE class member functions


//-------------------------------------------------------------------------------------------
// root_is() returns the root of the huffman tree
//
HUFF_NODE* HUFF_TREE::root_is()
{
	return root;
} // end root_is()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// freq_is() returns the total frequency of the huffman tree
//
int HUFF_TREE::freq_is()
{
	return root->freq_is();
} // freq_is()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// edit_root() sets the root value of the huffman tree to new_root
//
void HUFF_TREE::edit_root(HUFF_NODE* new_root)
{
	root = new_root;
} // end edit_root
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// overload "=" operator
//
void HUFF_TREE::operator=(HUFF_TREE other_tree)
{

	root = other_tree.root_is();

} // end operator=()
//-------------------------------------------------------------------------------------------
// end HUFF_TREE class member functions
//-------------------------------------------------------------------------------------------