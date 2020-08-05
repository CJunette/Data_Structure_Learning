// 002_D-5_001_BinarySearch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
using namespace std;

typedef int Rank;

template<typename T>
static Rank binSearch(T &S, typename T::value_type e, Rank lo, Rank hi)
{
	while(lo < hi)
	{
		Rank mid = (lo + hi) >> 1;
		if(e < S[mid])
		{
			hi = mid;
		}
		else//e >= S[mid]
		{
			//我们需要返回的是不大于e的最后一个元素，也就是大于e的第一个元素左侧的那个元素。
			//如果e > S[mid]，则说明e一定在当前的区间[lo, hi)之中，lo需要继续右移。
			//如果e = S[mid]，则说明当前的S[lo]可能是e，也可能是大于e的第一个元素。如果是前者，则说明lo还需继续右移，如果是后者，则说明当前的lo左侧的元素的秩即是返回值。
			//综上，这里令lo = mid + 1的做法并不会导致元素的遗漏。
			lo = mid + 1;
		}
	}
	return --lo;
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