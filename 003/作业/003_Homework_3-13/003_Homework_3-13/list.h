#pragma once
#include <stdexcept>

//这里如果加分号好像会出错......
#define pNode(T) Node<T>*

//注意，在Node<T>类中要声明List<T>为友元，所以提前声明一下。
template<class T> class List;

template<class T>
class Node
{
	public:
	Node(T e = 0, pNode(T) pred = NULL, pNode(T) succ = NULL):
		_elem(e), _pred(pred), _succ(succ)
	{}

	T data() const { return _elem; }
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

	private:
	T _elem;
	Node<T> *_pred, *_succ;
	//如果有两个变量的话，最好不要合在一个句子内用define的类型名写，容易出错。
	/*
	pNode(T) _pred, _succ;
	*/
};

template<class T>
class List
{
	public:
	List() { init(); }
	~List();
	int size() const;
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
	void selectionSort1(pNode(T) p, int n);
	void selectionSort2(pNode(T) p, int n);
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
	//因为单一一个元素一定不会重复，因此从第二个元素开始。
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
		/*
		insertA(search(head->data(), r++, head), remove((head = head->succ())->pred()));
		*/
		pNode(T) q = search(p->data(), r++, p);
		insertA(q, p->data());
		p = p->succ();
		remove(p->pred());
	}
}

template<class T>
void List<T>::selectionSort1(pNode(T) p, int n)
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
void List<T>::selectionSort2(pNode(T) p, int n)
{
	pNode(T) head = p;
	pNode(T) tail = p;
	for(int i = 0; i < n - 1; i++)
	{
		tail = tail->succ();
	}

	while(1 < n)
	{
		head = head->pred();
		pNode(T) max = selectMax(head, n--);
		T temp = tail->data();;
		tail->_elem = max->data();
		max->_elem = temp;
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

	//2.p所指代的元素在合并的过程中可能会发生位置的改变，进而导致在外层的迭代中无法继续指向各个区间的首元素位置。所以在合并完整后要将其恢复到原来的位置。
	/*
	p = head->succ();
	q = p;
	for(int i = 0; i < m; i++)
	{
		q = q->succ();
	}
	*/
	merge(p, m, *this, q, n - m);
	//1.我之前是把上面的位置修改放在这里的，希望在一轮迭代完成合并时对指针进行修改。
	//3.然而由于并不能在函数中修改指针的指向，这就导致在当前迭代轮次中想要修改上层迭代的指针地址是不可能的。因此只能把修改放到2次mergeSort()之后。	
	//4.根据课件我又纠正了一下这里，课件上是通过pNode(T)&的方式来使得内层迭代可以修改外层迭代的指针。
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