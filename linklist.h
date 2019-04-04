// header file for link list (LIST) and its node class (NODE) declaration

#ifndef	LINKLIST_H
#define LINKLIST_H


// header files
#include <stdio.h>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>
#include "codelist.h"
#include "hufftree.h"

//-------------------------------------------------------------------------------------------
class NODE {
	public:
		NODE();					    // constructor

		void edit_tree(HUFF_TREE);	// tree modifier
		HUFF_TREE tree_is() const;	// tree observer

		void edit_link(NODE*);		// link modifier
		NODE* link_is() const;		// link observer

		void edit_code(CODE_LIST);	// code_list modifier
		CODE_LIST code_is() const;	// code_list observer

		NODE* get_next(int) const;	// traverse LIST

		void output_bits(ofstream&,int&,unsigned char&);//displays all bits in the code

		void* operator new(size_t);	// overload new operator for freelist
		void operator delete(void*);//  overload delete operator for freelist
		void operator=(NODE*);		// equal operator
	
	private:
		static NODE* freelist;		// head of the freelist

		HUFF_TREE subtree;			// subtree stored in NODE
		NODE* link;					// link to next node

		CODE_LIST code_list;		// binary code value stored in a linked list
};
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
class LIST {
	public:
		LIST();							// constructor
		bool empty() const;				// checks if LIST is empty

		void edit_head(NODE*);			// head modifier
		NODE* head_is() const;			// head observer

		void output_all_bits(ofstream&);// output all bits in code in order
		int size() const;				// determines size of LIST
		void append(NODE*&);			// append NODE to the end of the LIST
		void insert(NODE*&);			// insert NODE in correct ordered position in LIST
		void insert_top(NODE*&);		// insert NODE at top of LIST

		HUFF_TREE remove_top();			// removes the top NODE of the LIST

		void operator=(LIST);			// equal operator

	private:
		NODE*	head;					// head pointer
};
//-------------------------------------------------------------------------------------------

#endif