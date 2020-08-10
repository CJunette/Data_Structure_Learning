// 003_B-1_001_List.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "List.h"
using namespace std;

int main()
{
    List<int> l;
	for(int i = 0; i < 6; i++)
	{
		l.insertAsLast(i % 3);
	}
	l.show();
	cout << endl;

	{
		//find.
		Node<int> *p = l.find(1, 6, l.last());
		if(p)
		{
			cout << p->data() << endl;
		}
		else
		{
			cout << "Can't find element." << endl;
		}
		cout << endl;
	}

	{
		//deduplicate.
		l.deduplicate();
		l.show();
		cout << endl;
	}
	
	{
		//find.
		Node<int> *p = l.search(1, 6, l.last());
		cout << p->data() << endl;		
		cout << endl << endl;
	}

	{
		//insertion sort.
		List<int> i;
		i.insertAsLast(4);
		i.insertAsLast(3);
		i.insertAsLast(4);
		i.insertAsLast(6);
		i.insertAsLast(1);
		i.insertAsLast(3);
		i.insertAsLast(2);
		i.show();
		cout << endl;
		i.insertionSort(i.first(), 7);
		i.show();
		cout << endl << endl;
	}

	{
		//selection sort.
		List<int> i;
		i.insertAsLast(4);
		i.insertAsLast(3);
		i.insertAsLast(4);
		i.insertAsLast(6);
		i.insertAsLast(1);
		i.insertAsLast(3);
		i.insertAsLast(2);
		i.show();
		cout << endl;
		i.selectionSort(i.first(), 7);
		i.show();
		cout << endl << endl;
	}

	{
		//merge sort.
		List<int> i;

		i.insertAsLast(4);
		i.insertAsLast(3);
		i.insertAsLast(4);
		i.insertAsLast(6);
		i.insertAsLast(1);
		i.insertAsLast(3);
		i.insertAsLast(2);
		i.show();
		cout << endl;
		Node<int> *p = i.first();
		i.mergeSort(p, 7);
		i.show();
	}

}
