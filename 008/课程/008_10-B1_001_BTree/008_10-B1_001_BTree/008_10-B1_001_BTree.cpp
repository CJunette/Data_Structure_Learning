// 008_10-B1_001_BTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "BTree.h"
using namespace std;

int main()
{	
	BTNode<int> *n20 = new BTNode<int>(19);

	BTNode<int> *n21 = new BTNode<int>(41);
	n21->key.insert(1, 51);
	n21->child.insert(1, NULL);
	
	BTNode<int> *n22 = new BTNode<int>(75);

	BTNode<int> *n23 = new BTNode<int>(79);
	n23->key.insert(1, 84);
	n23->child.insert(1, NULL);
	
	BTNode<int> *n24 = new BTNode<int>(97);

	BTNode<int> *n10 = new BTNode<int>(36, n20, n21);;
	n20->_parent = n10;
	n21->_parent = n10;

	BTNode<int> *n11 = new BTNode<int>(77, n22, n23);
	n11->key.insert(1, 89);
	n11->child.insert(2, n24);
	n22->_parent = n11;
	n23->_parent = n11;
	n24->_parent = n11;

	BTNode<int> *n00 = new BTNode<int>(53, n10, n11);
	n10->_parent = n00;
	n11->_parent = n00;

	//添加节点。
	/*
	{
		int order = 3;
		BTree<int> btr(order);

		btr._size = 11;
		btr._root = &n00;

		btr.show();
		cout << endl;

		btr.insert(23);
		btr.show();
		cout << endl;
		btr.insert(29);
		btr.show();
		cout << endl;
		btr.insert(45);
		btr.show();
		cout << endl;
		btr.insert(87);
		btr.show();
		cout << endl;
	}
	*/

	//删除底层节点。
	/*
	{
		int order = 3;
		BTree<int> btr(order);

		btr._size = 11;
		btr._root = n00;

		btr.show();
		cout << endl;

		btr.remove(41);
		btr.show();
		cout << endl;
		btr.remove(75);
		btr.show();
		cout << endl;
		btr.remove(84);
		btr.show();
		cout << endl;
		btr.remove(51);
		btr.show();
		cout << endl;
	}
	*/


	{
		int order = 3;
		BTree<int> btr(order);

		btr._size = 11;
		btr._root = n00;

		btr.show();
		cout << endl;

		btr.remove(36);
		btr.show();
		cout << endl;
		btr.remove(41);
		btr.show();
		cout << endl;
		btr.remove(53);
		btr.show();
		cout << endl;
	}

	//测试vector用。
	/*
	Vector<int> v;
	for(int i = 0; i < 5; i++)
	{
		v.push_back(i);
	}

	for(int i = 0; i < 5; i++)
	{
		cout << v.searchLinear(i - 1) << endl;
	}
	*/
}