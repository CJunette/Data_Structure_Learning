// Range.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
//#include <ctime>
using namespace std;

int inline searchlo(int *s, int s_size, int a)
{
	int lo = 0;
	int hi = s_size;
	while(lo < hi)
	{
		int mi = (lo + hi) >> 1;
		if(s[mi] < a)
		{
			lo = mi + 1;
		}
		else
		{
			hi = mi;
		}
	}
	return --lo;
}

int inline searchhi(int *s, int s_size, int a)
{
	int lo = 0;
	int hi = s_size;
	while(lo < hi)
	{
		int mi = (lo + hi) >> 1;
		if(s[mi] <= a)
		{
			lo = mi + 1;
		}
		else
		{
			hi = mi;
		}
	}
	return --lo;
}

int B[250000];
int *C;
void inline  merge(int *s, int lo, int mi, int hi)
{
	int lenB = mi - lo;
	int lenC = hi - mi;	
	for(int i = 0; i < lenB; i++)
	{
		B[i] = s[lo + i];
	}
	int iterB = 0;
	int iterC = 0;
	int iterA = 0;
	C = &s[mi];

	while(iterB < lenB)
	{
		s[lo + iterA++] = ((B[iterB] <= C[iterC]) || lenC <= iterC)? B[iterB++] : C[iterC++];
	}
}

void inline sort(int *s, int lo, int hi)
{
	if((hi - lo) <= 1)
	{
		return;
	}
	int mi = lo + ((hi - lo) >> 1);
	sort(s, lo, mi);
	sort(s, mi, hi);
	merge(s, lo, mi, hi);
}

int main()
{
	//timespec start, end;

    int n, m;
	scanf("%d%d", &n, &m);
	//用scanf和printf替换cin和cout，可以节省大量时间。
	int s[500000];
	//timespec_get(&start, 1);
	//timespec_get(&end, 1);
	//int time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
	//cout << "time1: " << time << endl;

	for(int i = 0; i < n; i++)
	{
		scanf("%d", &s[i]);		
	}
	sort(s, 0, n);

	//timespec_get(&start, 1);
	int a[500000];
	int b[500000];
	//timespec_get(&end, 1);
	//time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
	//cout << "time2: " << time << endl;
	
	int i = 0;
	while(i < m)
	{
		scanf("%d%d", &a[i], &b[i]);
		i++;
	}

	i = 0;
	int count;
	//timespec_get(&start, 1);
	while(i < m)
	{

		int lo = searchlo(s, n, a[i]);
		int hi = searchhi(s, n, b[i]);
		count = hi - lo;
		printf("%d\n", count);
		i++;
	}
	//timespec_get(&end, 1);
	//time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
	//cout << "time3: " << time << endl;

}
