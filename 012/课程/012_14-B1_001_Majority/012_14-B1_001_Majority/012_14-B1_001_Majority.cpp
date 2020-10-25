// 012_14-B1_001_Majority.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

template<typename T, size_t N>
T majEleCandidate(T(&A)[N])
{
	T maj;
	int count = 0;
	for(int i = 0; i < N; i++)
	{
		if(count == 0)
		{
			maj = A[i];
			count = 1;
		}
		else
		{
			maj == A[i] ? count++ : count--;
		}
	}
	return maj;
}

template<typename T, size_t N>
bool majEleCheck(T(&A)[N], T maj)
{
    int count = 0;
	for(int i = 0; i < N; i++)
	{
		if(A[i] == maj)
		{
			count++;
		}
	}
	return N < count * 2;
}

template<typename T, size_t N>
bool majority(T (&A)[N], T &maj)
{
	maj = majEleCandidate(A);
	return majEleCheck(A, maj);
}

int main()
{
	int A[] = { 1, 1, 1, 2, 2, 3, 3 };
	int maj = 0;
	majority(A, maj);
}