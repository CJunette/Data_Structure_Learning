// 012_14-C_001_ShellSort.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
using namespace std;

template<typename T>
void shellSort(T *_elem, int lo, int hi)
{
	//这里使用的是PS序列，即2^n - 1。0x3FFFFFFF就是2^30 - 1。我为了调试方便稍微把数值减小了一点。
	for(int d = 0xFF; 0 < d ; d >>= 1)
	{
		for(int j = lo + d; j < hi; j++)
		{
			int i = j - d;
			T x = _elem[j];

			cout << "d = " << d << ", j = " << j << endl;

			while(0 <= i && x < _elem[i])
			{
				cout << "swap " << i << " and " << i + d << endl;
				_elem[i + d] = _elem[i];
				i -= d;
			}
			_elem[i + d] = x;

			for(int k = 0; k < hi - lo; k++)
			{
				cout << setw(2) << k << " ";
			}
			cout << endl;
			for(int k = 0; k < hi - lo; k++)
			{
				cout << setw(2) << _elem[k] << " ";
			}
			cout << endl << endl;;
		}
	}
}

int main()
{
	int S[] = { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

	cout << "origin: " << endl;
	for(int k = 0; k < 13; k++)
	{
		cout << setw(2) << k << " ";
	}
	cout << endl;
	for(int k = 0; k < 13; k++)
	{
		cout << setw(2) << S[k] << " ";
	}
	cout << endl << endl;

	shellSort(S, 0, 13);
}