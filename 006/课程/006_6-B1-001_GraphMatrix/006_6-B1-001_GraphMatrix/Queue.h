#pragma once
#include "List.h"

template<typename T>
struct Queue: public List<T>
{
	void enqueue(T const &e);
	T dequeue();
	T &front();
};

template<typename T>
void Queue<T>::enqueue(T const &e)
{
	this->insertAsLast(e);
}

template<typename T>
T Queue<T>::dequeue()
{
	T temp = this->first()->data();
	this->remove(this->first());
	return temp;
}

template<typename T>
T &Queue<T>::front()
{
	return this->first()->data();
}