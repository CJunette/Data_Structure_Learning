#include <stdexcept>
#include "PQ_ComplHeap_Function.h"	

const int DEFAULT_CAPACITY = 1;

template<typename T>
struct Vector
{
	T *_elem;
	int _size, _capacity;

	Vector(int capacity = DEFAULT_CAPACITY, int size = 0, T const &e = 0);
	Vector(T const *e, int start, int end);
	Vector(const Vector<T> &);
	virtual ~Vector();

	virtual void push_back(T const &e);
	virtual void insert(int i, T const &e);
	virtual T remove(int i);
	virtual T &operator [] (int i);
	virtual Vector<T> &operator = (const Vector<T> &);

	virtual int size() const { return _size; }
	virtual bool empty() const { return _size == 0; }
	virtual void heapSort(int lo, int hi);

	protected:
	virtual void expand();
	virtual void shrink();
	void copy(const Vector<T> &);
	//template<typename S> void copy(const Vector<S*> &);
};

template<typename T>
Vector<T>::Vector(int capacity_, int size_, T const &e_)
{
	_capacity = capacity_;
	_size = size_;
	_elem = new T[_capacity];
	for(int i = 0; i < _size; i++)
	{
		_elem[i] = e_;
	}
}

template<typename T>
Vector<T>::Vector(T const *e, int start, int end)
{	
	_capacity = 2 * (end - start);
	_elem = new T[_capacity];
	_size = 0;
	for(int i = start; i < end; i++)
	{
		_elem[i] = e[i];
		_size++;
	}
}

template<typename T>
Vector<T>::Vector(const Vector<T> &v)
{
	_capacity = DEFAULT_CAPACITY;
	_size = 0;
	_elem = new T[_capacity];
	copy(v);
}

template<typename T>
Vector<T>::~Vector()
{
	delete[] _elem;
}

template<typename T>
void Vector<T>::push_back(T const &e)
{
	expand();
	_elem[_size] = e;
	_size++;
}

template<typename T>
void Vector<T>::insert(int i, T const &e)
{
	expand();
	if(_size - 1 < i)
	{
		return;
	}
	for(int j = _size; i < j; j--)
	{
		_elem[j] = _elem[j - 1];
	}
	_elem[i] = e;
	_size++;
}

template<typename T>
T Vector<T>::remove(int i)
{
	T temp = _elem[i];
	for(int j = i; j < _size - 1; j++)
	{
		_elem[j] = _elem[j + 1];
	}
	_size--;
	shrink();
	return temp;
}

template<typename T>
T &Vector<T>::operator [](int i)
{
	if(_size - 1 < i)
	{
		throw std::out_of_range("Index Out of Range!");
	}
	return _elem[i];
}


template<typename T>
Vector<T> &Vector<T>::operator = (const Vector<T> &v)
{
	copy(v);
	return *this;
}

//用copyElement()来进行深拷贝。
template<typename S>
void copyElement(S *temp, S *elem, int i)
{
	temp[i] = elem[i];
}
template<typename S>
void copyElement(Vector<S *> *temp, Vector<S *> *elem, int i)
{
	temp[i] = Vector<S *>(elem->_capacity, elem->_size);
	for(int j = 0; j < elem->_size; j++)
	{
		copyElement(temp->_elem, elem->_elem, j);
	}
}
template<typename S>
void copyElement(S **temp, S **elem, int i)
{
	if(elem[i])
	{
		temp[i] = new S;
		*temp[i] = *elem[i];
	}
	else
	{
		temp[i] = NULL;
	}
}

template<typename T>
void Vector<T>::expand()
{
	if(_capacity <= _size)
	{
		_capacity *= 2;
		T *temp = new T[_capacity];
		for(int i = 0; i < _size; i++)
		{
			//注意使用深拷贝。
			//temp[i] = elem[i];
			copyElement(temp, _elem, i);
		}
		delete[] _elem;
		_elem = temp;
	}
}

template<typename T>
void Vector<T>::shrink()
{
	if(_size <= _capacity / 4)
	{
		_capacity /= 2;
		T *temp = new T[_capacity];
		for(int i = 0; i < _size; i++)
		{
			//注意使用深拷贝。
			//temp[i] = elem[i];
			copyElement(temp, _elem, i);
		}
		delete[] _elem;
		_elem = temp;
	}
}

template<typename T>
void Vector<T>::copy(const Vector<T> &v)
{
	_size = v._size;

	//当当前容量不足时，必须先扩容，不然会出现越界的问题的。越界还会造成后续的重复delete的问题。
	if(_capacity != v._capacity)
	{
		_capacity = v._capacity;
		delete[] _elem;
		_elem = new T[_capacity];
	}
	for(int i = 0; i < v._size; i++)
	{
		//注意使用深拷贝。
		copyElement(_elem, v._elem, i);
		//elem[i] = v.elem[i];
	}
}

template<typename T>
void Vector<T>::heapSort(int lo, int hi)
{
	int n = hi - lo;
	heapify(_elem + lo, n);
	while(0 < --n)
	{
		T temp = _elem[n];
		_elem[n] = _elem[0];
		_elem[0] = temp;
		percolateDown(_elem, n, 0);
	}
}