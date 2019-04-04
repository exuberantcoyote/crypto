// This is the definition file for the linked list ADT (CODE_LIST) 
// and its node class (CODE_NODE)

//header files

#include <stdio.h>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>
#include "codelist.h"
using namespace std;


//-------------------------------------------------------------------------------------------
// freelist declaration
CODE_NODE* CODE_NODE::freelist = NULL;
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// CODE_NODE class member functions


//-------------------------------------------------------------------------------------------
// CODE_NODE() default constructor
//
CODE_NODE::CODE_NODE() 
{
	code_size = 0;
	link = NULL;
} // end CODE_NODE() constructor
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// edit_link() changes the pointer to the next CODE_NODE in the CODE_LIST
//
void CODE_NODE::edit_link(CODE_NODE* new_link) 
{
	link = new_link;
} // end edit_link()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// link_is() returns the value of the pointer to the next CODE_NODE in the LIST
//
CODE_NODE* CODE_NODE::link_is() const 
{
	return link;
} // end link_is()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// edit_code() sets the binary code value to new_code
//
void CODE_NODE::edit_code(unsigned char new_code)
{
	code = new_code;
} // end edit_code()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// code_is() returns the binary code value
//
unsigned char CODE_NODE::code_is() const
{
	return code;
} // end code_is()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// edit_code_size() sets the size of the code value equal to new_size
//
void CODE_NODE::edit_code_size(int new_size)
{
	code_size = new_size;
} // end edit_code_size()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// code_size_is() returns the size of the binary code value
//
int CODE_NODE::code_size_is() const
{
	return code_size;
} // end code_size_is()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// get_next() skips through the LIST "num" times and retrieves the CODE_NODE at that position
//
CODE_NODE* CODE_NODE::get_next(int num) const 
{

	CODE_NODE* link_ahead = link;	// link to next CODE_NODE

	num--;
	if ((num > 0) && (link_ahead != NULL))
		link_ahead = link_ahead->get_next(num);

	return link_ahead;
} // end get_next()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// overload "new" operator for freelist usage
//
void* CODE_NODE::operator new(size_t)
{
	// if freelist is empty allocate a new NODE
	if (freelist == NULL)
		return ::new CODE_NODE;
	
	// take NODE from freelist
	CODE_NODE* temp = freelist;
	freelist = freelist->link_is();
	return temp;

} // end operator new()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// overload "delete" operator for freelist usage
//
void CODE_NODE::operator delete(void* ptr)
{
	// put deleted NODE on freelist
	((CODE_NODE*)ptr)->edit_link(freelist);
	freelist = (CODE_NODE*)ptr;
} // end operator delete()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// overload "=" operator
//
void CODE_NODE::operator=(CODE_NODE* other_CODE_NODE)
{
	code = other_CODE_NODE->code_is();
	code_size = other_CODE_NODE->code_size_is();
	link = other_CODE_NODE->link_is();
} // end operator=()
//-------------------------------------------------------------------------------------------
// end CODE_NODE class member functions
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
// CODE_LIST class member functions


//-------------------------------------------------------------------------------------------
// CODE_LIST() default constructor
//
CODE_LIST::CODE_LIST() 
{
	head = new CODE_NODE;
	head->edit_link(NULL);
} // end CODE_LIST() constructor
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// edit_head() changes the head value of CODE_LIST
//
void CODE_LIST::edit_head(CODE_NODE* new_head) 
{	
	head = new_head;
} // end edit_head
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// head_is() returns the head value of CODE_LIST
//
CODE_NODE* CODE_LIST::head_is() const 
{
	return head;
} // end head_is
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// edit_code_size() sets the size of the code value equal to new_size
//
void CODE_LIST::edit_bit_size(int new_size)
{
	bit_size = new_size;
} // end edit_code_size()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// code_size_is() returns the size of the binary code value
//
int CODE_LIST::bit_size_is() const
{
	return bit_size;
} // end code_size_is()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// empty() returns true if the CODE_LIST has no CODE_NODEs and false if there are CODE_NODEs present
// in the CODE_LIST
//
bool CODE_LIST::empty() const 
{
	
	if (head->link_is() == NULL)
		return true;
	else
		return false;
} // end empty()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// total_bit_size() counts the total number of bits stored in the code
//
int CODE_LIST::total_bit_size() const
{
	// varaible declaration
	CODE_NODE* current_CODE_NODE;	// current CODE_NODE in number CODE_LIST
	int num_bits = 0;				// number of bits in the code in CODE_LIST

	// start current_CODE_NODE at the head of CODE_LIST
	current_CODE_NODE = head;

	// count number of CODE_NODEs in CODE_LIST
	while (current_CODE_NODE->link_is() != NULL)
	{
		num_bits += current_CODE_NODE->link_is()->code_size_is();
		current_CODE_NODE = current_CODE_NODE->link_is();
	} // end while loop

	return num_bits;

} // end total_bit_size()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// size() counts the number of CODE_NODEs in CODE_LIST and returns the value as an integer
//
int CODE_LIST::size() const
{
	// varaible declaration
	CODE_NODE* current_CODE_NODE;		// current CODE_NODE in number CODE_LIST
	int num_CODE_NODEs = 0;				// number of CODE_NODEs in CODE_LIST

	// start current_CODE_NODE at the head of CODE_LIST
	current_CODE_NODE = head;

	// count number of CODE_NODEs in CODE_LIST
	while (current_CODE_NODE->link_is() != NULL)
	{
		num_CODE_NODEs++;
		current_CODE_NODE = current_CODE_NODE->link_is();
	} // end while loop

	return num_CODE_NODEs;

} // end size()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// append() adds a CODE_NODE at the bottom of the CODE_LIST
// 
void CODE_LIST::append(CODE_NODE*& new_CODE_NODE) 
{
	// variable declaration
	CODE_NODE* current_CODE_NODE;		// current CODE_NODE in CODE_LIST

	// start current_CODE_NODE at the head of CODE_LIST
	current_CODE_NODE = head;

	// look for end of the CODE_LIST (i.e. NULL link)
	while (current_CODE_NODE->link_is() != NULL)
	{
		current_CODE_NODE = current_CODE_NODE->link_is();
	}

	// add new_CODE_NODE to end of CODE_LIST
	current_CODE_NODE->edit_link(new_CODE_NODE);
	new_CODE_NODE->edit_link(NULL);
	
} // end append()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// remove() removes the last CODE_NODE from the CODE_LIST
//
void CODE_LIST::remove()
{
	CODE_NODE* current;					// current CODE_NODE in CODE_LIST
	CODE_NODE* temp;					// last CODE_NODE in CODE_LIST

	// start current at the head of CODE_LIST
	current = head;

	// make sure list isnt empty
	if (!empty())
	{
		// find last position in CODE_LIST
		while (current->link_is()->link_is() != NULL)
		{
			current = current->link_is();
		} // end while loop
	
		// remove last node
		temp = current->link_is();
		current->edit_link(NULL);
		delete temp;

	} // end if

} // end remove()
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// overload "=" operator
//
void CODE_LIST::operator=(CODE_LIST other_CODE_LIST)
{

	head = other_CODE_LIST.head_is();
	bit_size = other_CODE_LIST.bit_size_is();

} // end operator=()
//-------------------------------------------------------------------------------------------
// end CODE_LIST class member functions
//-------------------------------------------------------------------------------------------