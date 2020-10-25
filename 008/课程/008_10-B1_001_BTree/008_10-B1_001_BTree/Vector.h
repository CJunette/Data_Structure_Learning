#pragma once

const int DEFAULT_CAPACITY = 1;

template<typename T>
struct Vector
{
	T *elem;
	int size, capacity;

	//Vector(T const &e);
	Vector(int capacity = DEFAULT_CAPACITY, int size = 0, T const &e = 0);
	Vector(const Vector<T> &);
	virtual ~Vector();

	virtual void push_back(T const &e);
	virtual void insert(int i, T const &e);
	virtual T remove(int i);
	virtual T &operator [] (int i);
	virtual Vector<T> &operator = (const Vector<T> &);
	virtual int getSize() const;	
	virtual int searchLinear(const T &e);
	virtual bool empty() const;

	protected:
	virtual void expand();
	//virtual void shrink();
	void copy(const Vector<T> &);	
};

template<typename T>
Vector<T>::Vector(int capacity_, int size_, T const &e_)
{
	capacity = capacity_;
	size = size_;
	elem = new T[capacity];
	for(int i = 0; i < size; i++)
	{
		elem[i] = e_;
	}
}

template<typename T>
Vector<T>::Vector(const Vector<T> &v)
{
	capacity = DEFAULT_CAPACITY;
	size = 0;
	elem = new T[capacity];
	copy(v);
}

template<typename T>
Vector<T>::~Vector()
{
	delete[] elem;
}

template<typename T>
void Vector<T>::push_back(T const &e)
{
	expand();
	elem[size] = e;
	size++;
}

template<typename T>
void Vector<T>::insert(int i, T const &e)
{
	expand();
	if(size < i)
	{
		return;
	}
	for(int j = size; i < j; j--)
	{
		elem[j] = elem[j - 1];
	}
	size++;
	elem[i] = e;
}

template<typename T>
T Vector<T>::remove(int i)
{
	T temp = elem[i];
	elem[i] = 0;
	for(int j = i; j < size - 1; j++)
	{
		elem[j] = elem[j + 1];
		elem[j + 1] = 0;
	}
	size--;
	//shrink();
	return temp;
}

template<typename T>
T &Vector<T>::operator [](int i)
{
	if(size - 1 < i)
	{
		throw std::out_of_range("Index Out of Range!");
	}
	return elem[i];
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
	temp[i] = Vector<S *>(elem->capacity, elem->size);
	for(int j = 0; j < elem->size; j++)
	{
		copyElement(temp->elem, elem->elem, j);
	}
}
template<typename S>
void copyElement(S **temp, S **elem, int i)
{
	if(elem[i])
	{
		//这里对原先的“深拷贝”做出一些调整，因为作为存放指针的容器，我希望指针始终指向我所指的那个对象。
		temp[i] = elem[i];
		elem[i] = NULL;
	}
	else
	{
		temp[i] = NULL;
	}
}

template<typename T>
void Vector<T>::expand()
{
	if(capacity <= size)
	{
		capacity *= 2;
		T *temp = new T[capacity];

		for(int i = 0; i < size; i++)
		{
			//注意使用深拷贝。
			//temp[i] = elem[i];
			copyElement(temp, elem, i);
		}
		delete[] elem;
		elem = temp;
	}
}

/*
template<typename T>
void Vector<T>::shrink()
{
	if(size <= capacity / 4)
	{
		capacity /= 2;
		T *temp = new T[capacity];
		for(int i = 0; i < size; i++)
		{
			//注意使用深拷贝。
			//temp[i] = elem[i];
			copyElement(temp, elem, i);
		}
		delete[] elem;
		elem = temp;
	}
}
*/

template<typename T>
void Vector<T>::copy(const Vector<T> &v)
{
	size = v.size;

	//当当前容量不足时，必须先扩容，不然会出现越界的问题的。越界还会造成后续的重复delete的问题。
	if(capacity != v.capacity)
	{
		capacity = v.capacity;
		delete[] elem;
		elem = new T[capacity];
	}
	for(int i = 0; i < v.size; i++)
	{
		//注意使用深拷贝。
		copyElement(elem, v.elem, i);
		//elem[i] = v.elem[i];
	}
}

template<typename T>
int Vector<T>::getSize() const
{
	return size;
}

template<typename T>
bool Vector<T>::empty() const
{
	return size == 0;
}

template<typename T>
int Vector<T>::searchLinear(const T &e)
{
	int r = -1;
	//返回不大于e的最大元素的秩。
	while(r + 1 < size && elem[r + 1] <= e)
	{
		r++;
	}
	return r;
}