// This is the header file for the huffman coding tree
// ADTs: HUFF_NODE, LEAF_NODE, IN_NODE, HUFF_TREE

#ifndef HUFFTREE_H
#define HUFFTREE_H



// header files

#include <stdio.h>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;



//-------------------------------------------------------------------------------------------
// abstract base node class
class HUFF_NODE
{
	public:
		// observers
		virtual int freq_is() = 0;
		virtual unsigned char value_is() = 0;
		virtual bool is_leaf() = 0;
		virtual HUFF_NODE* left_is() const = 0;
		virtual HUFF_NODE* right_is() const = 0;

		// manipulators
		virtual void edit_freq(int) = 0;	
		virtual void edit_right(HUFF_NODE*) = 0;
		virtual void edit_left(HUFF_NODE*) = 0;
};
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// leaf node subclass
class LEAF_NODE : public HUFF_NODE
{
	public:
		// constructors
		LEAF_NODE(int,unsigned char);	// constructor

		// observers
		int freq_is();					// returns frequency
		unsigned char value_is();		// returns binary value
		bool is_leaf();					// returns true
		HUFF_NODE* left_is() const;
		HUFF_NODE* right_is() const;

		// manipulators
		void edit_freq(int);			// edits the frequency value		
		void edit_value(unsigned char);	// edits the binary value of the node
		void edit_right(HUFF_NODE*);	// does nothing
		void edit_left(HUFF_NODE*);		// does nothing

	private:
		unsigned char value;			// binary value of character
		int freq;						// frequency of the character
};
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// internal node subclass
class IN_NODE : public HUFF_NODE
{
	public:
		// constructors
		IN_NODE(HUFF_NODE*,HUFF_NODE*);		// constructor
		IN_NODE(HUFF_NODE*,HUFF_NODE*,int);	// constructor

		// observers
		int freq_is();						// returns frequency
		unsigned char value_is();			// returns binary value
		bool is_leaf();						// returns false
		HUFF_NODE* left_is() const;		
		HUFF_NODE* right_is() const;

		// manipulators
		void edit_freq(int);				// edits the frequency value
		void edit_left(HUFF_NODE*);		
		void edit_right(HUFF_NODE*);

	private:
		HUFF_NODE*	left;					// left child
		HUFF_NODE*	right;					// right child
		
		int	freq;							// frequency of the combination
};
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// huffman tree class
class HUFF_TREE
{
	public:
		// observers
		HUFF_NODE* root_is();		// returns the root pointer;
		int freq_is();				// returns the frequency of the entire tree

		// manipulators
		void edit_root(HUFF_NODE*); // edit the root value

		// operators
		void operator=(HUFF_TREE);	// overload equal operator	

	private:
		HUFF_NODE*	root;

};
//-------------------------------------------------------------------------------------------
 
#endif