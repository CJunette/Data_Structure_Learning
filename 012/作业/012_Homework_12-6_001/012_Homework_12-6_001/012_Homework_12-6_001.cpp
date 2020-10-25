// 012_Homework_12-6_001.cpp : This file contains the 'main' function. Program execution begins and ends there.
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
T median(T *S1, int lo1, T *S2, int lo2, int n)
{    
    while(2 < n)
    {
        int mi1 = lo1 + n / 2;
        int mi2 = lo2 + (n - 1) / 2;

        if(S1[mi1] == S2[mi2])
        {
            return S1[mi1];
        }
        else if(S1[mi1] < S2[mi2])
        {
            lo1 = mi1 + 1;
            n = (n - 1) / 2;
        }
        else
        {
            lo2 = mi2;
            n = n / 2;
        }
    }

    return trivialMedian(S1, lo1, n, S2, lo2, n);
}


int main()
{
    int S1[] = { 1, 2, 3, 4, 5 };
    int S2[] = { 5, 6, 7, 8, 9 };
    median(S1, 0, S2, 0, 5);
    median(S2, 0, S1, 0, 5);

	int S3[] = { 1, 2, 3, 4 };
	int S4[] = { 4, 5, 6, 7 };
	median(S3, 0, S4, 0, 4);
	median(S4, 0, S3, 0, 4);
}