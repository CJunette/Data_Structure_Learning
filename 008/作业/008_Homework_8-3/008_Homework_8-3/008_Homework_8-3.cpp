// 008_Homework_8-3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "RBTree.h"
using namespace std;

int main()
{
    RBTree<int> rbt;
    rbt.insert(1);
    rbt.insert(1);
    rbt.insert(1);
    rbt.insert(1);
    rbt.insert(1);
    rbt.insert(1);
    rbt.show();
    cout << endl;
    cout << rbt.searchAll(1) << endl;
    rbt.removeAll(1);
    cout << rbt.searchAll(1) << endl;
}
