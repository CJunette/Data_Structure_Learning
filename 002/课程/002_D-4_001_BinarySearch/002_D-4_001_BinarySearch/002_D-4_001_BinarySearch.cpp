// 002_D-4_001_BinarySearch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
using namespace std;

typedef int Rank;

template<typename T>
static Rank binSearch(T &S, typename T::value_type e, Rank lo, Rank hi)
{
	while(hi - lo > 1)
	{
		Rank mid = (lo + hi) >> 1;
		if(S[mid] <= e)
		{
			lo = mid;
		}
		else
		{
			hi = mid;
		}
	}
	return (S[lo] == e) ? lo : -1;
}

int main()
{
	vector<int> a;
	for(int i = 0; i < 10; i++)
	{
		a.push_back(i);
	}
	cout << binSearch(a, 3, 0, 10) << endl;
}