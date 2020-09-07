// 007_Homework_7-10.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "BST.h"
using namespace std;

int main()
{
    BST<int> bst;

    bst.insert(4);
    bst.insert(2);
    bst.insert(5);
    bst.insert(1);
    bst.insert(3);
    bst.insert(4);
    bst.insert(6);  
    bst.insert(4);

    cout << bst.searchAll(4) << endl;

    bst.remove(4);
    cout << bst.searchAll(4) << endl;
}
