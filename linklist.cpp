// This is the definition file for the linked list ADT (LIST) and its node class (NODE)


// header files

#include <stdio.h>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>
#include "codelist.h"
#include "linklist.h"
#include "hufftree.h"


//-------------------------------------------------------------------------------------------
// freelist declaration
NODE* NODE::freelist = NULL;
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
// NODE class member functions


//-------------------------------------------------------------------------------------------
// NODE() default constructor
//
NODE::NODE() 
{
	link = NULL;
} // end NODE() constructor
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// edit_tree() sets the value of the subtree stored in the NODE to new_tree
//
void NODE::edit_tree(HUFF_TREE new_tree) 
{
	subtree = new_tree;
} // end edit_tree()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// tree_is() returns the value of the subtree stored in the NODE
//
HUFF_TREE NODE::tree_is() const 
{
	return subtree;
} // end tree_is()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// edit_link() changes the pointer to the next node in the LIST
//
void NODE::edit_link(NODE* new_link) 
{
	link = new_link;
} // end edit_link()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// link_is() returns the value of the pointer to the next node in the LIST
//
NODE* NODE::link_is() const 
{
	return link;
} // end link_is()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// edit_code() sets the binary code value to new_code
//
void NODE::edit_code(CODE_LIST new_code)
{
	code_list = new_code;
} // end edit_code()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// code_is() returns the binary code value
//
CODE_LIST NODE::code_is() const
{
	return code_list;
} // end code_is()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// get_next() skips through the LIST "num" times and retrieves the NODE at that position
//
NODE* NODE::get_next(int num) const 
{

	NODE* link_ahead = link;	// link to next node

	num--;
	if ((num > 0) && (link_ahead != NULL))
		link_ahead = link_ahead->get_next(num);

	return link_ahead;
} // end get_next()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// output_bits() prints to outfile each bit in the CODE_LIST of the selected NODE
//
void NODE::output_bits(ofstream& outfile, int& spaces, unsigned char& current_code)
{
	// variable declaration
	bool done_output = false;		// true when all bits have been printed to ofstream

	unsigned char zero = ((int) 0);	// used in byte output (0000 0000)
	unsigned char add_code;			// code to be added to current_code
	CODE_NODE* cnode;				// pointer to CODE_NODE in CODE_LIST in original_LIST


	// initialize cnode and boolean flag
	cnode = code_list.head_is()->link_is();
	done_output = false;


	// while the entire code for the NODE has not been placed in a byte that
	// will be placed in the ofstream, continue to compact the bits in the NODE
	while (!done_output)
	{	
		// determine how many spaces are left in byte
		// no spaces (i.e. last code filled entire byte, start a new one)
		if (spaces == 0)
		{
			// set current byte
			current_code = zero;
			current_code = cnode->code_is();
			spaces = (8 - cnode->code_size_is());

			// get next CODE_NODE if there is one
			// otherwise skip to the next code in the LIST
			if (cnode->link_is() != NULL)
				cnode = cnode->link_is();
			else done_output = true;

			// before passing through loop again, check to see if the byte has been
			// complete (i.e. no spaces)
			// if so, output the byte to the ofstream
			if (spaces == 0)
				outfile << current_code;

		} // end if
						
		// if are spaces in the last byte, fill the last byte
		else if ( (spaces > 0) && (!done_output) )
		{
			// shift code value to proper position
			add_code = cnode->code_is();
			add_code = add_code >> (8 - spaces);
				
			// add codes (completing the byte)
			current_code = current_code + add_code;

			// set the value of spaces
			spaces = spaces - cnode->code_size_is();

			// if the entire code was used to fill the rest of the byte
			// then get the next CODE_NODE
			if (spaces >= 0)
			{
				if (cnode->link_is() != NULL)
					cnode = cnode->link_is();
				else done_output = true;
			} // end if

			// before passing through loop again, check to see if the byte has been
			// complete (i.e. no spaces)
			// if so, output the byte to the ofstream
			if (spaces <= 0)
				outfile << current_code;

		} // end else if

		// there are spaces that are already filled (but need to be inserted)
		else if ( (spaces < 0) && (!done_output) )
		{
			// set current_code
			current_code = zero;

			// shift code value to proper position
			current_code = cnode->code_is();
			current_code = current_code << (cnode->code_size_is() + spaces);

			// set the value of spaces
			spaces = (8 + spaces);

			// get the next CODE_NODE
			if (cnode->link_is() != NULL)
				cnode = cnode->link_is();
			else done_output = true;

			// before passing through loop again, check to see if the byte has been
			// complete (i.e. no spaces)
			// if so, output the byte to the ofstream
			if (spaces == 0)
				outfile << current_code;
		
		} // end else if
			
	} // end while loop

} // end output_bits()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// overload "new" operator for freelist usage
//
void* NODE::operator new(size_t)
{
	// if freelist is empty allocate a new NODE
	if (freelist == NULL)
		return ::new NODE;
	
	// take NODE from freelist
	NODE* temp = freelist;
	freelist = freelist->link_is();
	return temp;

} // end operator new()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// overload "delete" operator for freelist usage
//
void NODE::operator delete(void* ptr)
{
	// put deleted NODE on freelist
	((NODE*)ptr)->edit_link(freelist);
	freelist = (NODE*)ptr;
} // end operator delete()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// overload "=" operator
//
void NODE::operator=(NODE* other_node)
{
	subtree = other_node->tree_is();
	link = other_node->link_is();
	code_list = other_node->code_is();
} // end operator=()
//-------------------------------------------------------------------------------------------
// end NODE class member functions
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
// LIST class member functions


//-------------------------------------------------------------------------------------------
// LIST() default constructor
//
LIST::LIST() 
{
	head = new NODE;

	head->edit_link(NULL);
} // end LIST() constructor
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// edit_head() changes the head value of LIST
//
void LIST::edit_head(NODE* new_head) 
{
	
	head = new_head;
} // end edit_head
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// head_is() returns the head value of LIST
//
NODE* LIST::head_is() const 
{
	return head;
} // end head_is
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// output_all_bits() prints to outfile all bits in each CODE_LIST of all the NODES
// in the LIST
//
void LIST::output_all_bits(ofstream& outfile)
{
	// variable declaration
	NODE* current;					// current position in LIST

	CODE_NODE* cnode;				// current position  in CODE_LIST
	int spaces;						// spaces remaining in byte
	bool done_output = false;		// true when all bits have been printed to ofstream

	unsigned char zero = ((int) 0);		// used in byte output (0000 0000)

	unsigned char current_code;			// code divided into bytes
	unsigned char add_code;				// code to be added to current_code


	// initialize variables
	current = head;
	spaces = 0;
	current_code = zero;

	// output codes from each node in order
	while (current->link_is() != NULL)
	{
		// get next NODE
		current = current->link_is();

		// initialize cnode and boolean flag
		cnode = current->code_is().head_is()->link_is();
		done_output = false;


		// while the entire code for the NODE has not been placed in a byte that will be placed
		// in the ofstream, continue to compact the bits in the NODE
		while (!done_output)
		{	

			// determine how many spaces are left in byte
			// no spaces (i.e. last code filled entire byte, start a new one)
			if (spaces == 0)
			{
				// set current byte
				current_code = zero;
				current_code = cnode->code_is();
				spaces = (8 - cnode->code_size_is());

				// get next CODE_NODE if there is one
				// otherwise skip to the next code in the LIST
				if (cnode->link_is() != NULL)
					cnode = cnode->link_is();
				else done_output = true;

				// before passing through loop again, check to see if the byte has been
				// complete (i.e. no spaces)
				// if so, output the byte to the ofstream
				if (spaces == 0)
					outfile << current_code;

			} // end if
			
			// if are spaces in the last byte, fill the last byte
			else if ( (spaces > 0) && (!done_output) )
			{
				// shift code value to proper position
				add_code = cnode->code_is();
				add_code = add_code >> (8 - spaces);
				
				// add codes (completing the byte)
				current_code = current_code + add_code;

				// set the value of spaces
				spaces = spaces - cnode->code_size_is();

				// if the entire code was used to fill the rest of the byte
				// then get the next CODE_NODE
				if (spaces >= 0)
				{
					if (cnode->link_is() != NULL)
						cnode = cnode->link_is();
					else done_output = true;
				} // end if

				// before passing through loop again, check to see if the byte has been
				// complete (i.e. no spaces)
				// if so, output the byte to the ofstream
				if (spaces <= 0)
					outfile << current_code;

			} // end else if

			// there are spaces that are already filled (but need to be inserted)
			else if ( (spaces < 0) && (!done_output) )
			{
				// set current_code
				current_code = zero;

				// shift code value to proper position
				current_code = cnode->code_is();
				current_code = current_code << (cnode->code_size_is() + spaces);

				// set the value of spaces
				spaces = (8 + spaces);

				// get the next CODE_NODE
				if (cnode->link_is() != NULL)
					cnode = cnode->link_is();
				else done_output = true;

				// before passing through loop again, check to see if the byte has been
				// complete (i.e. no spaces)
				// if so, output the byte to the ofstream
				if (spaces == 0)
					outfile << current_code;

			} // end else if
			
		} // end inner while loop

	} // end outer while loop

	// output final byte even if it isnt finished
	if (spaces != 0)
			outfile << current_code;

} // end output_bits()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// empty() returns true if the LIST has no NODEs and false if there are NODEs present
// in the LIST
//
bool LIST::empty() const 
{
	
	if (head->link_is() == NULL)
		return true;
	else
		return false;
} // end empty()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// size() counts the number of NODEs in LIST and returns the value as an integer
//
int LIST::size() const
{
	// varaible declarati
	NODE* current_NODE;		// current NODE in number LIST
	int num_NODEs = 0;		// number of NODEs in LIST

	// start current_NODE at the head of LIST
	current_NODE = head;

	// count number of NODEs in LIST
	while (current_NODE->link_is() != NULL)
	{
		num_NODEs++;
		current_NODE = current_NODE->link_is();
	} // end while loop

	return num_NODEs;

} // end size()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// append() adds a NODE at the bottom of the LIST
// 
void LIST::append(NODE*& new_NODE) 
{
	// variable declaration
	NODE* current_NODE;		// current NODE in number LIST

	// start current_NODE at the head of LIST
	current_NODE = head;

	// look for end of the LIST (i.e. NULL link)
	while (current_NODE->link_is() != NULL)
	{
		current_NODE = current_NODE->link_is();
	}

	// add new_NODE to end of LIST
	current_NODE->edit_link(new_NODE);
	new_NODE->edit_link(NULL);
	
} // end append()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// insert() adds a NODE (new_NODE) in the LIST in order of frequency
// 
void LIST::insert(NODE*& new_NODE) 
{
	// variable declaration
	NODE* current_NODE;			// current NODE in number LIST
	bool insert_done = false;	// determines whether the new NODE has been
								// properly inserted into the LIST

	// start current_NODE at the head of LIST
	current_NODE = head;

	// insert the new_NODE in the LIST in order of frequency, at the earliest possible spot
	while ( (current_NODE->link_is() != NULL) && (!insert_done) )
	{
		if (new_NODE->tree_is().freq_is() <= current_NODE->link_is()->tree_is().freq_is())
		{
			new_NODE->edit_link(current_NODE->link_is());
			current_NODE->edit_link(new_NODE);

			insert_done = true;
		} // end if

		else current_NODE = current_NODE->link_is();

	} // end while loop

	// if new_NODE has not been inserted into LIST
	// add new_NODE to end of LIST
	if (!insert_done)
	{
		current_NODE->edit_link(new_NODE);
		new_NODE->edit_link(NULL);
	} // end if

} // end insert()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// insert_top() adds a NODE at the top of the LIST
//
void LIST::insert_top(NODE*& new_NODE) 
{

	new_NODE->edit_link(head->link_is());
	head->edit_link(new_NODE);	

} // end insert_top()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// remove_top() removes the top NODE from the LIST (i.e. the one right after dummy head NODE)
// and returns its huffman tree
//
HUFF_TREE LIST::remove_top()
{
	// variable declaration
	NODE* top_NODE;							// top NODE on the LIST
	HUFF_TREE new_tree;						// huffman tree removed from the LIST
	LEAF_NODE* new_root = NULL;				// new root for the empty HUFF_TREE	
	

	

	// if the LIST is not empty
	// remove the top NODE from the LIST and return it
	if (head->link_is() != NULL)
	{
		top_NODE = head->link_is();
		head->edit_link(head->link_is()->link_is());
		new_tree = top_NODE->tree_is();
		delete top_NODE;
		return new_tree;
	} // end if
	
	// return empty tree
	else
	{
		// initialize empty_tree
		new_root->edit_freq(0);
		new_root->edit_value(0);
		new_tree.edit_root(new_root);
		return new_tree;
	} // end else

} // remove_top()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// overload "=" operator
//
void LIST::operator=(LIST other_LIST)
{

	head = other_LIST.head_is();

} // end operator=()
//-------------------------------------------------------------------------------------------
// end LIST class member functions
//-------------------------------------------------------------------------------------------