// TSP_2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//邻接矩阵占据空间过大，做出修改。将E转为用类列表节点的结构构成的数组。

#include <iostream>
using namespace std;

enum VStatus
{
	UNDISCOVERED, DISCOVERED, VISITED
};

enum EType
{
	UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD
};

template<typename T>
struct Stack
{
	int size;
	int topPtr;
	int capacity;
	T *stack;

	Stack(int capacity_):
		size(0), topPtr(-1), capacity(capacity_)
	{
		stack = new T[capacity_];
	}
	~Stack()
	{
		delete[] stack;
	}

	bool empty() const { return size == 0; }
	bool full() const { return size == capacity; }

	bool push(const T &e)
	{
		if(full())
		{
			return false;
		}
		else
		{
			stack[++topPtr] = e;
			++size;
			return true;
		}
	}

	T pop()
	{
		if(!empty())
		{
			T temp = stack[topPtr];
			stack[topPtr--] = 0;
			size--;
			return temp;
		}
	}

	T &top()
	{
		return stack[topPtr];
	}
};

#pragma once
#include <stdexcept>


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



template<typename T>
struct ListNode
{
	T data;
	ListNode<T> *pred;
	ListNode<T> *succ;

	ListNode<T>(const T &e = 0, ListNode<T> *pred_ = NULL, ListNode<T> *succ_ = NULL) :
		data(e), pred(pred_), succ(succ_)
	{}
};

struct Vertex
{
	int data;
	int inDegree, outDegree;
	int height;
	VStatus status;
	int parent;

	Vertex(const int &e = 0):
		data(0), height(e), inDegree(0), outDegree(0), status(UNDISCOVERED), parent(-1)
	{}
};

struct GraphMatrix
{
	Vertex *V;
	ListNode<int> *E;

	int n;
	int e;

	GraphMatrix(int n_ = 0):
		n(n_), e(0)
	{
		V = new Vertex[n]();
		E = new ListNode<int>[n];
		for(int i = 0; i < n; i++)
		{
			V[i].data = i + 1;
			E[i].data = i + 1;
		}
	}
	~GraphMatrix()
	{
		delete[] V;
		for(int i = 0; i < n; i++)
		{
			ListNode<int> *p = E[i].succ;
			while(p != NULL)
			{
				ListNode<int> *next = p->succ;
				delete p;
				p = next;
			}
		}
		delete[] E;
	}

	void insert(int i, int j)
	{
		//查重检验暂时搁置。
		e++;

		ListNode<int> *p = &E[j];
		while(p->succ != NULL)
		{
			p = p->succ;
		}
		p->succ = new ListNode<int>(i);

		V[i].outDegree++;
		V[j].inDegree++;
	}

	ListNode<int> *lastNbr(int i, ListNode<int> *start)
	{
		ListNode<int> *p = start->succ;
		return p;
	}

	ListNode<int> *firstNbr(int i)
	{
		return E[i].succ;
	}

	void longest()
	{
		int i = 0;
		Queue<int> store;

		while(i < n && V[i].status == UNDISCOVERED)
		{
			if(V[i].outDegree == 0)
			{
				V[i].status = DISCOVERED;
				store.enqueue(i);

				while(!store.empty())
				{
					int k = store.dequeue();

					for(ListNode<int> *j = firstNbr(k); j != NULL; j = lastNbr(k, j))
					{
						V[j->data].height = (V[j->data].height < V[k].height + 1) ? V[k].height + 1 : V[j->data].height;
						V[j->data].status = DISCOVERED;
						store.enqueue(j->data);
					}
				}
			}
			i++;
		}
	}
};

int main()
{
	int n, m;
	scanf("%d%d", &n, &m);

	GraphMatrix graph(n);

	for(int k = 0; k < m; k++)
	{
		int i, j;
		scanf("%d%d", &i, &j);

		graph.insert(i - 1, j - 1);
	}

	graph.longest();

	int max = 0;
	for(int i = 0; i < n; i++)
	{
		max = max < graph.V[i].height ? graph.V[i].height : max;
	}

	printf("%d", max + 1);
}
