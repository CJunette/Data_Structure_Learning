// 007_Homework_7-16.cpp : This file contains the 'main' function. Program execution begins and ends there.
//结合AVL树的代码和7-10中BST树重复数搜索的代码，为AVL数添加重复数搜索的功能。

#include <iostream>
#include "AVL.h"
using namespace std;

int main()
{
    AVL<int> avl;

    avl.insert(4);
    avl.insert(2);
    avl.insert(5);
    avl.insert(1);
    avl.insert(3);
    avl.insert(4);
    avl.insert(6);
    avl.insert(4);
    avl.insert(4);

    cout << avl.searchAll(4) << endl;

    avl.remove(4);
    cout << avl.searchAll(4) << endl;
    
    avl.remove(1);
    avl.remove(3);
}
