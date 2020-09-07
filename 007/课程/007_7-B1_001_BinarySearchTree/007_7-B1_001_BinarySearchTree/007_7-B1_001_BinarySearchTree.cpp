// 007_7-B1_001_BinarySearchTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "BST.h"
using namespace std;

int main()
{
    //BST基本接口测试。
    {
        BST<int> bst;
        bst.insert(36);
        bst.insert(27);
        bst.insert(6);
        bst.insert(58);
        bst.insert(69);
        bst.insert(64);
        bst.insert(53);
        bst.insert(46);
        bst.insert(40);
        bst.insert(55);

        bst.remove(58);
    }
    
}
