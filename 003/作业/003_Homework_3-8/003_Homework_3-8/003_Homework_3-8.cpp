// 003_Homework_3-8.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
using namespace std;

template<typename T>
void insertionSort(vector<T> &v)
{
	for(int i = 1; i < v.size(); i++)
	{
		int j = i;
		T temp = v[i];
		while(j > 0)
		{
			if(v[j - 1] > temp)
			{
				v[j] = v[j - 1];
				j--;
			}
			else
			{
				break;
			}
			
		}
		v[j] = temp;		
	}
}

int main()
{
    vector<int> v;
	v.push_back(3);
	v.push_back(5);	
	v.push_back(1);
	v.push_back(1);
	v.push_back(2);
	v.push_back(8);
	v.push_back(7);
	v.push_back(4);

	insertionSort(v);
	for(int i = 0; i < 8; i++)
	{
		cout << v[i] << " ";
	}
	cout << endl;
}
