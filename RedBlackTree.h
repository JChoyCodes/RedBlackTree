//Assignment 4
//Jeremy Choy

#pragma once
//#include <iostream>
#include <vector>
//#include <string>
using std::vector;
//using std::string;
//using std::cout;
//using std::endl;

template <class T1, class T2>
class NodeT{

public:
	NodeT(T1 theKey, T2 theValue){
		left = nullptr;
		right = nullptr;
		parent = nullptr;
		key = theKey;
		value = theValue;
		isBlack = false;
	}

	T1 key;	
	T2 value;
	NodeT<T1, T2>* left;
	NodeT<T1, T2>* right;
	NodeT<T1, T2>* parent;
	bool isBlack;
};

template <class T1, class T2>
class RedBlackTree{
public:
	//Default constructor
	RedBlackTree();
	//Copy constructor
	RedBlackTree(const RedBlackTree & rbt);
	//Overloaded assignment operator
	RedBlackTree & operator=(const RedBlackTree & rbt);
	//Destructor
	~RedBlackTree();
	//Inserts node containing a key and value into the red black tree.
	//Returns true and updates the size upon successful insertion, false otherwise.
	bool insert(T1 newKey, T2 newValue);
	//Removes node containing its template key parameter
	//Returns true upon successful removal, false otherwise.
	bool remove(T1 targetKey);
	//searches to see if key exists in the tree
	bool search(T1 targetKey) const;
	//Returns a vector containing all values between the two key parameters
	vector<T2> search(T1 key1, T1 key2);
	//Returns a vector containing all values in the tree in ascending key order
	vector<T2> values();
	//Retruns a vector containing all keys in the tree in ascending order.
	vector<T1> keys();
	//Returns the size of the tree;
	int size() const;
	//Returns a node pointing to the root
	NodeT<T1, T2>* getRoot();

	//HELPER FOR TESTING
	void print();
	void vectorPrint(vector<T1> v, vector<T1> v2, vector<T1> v3);
	void teststruct();
	
	
private:
	NodeT<T1, T2>* root;
	int theSize;
	//creates copy of RBT
	NodeT<T1, T2>* copy(NodeT<T1, T2>* nd);
	//Performs regular BST insertion
	NodeT<T1, T2>* bstInsert(T1 key, T2 value);
	//Returns a pointer to the predecessor
	NodeT<T1, T2>* predecessor(NodeT<T1, T2>* nd);
	//Rebalances RBT when deleted node is black
	void rbFix(NodeT<T1, T2>* x, NodeT<T1, T2>* z);
	//rotates tree left around NodeT x
	void leftRotate(NodeT<T1, T2>* x);
	//Rotates tree around NodeT x
	void rightRotate(NodeT<T1, T2>* x);
	//helper for rbFix
	bool hasTwoBlack(NodeT<T1, T2>* nd);
	//frees dynamic memory
	void clear(NodeT<T1, T2>* nd);
	//checks if RBT is empty
	bool isEmpty() const;
	//Helper for search(T1 Key, T2 Key)
	void inOrderVectorValues(vector<T2> &v, NodeT<T1, T2>* nd, T1 key1, T1 key2);
	//Helper for values
	void inOrderVectorValues(vector<T2> &v, NodeT<T1, T2>* nd);
	//Helper for keys
	void inOrderVectorKeys(vector<T1> &v, NodeT<T1, T2>* nd);

	//HELPER FOR TESTING;
	void inorderPrint(NodeT<T1, T2>* nd);
};

template <class T1, class T2>
RedBlackTree<T1, T2>::RedBlackTree(){
	root = nullptr;
	theSize = 0;
}

template <class T1, class T2>
RedBlackTree<T1, T2>::RedBlackTree(const RedBlackTree & rbt){
	root = nullptr;
	theSize = 0;
	if(isEmpty()){
		NodeT<T1, T2>* current = rbt.root;
		root = copy(current);
	}
}

template <class T1, class T2>
RedBlackTree<T1, T2> & RedBlackTree<T1, T2>::operator=(const RedBlackTree & rbt){
	if(this != &rbt){
		clear(root);
		NodeT<T1, T2>* current = rbt.root;
		root = copy(current);
	}
	return *this;
}

template <class T1, class T2>
RedBlackTree<T1, T2>::~RedBlackTree(){
	clear(root);
}

template <class T1, class T2>
bool RedBlackTree<T1, T2>::insert(T1 key, T2 value){
	NodeT<T1, T2>* current = bstInsert(key, value);		//Perform normal BST insertion, then balance the tree
	if (current == nullptr){
		return false;
	}
	else{
		while(current != root && current->parent->isBlack == false){	
			if(current->parent == current->parent->parent->left){		//check if parent is left child
				NodeT<T1, T2>* uncle = current->parent->parent->right;
				if(uncle != nullptr && uncle->isBlack == false){
					current->parent->isBlack = true;
					uncle->isBlack = true;
					current->parent->parent-> isBlack = false;
					current = current->parent->parent;
					
				}
				else{	//uncle is red
					if (current == current->parent->right){		//case 2: The path from current to root is a "triangle"
						current = current->parent;	//turns case 2 into case 3
						leftRotate(current);
					}
					current->parent->isBlack=true;		//case 3: The path from current to root is a straight line.
					current->parent->parent->isBlack = false;	//recolour the grandparent of current red and perform a rotation
					rightRotate(current->parent->parent);
				}
			}
			else{	//symmetric to if
				NodeT<T1, T2>* uncle = current->parent->parent->left;
				if(uncle != nullptr && uncle->isBlack == false){
					current->parent->isBlack = true;
					uncle->isBlack = true;
					current->parent->parent-> isBlack = false;
					current = current->parent->parent;
				}
				else{	
					if (current == current->parent->left){
						current = current->parent;	
						rightRotate(current);
					}
					current->parent->isBlack=true;
					current->parent->parent->isBlack = false;
					leftRotate(current->parent->parent);
				}
			}		
		}
		root->isBlack = true;
		return true;
	}
	
}

template <class T1, class T2>
bool RedBlackTree<T1, T2>::remove(T1 targetKey){
	if(isEmpty()){
		return false;
	}

	NodeT<T1, T2>* z = root;
	NodeT<T1, T2>* y = nullptr;
	NodeT<T1, T2>* x = nullptr;
	NodeT<T1, T2>* xParent = nullptr;
	while(z->key != targetKey){			//search for the targetKey
		if(targetKey < z->key){
				z = z->left;
				if(z == nullptr){
					return false;
				}
			}
		else if(targetKey > z->key){
			z = z->right;
			if(z == nullptr){
				return false;
			}
		}
	}
	if(size() == 1){	//only one node, so just delete it.
		delete z;
		theSize--;
		root = nullptr;
		return true;
	}
	if(z->left == nullptr || z->right == nullptr){		//z has one or no children
		y = z;
	}
	else{
		y = predecessor(z);		//z has two children so find the predecessor
	}
	if(y->left != nullptr){		//y's only child is to the left.
		xParent = y;
		x = y->left;
	}
	else{
		xParent = y;			//y's only child is right or it has no children
		x = y->right;
	}
	if(x!=nullptr){				//attach x to y's parent
		xParent = y->parent;
		x->parent = y->parent;
	}
	
	if(y->parent == nullptr){	//update the root if y is a root
		root = x;
		xParent = x->parent;
	}
	else{
		if(y == y->parent->left){	//detach x from y
			xParent = y->parent;
			y->parent->left = x;

		}
		else{
			xParent = y->parent;
			y->parent->right = x;
			//cout << "xParent->left: " << xParent->left->key << endl;
		}
	}
	if(y!=z){			//y has been "moved up"
		z->key = y->key;		
	}
	
	if(y->isBlack == true){
		//cout << "xParent->Key: " << xParent->key << endl;
		rbFix(x, xParent);
	}
	delete y;
	theSize--;
	return true;
}

template <class T1, class T2>
bool RedBlackTree<T1, T2>::search(T1 targetKey) const{
	if(!isEmpty()){
		NodeT<T1, T2>* current = root;
		while(current != nullptr){
			if(current->key == targetKey){
				return true;
			}
			else if(current->key < targetKey){
				current = current->left;
			}
			else if(current->key > targetKey){
				current = current->right;
			}
		}
	}
	return false;
}

template <class T1, class T2>
vector<T2> RedBlackTree<T1, T2>::search(T1 key1, T1 key2){
	vector<T2> values;
	if(!isEmpty()){
		inOrderVectorValues(values, root, key1, key2);
	}
	return values;
}

template <class T1, class T2>
vector<T2> RedBlackTree<T1, T2>::values(){
	vector<T2> theValues;
	if(!isEmpty()){
		inOrderVectorValues(theValues, root);
	}
	return theValues;
}

template <class T1, class T2>
vector<T1> RedBlackTree<T1, T2>::keys(){
	vector<T1> theKeys;
	if(!isEmpty()){
		inOrderVectorKeys(theKeys, root);
	}
	return theKeys;
}

template <class T1, class T2>
int RedBlackTree<T1, T2>::size() const{
	return theSize;
}

template <class T1, class T2>
NodeT<T1, T2>* RedBlackTree<T1, T2>::getRoot(){
	if(isEmpty()){
		return nullptr;
	}
	NodeT<T1, T2>* theRoot = root;
	return theRoot;
}

				/*Private Helper Functions*/
/*****************************************************************************************/

template <class T1, class T2>
NodeT<T1, T2>* RedBlackTree<T1, T2>::bstInsert(T1 newKey, T2 newValue){
	NodeT<T1, T2>* newNode = new NodeT<T1, T2>(newKey, newValue);
	NodeT<T1, T2>* previous = root;
	NodeT<T1, T2>* next = root;

	if (isEmpty()){
		root = newNode;
		newNode->isBlack = true;
		theSize++;
		//cout << "Inserted the root " << newNode->key << endl;
		return newNode;
	}

	while(next != nullptr){
		if(newNode->key == next->key){	//duplicate found, so return and don't insert
			return nullptr;
		}
		if(newNode->key < next->key){
			previous = next;
			//cout << "Go left of " << previous->key << endl;
			next = next->left;
		}
		else{
			previous = next;
			//cout << "Go right of " << previous-> key << endl;
			next = next->right;
		}
	}

	if(newNode->key < previous->key){
		previous->left = newNode;
		newNode->parent = previous;
		//cout << "Insert " << newNode->key << " to the left of " << newNode-> parent->key << endl;
		theSize++;
		return newNode;
	}
	else{
		previous->right = newNode;
		newNode->parent = previous;
		//cout << "Insert " << newNode->key << " to the right of "<< newNode->parent->key << endl;
		theSize++;
		return newNode;
	}
}

template <class T1, class T2>
NodeT<T1, T2>* RedBlackTree<T1, T2>::predecessor(NodeT<T1, T2>* nd){
	if (nd!= nullptr){
		nd = nd->left;
	}

	while(nd->right != nullptr){
		if(nd->right == nullptr){
			return nd;
		}
		nd = nd->right;
	}
	return nd;
}

template <class T1, class T2>
void RedBlackTree<T1, T2>::leftRotate(NodeT<T1, T2>* x){
	NodeT<T1, T2>* y = x->right;	//make poitner to x's right child
	x->right = y->left;				//y's left subtree becomes the right subtree of x
	if(y->left!=nullptr){
		y->left->parent = x;
	}
	y->parent = x->parent;		//y's parent is linked to x's parent
	if(x->parent == nullptr){	//x was a root, so set the root to y
		root = y;
	}
	else if(x == x->parent->left){	//x is left child
		x->parent->left = y;	//so make the left subtree of x equal to y
	}
	else{						//x was a right child
		x->parent->right = y;	//make the right subtree of x equel to y
	}
	y->left = x;
	x->parent = y;
}

template <class T1, class T2>
void RedBlackTree<T1, T2>::rightRotate(NodeT<T1, T2>* x){	//symmetric to leftRotate
	NodeT<T1, T2>* y = x->left; 
	x->left = y->right;			
	if(y->right!=nullptr){		
		y->right->parent = x;
	}
	y->parent = x->parent;		
	if(x->parent == nullptr){	
		root = y;
	}
	else if(x == x->parent->right){	
		x->parent->right = y;
	}
	else {	
		x->parent->left = y;
	}

	y->right = x;
	x->parent = y;
}

template <class T1, class T2>
void RedBlackTree<T1, T2>::rbFix(NodeT<T1, T2>* x, NodeT<T1, T2>* xParent){
	while(x == nullptr || (x != root && x->isBlack == true)){
		if(x == xParent->left){							//x is a left child
			NodeT<T1, T2>* y = xParent->right;			//y is the sibling
			if( y != nullptr && y->isBlack == false){	//Case 1: y is red, colour xParent black and rotate to fix violation
				y->isBlack = true;
				xParent->isBlack = false;
				leftRotate(xParent);
				y = xParent->right;
			}
			if(hasTwoBlack(y)){						//Case 2: y has two black children, so make y red to balance the black height and push the fix up the tree
				y->isBlack = false;
				x = xParent;
				xParent = x->parent;
			}
			else{
				if(y->right == nullptr || y->right->isBlack == true){	  //Case 3: y's left child is red and its right child is black. Make y red and perform right rotation.
					if(y->left != nullptr){								  //        to turn it into Case 4.
						y->left->isBlack = true;
					}
					y->isBlack = false;
					rightRotate(y);
					y = xParent->right;
				}
				y->isBlack = xParent->isBlack;							//Case 4: y's right child is red, change the colour of xParent and y->right and perform a left rotation
				xParent->isBlack = true;								//        followed by making x the root to terminate the while loop
				y->right->isBlack = true;
				leftRotate(xParent);
				x = root;
				//cout << "new root: " << root->key << endl;
			}
		}
		else{		//symmetric to if
			NodeT<T1, T2>* y = xParent->left;
			if(y!=nullptr && y->isBlack == false){
				y->isBlack = true;
				xParent->isBlack = false;
				rightRotate(xParent);
				y = xParent->left;
			}
			if(hasTwoBlack(y)){
				y->isBlack = false;
				x = xParent;
				xParent = x->parent;
			}
			else{	
				if(y->left == nullptr || y->left->isBlack == true){
					if(y->right != nullptr){
						y->right->isBlack = true;
					}
					y->isBlack = false;
					leftRotate(y);
					y = xParent->left;
				}
				y->isBlack = xParent->isBlack;
				xParent->isBlack = true;
				y->left->isBlack = true;
				rightRotate(xParent);
				x = root;
				//cout << "new root: " << root->key << endl;
			}
		}
	}
	x->isBlack = true;
		
}

template <class T1, class T2>
bool RedBlackTree<T1, T2>::hasTwoBlack(NodeT<T1, T2>* nd){
	//cout << "nd->key" << nd->key << endl;
	if(nd == nullptr){
		return false;
	} 
	if(nd->left == nullptr && nd->right != nullptr){
		return false;
	}
	if(nd->right == nullptr && nd->left != nullptr){
		return false;
	}

	if(nd->left == nullptr && nd-> right == nullptr){
		return true;
	}
	
	if(nd->right != nullptr){
		if(nd->left == nullptr && nd->right->isBlack == true){
			return true;
		}
	}
	
	if(nd->left != nullptr){
		if(nd->right == nullptr && nd->left->isBlack == true){
			return true;
		}
	}
	if(nd->left->isBlack == true && nd->right->isBlack == true){
		return true;
	}
	return false;
}

template <class T1, class T2>
NodeT<T1, T2>* RedBlackTree<T1, T2>::copy(NodeT<T1, T2>* nd){
	if(nd == nullptr){
		return nullptr;
	}
	theSize++;
	NodeT<T1, T2>* newNode = new NodeT<T1, T2>(nd->key, nd->value);
	newNode->isBlack = nd->isBlack;
	newNode->left = copy(nd->left);
	newNode->right = copy(nd->right);
	return newNode;
}

template <class T1, class T2>
void RedBlackTree<T1, T2>::clear(NodeT<T1, T2>* nd){
	if (nd == nullptr){
		return;
	}
	clear(nd->left);
	clear(nd->right);
	delete nd;
	theSize = 0;
}

template <class T1, class T2>
bool RedBlackTree<T1, T2>::isEmpty() const{
	if (root == nullptr){
		return true;
	}
	return false;
}

template <class T1, class T2>
void RedBlackTree<T1, T2>::inOrderVectorValues(vector<T2> &v, NodeT<T1, T2>* nd, T1 key1, T1 key2){
	if(nd == nullptr){	
		return;
	}
	if(key1 > key2){		//if key1 is greater than key2, swap values to maintain the range
		T1 temp = key1;
		key1 = key2;
		key2 = temp;
	}
	inOrderVectorValues(v, nd->left, key1, key2);
	if(nd->key >= key1 && nd->key <= key2){
		v.push_back(nd->value);
	}
	inOrderVectorValues(v, nd->right, key1, key2);
}

template <class T1, class T2>
void RedBlackTree<T1, T2>::inOrderVectorValues(vector<T2> &v, NodeT<T1, T2>* nd){
	if(nd == nullptr){
		return;
	}
	inOrderVectorValues(v, nd->left);
	v.push_back(nd->value);
	inOrderVectorValues(v, nd->right);
}

template <class T1, class T2>
void RedBlackTree<T1, T2>::inOrderVectorKeys(vector<T1> &v, NodeT<T1, T2>* nd){
	if(nd == nullptr){
		return;
	}
	inOrderVectorKeys(v, nd->left);
	v.push_back(nd->key);
	inOrderVectorKeys(v, nd->right);
}

