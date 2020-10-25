// 012_14-A_001_QuickSort.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

template<typename T>
void Swap(T &a, T &b)
{
	T temp = a;
	a = b;
	b = temp;
}

template<typename T>
int partition_LUG(T *_elem, int lo, int hi)
{
	//下面的Swap()等效于随机选取一个元素作为候选元素。
	//Swap(_elem[lo], _elem[lo + rand() % (hi - lo)]);
	hi--;
	T pivot = _elem[lo];

	while(lo < hi)
	{
		while(lo < hi && pivot <= _elem[hi])
		{
			hi--;
		}
		_elem[lo] = _elem[hi];

		while(lo < hi && _elem[lo] <= pivot)
		{
			lo++;
		}
		_elem[hi] = _elem[lo];
	}

	_elem[lo] = pivot;
	return lo;
}

template<typename T>
int partition_LUG_(T *_elem, int lo, int hi)
{
	//下面的Swap()等效于随机选取一个元素作为候选元素。
	//Swap(_elem[lo], _elem[lo + rand() % (hi - lo)]);
	hi--;
	T pivot = _elem[lo];

	while(lo < hi)
	{
		while(lo < hi && pivot <= _elem[hi])
		{
			hi--;
		}
		if(lo < hi)
		{
			_elem[lo++] = _elem[hi];
		}

		while(lo < hi && _elem[lo] <= pivot)
		{
			lo++;
		}
		if(lo < hi)
		{
			_elem[hi--] = _elem[lo];
		}
	}

	_elem[lo] = pivot;
	return lo;
}

template<typename T>
int partition_DUP_(T *_elem, int lo, int hi)
{
	//下面的Swap()等效于随机选取一个元素作为候选元素。
	//Swap(_elem[lo], _elem[lo + rand() % (hi - lo)]);
	hi--;
	T pivot = _elem[lo];

	while(lo < hi)
	{
		while(lo < hi && pivot < _elem[hi])
		{
			hi--;
		}
		if(lo < hi)
		{
			_elem[lo++] = _elem[hi];
		}

		while(lo < hi && _elem[lo] < pivot)
		{
			lo++;
		}
		if(lo < hi)
		{
			_elem[hi--] = _elem[lo];
		}
	}

	_elem[lo] = pivot;
	return lo;
}

template<typename T>
int partition_DUP(T *_elem, int lo, int hi)
{
	//下面的Swap()等效于随机选取一个元素作为候选元素。
	//Swap(_elem[lo], _elem[lo + rand() % (hi - lo)]);
	hi--;
	T pivot = _elem[lo];

	while(lo < hi)
	{
		while(lo < hi)
		{
			if(pivot < _elem[hi])
			{
				hi--;
			}
			else
			{
				_elem[lo++] = _elem[hi];
				break;
			}
		}

		while(lo < hi)
		{
			if(_elem[lo] < pivot)
			{
				lo++;
			}
			else
			{
				_elem[hi--] = _elem[lo];
				break;
			}
		}		
	}

	_elem[lo] = pivot;
	return lo;
}

template<typename T>
int partition_LGU(T *_elem, int lo, int hi)
{
	//Swap(_elem[lo], _elem[lo + rand() % (hi - lo)]);

	int k = lo + 1;
	int mi = lo;

	while(k < hi)
	{
		if(_elem[lo] <= _elem[k])
		{
			k++;
		}
		else
		{
			Swap(_elem[++mi], _elem[k++]);
		}
	}

	Swap(_elem[lo], _elem[mi]);
	return mi;
}

template<typename T>
void quickSort(T *_elem, int lo, int hi)
{
	if(lo == hi)
	{
		return;
	}

	int pivot = partition_DUP(_elem, lo, hi);
	quickSort(_elem, lo, pivot);
	quickSort(_elem, pivot + 1, hi);
}

int main()
{
	int a[] = { 6, 3, 8, 2, 5, 9, 4, 5, 1, 7 };
	int b[] = { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 };
	quickSort(b, 0, 10);
	for(int i = 0; i < 10; i++)
	{
		cout << b[i] << " ";
	}
	cout << endl;
}