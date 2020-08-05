// 001_Homework_1-15.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

int max1(int a[], int n)
{
	int max = a[0];
	int i = 0;
	while(i < n)
	{
		max = max > a[i] ? max : a[i];
		i++;
	}
	return max;
}

int max2(int a[], int n)
{
	if(n == 1)
	{
		return a[n - 1];
	}
	else
	{
		int subMax = max2(a, n - 1);
		return a[n - 1] > subMax ? a[n - 1] : subMax;
	}
}

int main()
{
	int a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	cout << max1(a, 10) << endl;
	cout << max2(a, 10) << endl;
}
