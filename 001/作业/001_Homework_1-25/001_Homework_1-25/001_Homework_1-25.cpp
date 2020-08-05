// 001_Homework_1-25.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

int countGCD1(int a, int b)
{	
	if(a == b)
	{
		return a;
	}
	if(!(a & 1) && !(b & 1))
	{
		return countGCD1(a / 2, b / 2) * 2;
	}
	else
	{
		int t = a > b ? (a - b) : (b - a);
		while(!(t & 1))
		{
			t /= 2;
		}
		if(a > b)
		{
			a = t;			
		}
		else
		{
			b = t;
		}
		return countGCD1(a, b);
	}
}

//由于只涉及移位运算，因此在计算机中比辗转相除法求最大公因数要更有效率。
int countGCD2(int a, int b)
{
	int r = 0;
	while(!(a & 1) && !(b & 1))
	{
		a >>= 1;
		b >>= 1;
		r++;
	}
	while(a && b)
	{
		while(!(a & 1))
		{
			a >>= 1;
		}
		while(!(b & 1))
		{
			b >>= 1;
		}
		(a - b > 0) ? (a = a - b) : (b = b - a);
	}
	if(a == 0)
	{
		return b << r;
	}
	else
	{
		return a << r;
	}
}

int main()
{
	cout << countGCD2(6, 32) << endl;
}
