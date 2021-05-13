#include <iostream>
#include <string>
#include "RedBlackTree.h"
using namespace std;

template <class T1, class T2>
void RedBlackTree<T1, T2>::print(){
	if(isEmpty()){
		cout << "Empty tree\n";
		return;
	}
	inorderPrint(root);
	cout << "\nSize: " << size() << endl;
	cout << "Root key: " << getRoot()->key << endl;
}

template <class T1, class T2>
void RedBlackTree<T1, T2>::inorderPrint(NodeT<T1, T2>* nd){
	int colour = 0;
	if(nd != nullptr && nd->isBlack == true){
		colour = 1;
	}
	if (nd == nullptr){
		return;
	}
	inorderPrint(nd->left);
	cout << "(" << nd->key << ", " << nd->value <<", " << colour << ") ";
	inorderPrint(nd->right);
	
}

template <class T1, class T2>
void RedBlackTree<T1,T2>::teststruct(){
	NodeT<T1,T2>* nd = root;
		while(nd!=nullptr){
			cout << "nd->key: " << nd->key << endl;
			nd = nd->left;
		}
}

void simpleTest()
{
	// Int Tree Tests	
	RedBlackTree<int, char> rb1;
	if (rb1.insert(6, 'f')) cout << "inserted 42" << endl;
	rb1.insert(10, 'j');
	rb1.insert(2, 'b');
	RedBlackTree<int, char> rb2(rb1);
 
	if (rb1.remove(2)) cout << "removed 2" << endl;
	if (rb1.search(6)) cout << "found 6" << endl;
 
	vector<char> v1 = rb1.search(8, 21); //should contain j
	vector<char> v2 = rb1.values(); //should contain {f, j}
	vector<int> v3 = rb1.keys(); //should contain {6, 10}
 
	cout << "size = " << rb1.size() << endl; //should be 2
	NodeT<int, char>* pRoot = rb1.getRoot(); //BAD design - for our testing only
}

void test1(){
	RedBlackTree <int, string> rb1;
	rb1.insert(10, "fat");
	rb1.insert(11, "ree");
	rb1.insert(12, "gay");
	rb1.insert(13, "app");
	rb1.insert(14, "zebra");
	rb1.insert(15, "xbox");
	rb1.insert(21, "retard");
	rb1.insert(16,"bob");
	rb1.insert(19,"ok");
	rb1.insert(20, "f");
	rb1.insert(18, "sda");
	rb1.teststruct();
	rb1.remove(12);
	rb1.remove(10);
	rb1.print();
	rb1.remove(15);
	

	cout << endl;
	vector<string> v1 = rb1.values();
	for(int i = 0; i < v1.size(); i++){
		cout << "" << v1[i] << endl;
	}
	rb1.print();
}

void test2(){
	RedBlackTree<int, string> rb1;
	for(int i = 15; i >0; i--){
		rb1.insert(i, "g");
	}
	rb1.print();

	rb1.remove(7);
	rb1.remove(14);
	rb1.remove(13);
	rb1.remove(8);
	rb1.print();
}

void test3(){
	RedBlackTree<int, string> rb1;
	for(int i = 0; i <50; i+=4){
		rb1.insert(i,"g");
	}
	rb1.insert(13,"x");
	rb1.remove(42);
	rb1.print();
	rb1.remove(16);
	rb1.remove(36);
	rb1.print();
	
}

void test5(){
	RedBlackTree<int, string> rb1;
	rb1.insert(0, "x");
	rb1.insert(4, "b");
	rb1.insert(8, "z");
	rb1.print();
}

int main(){
	test3();
}