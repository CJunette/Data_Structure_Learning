// 008_Homework_8-1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "SplayTree.h"
using namespace std;

int main()
{
    Splay<int> spy;

    spy.insert(1);
    spy.insert(1);
    spy.insert(1);
    spy.insert(1);
    spy.insert(1);
    cout << spy.searchAll(1) << endl;
    spy.removeAll(1);
    cout << spy.searchAll(1) << endl;
}