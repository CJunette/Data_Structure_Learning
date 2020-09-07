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
			//注意使用深拷贝。
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
			//注意使用深拷贝。
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

/*---------------------------------------------------------------------*/

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

/*---------------------------------------------------------------------*/

#define pNode(T) Node<T>*

template<class T> class List;

template<class T>
class Node
{
	public:
	Node(T e = 0, pNode(T) pred = NULL, pNode(T) succ = NULL):
		_elem(e), _pred(pred), _succ(succ)
	{}

	T &data() { return _elem; }
	pNode(T) pred() const { return _pred; }
	pNode(T) succ() const { return _succ; }
	pNode(T) insertAsPred(T const &e)
	{
		pNode(T) n = new Node(e, _pred, this);
		_pred->_succ = n;
		_pred = n;
		return n;
	}
	pNode(T) insertAsSucc(T const &e)
	{
		pNode(T) n = new Node(e, this, _succ);
		_succ->_pred = n;
		_succ = n;
		return n;
	}

	friend class List<T>;

	protected:
	T _elem;
	Node<T> *_pred, *_succ;
};

template<class T>
class List
{
	public:
	List() { init(); }
	~List();
	int size() const;
	bool empty() const;
	void show() const;
	pNode(T) first();
	pNode(T) last();
	T remove(pNode(T) p);
	pNode(T) insertA(pNode(T) p, T const &e);
	pNode(T) insertB(pNode(T) p, T const &e);
	pNode(T) insertAsFirst(T const &e);
	pNode(T) insertAsLast(T const &e);
	pNode(T) find(T const &e, int n, pNode(T) p) const;
	int deduplicate();
	pNode(T) search(T const &e, int n, pNode(T) p) const;
	//对p位置后的n个元素进行排序。
	void insertionSort(pNode(T) p, int n);
	void selectionSort(pNode(T) p, int n);
	pNode(T) selectMax(pNode(T) p, int n);
	void mergeSort(pNode(T) &p, int n);


	protected:
	void init();
	int clear();
	void merge(pNode(T) p1, int n1, List<T> &list, pNode(T) p2, int n2);

	private:
	int _size;
	Node<T> *header, *tailer;

};

template<class T>
void List<T>::init()
{
	_size = 0;
	header = new Node<T>(0, NULL, tailer);
	tailer = new Node<T>(0, header, NULL);
}

template<class T>
int List<T>::clear()
{
	int oldsize = _size;
	while((header->succ()) != tailer)
	{
		remove(header->succ());
	}
	_size = 0;
	return oldsize;
}

template<class T>
List<T>::~List()
{
	clear();
	delete header;
	delete tailer;
}

template<class T>
int List<T>::size() const
{
	return _size;
}

template<class T>
bool List<T>::empty() const
{
	return _size == 0;
}

template<class T>
void List<T>::show() const
{
	pNode(T) p = header;
	while((p = p->succ()) != tailer)
	{
		std::cout << p->data() << " ";
	}
	std::cout << std::endl;
}

template<class T>
pNode(T) List<T>::first()
{
	return header->succ();
}

template<class T>
pNode(T) List<T>::last()
{
	return tailer->pred();
}

template<class T>
T List<T>::remove(pNode(T) p)
{
	if((p == header) || (p == tailer))
	{
		throw "Error! Can't remove header or tailer.";
	}
	p->_succ->_pred = p->_pred;
	p->_pred->_succ = p->_succ;
	T e = p->data();
	delete p;
	_size--;
	return e;
}

template<class T>
pNode(T) List<T>::insertA(pNode(T) p, T const &e)
{
	if(p == tailer)
	{
		throw "Error! Can't insert after tailer.";
	}
	_size++;
	return p->insertAsSucc(e);
}

template<class T>
pNode(T) List<T>::insertB(pNode(T) p, T const &e)
{
	if(p == header)
	{
		throw "Error! Can't insert before header.";
	}
	_size++;
	return p->insertAsPred(e);
}

template<class T>
pNode(T) List<T>::insertAsFirst(T const &e)
{
	return insertA(header, e);
}

template<class T>
pNode(T) List<T>::insertAsLast(T const &e)
{
	return insertB(tailer, e);
}

template<class T>
pNode(T) List<T>::find(T const &e, int n, pNode(T) p) const
{
	while(0 < n--)
	{
		if((p = p->pred())->data() == e)
		{
			return p;
		}
	}
	return NULL;
}

template<class T>
int List<T>::deduplicate()
{
	if(_size < 2)
	{
		return 0;
	}
	int oldsize = _size;
	int r = 1;
	pNode(T) p = header->_succ->_succ;
	while(p != tailer)
	{
		pNode(T) t = p;
		pNode(T) q = find(p->data(), r, p);
		q ? remove(q) : r++;
		p = t->succ();
	}
	return _size - oldsize;
}

template<class T>
pNode(T) List<T>::search(T const &e, int n, pNode(T) p) const
{
	while(0 < n--)
	{
		if((p = p->pred())->data() <= e)
		{
			return p;
		}
	}
	return header;
}

template<class T>
void List<T>::insertionSort(pNode(T) p, int n)
{
	int r = 0;
	while(r < n)
	{
		pNode(T) q = search(p->data(), r++, p);
		insertA(q, p->data());
		p = p->succ();
		remove(p->pred());
	}
}

template<class T>
void List<T>::selectionSort(pNode(T) p, int n)
{
	pNode(T) head = p;
	pNode(T) tail = p;
	for(int i = 0; i < n; i++)
	{
		tail = tail->succ();
	}

	while(1 < n)
	{
		head = head->pred();
		pNode(T) max = selectMax(head, n--);
		insertB(tail, max->data());
		remove(max);
		tail = tail->pred();
		head = head->succ();
	}
}

template<class T>
pNode(T) List<T>::selectMax(pNode(T) p, int n)
{
	pNode(T) max = p;

	while(0 < n--)
	{
		if(max->data() <= (p = p->succ())->data())
		{
			max = p;
		}
	}
	return max;
}

template<class T>

void List<T>::mergeSort(pNode(T) &p, int n)
{
	if(n == 1)
	{
		return;
	}
	int m = n >> 1;
	pNode(T) q = p;
	for(int i = 0; i < m; i++)
	{
		q = q->succ();
	}
	pNode(T) head = p->pred();
	mergeSort(p, m);
	mergeSort(q, n - m);
	merge(p, m, *this, q, n - m);

	p = head->succ();
}

template<class T>
void List<T>::merge(pNode(T) p, int n, List<T> &list, pNode(T) q, int m)
{
	while(0 < m)
	{
		if(n == 0)
		{
			return;
		}
		if(p->data() <= q->data())
		{
			n--;
			p = p->succ();
		}
		else
		{
			insertB(p, q->data());
			q = q->succ();
			remove(q->pred());
			m--;
		}
	}
}

/*---------------------------------------------------------------------*/

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