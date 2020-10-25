#pragma once

#include "Vector.h"
#include "PQ.h"
#include "PQ_ComplHeap_Function.h"

template<typename T>
struct PQ_ComplHeap: public PQ<T>, public Vector<T>
{
	PQ_ComplHeap()
	{}

	PQ_ComplHeap(T *A, int n):
		Vector<T>(A, 0, n)
	{
		heapify(A, n);
	}

	void insert(const T &e);
	T getMax();
	T delMax();
	void decrease(int i, int delta);
	void increase(int i, int delta);
	T remove(int i);		
};

template<typename T>
void PQ_ComplHeap<T>::insert(const T &e)
{
	this->push_back(e);
	percolateUp(this->_elem, this->_size - 1);
}

template<typename T>
T PQ_ComplHeap<T>::getMax()
{
	return this->_elem[0];
}

template<typename T>
T PQ_ComplHeap<T>::delMax()
{
	T temp = this->_elem[0];
	this->_elem[0] = this->_elem[this->_size--];
	percolateDown(this->_elem, this->_size, 0);
	return temp;
}

template<typename T>
void PQ_ComplHeap<T>::decrease(int i, int delta)
{
	this->_elem[i] -= delta;
	percolateDown(this->_elem, this->_size, i);
}

template<typename T>
void PQ_ComplHeap<T>::increase(int i, int delta)
{
	this->_elem[i] += delta;
	percolateUp(this->_elem, i);
}

template<typename T>
T PQ_ComplHeap<T>::remove(int i)
{
	T temp = this->_elem[i];
	if(i == this->_size - 1)
	{
		return temp;
	}
	else
	{
		this->_elem[i] = this->_elem[--this->_size];
		//这里上下滤都要做一下，因为可能出现末节点对应的元素值大于被删除的节点的父节点的元素值的情况。
		percolateUp(this->_elem, i);
		percolateDown(this->_elem, this->_size, i);
		return temp;
	}	
}