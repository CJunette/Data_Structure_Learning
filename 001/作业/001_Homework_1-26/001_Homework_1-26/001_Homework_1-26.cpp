// 001_Homework_1-26.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

void reverse(int a[], int lo, int hi)
{
	if(lo < hi)
	{
		int temp = a[lo];
		a[lo] = a[hi];
		a[hi] = temp;
		reverse(a, lo + 1, hi - 1);
	}
}

//对于大小为n的数组a，将其中所有元素都向左侧移动k。
void offset(int a[], int n, int k)
{
	reverse(a, 0, k - 1);
	reverse(a, k, n - 1);
	reverse(a, 0, n - 1);
}

int main()
{
    int a[] = { 1, 2, 3, 4, 5, 6 };
	offset(a, 6, 2);
	for(int i = 0; i < sizeof(a) / sizeof(int); i++)
	{
		cout << a[i] << " ";
	}
	cout << endl;
}