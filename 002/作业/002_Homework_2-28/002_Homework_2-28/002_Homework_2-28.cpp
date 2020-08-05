// 002_Homework_2-28.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
using namespace std;

template<typename T>
void merge1(vector<T> &a, int lo, int mi, int hi)
{
	int lenA = hi - lo;
	int lenB = mi - lo;
	int lenC = hi - mi;
	int *B = new T[mi - lo];
	for(int i = 0; i < mi - lo; i++)
	{
		B[i] = a[lo + i];
	}
	int *C = &a[mi];	
	int *A = &a[lo];
	int iterA = 0;
	int iterB = 0;
	int iterC = 0;
	while(iterB < lenB)
	{
		A[iterA++] = ((B[iterB] < C[iterC]) || (lenC <= iterC)) ? B[iterB++] : C[iterC++];
	}
}

template<typename T>
void mergeSearch1(vector<T>& A, int lo, int hi)
{
	if(hi - lo == 1)
	{
		return;
	}
	int mi = (lo + hi) / 2;
	mergeSearch1(A, lo, mi);
	mergeSearch1(A, mi, hi);
	merge1(A, lo, mi, hi);
}

int B[8];

template<typename T>
void merge2(vector<T> &a, int lo, int mi, int hi)
{
	int lenA = hi - lo;
	int lenB = mi - lo;
	int lenC = hi - mi;
	
	for(int i = 0; i < mi - lo; i++)
	{
		B[i] = a[lo + i];
	}
	int *C = &a[mi];
	int *A = &a[lo];
	int iterA = 0;
	int iterB = 0;
	int iterC = 0;
	while(iterB < lenB)
	{
		A[iterA++] = ((B[iterB] < C[iterC]) || (lenC <= iterC)) ? B[iterB++] : C[iterC++];
	}
}

template<typename T>
void mergeSearch2(vector<T> &A, int lo, int hi)
{
	if(hi - lo == 1)
	{
		return;
	}
	int mi = (lo + hi) / 2;
	mergeSearch2(A, lo, mi);
	mergeSearch2(A, mi, hi);
	merge2(A, lo, mi, hi);
}

int main()
{
	vector<int> a;
	a.push_back(6);
	a.push_back(3);
	a.push_back(2);
	a.push_back(7);
	a.push_back(1);
	a.push_back(5);
	a.push_back(8);
	a.push_back(4);

	timespec start, end;
	timespec_get(&start, 1);
	mergeSearch1(a, 0, 8);
	timespec_get(&end, 1);

	//因为运行太快所以只能用timespec，而不能用clock_t。
	double time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
	cout << "Time for mergeSearch1: " << time << endl;

	for(int i = 0; i < a.size(); i++)
	{
		cout << a[i] << " ";
	}
	cout << endl;
	
	vector<int> b;
	b.push_back(6);
	b.push_back(3);
	b.push_back(2);
	b.push_back(7);
	b.push_back(1);
	b.push_back(5);
	b.push_back(8);
	b.push_back(4);

	timespec_get(&start, 1);
	mergeSearch2(b, 0, 8);
	timespec_get(&end, 1);

	//可以看到，mergeSearch2的速度显然更快一些。
	time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
	cout << "Time for mergeSearch2: " << time << endl;

	for(int i = 0; i < b.size(); i++)
	{
		cout << b[i] << " ";
	}
	cout << endl;
}