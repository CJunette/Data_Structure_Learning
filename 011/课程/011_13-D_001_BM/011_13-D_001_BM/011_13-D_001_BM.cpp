// 011_13-D_001_BM.cpp : This file contains the 'main' function. Program execution begins and ends there.
//代码的相关解释看书上。结合书本内容、图和我做的注释去理解。

#include <iostream>
using namespace std;

int max(int a, int b)
{
	return a > b ? a : b;
}

int min(int a, int b)
{
	return a < b ? a : b;
}

const int SIZE = 256;

int *buildBC(char *P)
{
	int *bc = new int[SIZE];

	for(int i = 0; i < SIZE; i++)
	{
		bc[i] = -1;
	}

	for(int i = 0; i < strlen(P); i++)
	{
		bc[P[i]] = i;
	}
	return bc;
}

int *buildSS(char *P)
{
	int m = strlen(P);
	int *ss = new int[m];
	ss[m - 1] = m;

	int lo = m - 1;
	int hi = m - 1;
	int j = lo - 1;

	for(j; j < 0 <= j; j--)
	{
		if(lo < j && ss[m - 1 - hi + j] < (j - lo))
		{
			ss[j] = ss[m - 1 - hi + j];
		}
		else
		{
			hi = j;
			lo = min(lo, hi);
			while(0 <= lo && P[lo] == P[m - 1 - hi + lo])
			{
				lo--;
			}
			ss[j] = hi - lo;
		}
	}
	return ss;
}

int *buildGS(char *P)
{
	int *ss = buildSS(P);
	int m = strlen(P);
	int *gs = new int[m];

	for(int j = 0; j < m; j++)
	{
		gs[j] = m;
	}

	int i = 0;
	for(int j = m - 1; 0 <= j; j--)
	{
		if(ss[j] == j + 1)
		{
			while(i < m - 1 - j)
			{
				gs[i++] = m - 1 - j;
			}
		}
	}

	for(int j = 0; j < m - 1; j++)
	{
		gs[m - 1 - ss[j]] = m - j - 1;
	}

	delete[] ss;
	return gs;
}

int match(char *P, char *T)
{
	int *bc = buildBC(P);
	int *gs = buildGS(P);

    int i = 0;
	while(i <= strlen(T) - strlen(P))
	{
		int j = strlen(P) - 1;
		while(j >= 0 && T[i + j] == P[j])
		{
			j--;
		}
		if(j < 0)
		{
			break;
		}
		else
		{
			i += max(gs[j], j - bc[T[i + j]]);
		}
	}

	delete[] bc;
	delete[] gs;
	return i;
}

int main()
{
	char T[] = "0001000";
	char P1[] = "004";
	char P2[] = "ICED_RICE_PRICE";
	char P3[] = "300020001000";
	char P4[] = "01010101";
	char P5[] = "000000";

	buildGS(P5);

	int i = match(P3, T);

	if(i <= strlen(T) - strlen(P1))
	{
		cout << "match!" << endl;
	}
	else
	{
		cout << "dismatch!" << endl;
	}
}