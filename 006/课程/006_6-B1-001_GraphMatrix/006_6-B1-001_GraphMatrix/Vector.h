#pragma once
#include <stdexcept>

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

	protected:
	virtual void expand();
	virtual void shrink();
	void copy(const Vector<T> &);
	//template<typename S> void copy(const Vector<S*> &);
};
/*
template<typename T>
Vector<T>::Vector(T const &e_)
{
	elem = new T[1];
	size = 1;
	capacity = 1;
	elem[0] = e_;
}
*/
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
Vector<T>::Vector(const Vector<T> & v)
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
	if(size - 1 < i)
	{
		return;
	}
	for(int j = size; i < j; j--)
	{
		elem[j] = elem[j - 1];
	}
	elem[i] = e;	
	size++;
}

template<typename T>
T Vector<T>::remove(int i)
{
	T temp = elem[i];
	for(int j = i; j < size - 1; j++)
	{
		elem[j] = elem[j + 1];
	}
	size--;
	shrink();
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

//��copyElement()�����������
template<typename S>
void copyElement(S *temp, S *elem, int i)
{
	temp[i] = elem[i];
}
template<typename S>
void copyElement(Vector<S *>* temp, Vector<S *>* elem, int i)
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
	if(capacity <= size)
	{
		capacity *= 2;
		T *temp = new T[capacity];
		for(int i = 0; i < size; i++)
		{
			//ע��ʹ�������
			//temp[i] = elem[i];
			copyElement(temp, elem, i);
		}
		delete[] elem;
		elem = temp;
	}
}

template<typename T>
void Vector<T>::shrink()
{
	if(size <= capacity / 4)
	{
		capacity /= 2;
		T *temp = new T[capacity];
		for(int i = 0; i < size; i++)
		{
			//ע��ʹ�������
			//temp[i] = elem[i];
			copyElement(temp, elem, i);
		}
		delete[] elem;
		elem = temp;
	}
}

template<typename T>
void Vector<T>::copy(const Vector<T> &v)
{	
	size = v.size;	

	//����ǰ��������ʱ�����������ݣ���Ȼ�����Խ�������ġ�Խ�绹����ɺ������ظ�delete�����⡣
	if(capacity != v.capacity)
	{
		capacity = v.capacity;
		delete[] elem;
		elem = new T[capacity];
	}
	for(int i = 0; i < v.size; i++)
	{
		//ע��ʹ�������
		copyElement(elem, v.elem, i);
		//elem[i] = v.elem[i];
	}
}

/*
template<typename T>
template<typename S>
void Vector<T>::copy(const Vector<S *> &v)
{
	capacity = v.capacity;
	size = v.size;

	for(int i = 0; i < v.size; i++)
	{
		T *element = new T;
		*element = *(v.elem[i]);
		elem[i] = *element;
	}
}
*/

/*---------------------------------------------------------*/
/*
template<typename T>
struct Vector<T *>: public Vector<T>
{
	T *elem;
	int size, capacity;
	
	Vector(int capacity = DEFAULT_CAPACITY, int size = 0, T const &e = 0);
	Vector(const Vector<T *> &);
	void copy(const Vector<T *> &);
	Vector<T *> &operator = (const Vector<T *> &);
};

template<typename T>
Vector<T *>::Vector(int capacity, int size, T const &e):
	Vector(capacity, size, e)
{
	elem = this->elem;
	size = this->size;
	capacity = this->size;
}

template<typename T>
Vector<T*>::Vector(const Vector<T *> &v)
{
	copy(v);
}

template<typename T>
void Vector<T*>::copy(const Vector<T *> &v)
{
	capacity = v.capacity;
	size = v.size;

	for(int i = 0; i < v.size; i++)
	{
		T *element = new T;
		*element = v[i];
		elem[i] = element;
	}
}

template<typename T>
Vector<T *> &Vector<T*>::operator = (const Vector<T *> & v)
{
	copy(v);
	return *this;
}
*/