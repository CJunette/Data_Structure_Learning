// 012_14-B3_001_QuickSelect.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

//函数作用为，将有序序列中秩为k的元素移动到当前无序序列的k位置。
template<typename T, size_t N>
void quickSelect(T (&A)[N], int k)
{
    int lo, hi;
	for(lo = 0, hi = N - 1; lo < hi; )
	{
		int i = lo, j = hi;
		T pivot = A[lo];

		while(i < j)
		{
			while(i < j)
			{
				if(pivot < A[j])
				{
					j--;
				}
				else
				{
					A[i++] = A[j];
					break;
				}
			}
			while(i < j)
			{
				if(A[i] < pivot)
				{
					i++;
				}
				else
				{
					A[j--] = A[i];
					break;
				}
			}
		}

		A[i] = pivot;
		if(i <= k)
		{
			lo = i + 1;
		}
		if(k <= i)
		{
			hi = i - 1;
		}
	}
}

int main()
{
	int A[] = { 5, 4, 3, 2, 1 };
	quickSelect(A, 0);
	cout << A[0] << endl;
}