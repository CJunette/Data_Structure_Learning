// 009_11-XA1_001_SkipList.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Skiplist.h"
using namespace std;

int main()
{
    Skiplist<int, char> skip;

    skip.put(1, 'a');
    skip.put(1, 'b');
    skip.put(1, 'c');
    skip.show();
    cout << endl;
    skip.remove(1);
    skip.show();
    cout << endl;
    cout << *skip.get(1) << endl;
}
