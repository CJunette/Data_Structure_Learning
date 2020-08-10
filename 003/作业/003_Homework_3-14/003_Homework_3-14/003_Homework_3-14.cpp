// 003_Homework_3-14.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "list.h"
using namespace std;

int main()
{
	List<int> l;
	for(int i = 1980; i < 2013; i++)
	{
		l.insertAsLast(i);
	}
	for(int i = 0; i < 1980; i++)
	{
		l.insertAsLast(i);
	}
	l.selectionSort2(l.first(), l.size());

	cout << endl;

	l.show();
}