// 003_Homework_3-13.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ctime>
#include "list.h"
using namespace std;

int main()
{
    {
        List<int> i;
		i.insertAsLast(4);
		i.insertAsLast(3);
		i.insertAsLast(4);
		i.insertAsLast(6);
		i.insertAsLast(1);
		i.insertAsLast(3);
		i.insertAsLast(2);
		timespec start, end;
		timespec_get(&start, 1);
		i.selectionSort1(i.first(), i.size());
		timespec_get(&end, 1);
		double time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
		cout << "Time for selectionSearch1: " << time << endl;
		cout << endl;
    }

	{
		List<int> i;
		i.insertAsLast(4);
		i.insertAsLast(3);
		i.insertAsLast(4);
		i.insertAsLast(6);
		i.insertAsLast(1);
		i.insertAsLast(3);
		i.insertAsLast(2);
		timespec start, end;
		timespec_get(&start, 1);
		i.selectionSort2(i.first(), i.size());
		timespec_get(&end, 1);
		double time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
		cout << "Time for selectionSearch1: " << time << endl;
		cout << endl;
	}
}
