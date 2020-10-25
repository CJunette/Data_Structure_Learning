#pragma once

//为了方便Vector的heapSort()对heapfy的调用，我把原先只在PQ_ComplHeap中用到的函数统一起来写到一个头文件中。

int parent(int i)
{
	return (i - 1) / 2;
}

int lChild(int i)
{
	return i * 2 + 1;
}

int rChild(int i)
{
	return (i + 1) * 2;
}

bool inHeap(int n, int i)
{
	return (0 <= i) && (i < n);
}

bool lChildValid(int n, int i)
{
	return inHeap(n, lChild(i));
}

bool rChildValid(int n, int i)
{
	return inHeap(n, rChild(i));
}

template<typename T>
int bigger(const T *PQ, int i, int j)
{
	return PQ[i] < PQ[j] ? j : i;
}

template<typename T>
int properParent(const T *PQ, int n, int i)
{
	if(rChildValid(n, i))
	{
		return bigger(PQ, bigger(PQ, i, lChild(i)), rChild(i));
	}
	else if(lChildValid(n, i))
	{
		return bigger(PQ, i, lChild(i));
	}
	else
	{
		return i;
	}
}

/*----------------------------------------------------------------------------*/

template<typename T>
int percolateUp(T *A, int i)
{
	T temp = A[i];
	while(0 < i)
	{
		int j = parent(i);
		if(A[j] < temp)
		{
			A[i] = A[j];
			i = j;
		}
		else
		{
			break;
		}
	}
	A[i] = temp;
	return i;
}

template<typename T>
int percolateDown(T *A, int n, int i)
{
	int j;
	while(i != (j = properParent(A, n, i)))
	{
		//这里可以按percolateUp()中的思路稍微进行一下修改。但因为可能涉及对properParent()的改动，所以我就暂时不做调整了。
		T temp = A[i];
		A[i] = A[j];
		A[j] = temp;
		i = j;
	}
	return i;
}

template<typename T>
void heapify(T *A, int n)
{
	for(int i = n / 2 - 1; -1 < i; i--)
	{
		percolateDown(A, n, i);
	}
}