// Title:         Huffman Decoder
// Compiler:      Visual C++ Version 6.0
// Platform:      Pentium III 700 Laptop / Windows ME
// Last Modified: Oct 10, 2001
//-------------------------------------------------------------------------------------------
//	This program is designed to decode a given input file using a Huffman coding tree.
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
void input_binary(ifstream&,LIST&,int&);
void read_codes(ifstream&,LIST&,HUFF_TREE&);
void decode(ifstream&,ofstream&,HUFF_TREE);
//-------------------------------------------------------------------------------------------




//-------------------------------------------------------------------------------------------
void main(int argc, char* argv[])
{
	// variable declaration
	ifstream infile;					// binary input file
	ofstream outfile;					// output binary file

	const int max_list_size = 257;		// maximum size of tree_list (0-128 ... i.e. 129)
	
	int num_codes = 0;					// number of codes in the encoded input file

	LIST code_LIST;						// linked list of HUFF_TREEs
	LIST original_LIST;					// contains the original value of code_LIST
										// before any manipulation
	
	HUFF_TREE final_tree;				// reconstructed huffman coding tree
	IN_NODE* new_root;					// new root for final_tree

	NODE* new_NODE;						// new NODE for code_LIST or original_LIST
	NODE* current_NODE;					// current NODE position in code_LIST or original_LIST



	// initialize code_LIST and original_LIST
	new_NODE = new NODE;
	new_NODE->edit_link(NULL);
	code_LIST.edit_head(new_NODE);

	new_NODE = new NODE;
	new_NODE->edit_link(NULL);
	original_LIST.edit_head(new_NODE);

	
	//declare file names
	infile.open(argv[1], ios::in | ios::binary);
	outfile.open(argv[2], ios::out | ios::binary);     

	//activate manipulators
	outfile.setf(ios::fixed, ios::floatfield);
	outfile.setf(ios::showpoint);

	// read the initial encoded binary input stream from the infile and store the
	// values for each character in code_LIST
	input_binary(infile,code_LIST,num_codes);


	// initialize current_NODE
	current_NODE = new NODE;
	current_NODE = code_LIST.head_is()->link_is();
	
	// deep copy code_LIST into original_LIST
	for (int i = 0; i < code_LIST.size(); i++)
	{
		new_NODE = new NODE;
		new_NODE->edit_tree(current_NODE->tree_is());
		new_NODE->edit_code(current_NODE->code_is());

		original_LIST.append(new_NODE);

		current_NODE = current_NODE->link_is();
	} // end for loop



	// initialize final_tree
	new_root = new IN_NODE(NULL,NULL,0);
	final_tree.edit_root(new_root);


	// read code values from ifstream
	read_codes(infile,original_LIST,final_tree);
	

	// decode the encoded input file
	decode(infile,outfile,final_tree);

	// close files
	infile.close();
	outfile.close();

} // end main()
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
// input_characters() reads the number of codes, each value, and each values code bit length
// from the encoded infile. These values are stored in code_LIST and num_codes.
//
void input_binary(ifstream& infile, LIST& code_LIST, int& num_codes)
{
	// variable declaration
	int new_char = ' ';				// new character in the binary input stream
	NODE* new_NODE;					// new NODE in code_LIST
	HUFF_TREE new_tree;				// new huffman tree that will contain one node
									// with its only value in it's root
	
	LEAF_NODE* new_root;			// new root value for the new_tree

	unsigned char zero = ((int) 0);	// used in code storage (0000 0000)
	CODE_LIST* new_code;				// new code for the new NODE
	CODE_NODE* new_cnode;			// new CODE_NODE for the new_code
	int new_size = 0;					// size of new code in bits


	// read number of codes from the input stream
	infile.read((char*) &new_char,1);
	num_codes = new_char;
	if (num_codes == 0)
		num_codes = 256;

	// read all the unique coded values and their code sizes
	// and store them in code_LIST
	for (int i = 0; i < num_codes; i++)
	{

		// read the next value from the encoded input file
		infile.read((char*) &new_char,sizeof(unsigned char));


		// initialize the new_root and new_tree
		new_root = new LEAF_NODE(0,new_char);
		new_tree.edit_root(new_root);


		// initialize new_cnode and new_code
		// dummy head value
		new_cnode = new CODE_NODE;
		new_cnode->edit_code(zero);
		new_cnode->edit_code_size(0);
		new_cnode->edit_link(NULL);


		// read the bit size of the code for the current value
		infile.read((char*) &new_char,sizeof(unsigned char));


		// store bit information in new_code
		new_code = new CODE_LIST;
		new_code->edit_bit_size(new_char);
		new_code->edit_head(new_cnode); 



		// first code node
		new_cnode = new CODE_NODE;
		new_cnode->edit_code(zero);
		new_cnode->edit_code_size(0);
		new_cnode->edit_link(NULL);

		new_code->append(new_cnode);

		// create a new NODE for code_LIST
		new_NODE = new NODE;
		new_NODE->edit_code(*new_code);
		new_NODE->edit_tree(new_tree);
		new_NODE->edit_link(NULL);

	

		// add the newly initialized NODE to the end of code_LIST
		code_LIST.append(new_NODE);

	} // end for loop

} // end input_binary()
//-------------------------------------------------------------------------------------------




//-------------------------------------------------------------------------------------------
// read_codes() reads the actual binary code values for all values from the infile. 
//
void read_codes(ifstream& infile, LIST& original_LIST, HUFF_TREE& final_tree)
{
	// variable declarations
	unsigned char new_char = ' ';				// new character in the binary input stream
	NODE* current_NODE;				// current position in original_LIST
	int num_bits = 0;				// number of bits allocated for codes
	int num_bytes = 0;				// number of bytes allocated for codes
	int position;					// position in current byte
	int bit_size;					// number of bits in current code

	bool done_insert;					// true when the code for a NODE has been stored
	HUFF_NODE* tree_node;				// current node in final_tree
	HUFF_NODE* new_tnode;			// new node for final_tree

	unsigned char zero = ((int) 0);		// binary value used in tree traversal (0000 0000)
	unsigned char one = ((int) 1);		// binary value used in tree traversal (0000 0001)
	unsigned char two = ((int) 2);		// binary value used in tree traversal (0000 0010)
	unsigned char three = ((int) 4);	// binary value used in tree traversal (0000 0100)
	unsigned char four = ((int) 8);		// binary value used in tree traversal (0000 1000)
	unsigned char five = ((int) 16);	// binary value used in tree traversal (0001 0000)
	unsigned char six = ((int) 32);		// binary value used in tree traversal (0010 0000)
	unsigned char seven = ((int) 64);	// binary value used in tree traversal (0100 0000)
	unsigned char eight = ((int) 128);	// binary value used in tree traversal (1000 0000)
	unsigned char test_char;			// character used to determine direction
										// during tree traversal


	// initialize current_NODE
	current_NODE = original_LIST.head_is();

	// determine number of bits allocated for code storage
	for (int i = 0; i < original_LIST.size(); i++)
	{
		current_NODE = current_NODE->link_is();
		num_bits += current_NODE->code_is().bit_size_is();
	} // end for loop


	// determine number of bytes allocated for code storage
	if ((num_bits % 8) == 0)
		num_bytes = (num_bits / 8);
	else num_bytes = ((num_bits / 8) + 1);


	// initialize position
	position = 1;

	// read first character
	infile.read((char*) &new_char,1);

	// initialize current_NODE
	current_NODE = original_LIST.head_is();

	// read in number of bytes allocated for code storage
	for (i = 0; i < original_LIST.size(); i++)
	{	
		// initialize
		current_NODE = current_NODE->link_is();
		bit_size = current_NODE->code_is().bit_size_is();
		done_insert = false;
		tree_node = final_tree.root_is();

		// while the current character has not been printed to the outfile
		// continue to traverse the tree
		while (!done_insert)
		{

			switch (position)
			{

				case 1: test_char = ((int) new_char);

						// isolate selected position
						test_char = test_char >> 7;						

						break;

				case 2: test_char = ((int) new_char);
						
						// isolate selected position
						test_char = test_char << 1;
						test_char = test_char >> 7;
						
						break;

				case 3: test_char = ((int) new_char);

						// isolate selected position
						test_char = test_char << 2;
						test_char = test_char >> 7;
						
						break;

				case 4: test_char = ((int) new_char);

						// isolate selected position
						test_char = test_char << 3;
						test_char = test_char >> 7;
					
						break;

				case 5: test_char = ((int) new_char);

						// isolate selected position
						test_char = test_char << 4;
						test_char = test_char >> 7;
						
						break;

				case 6: test_char = ((int) new_char);

						// isolate selected position
						test_char = test_char << 5;
						test_char = test_char >> 7;
						
						break;

				case 7: test_char = ((int) new_char);

						// isolate selected position
						test_char = test_char << 6;
						test_char = test_char >> 7;
						
						break;

				case 8: test_char = ((int) new_char);

						// isolate selected position
						test_char = test_char << 7;
						test_char = test_char >> 7;
						
						break;

			} // end switch


			// traverse tree in proper direction
			// go right
			if (test_char == one)
			{
				// check to see if you are at the end of the code bit
				if (bit_size == 1)
				{
					// code complete
					// insert LEAF_NODE
					new_tnode = new LEAF_NODE(0,current_NODE->tree_is().root_is()->value_is());
					tree_node->edit_right(new_tnode);
					done_insert = true;			
				} // end if

				// code is not complete
				else
				{
					// if necessary, insert a node before you traverse tree
					if (tree_node->right_is() == NULL)
					{
						new_tnode = new IN_NODE(NULL,NULL,0);
						tree_node->edit_right(new_tnode);
						tree_node = tree_node->right_is();
					} // end if

					else
					{
						tree_node = tree_node->right_is();
					} // end else

				} // end else

			} // end if

			// go left
			else
			{
				// check to see if you are at the end of the bit
				if (bit_size == 1)
				{
					// code complete
					// insert LEAF_NODE
					new_tnode = new LEAF_NODE(0,current_NODE->tree_is().root_is()->value_is());
					tree_node->edit_left(new_tnode);	
					done_insert = true;				
				} // end if

				// code is not complete
				else
				{
					// if necessary, insert a node before you traverse tree
					if (tree_node->left_is() == NULL)
					{
						new_tnode = new IN_NODE(NULL,NULL,0);
						tree_node->edit_left(new_tnode);
						tree_node = tree_node->left_is();
					} // end if

					else
					{
						tree_node = tree_node->left_is();
					} // end else

				} // end else

			} // end else

			// decrement bit_size and set position value
			bit_size--;
			
			if (position < 8)
				position++;
			else
			{
				// read next character
				infile.read((char*) &new_char,1);
				position = 1;
			} // done else

		} // end while loop

	} // end for loop





} // read_codes()
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
// decode() reads the remaining encoded bytes from the infile and decodes the messages
// using a reconstructed huffman coding tree. The function then prints the original
// encoded message to the outfile.
//
void decode(ifstream& infile, ofstream& outfile, HUFF_TREE final_tree)
{
	// variable declarations
	int new_char = ' ';				// new character in the binary input stream

	unsigned char zero = ((int) 0);		// binary value used in tree traversal (0000 0000)
	unsigned char one = ((int) 1);		// binary value used in tree traversal (0000 0001)
	unsigned char two = ((int) 2);		// binary value used in tree traversal (0000 0010)
	unsigned char three = ((int) 4);	// binary value used in tree traversal (0000 0100)
	unsigned char four = ((int) 8);		// binary value used in tree traversal (0000 1000)
	unsigned char five = ((int) 16);	// binary value used in tree traversal (0001 0000)
	unsigned char six = ((int) 32);		// binary value used in tree traversal (0010 0000)
	unsigned char seven = ((int) 64);	// binary value used in tree traversal (0100 0000)
	unsigned char eight = ((int) 128);	// binary value used in tree traversal (1000 0000)
	unsigned char test_char;			// character used to determine direction
										// during tree traversal

	bool done_char;						// true when the current character has been found
										// in the HUFF_TREE
	HUFF_NODE* tree_node;				// position in final_tree during tree traversal
	int position;						// position in byte (1-8) during tree traversal
	
	long total_chars = 0;				// total number of encoded characters


	// read in number of encoded characters
	infile.read((char*) &new_char,1);
	total_chars += new_char;
	infile.read((char*) &new_char,1);
	total_chars += (new_char * 256);
	infile.read((char*) &new_char,1);
	total_chars += (new_char * 256 * 256);
	infile.read((char*) &new_char,1);
	total_chars += (new_char * 256 * 256 * 256);


	// initialize position
	position = 1;

	// read first character
	infile.read((char*) &new_char,1);

	// output all characters to the ofstream
	for (int i = 0; i < total_chars; i++)
	{
		
		done_char = false;
		tree_node = final_tree.root_is();

		// while the current character has not been printed to the outfile
		// continue to traverse the tree
		while (!done_char)
		{

			switch (position)
			{

				case 1: test_char = ((int) new_char);

						// isolate selected position
						test_char = test_char >> 7;
						
						break;

				case 2: test_char = ((int) new_char);
						
						// isolate selected position
						test_char = test_char << 1;
						test_char = test_char >> 7;

						break;

				case 3: test_char = ((int) new_char);

						// isolate selected position
						test_char = test_char << 2;
						test_char = test_char >> 7;

						break;

				case 4: test_char = ((int) new_char);

						// isolate selected position
						test_char = test_char << 3;
						test_char = test_char >> 7;

						break;

				case 5: test_char = ((int) new_char);

						// isolate selected position
						test_char = test_char << 4;
						test_char = test_char >> 7;

						break;

				case 6: test_char = ((int) new_char);

						// isolate selected position
						test_char = test_char << 5;
						test_char = test_char >> 7;

						break;

				case 7: test_char = ((int) new_char);

						// isolate selected position
						test_char = test_char << 6;
						test_char = test_char >> 7;

						break;

				case 8: test_char = ((int) new_char);

						// isolate selected position
						test_char = test_char << 7;
						test_char = test_char >> 7;

						break;

			} // end switch

			// traverse tree in proper direction
			if (test_char == one)
				tree_node = tree_node->right_is();
			else tree_node = tree_node->left_is();
			
			if (tree_node->is_leaf())
			{
				done_char = true;	
				outfile << tree_node->value_is();
			} // end if

			
			if (position < 8)
				position++;
			else
			{
				// read next character
				infile.read((char*) &new_char,1);
				position = 1;
			} // done else

		} // end while loop

	} // end for loop

	// confirm completion
	cout << "Decoding completed successfully..." << endl;

} // end decode()
//-------------------------------------------------------------------------------------------
