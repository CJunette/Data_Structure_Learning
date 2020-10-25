// 008_10-A1_001_SplayTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "SplayTree.h"
using namespace std;


int main()
{
    Splay<int> splay;

    splay.insert(1);
    splay.insert(2);
    splay.insert(3);
    splay.insert(4);
    splay.insert(5);
    splay.insert(6);
    splay.insert(7);

    splay.remove(2);
}