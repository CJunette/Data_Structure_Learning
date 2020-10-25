#pragma once

//Ϊ�˷���Vector��heapSort()��heapfy�ĵ��ã��Ұ�ԭ��ֻ��PQ_ComplHeap���õ��ĺ���ͳһ����д��һ��ͷ�ļ��С�

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
		//������԰�percolateUp()�е�˼·��΢����һ���޸ġ�����Ϊ�����漰��properParent()�ĸĶ��������Ҿ���ʱ���������ˡ�
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