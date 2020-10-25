// 012_14-B2_001_Median.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

template<typename T>
T trivialMedian(T *S1, int lo1, int n1, T *S2, int lo2, int n2)
{
    T *S = new T[n1 + n2];
	int hi1 = lo1 + n1;
	int hi2 = lo2 + n2;
	int i = 0;

	while(lo1 < hi1 && lo2 < hi2)
	{
		while(lo1 < hi1 && S1[lo1] <= S2[lo2])
		{
			S[i++] = S1[lo1++];
		}
		while(lo2 < hi2 && S2[lo2] <= S1[lo1])
		{
			S[i++] = S2[lo2++];
		}
	}

	while(lo1 < hi1)
	{
		S[i++] = S1[lo1++];
	}
	while(lo2 < hi2)
	{
		S[i++] = S2[lo2++];
	}

	T v = S[(n1 + n2) / 2];
	delete[] S;

	return v;
}

template<typename T>
T median_SameLength(T *S1, int lo1, T *S2, int lo2, int n)
{
	if(n < 3)
	{
		return trivialMedian(S1, lo1, n, S2, lo2, n);
	}

	int mi1 = lo1 + n / 2;
	int mi2 = lo2 + (n - 1) / 2;

	if(S1[mi1] < S2[mi2])
	{
		return median_SameLength(S1, mi1, S2, lo2, (n - 1) / 2 + 1);
	}
	else if(S2[mi2] < S1[mi1])
	{
		return median_SameLength(S1, 0, S2, mi2, n / 2 + 1);
	}
	else
	{
		return S1[mi1];
	}
}

template<typename T>
T median(T *S1, int lo1, int n1, T *S2, int lo2, int n2)
{
	if(n2 < n1)
	{
		return median(S2, lo2, n2, S1, lo1, n1);
	}

	if(n1 * 2 < n2)
	{
		return median(s1, lo1, n1, S2, lo2 + (n2 - n1 - 1) / 2, n1 + 2 - (n2 - n1) % 2);
	}

	int mi1 = lo1 + n1 / 2;
	int mi2a = lo2 + (n1 - 1) / 2;
	int mi2b = lo2 + n2 - 1 - n1 / 2;

	if(S2[mi2b] < S1[mi1])
	{
		return median(S1, lo1, n1 / 2 + 1, S2, mi2a, n2 - (n1 - 1) / 2);
	}
	else if(S1[mi1] < S2[mi2a])
	{
		return median(S1, mi1, (n1 - 1) / 2 + 1, S2, lo2, n2 - n1 / 2);
	}
	else
	{
		return median(S1, mi1, n1, S2, mi2a, n2 - (n1 - 1) / 2 * 2);
	}
}

int main()
{
	int S1[] = { 3, 4, 5 };
	int S2[] = { 0, 1, 2, 3, 4, 5, 6, 7 };

	int med = trivialMedian(S1, 0, 3, S2, 0, 8);

	int S3[] = { 1, 2, 3, 4 };
	int S4[] = { 1, 2, 3, 4 };
	med = median_SameLength(S3, 0, S4, 0, 4);
}