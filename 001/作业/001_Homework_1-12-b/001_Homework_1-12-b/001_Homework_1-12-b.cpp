// 001_Homework_1-12-b.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <bitset>
#include <cmath>
using namespace std;

//这里的宏定义练习册上给出的标准答案。
#define POW(c) (1 << (c))
#define MASK(c) ((unsigned long) - 1) / (POW(POW(c)) + 1)
#define ROUND(n, c) (((n) & MASK(c)) + ((n) >> POW(c) & MASK(c)))

//下面的这些函数是我根据宏定义改写的。

//按书上p20提到的算法写的用于返回2^c的函数。
unsigned long Pow(unsigned long c)
{
	if(c == 0)
	{
		return 1;
	}
	if(c % 2 == 0)
	{
		return pow(Pow(c / 2), 2);
	}
	else
	{
		return pow(Pow(c / 2), 2) * 2;
	}
}

unsigned long Mask(unsigned long  c)
{
	unsigned long i = 0;
	//这里d最后得到的结果会是2^1+1(11)、2^2+1(101)、2^4+1(10001)、2^8+1(100000001)这样的数。
	unsigned long d = Pow(Pow(c)) + 1;
	//这里i的值是2^32 - 1，即unsigned long下所能表示的最大的数字（11111111111111111111111111111111）。
	i -= 1;
	//i/d会得到类似01010101010101010101010101010101、00110011001100110011001100110011、00001111000011110000111100001111、00000000111111110000000011111111这样的数字，将这样的数字作为Mask。
	return i / d;
}

const int N = 16;

unsigned long Round(unsigned n, unsigned long  c)
{
	//以2 * Pow(c)作为一个单位，通过Mask(c)取出右侧Pow(c)单位中的数。
	//当c为0时，Mask为01010101010101010101010101010101，此时以2为一个单位。
	cout << "mask:   \t" << bitset<N>(Mask(c)) << endl;
	unsigned long m1 = n & Mask(c);
	cout << "m1:      \t" << bitset<N>(m1) << endl;
	//下面这个等式等同于p = n >> Pow(c)。即将n向右移Pow(c)个单位。这么做等同于将之前左侧Pow(c)单位的数移动到右侧Pow(c)单位上。
	//当c为0时，这里将n向右移1个单位。
	unsigned long p = n / Pow(Pow(c));
	cout << "p:      \t" << bitset<N>(p) << endl;
	//此时再用相同的Mask再进行一次与操作，取出原左侧Pow(c)单位现右侧Pow(c)单位的数。
	unsigned long m2 = p & Mask(c);
	cout << "m2:      \t" << bitset<N>(m2) << endl;
	//将两个Pow(c)单位的数相加，就能得到当前2 * Pow(c)单位内，1的个数。
	cout << "m1 + m2: \t" << bitset<N>(m1 + m2) << endl << endl;
	return m1 + m2;
}

int main()
{
	
	int i = MASK(1);
	int j = Mask(2);
	unsigned long k = 5;
	cout << "origin: \t" << bitset<N>(k) << endl << endl;
	k = Round(k, 0);
	k = Round(k, 1);
	k = Round(k, 2);
	k = Round(k, 3);
	k = Round(k, 4);
	
	/*
	int l = 4;
	l = ROUND(l, 0);
	l = ROUND(l, 1);
	l = ROUND(l, 2);
	l = ROUND(l, 3);
	l = ROUND(l, 4);
	*/
	cout << k << endl;
}