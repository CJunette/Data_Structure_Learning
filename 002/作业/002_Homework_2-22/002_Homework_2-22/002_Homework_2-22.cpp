// 002_Homework_2-22.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

const int N = 5;

int binarySearch(int a[], int e, int lo, int hi)
{
	while(lo < hi)
	{
		int mi = (lo + hi) / 2;
		if(e < a[mi])
		{
			hi = mi;
		}
		else
		{
			lo = mi + 1;
		}		
	}
	return --lo;
}
//马鞍搜索。
//要求a中元素全部严格递增。
void saddleBackSearch(int a[N][N], int x)
{
    int i = 0;
	int j = binarySearch(a[0], x, 0, N);
	while(i < N && j > -1)
	{
		if(a[i][j] < x)
		{
			i++;
		}
		else if(x < a[i][j])
		{
			j--;
		}
		else
		{
			printf("a[%d][%d] = %d\n", i, j, x);
			j--;
			i++;
		}
	}
}

int main()
{
	int a[N][N];
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			a[i][j] = i + j;
		}
	}

	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			cout << a[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	saddleBackSearch(a, 4);
}
