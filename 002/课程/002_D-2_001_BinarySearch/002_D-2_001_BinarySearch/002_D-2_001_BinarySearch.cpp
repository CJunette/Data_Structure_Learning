// 002_D-2_001_BinarySearch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
using namespace std;

typedef int Rank;

template<typename T>
static Rank binSearch(T& S, typename T::value_type e, Rank lo, Rank hi)
{
	while(lo < hi)
	{
		Rank mid = (lo + hi) >> 1;
		if(e < S[mid])
		{
			hi = mid;
		}
		else
		{
			if(S[mid] < e)
			{
				//注意，这里必须是mid + 1。
				lo = mid + 1;
			}
			else
			{
				return mid;
			}
		}
	}
	return -1;
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