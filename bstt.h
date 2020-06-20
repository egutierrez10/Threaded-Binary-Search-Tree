/*bstt.h*/

//
// Threaded binary search tree
//

#pragma once

#include <iostream>

using namespace std;

template<typename KeyT, typename ValueT>
class bstt
{
private:
  struct NODE
  {
    KeyT   Key;
    ValueT Value;
    NODE*  Left;
    NODE*  Right;
    bool   isThreaded;
  };

  NODE* Root;  // pointer to root node of tree (nullptr if empty)
  int   Size;  // # of nodes in the tree (0 if empty)
  NODE* state; //pointer holding current internal state in the tree
  
  // 
  // _clearTree:
  // Parameters: Pointer to root node of "this" tree
  // Helper function to recursively delete each node in the tree from memory by performing a postorder traversal.
  //
  void _clearTree(NODE* cur){
	if(cur == nullptr)
		  return;
	  else{
		  _clearTree(cur->Left);
		  if(cur->isThreaded)
			  _clearTree(nullptr);
		  else
			  _clearTree(cur->Right);
		  delete cur;
	  }
	}
	
  // 
  // _inorderDump:
  // Parameters: Pointer to root node of "this" tree
  // Helper function to recursively dump out (key, value) pairs from each node in the tree by performing a inorder traversal.
  // A different value is to be printed out depending on if node is threaded or not. 
  //
	void _inorderDump(ostream& output, NODE* cur)const{
	if(cur == nullptr)
		  return;
	  else{
		  _inorderDump(output, cur->Left);
		  
		  if(cur->isThreaded){ //check to see if current node is threaded 
			  if(cur->Right == nullptr) //case that we hit the last node that points to nullptr
				output << "(" << cur->Key << "," << cur->Value << ")" << endl;
			  else 
			    output << "(" << cur->Key << "," << cur->Value << "," << cur->Right->Key << ")" << endl;
			  _inorderDump(output, nullptr);
		  }else{
			  output << "(" << cur->Key << "," << cur->Value << ")" << endl;
			  _inorderDump(output, cur->Right);
			}
	  }
	}
	
  // 
  // _copyInsert:
  // Parameters: Pointer to root node of "other" tree
  // Helper function to recursively copy each node from one tree and into another by performing a preorder traversal. 
  // A check for threaded is done to correctly threaded the current node to the next node. 
  //
	void _copyInsert(NODE* cur){
	  if(cur == nullptr){
		  return;
	  }else{
		  this->insert(cur->Key, cur->Value);
		  _copyInsert(cur->Left);	 
		  if(cur->isThreaded){ //check to see if current node is threaded 
			  _copyInsert(nullptr);
		  }else{
			  _copyInsert(cur->Right);
		  }
		  
	  }
	}
	
  // 
  // _findSmallest:
  // Parameters: Pointer to current internal state node 
  // Helper function to find the next inorder node to update state pointer to point to. 
  // Function is to head right and head left until nullptr is hit. State is updated as we traverse leftmost.
  // 
	void _findSmallest(NODE* cur){
		state = cur->Right;
		while(state->Left != nullptr){
			state = state->Left;
		}			
	}
public:
  //
  // default constructor:
  //
  // Creates an empty tree.
  //
  bstt()
  {
    Root = nullptr;
    Size = 0;
  }

  //
  // copy constructor
  //
  bstt(const bstt& other)
  {
    //
    // TODO DONE 
    //
    this->Root = nullptr; //set "this" root to null
	this->Size = 0; //set size of tree for "this" to zero
    this->_copyInsert(other.Root); //call to helper function to copy each node
  }

	//
  // destructor:
  //
  // Called automatically by system when tree is about to be destroyed;
  // this is our last chance to free any resources / memory used by
  // this tree.
  //
  virtual ~bstt()
  {
    //
    // TODO DONE
    //
    _clearTree(Root); //call to helper function to deallocate memory 
  }
  
  //
  // operator=
  //
  // Clears "this" tree and then makes a copy of the "other" tree.
  //
  bstt& operator=(const bstt& other)
  {
    //
    // TODO: DONE
    //
	this->clear(); //call clear function to clear tree from memory
	this->_copyInsert(other.Root); //call helper function to copy and insert each node 
    return *this; //return a pointer to the new tree
  }

  //
  // clear:
  //
  // Clears the contents of the tree, resetting the tree to empty.
  //
  void clear()
  {
    //
    // TODO DONE
    //
    _clearTree(Root); //call helper function to clear the tree 
	//set root and size to their proper default values 
	Root = nullptr; 
	Size = 0;  
  }

  // 
  // size:
  //
  // Returns the # of nodes in the tree, 0 if empty.
  //
  // Time complexity:  O(1) 
  //
  int size() const
  {
    return Size;
  }
  
  // 
  // search:
  //
  // Searches the tree for the given key, returning true if found
  // and false if not.  If the key is found, the corresponding value
  // is returned via the reference parameter.
  //
  // Time complexity:  O(lgN) on average
  //
  bool search(KeyT key, ValueT& value) const
  {
    //
    // TODO DONE
    //
    NODE* cur = Root;
	while(cur != nullptr){
		if(cur->Key == key){ //Key exists in tree and value is returned 
			value = cur->Value;
			return true;
		}else if(key < cur->Key){
			cur = cur->Left;
		}else{
			if(cur->isThreaded) //check if current node is threaded or not
				cur = nullptr; //key does not exist in tree so loop is terminated 
			else 
				cur = cur->Right; //right node is not threaded, continue searching
		}
	}
		return false;
  }

  //
  // insert
  //
  // Inserts the given key into the tree; if the key has already been insert then
  // the function returns without changing the tree.
  //
  // Time complexity:  O(lgN) on average
  //
  void insert(KeyT key, ValueT value)
  {
    //
    // TODO DONE
    //

	NODE* prev = nullptr;
    NODE* cur = Root;
    while (cur != nullptr)
    {
      if (key == cur->Key)  // already in tree
        return;
      if (key < cur->Key)  // search left:
      {
        prev = cur;
        cur = cur->Left;
      }
      else //search right: 
      {
        prev = cur;
        if(cur->isThreaded) //check if current node is threaded or not
			cur = nullptr; 
		else 
			cur = cur->Right;
      }
    }//while
	
	//Allocate memory for a new node and assign corresponding values to it
    NODE* newNode = new NODE();
    newNode->Key = key;
    newNode->Value = value;
    newNode->Left = nullptr;
	newNode->isThreaded = true; //true for node is to be threaded rather to root, previous node, or nullptr
	
    if (prev == nullptr){ //node is to be the root of the tree
	  newNode->Right = nullptr;	//node is threaded to nullptr
      Root = newNode;
    }else if (key < prev->Key){ //node is assigned to the left of the parent node
	  prev->Left = newNode; 
	  newNode->Right = prev; //node is threaded to previous node
    }else{
	  newNode->Right = prev->Right; //node is threaded to whatever the previous node was threading to
      prev->Right = newNode; 
	  prev->isThreaded = false; 
	  }

    Size++;
  }

  //
  // []
  //
  // Returns the value for the given key; if the key is not found,
  // the default value ValueT{} is returned.
  //
  // Time complexity:  O(lgN) on average
  //
  ValueT operator[](KeyT key) const
  {
    //
    // TODO DONE
    //
    ValueT val;
	if(search(key, val)) //call function to determine if current key already exists in tree.
		return val; // If it does then return its corresponding value
    return ValueT{ };
  }

  //
  // ()
  //
  // Finds the key in the tree, and returns the key to the "right".
  // If the right is threaded, this will be the next inorder key.
  // if the right is not threaded, it will be the key of whatever
  // node is immediately to the right.
  //
  // If no such key exists, or there is no key to the "right", the
  // default key value KeyT{} is returned.
  //
  // Time complexity:  O(lgN) on average
  //
  KeyT operator()(KeyT key) const
  {
    //
    // TODO DONE
    //
    NODE* cur = Root;
	while(cur != nullptr){ //loop through the tree
		if(cur->Key == key){ //current key does exist return the key 
 			if(cur->Right == nullptr)
				break;
			else
				return cur->Right->Key;
		}else if(key < cur->Key) //search left: 
			cur = cur->Left;
		else{ //search right: 
			if(cur->isThreaded) //determine if current node is threaded
				break;
			else 
				cur = cur->Right;
		}
	}
    return KeyT{ };
  }

  //
  // begin
  //
  // Resets internal state for an inorder traversal.  After the 
  // call to begin(), the internal state denotes the first inorder
  // key; this ensure that first call to next() function returns
  // the first inorder key.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) on average
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
  void begin()
  {
    //
    // TODO DONE
    //
    state = Root; //set state data member equal to the root of the tree
	if(state == nullptr)
		return;
	while(state->Left != nullptr) //keep looping until we hit the leftmost node in the tree
		state = state->Left;
  }

  //
  // next
  //
  // Uses the internal state to return the next inorder key, and 
  // then advances the internal state in anticipation of future
  // calls.  If a key is in fact returned (via the reference 
  // parameter), true is also returned.
  //
  // False is returned when the internal state has reached null,
  // meaning no more keys are available.  This is the end of the
  // inorder traversal.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) on average
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
  bool next(KeyT& key)
  {
    //
    // TODO DONE
    //
	if(state != nullptr){ //current node is not null and we can return a value for key
		key = state->Key;
		if(state->isThreaded) //determine if node is threaded or not
			state = state->Right; //if so head right
		else 
			_findSmallest(state); //call helper function to find next inorder node	
		return true;
	}
    return false;
  }
  
  //
  // dump
  // 
  // Dumps the contents of the tree to the output stream, using a
  // recursive inorder traversal.
  //
  void dump(ostream& output) const
  {
    output << "**************************************************" << endl;
    output << "********************* BSTT ***********************" << endl;

    output << "** size: " << this->size() << endl;

    //
    // inorder traversal, with one output per line: either 
    // (key,value) or (key,value,THREAD)
    //
    // (key,value) if the node is not threaded OR thread==nullptr
    // (key,value,THREAD) if the node is threaded and THREAD denotes the next inorder key
    //
	
	_inorderDump(output, Root);
	
    //
    // TODO DONE
    //

    output << "**************************************************" << endl;
  }
	
};

