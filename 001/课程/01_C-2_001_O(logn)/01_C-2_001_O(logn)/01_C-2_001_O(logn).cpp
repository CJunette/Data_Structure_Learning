// 01_C-2_001_O(logn).cpp : This file contains the 'main' function. Program execution begins and ends there.
//我对把这个例子作为O(logn)的例子有点不理解。
//书上的说法是，输入数就是问题的规模n，但实际上问题是针对二进制数处理的，换言之输入数的二进制才应该是问题规模n才对。

#include <iostream>
using namespace std;

int main()
{
	cout << "Please input an integer number: ";
	int i;
	cin >> i;
	int count = 0;
	while(i != 0)
	{
		count += (i & 1);
		i >>= 1;
	}
	cout << count << endl;
}
