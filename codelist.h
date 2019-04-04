// This is the header file for the CODE_LIST and CODE_NODE ADTs

#ifndef CODELIST_H
#define CODELIST_H

//header files

#include <stdio.h>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

//-------------------------------------------------------------------------------------------
class CODE_NODE 
{
	public:
		CODE_NODE();					// constructor

		void edit_link(CODE_NODE*);		// link modifier
		CODE_NODE* link_is() const;		// link observer

		void edit_code(unsigned char);	// code modifier
		unsigned char code_is() const;	// code observer

		void edit_code_size(int);		// code_size modifier
		int code_size_is() const;		// code_size observer

		CODE_NODE* get_next(int) const;	// traverse LIST

		void* operator new(size_t);		// overload new operator for freelist
		void operator delete(void*);	//  overload delete operator for freelist
		void operator=(CODE_NODE*);		// equal operator
	
	private:
		static CODE_NODE* freelist;		// head of the freelist

		CODE_NODE* link;				// link to next CODE_NODE in CODE_LIST

		unsigned char code;				// 8 bit space for code value

		int	code_size;					// number of code bits in code_byte
		
};
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
class CODE_LIST 
{
	public:
		CODE_LIST();						// constructor
		bool empty() const;					// checks if CODE_LIST is empty

		void edit_head(CODE_NODE*);			// head modifier
		CODE_NODE* head_is() const;			// head observer

		void edit_bit_size(int);			// bit_size modifier
		int bit_size_is() const;			// bit_size observer

		int total_bit_size() const;			// calculates total number of bits in the code

		int size() const;					// determines size of CODE_LIST
		void append(CODE_NODE*&);			// append CODE_NODE to the end of the CODE_LIST
		void remove();						// removes the last CODE_NODE in the CODE_LIST

		void operator=(CODE_LIST);			// equal operator

	private:
		CODE_NODE*	head;					// head pointer
		int bit_size;						// number of code bits in the whole code
};
//-------------------------------------------------------------------------------------------

#endif