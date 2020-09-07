#pragma once
#include <stdexcept>

template<typename T>
class Stack
{
	public:
	T *stack;
	int size;
	int capacity;

	Stack(int n = 10);
	~Stack();

	void push(T const &e);
	T pop();
	T &top();

	int getSize() const;
	bool empty() const;

	private:
	int _topIndex;
	void expand();
};


template<typename T>
Stack<T>::Stack(int n)
{
	size = 0;
	capacity = n;
	_topIndex = -1;
	stack = new T[n];
}

template<typename T>
Stack<T>::~Stack()
{
	delete[] stack;
}

template<typename T>
void Stack<T>::push(T const &e)
{
	expand();
	stack[++_topIndex] = e;
	size++;
}

template<typename T>
T Stack<T>::pop()
{
	if(empty())
	{
		throw std::range_error("No Element!");
	}
	T temp = stack[_topIndex];
	stack[_topIndex] = 0;
	_topIndex--;
	size--;
	return temp;
}

template<typename T>
T &Stack<T>::top()
{
	if(empty())
	{
		throw std::range_error("No Element!");
	}
	return stack[_topIndex];
}

template<typename T>
void Stack<T>::expand()
{
	if(capacity <= size)
	{
		capacity *= 2;
		T *temp = new T[capacity];
		for(int i = 0; i < size; i++)
		{
			temp[i] = stack[i];
		}
		delete[] stack;
		stack = temp;
	}
}

template<typename T>
int Stack<T>::getSize() const
{
	return size;
}

template<typename T>
bool Stack<T>::empty() const
{
	return size == 0;
}