// Range.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

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
    int n, m;
	scanf("%d%d", &n, &m);
	//用scanf和printf替换cin和cout，可以节省大量时间。
	int s[500000];

	for(int i = 0; i < n; i++)
	{
		scanf("%d", &s[i]);		
	}
	sort(s, 0, n);

	int a[500000];
	int b[500000];
	
	int i = 0;
	while(i < m)
	{
		scanf("%d%d", &a[i], &b[i]);
		i++;
	}

	i = 0;
	int count;

	while(i < m)
	{
		int lo = searchlo(s, n, a[i]);
		int hi = searchhi(s, n, b[i]);
		count = hi - lo;
		printf("%d\n", count);
		i++;
	}
}
