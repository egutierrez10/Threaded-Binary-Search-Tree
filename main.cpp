#include <iostream>
#include <vector>

#include "bstt.h"

using namespace std;

int main(){
	int key, val;
	
	bstt <string, string> n1;
//Add some key-value pairs
	n1.insert("1", "Hi");
    bstt <string, string> n2(n1);
	n1.dump(cout);
	n2.dump(cout);

	bstt<int, string> tr;
	tr.begin();
	while(tr.next(key))
		cout << key << endl;
	tr.insert(2, "Hi");
	tr.insert(1, "Hello");
	tr.insert(3, "Hello");
	cout << tr(2) << endl;
	tr.dump(cout);
	bstt<int, int>  tree;

	tree.insert(30, 30);
	tree.insert(50, 50);
	tree.insert(15, 15);
	tree.insert(8, 8);
	tree.insert(25, 25);
	tree.insert(70, 70);
	tree.insert(60, 60);
	tree.insert(20, 20);
	tree.insert(28, 28);
	
	tree.insert(9, 9);
	tree.insert(6, 6);
	tree.insert(10, 10);
	tree.insert(8,8);
 	cout << tree.size() << endl;

 	tree.begin();
 	while(tree.next(key)){
 		cout << key << " ";
 	}
 	cout << endl;
	if(tree.search(8,val))
		cout << "Found it " << val << endl;
	cout << tree(8) << endl;
	tree.dump(cout);
	
	bstt<int, int>  tree2;
	tree2 = tree;
	tree2.dump(cout);
	return 0;
}