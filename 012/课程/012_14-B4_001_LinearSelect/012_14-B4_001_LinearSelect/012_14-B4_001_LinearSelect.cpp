// 012_14-B4_001_LinearSelect.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
using namespace std;

template<typename T>
struct Vector
{
	int data;
	int size;
	int capacity;
	T *elem;

	Vector(int capacity_ = 8):
		capacity(capacity_), size(0)
	{
		elem = new T[capacity];
		data = 0;
	}
	~Vector()
	{
		delete[] elem;
	}

	bool full() const { return capacity == size; }
	bool empty() const { return size == 0; }

	void push_back(const T &e)
	{
		if(full())
		{
			expand();
		}
		elem[size++] = e;
	}

	T &operator[] (int i)
	{
		return elem[i];
	}

	private:
	void expand()
	{
		T *newArea = new T[capacity * 2];
		for(int i = 0; i < size; i++)
		{
			newArea[i] = elem[i];
		}
		delete[] elem;
		elem = newArea;
		capacity *= 2;
	}
};

template<typename T>
void selectSort(T *A, int N)
{
	for(int i = 0; i < N; i++)
	{
		T min = A[i];
		int rank = i;
		for(int j = i; j < N; j++)
		{
			if(A[j] < min)
			{
				min = A[j];
				rank = j;
			}						
		}
		A[rank] = A[i];
		A[i] = min;
	}
}

const int Q = 5;

template<typename T>
T linearSelect(T *A, int N, int k)
{	
	if(N < Q + 1)
	{
		selectSort(A, N);
		return A[k];
	}

	int count = (N - 1) / Q + 1;
	Vector<T> *subsequences = new Vector<T>[count];
	
	for(int i = 0; i < count; i++)
	{
		for(int j = 0; j < Q && i * Q + j < N; j++)
		{
			subsequences[i].push_back(A[i * Q + j]);
		}
	}

	for(int i = 0; i < count; i++)
	{
		selectSort(subsequences[i].elem, subsequences[i].size);
	}

	T *medians = new T[count];

	for(int i = 0; i < count; i++)
	{
		medians[i] = subsequences[i].elem[subsequences[i].size / 2];
	}

	T M = linearSelect(medians, count, count / 2);

	delete[] subsequences;
	delete[] medians;

	Vector<T> L, E, G;

	for(int i = 0; i < N; i++)
	{
		if(A[i] < M)
		{
			L.push_back(A[i]);
		}
		else if(A[i] == M)
		{
			E.push_back(A[i]);
		}
		else
		{
			G.push_back(A[i]);
		}
	}

	if(k <= L.size)
	{
		return linearSelect(L.elem, L.size, k);
	}
	else if(k <= L.size + E.size)
	{
		return M;
	}
	else
	{
		return linearSelect(G.elem, G.size, k - L.size - E.size);
	}
}


int main()
{
	int A[] = { 7, 6, 5, 4, 3, 2, 1 };

	cout << linearSelect(A, 7, 0) << endl;
}