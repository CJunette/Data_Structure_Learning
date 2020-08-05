// 001_Homework_1-21.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

//二分递归法。
int fib1(int n)
{
	if(n == 1)
	{
		return 1;
	}
	if(n == 0)
	{
		return 0;
	}

	return fib1(n - 1) + fib1(n - 2);
}

//线性递归法。
int fib2(int n, int &prev)
{
	if(n == 0)
	{
		prev = 1;
		return n;
	}
	int p;
	prev = fib2(n - 1, p);
	return prev + p;
}

//迭代法。
int fib3(int n)
{
	int a = 1;
	int b = 0;
	while(n-- > 1)
	{
		int temp = a;
		a = a + b;
		b = temp;		
	}
	return a;
}

int main()
{
	const int N = 24;

	cout << fib1(N) << endl;
	int a;
	cout << fib2(N, a) << endl;
	cout << fib3(N) << endl;
}
