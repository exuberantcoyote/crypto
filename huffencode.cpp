// Title:         Huffman Encoder
// Programmer:    Trent Tolley
// Compiler:      Visual C++ Version 6.0
// Platform:      Pentium III 700 Laptop / Windows ME
// Last Modified: Oct 10, 2001
//-------------------------------------------------------------------------------------------
//	This program is designed to encode a given input file using a Huffman coding tree.
//-------------------------------------------------------------------------------------------


//header files

#include <stdio.h>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>
#include "codelist.h"
#include "linklist.h"
#include "hufftree.h"
using namespace std;


//-------------------------------------------------------------------------------------------
// function prototypes
void input_characters(ifstream&,ofstream&,HUFF_TREE[]);
void sort_list(HUFF_TREE[],int&);
void create_code_list(HUFF_TREE[],int,LIST&);
void build_tree(LIST&,HUFF_TREE&);
void create_code_values(HUFF_TREE&,LIST&);
bool find_code(NODE*&,HUFF_NODE*,CODE_LIST,int);
void display_info(LIST);
void create_output(ofstream&,ifstream&,LIST&,HUFF_TREE&);
//-------------------------------------------------------------------------------------------




//-------------------------------------------------------------------------------------------
void main(int argc, char* argv[])
{
	// variable declaration
	ifstream infile;					// binary input file
	ofstream outfile;					// output binary file

	const int max_list_size = 257;		// maximum size of tree_list (0-128 ... i.e. 129)
	
	int list_size = 0;					// actual number of unique values encountered
										// in the binary input stream

	HUFF_TREE tree_list[max_list_size];	// array of single node HUFF_TREEs
										// size is 128 to account for all ascii values
	
	HUFF_TREE final_tree;				// final huffman coding tree
	LEAF_NODE* new_root;				// new root value for each HUFF_TREE in tree_list

	LIST code_LIST;						// linked list of HUFF_TREEs
	LIST original_LIST;					// contains the original value of code_LIST
										// before any manipulation

	NODE* new_NODE;						// new NODE for code_LIST or original_LIST
	NODE* current_NODE;					// current NODE position in code_LIST or original_LIST
 


	// initialize code_LIST and original_LIST
	new_NODE = new NODE;
	new_NODE->edit_link(NULL);
	code_LIST.edit_head(new_NODE);

	new_NODE = new NODE;
	new_NODE->edit_link(NULL);
	original_LIST.edit_head(new_NODE);


	// initialize tree_list
	for (int i = 0; i < max_list_size; i++)
	{
		// initialize new_root
		new_root = new LEAF_NODE(0,i);

		// add the new_root to the tree_list
		tree_list[i].edit_root(new_root);
	} // end for loop

	
	//declare file names
	infile.open(argv[1], ios::in | ios::binary);
	outfile.open(argv[2], ios::out | ios::binary);     

	//activate manipulators
	outfile.setf(ios::fixed, ios::floatfield);
	outfile.setf(ios::showpoint);


	// read the binary input
	input_characters(infile,outfile,tree_list);


	// sort the list of value frequencies stored in the tree_list as single node HUFF_TREEs
	sort_list(tree_list,list_size);


	// create code list (linked list)
	create_code_list(tree_list,list_size,code_LIST);


	// initialize current_NODE
	current_NODE = new NODE;
	current_NODE = code_LIST.head_is()->link_is();
	
	// deep copy code_LIST into original_LIST
	for (i = 0; i < code_LIST.size(); i++)
	{
		new_NODE = new NODE;
		new_NODE->edit_tree(current_NODE->tree_is());
		new_NODE->edit_code(current_NODE->code_is());

		original_LIST.append(new_NODE);

		current_NODE = current_NODE->link_is();
	} // end for loop


	// build huffman coding tree
	build_tree(code_LIST,final_tree);


	// determine code values for individual characters
	create_code_values(final_tree,original_LIST);


	// print to standard output (ascii values, frequencies, and code values)
	display_info(original_LIST);


	// output necessary information to output file
	create_output(outfile,infile,original_LIST,final_tree);


	// close files
	infile.close();
	outfile.close();

} // end main()
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
// input_characters() reads from the binary input file a given character stream and
// determines the number of times each ascii character is encountered (frequency).
// The array of HUFF_TREEs is used to store each ascii character
//
void input_characters(ifstream& infile, ofstream& outfile,HUFF_TREE tree_list[])
{
	// variable declaration
	int new_char = ' ';				// new character in the binary input stream


	// while there are binary characters in the input stream, get the next byte from input
	while (infile)
	{

		// read next character from binary input stream
		infile.read((char*) &new_char,1);

		if (infile)
		{
			// increment frequency for given ascii value in tree_list
			tree_list[new_char].root_is()->edit_freq(tree_list[new_char].freq_is() + 1);
		} // end if

	} // end while loop

} // end input_characters()
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
// sort_list() takes the unordered tree array and sorts the values by frequency and 
// determines the actual number of encountered ascii values
//
void sort_list(HUFF_TREE tree_list[],int& list_size)
{
	// variable declaration	
	const int max_list_size = 257;		// maximum size of tree_list (0-128 ... i.e. 129)
	HUFF_TREE temp_tree;				// temporary HUFF_TREE to use during swapping

	
	// outer for loop runs through list once checking each node
	for (int i = 1; i < max_list_size; i++)
	{
		// inner for loop checks to see if the current frequency of position i needs to
		// be moved to a lower position
		for (int j = i; j > 0; j--)
		{
			// check to see if current frequency is less than frequency of previous position
			// swap HUFF_TREEs so that the one with less frequency is first
			// if the frequencies are equal then the one with the lowest ascii value
			// will be first
			if ( 
				 ( (tree_list[j].freq_is() < tree_list[j-1].freq_is()) && (tree_list[j].freq_is() != 0) )
				 ||
				 ( (tree_list[j].freq_is() != 0) && (tree_list[j-1].freq_is() == 0) ) 
			   )
			{
				temp_tree = tree_list[j-1];
				tree_list[j-1] = tree_list[j];
				tree_list[j] = temp_tree;
			} // end if

		} // end inner for loop

	} // end outer for loop

	// initialize list_size
	list_size = 0;

	// determine actual size of list (i.e. number of unique encountered ascii values)
	for (i = 0; i < max_list_size; i++)
	{
		if (tree_list[i].freq_is() != 0)
			list_size++;
	} // end for loop

} // end sort_list()
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
// create_code_list() takes the sorted array of HUFF_TREEs and builds an ordered
// linked list (code_LIST) with its values
//
void create_code_list(HUFF_TREE tree_list[], int list_size, LIST& code_LIST)
{
	// variable declarations
	NODE* new_NODE;					// new NODE to add on to code_LIST
	unsigned char zero = ((int) 0);	// binary value used in tree initialization (0000 0000)
	
	CODE_LIST new_code;				// new code for each value in code_LIST
	CODE_NODE* new_cnode;			// new node for each code_LIST


	for (int i = 0; i < list_size; i++)
	{
		// initialize new_cnode and new_code
		// dummy head value
		new_cnode = new CODE_NODE;
		new_cnode->edit_code(zero);
		new_cnode->edit_code_size(0);
		new_cnode->edit_link(NULL);

		new_code.edit_head(new_cnode); 

		// first code node
		new_cnode = new CODE_NODE;
		new_cnode->edit_code(zero);
		new_cnode->edit_code_size(0);
		new_cnode->edit_link(NULL);

		new_code.append(new_cnode);

		// initialize new NODE
		new_NODE = new NODE;
		new_NODE->edit_link(NULL);
		new_NODE->edit_code(new_code);
		new_NODE->edit_tree(tree_list[i]);

		code_LIST.append(new_NODE);
		
	} // end for loop

} // end create_code_list()
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
// build_tree() takes the ordered link list (code_LIST) and combines each of the NODEs
// using a huffman coding sceme until there is only one NODE remaining in the code_LIST.
// it then sets final_tree equal to the last HUFF_TREE in code_LIST
//
void build_tree(LIST& code_LIST, HUFF_TREE& final_tree)
{
	// variable declaration
	HUFF_TREE first_freq;		// subtree in list with lowest frequency
	HUFF_TREE second_freq;		// subtree in list with second lowest frequency

	HUFF_TREE new_tree;			// new tree that is the combination of the two
								// subtrees with the lowest frequency
	
	IN_NODE* new_root;			// new root pointer for the new_tree

	NODE* new_NODE;				// new NODE to insert back into code_LIST


	// while there is more than one tree in the LIST continue to combine 2 smallest subtrees
	// and then re-insert them into the LIST in order of frequency
	while (code_LIST.size() > 1)
	{
		first_freq = code_LIST.remove_top();
		second_freq = code_LIST.remove_top();

		// intialize the new tree and its root
		new_root = new IN_NODE(first_freq.root_is(),second_freq.root_is());
		new_tree.edit_root(new_root);

		// initialize new_NODE
		new_NODE = new NODE;
		new_NODE->edit_link(NULL);
		new_NODE->edit_tree(new_tree);

		// insert new NODE into LIST
		code_LIST.insert(new_NODE);

	} // end while loop

	// set final_tree equal to the last NODE in code_LIST
	final_tree = code_LIST.head_is()->link_is()->tree_is();

} // end build_tree()
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
// create_code_values() builds the binary codes for each ascii value in original_LIST
// buy traversing through the huffman coding tree (final_tree) and designating a "0"
// for left nodes and a "1" for right nodes
//
void create_code_values(HUFF_TREE& final_tree, LIST& original_LIST)
{
	// variable declaration
	NODE* current_NODE;					// current node in the original_LIST

	unsigned char zero = ((int) 0);		// binary value used in tree initialization (0000 0000)
	int code_size;						// size of the new code

	CODE_LIST* new_code;				// new code for given character
	CODE_NODE* new_cnode;				// new node for new_code

	bool found;							// true when value is found in original_LIST


	// initialize current_NODE to the first position in original_LIST
	current_NODE = new NODE;
	current_NODE = original_LIST.head_is()->link_is();

	// for each subtree value in original_LIST
	// find the value in the code_LIST tree and calculate its binary value
	for (int i = 0; i < original_LIST.size(); i++)
	{
		// initialize new_cnode, new_code and code_size
		new_cnode = new CODE_NODE;
		new_cnode->edit_code(zero);
		new_cnode->edit_code_size(0);
		new_cnode->edit_link(NULL);

		new_code = new CODE_LIST;
		new_code->edit_head(new_cnode); 

		new_cnode = new CODE_NODE;
		
		new_cnode = new CODE_NODE;
		new_cnode->edit_code(zero);
		new_cnode->edit_code_size(0);
		new_cnode->edit_link(NULL);

		new_code->append(new_cnode);
	
		code_size = 0;

		// perform a pre-order traversal of final_tree in order to find matching
		// binary value of the single HUFF_NODE in the current NODE of original_LIST
		// meanwhile keeping tracking of the new code value
		found = find_code(current_NODE,final_tree.root_is(),*new_code,code_size);

		current_NODE = current_NODE->link_is();

	} // end for loop


} // end create_code_values()
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
// find_code() takes a NODE from the original_LIST and traverses through the huffman tree
// starting at the root. The function then builds the binary code by storing 1s and 0s in
// the CODE_LIST new_code. The funtion recursively calls itself until the matching ascii
// value has been found in the huffman tree. The code_size is incremented each pass through
// the function
//
bool find_code(NODE*& current_NODE, HUFF_NODE* root, CODE_LIST new_code, int code_size)
{
	// variable declaration
	unsigned char zero = ((int) 0);		// binary value used in tree traversal (0000 0000)
	unsigned char one = ((int) 1);		// binary value used in tree traversal (0000 0001)
	unsigned char two = ((int) 2);		// binary value used in tree traversal (0000 0010)
	unsigned char three = ((int) 4);	// binary value used in tree traversal (0000 0100)
	unsigned char four = ((int) 8);		// binary value used in tree traversal (0000 1000)
	unsigned char five = ((int) 16);	// binary value used in tree traversal (0001 0000)
	unsigned char six = ((int) 32);		// binary value used in tree traversal (0010 0000)
	unsigned char seven = ((int) 64);	// binary value used in tree traversal (0100 0000)
	unsigned char eight = ((int) 128);	// binary value used in tree traversal (1000 0000)

	unsigned char old_code;				// used to keep track of past values of the binary code
	CODE_NODE* new_cnode;				// new CODE_NODE for new_code
	CODE_NODE* current;					// current node position in new_code

	const int byte_size = 8;
	

	// base case
	if (root == NULL)
		return false;

	// recursive case
	else
	{
		// check for a LEAF_NODE
		if (root->is_leaf() == true)
		{
			// check for match and save binary code and code size
			if (current_NODE->tree_is().root_is()->value_is() == root->value_is())
			{

				// set the new_code and new_code_size
				current_NODE->edit_code(new_code);
				current_NODE->code_is().head_is()->get_next(new_code.size())->edit_code_size(code_size);
	
				// value found in this subtree
				return true;

			} // end if

			// value not found in this subtree
			else return false;
			
		} // end if

		// root is an IN_NODE
		// continue pre-order traversal
		// determine whether value is left or right
		// then add appropriate binary bit (1,0) to the last node in new_code
		else
		{
			switch (code_size)
			{
			case 0:		// save binary code value
						old_code = new_code.head_is()->get_next(new_code.size())->code_is();

						// if the value is not found in the left subtree, search the right
						if (! find_code(current_NODE,root->left_is(),new_code,(code_size + 1)) )
						{
							new_code.head_is()->get_next(new_code.size())->edit_code(old_code + eight);
							find_code(current_NODE,root->right_is(),new_code,(code_size + 1));
						}
						
						break;
	
			case 1:		// save binary code value
						old_code = new_code.head_is()->get_next(new_code.size())->code_is();

						// if the value is not found in the left subtree, search the right
						if (! find_code(current_NODE,root->left_is(),new_code,(code_size + 1)) )
						{
							new_code.head_is()->get_next(new_code.size())->edit_code(old_code + seven);
							find_code(current_NODE,root->right_is(),new_code,(code_size + 1));
						}

						break;
			
			case 2:		// save binary code value
						old_code = new_code.head_is()->get_next(new_code.size())->code_is();

						// if the value is not found in the left subtree, search the right
						if (! find_code(current_NODE,root->left_is(),new_code,(code_size + 1)) )
						{
							new_code.head_is()->get_next(new_code.size())->edit_code(old_code + six);
							find_code(current_NODE,root->right_is(),new_code,(code_size + 1));
						}

						break;
			
			case 3:		// save binary code value
						old_code = new_code.head_is()->get_next(new_code.size())->code_is();

						// if the value is not found in the left subtree, search the right
						if (! find_code(current_NODE,root->left_is(),new_code,(code_size + 1)) )
						{
							new_code.head_is()->get_next(new_code.size())->edit_code(old_code + five);
							find_code(current_NODE,root->right_is(),new_code,(code_size + 1));
						}

						break;
			
			case 4:		// save binary code value
						old_code = new_code.head_is()->get_next(new_code.size())->code_is();

						// if the value is not found in the left subtree, search the right
						if (! find_code(current_NODE,root->left_is(),new_code,(code_size + 1)) )
						{
							new_code.head_is()->get_next(new_code.size())->edit_code(old_code + four);
							find_code(current_NODE,root->right_is(),new_code,(code_size + 1));
						}

						break;
			
			case 5:		// save binary code value
						old_code = new_code.head_is()->get_next(new_code.size())->code_is();

						// if the value is not found in the left subtree, search the right
						if (! find_code(current_NODE,root->left_is(),new_code,(code_size + 1)) )
						{
							new_code.head_is()->get_next(new_code.size())->edit_code(old_code + three);
							find_code(current_NODE,root->right_is(),new_code,(code_size + 1));
						}

						break;
			
			case 6:		// save binary code value
						old_code = new_code.head_is()->get_next(new_code.size())->code_is();

						// if the value is not found in the left subtree, search the right
						if (! find_code(current_NODE,root->left_is(),new_code,(code_size + 1)) )
						{
							new_code.head_is()->get_next(new_code.size())->edit_code(old_code + two);
							find_code(current_NODE,root->right_is(),new_code,(code_size + 1));
						}

						break;
			
			case 7:		// save binary code value
						old_code = new_code.head_is()->get_next(new_code.size())->code_is();

						// if the value is not found in the left subtree, search the right
						if (! find_code(current_NODE,root->left_is(),new_code,(code_size + 1)) )
						{
							new_code.head_is()->get_next(new_code.size())->edit_code(old_code + one);
							find_code(current_NODE,root->right_is(),new_code,(code_size + 1));
						}

						break;
			
			// code is now 8 bit
			// save current code information and length
			// and begin code construction again using a new node in new_code
			// thus allowing a code of infinite bits
			case 8:		// temporary variable declaration
						CODE_LIST temp_code;	// used to check for a correct path
												// before assigning a value to new_code

						
						// initialize new_cnode
						current = new CODE_NODE;
						current = new_code.head_is()->link_is();
	

						// deep copy old value of new_code
						for (int i = 0; i < new_code.size(); i++)
						{
							new_cnode = new CODE_NODE;
							new_cnode->edit_code(current->code_is());
							new_cnode->edit_code_size(current->code_size_is());
							new_cnode->edit_link(NULL);

							temp_code.append(new_cnode);

							current = current->link_is();
						} // end for loop
				

						temp_code.head_is()->get_next(temp_code.size())->edit_code_size(8);
												
						// create and initialize new CODE_NODE
						new_cnode = new CODE_NODE;
						new_cnode->edit_code(zero);
						new_cnode->edit_code_size(0);
						new_cnode->edit_link(NULL);

						// save old binary code value
						old_code = new_cnode->code_is();
						code_size = 0;

						temp_code.append(new_cnode);

						if (! find_code(current_NODE,root->left_is(),temp_code,(code_size + 1)) )
						{
							temp_code.head_is()->get_next(temp_code.size())->edit_code(old_code + eight);
							
							// if neither subtree contains the value, then remove the last node
							if (find_code(current_NODE,root->right_is(),temp_code,(code_size + 1)) )
							{
							//	new_code = temp_code;							
							} // end if

						} // end if
						
						//else new_code = temp_code;

						break;
			
			} // end switch statement

		} // end else

	} // end else

} // end find_code
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
// display_info() prints to standard output the code and frequency for each NODE
// in original_LIST
//
void display_info(LIST original_LIST)
{
	// variable declaration
	NODE* current_NODE;				// current position in original_LIST
	CODE_NODE* cnode;				// current CODE_NODE in CODE_LIST

	unsigned char code_bit;			// single bit in the code that will be displayed
	unsigned char one = ((int) 1);	// used in byte output (0000 0001)

	bool done_code;					// true when the code for a given NODE has be displayed
	int current_size;				// number of bits in the current code
	int position;					// bit position in code byte



	// standard output header information
	cout << "ASCII - FREQ -- CODE" << endl;

	// initialize current_NODE
	current_NODE = new NODE;
	current_NODE = original_LIST.head_is()->link_is();

	for (int i = 0; i < original_LIST.size(); i++)
	{

		cout << ((int) current_NODE->tree_is().root_is()->value_is()) << "\t"
			 << current_NODE->tree_is().root_is()->freq_is() << "\t";


		// initialize values
		done_code = false;
		cnode = new CODE_NODE;
		cnode = current_NODE->code_is().head_is()->link_is();
		code_bit = cnode->code_is();
		current_size = current_NODE->code_is().total_bit_size();
		position = 1;

		// while the current character has not been printed to the outfile
		// continue to traverse the tree
		while (!done_code)
		{

			switch (position)
			{

				case 1:	// isolate selected position
						code_bit = code_bit >> 7;
						
						break;

				case 2: // isolate selected position
						code_bit = code_bit << 1;
						code_bit = code_bit >> 7;

						break;

				case 3: // isolate selected position
						code_bit = code_bit << 2;
						code_bit = code_bit >> 7;

						break;

				case 4: // isolate selected position
						code_bit = code_bit << 3;
						code_bit = code_bit >> 7;

						break;

				case 5: // isolate selected position
						code_bit = code_bit << 4;
						code_bit = code_bit >> 7;

						break;

				case 6: // isolate selected position
						code_bit = code_bit << 5;
						code_bit = code_bit >> 7;

						break;

				case 7: // isolate selected position
						code_bit = code_bit << 6;
						code_bit = code_bit >> 7;

						break;

				case 8: // isolate selected position
						code_bit = code_bit << 7;
						code_bit = code_bit >> 7;

						break;

			} // end switch

			// traverse tree in proper direction
			if (code_bit == one)
				cout << "1";
			else cout << "0";
			
			// decrement remaining code size
			current_size--;

			if (current_size == 0)
				done_code = true;
			else
			{
				// reset code_bit value
				code_bit = cnode->code_is();


				// set bit position
				if (position < 8)
					position++;
				else
				{
					cnode = cnode->link_is();
					code_bit = cnode->code_is();
					position = 1;
				} // done else

			} // end else

		} // end while loop


		// get next position in original_LIST
		current_NODE = current_NODE->link_is();
		cout << endl;

	} // end for loop

} // end display_info()
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
// create_output() creates an encoded output file by matching values in the input file to
// values in the huffman coding tree
//
// the output file format is as follows:
// [num codes][code values and lengths][actual codes][num coded characters][encoded message]
//
void create_output(ofstream& outfile, ifstream& infile, LIST& original_LIST, HUFF_TREE& final_tree)
{
	// variable declaration
	unsigned char code_bit_size;	// number of bits in the current code
	unsigned char number_of_codes;	// number of codes in list
	unsigned char freq_byte;		// total frequency of input file (one byte at a time)
	int total_freq;					// total frequency of input file

	const long beginning = 0;		// represents the beginning of the input file
									// used for pointer repositioning

	int new_char = ' ';				// new character in the binary input stream
	unsigned char char_value;		// actual value of the new character in the input stream
	NODE* current_NODE;				// current position in original_LIST

	bool found;						// true when the value of a character is found in the
									// original_LIST (used for final encoding)

	unsigned char zero = ((int) 0);	// used in byte output (0000 0000)
	unsigned char current_code;		// code divided into bytes
	int spaces;						// spaces remaining in byte


	
	// output number of codes to outfile
	number_of_codes = ((int) original_LIST.size());
	outfile << number_of_codes;




	// initialize current_NODE
	current_NODE = original_LIST.head_is();

	// output code values and lengths to output file
	while (current_NODE->link_is() != NULL)
	{	
		current_NODE = current_NODE->link_is();
		outfile << current_NODE->tree_is().root_is()->value_is();

		code_bit_size = ((int) current_NODE->code_is().total_bit_size());
		outfile << code_bit_size;
	
	} // end while loop


	// output all bits in order
	original_LIST.output_all_bits(outfile);


	// calculate total frequency and output to file
	// order int bytes in order from lowest to highest
	// first byte (lowest)
	total_freq = final_tree.freq_is();
	freq_byte = ((int) total_freq);
	outfile << freq_byte;
	// second byte
	total_freq = total_freq / 256;
	freq_byte = ((int) total_freq);
	outfile << freq_byte;
	// third byte
	total_freq = total_freq / 256;
	freq_byte = ((int) total_freq);
	outfile << freq_byte;
	// fourth byte
	total_freq = total_freq / 256;
	freq_byte = ((int) total_freq);
	outfile << freq_byte;	

	// reposition the "get" pointer
	infile.clear();
	infile.seekg(0);


	// initialize variables
	spaces = 0;
	current_code = zero;

	// while there are binary characters in the input stream, get the next byte from input
	while (infile)
	{

		// read next character from binary input stream
		infile.read((char*) &new_char,1);

		// initialize search values
		current_NODE = original_LIST.head_is();
		found = false;

		// encode each character until the input file is finished
		if (infile)
		{
			char_value = ((int) new_char);


			while ( (!found) && (current_NODE->link_is() != NULL) )
			{
			
				current_NODE = current_NODE->link_is();
			

				// check to see if character values match and set up output
				if (current_NODE->tree_is().root_is()->value_is() == char_value)
				{
					found = true;

					// output bits to outfile
					current_NODE->output_bits(outfile,spaces,current_code);
				} // end if

			} // end inner while loop
					
		} // end if

	} // end outer while loop

	// output final byte even if it isnt finished
	if (spaces != 0)
		outfile << current_code;

	// confirm completion
	cout << "Encoding completed successfully..." << endl;

} // end create_output()
//-------------------------------------------------------------------------------------------
