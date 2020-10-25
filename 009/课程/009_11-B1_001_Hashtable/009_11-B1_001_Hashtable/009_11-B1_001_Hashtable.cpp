// 009_11-B1_001_Hashtable.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Hashtable.h"
using namespace std;

int main()
{
    Hashtable<int, char> ht;

    ht.put(1, 'a');
    ht.put(2, 'b');
    ht.put(3, 'c');

    cout << *ht.get(1) << endl;
    ht.remove(1);
    
    if(ht.get(1))
    {
        cout << *ht.get(1) << endl;
    }
    else
    {
        cout << "0" << endl;
    }

    cout << *ht.get(2) << endl;
    cout << *ht.get(3) << endl;
}
