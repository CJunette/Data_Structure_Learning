// 003_Homework_3-19.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "list.h"
using namespace std;

void josephus(int n, int k)
{
    List<int> l;
	for(int i = 0; i < n; i++)
	{
		l.insertAsLast(i);
	}
	
	Node<int> *p = l.first();
	while(l.size() > 1)
	{
		for(int j = 0; j < k; j++)
		{
			p = (p == l.last()) ? l.first() : p->succ();
		}

		if(p == l.last())
		{
			l.remove(p);
			p = l.first();
		}
		else
		{
			p = p->succ();
			l.remove(p->pred());
		}
	}

	cout << "Last child: " << l.first()->data() << endl;
}

int main()
{
	josephus(20, 30);
}