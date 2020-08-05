// 001_Homework_1-14.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

int power(int c)
{
    int base = 1;
    int mult = 2;
    while(c > 0)
    {
        if(c & 1)
        {
            base *= mult;
        }
        mult *= mult;
        c /= 2;
    }
    return base;
}

int main()
{
    cout << power(5) << endl;
}