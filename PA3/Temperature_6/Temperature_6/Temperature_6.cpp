// Temperature_6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//�����ķ�ʽ��ͷд��

#include <iostream>
#include "temperature.h"
using namespace std;

enum SORT { X, Y, NEITHER };

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

bool isLeaf(int n, int i)
{
	return n / 2 - 1 < i;
}

bool lChildValid(int n, int i)
{
	return inHeap(n, lChild(i));
}

bool rChildValid(int n, int i)
{
	return inHeap(n, rChild(i));
}

typedef struct _station_type
{
	int x = 0;
	int y = 0;
	int temp = 0;
} station_type;

template<typename T> struct RangeNode;

int bigger(RangeNode<station_type> **PQ, int i, int j, SORT sort);

int properParent(RangeNode<station_type> **PQ, int n, int i, SORT sort)
{
	if(rChildValid(n, i))
	{
		return bigger(PQ, bigger(PQ, i, lChild(i), sort), rChild(i), sort);
	}
	else if(lChildValid(n, i))
	{
		return bigger(PQ, i, lChild(i), sort);
	}
	else
	{
		return i;
	}
}

int percolateDown(RangeNode<station_type> **A, int n, int i, SORT sort)
{
	int j;
	while(i != (j = properParent(A, n, i, sort)))
	{		
		RangeNode<station_type>* temp = A[i];
		A[i] = A[j];
		A[j] = temp;
		i = j;
	}
	return i;
}

void heapify(RangeNode<station_type> **A, int n, SORT sort)
{
	for(int i = n / 2 - 1; -1 < i; i--)
	{
		percolateDown(A, n, i, sort);
	}
}

void heapSort(RangeNode<station_type> **_elem, int lo, int hi, SORT sort)
{
	int n = hi - lo;
	heapify(_elem + lo, n, sort);
	while(0 < --n)
	{
		RangeNode<station_type> *temp = _elem[n];
		_elem[n] = _elem[0];
		_elem[0] = temp;
		percolateDown(_elem, n, 0, sort);
	}
}

/*------------------------------------------------------------------------------------*/

template<typename T>
struct Stack
{
	int size;
	int topPtr;
	int capacity;
	T *stack;

	Stack(int capacity_ = 1):
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
			expand();
		}

		stack[++topPtr] = e;
		++size;
		return true;
	}

	void unrepeatedPush(const T &e)
	{
		if(full())
		{
			expand();
		}

		for(int i = 0; i < size; i++)
		{
			if(stack[i] == e)
			{
				return;
			}
		}
		stack[++topPtr] = e;
		++size;
		return;
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

	private:
	void expand()
	{
		T *newArea = new T[capacity * 2];
		for(int i = 0; i < size; i++)
		{
			newArea[i] = stack[i];
		}
		delete[] stack;
		stack = newArea;
		capacity *= 2;
	}
};

template<typename T>
struct QueNode
{
	T _data;
	QueNode<T> *_prev;
	QueNode<T> *_succ;
	QueNode(T data_, QueNode<T> *prev_ = NULL, QueNode<T> *succ_ = NULL):
		_data(data_), _prev(prev_), _succ(succ_) {}
};
template<typename T>
struct Queue
{
	int _size;
	QueNode<T> *_front;
	QueNode<T> *_tailer;

	Queue():
		_size(0)
	{
		_front = new QueNode<T>(0, NULL, NULL);
		_tailer = new QueNode<T>(0, NULL, NULL);
		_front->_succ = _tailer;
		_tailer->_prev = _front;
	}

	~Queue()
	{
		QueNode<T> *trav = _front->_succ;
		while(trav != _tailer)
		{
			QueNode<T> *next = trav->_succ;
			delete trav;
			trav = next;
		}

		delete _front;
		delete _tailer;
	}

	bool empty() const { return _size == 0; }

	void enqueue(const T &e)
	{
		QueNode<T> *beforeTailer = _tailer->_prev;
		beforeTailer->_succ = new QueNode<T>(e, _tailer->_prev, _tailer);
		_tailer->_prev = beforeTailer->_succ;
		_size++;
	}

	T dequeue()
	{
		if(_size == 0)
		{
			return T();
		}
		QueNode<T> *afterFront = _front->_succ;
		T data = afterFront->_data;
		_front->_succ = afterFront->_succ;
		_front->_succ->_prev = _front;
		delete afterFront;
		_size--;
		return data;
	}

	T &front()
	{
		return _front->_succ->_data;
	}
};

/*------------------------------------------------------------------------------------*/

template<typename T>
struct RangeTree;

template<typename T>
struct RangeNode
{
    T _data;
    int _sizeBelow;
    RangeNode<T> *_parent;
    RangeNode<T> *_lc;
    RangeNode<T> *_rc;
	RangeNode<T> *_largestBelow;
	RangeTree<T> *_ptr;
	RangeNode<T> *_succ;

    RangeNode(T data_ = {0, 0, 0}, int sizeBelow_ = 0, RangeNode *parent_ = NULL, RangeNode *lc_ = NULL, RangeNode *rc_ = NULL, RangeNode *largestBelow_ = NULL):
        _data(data_), _sizeBelow(sizeBelow_), _parent(parent_), _lc(lc_), _rc(rc_), _largestBelow(largestBelow_), _ptr(NULL), _succ(NULL){}

    RangeNode *insertAsLChild(T _data)
    {
        _lc = new RangeNode(_data, 0, this, NULL, NULL);
        return _lc;
    }
    RangeNode *insertAsRChild(T _data)
    {
        _rc = new RangeNode(_data, 0, this, NULL, NULL);
        return _rc;
    }
};

int bigger(RangeNode<station_type> **PQ, int i, int j, SORT sort)
{
	switch(sort)
	{
		case X:
			return PQ[i]->_data.x < PQ[j]->_data.x ? j : i;
		case Y:
			return PQ[i]->_data.y < PQ[j]->_data.y ? j : i;
	}
}

/*------------------------------------------------------------------------------------*/

template<typename T>
bool isLChild(RangeNode<T>* x)
{
    return x->_parent->_lc == x;
}

template<typename T>
bool isRChild(RangeNode<T> * x)
{
    return x->_parent->_rc == x;
}

template<typename T>
RangeNode<T> *&fromParent(RangeNode<T> *x)
{
    if(isLChild(x))
    {
        return x->_parent->_lc;
    }
    else
    {
        return x->_parent->_rc;
    }
}

/*------------------------------------------------------------------------------------*/

int calTreeSizeExponent(int n)
{
	int i = 1;
	int j = 0;
	while(i < n)
	{
		i *= 2;
		j++;
	}
	return j + 1;
}

int facOfTwo(int n)
{
	int i = 1;
	while(0 < n--)
	{
		i *= 2;
	}
	return i;
}

template<typename T>
struct Delete
{
	void operator ()(T *&x)
	{
		delete x;
		x = NULL;
	}
};

template<typename T>
struct RangeTree
{
    RangeNode<T> *_root;
    int _size;

    RangeTree():
        _root(NULL), _size(0){}

	~RangeTree()
	{
		travPre(_root, Delete<RangeNode<T>>());
	}

	template<typename VST>
	void travPre(RangeNode<T> *x, VST visit)
	{
		Stack<RangeNode<T> *> stack;
		stack.push(x);
		while(!stack.empty())
		{			
			x = stack.pop();

			if(x->_rc)
			{
				stack.push(x->_rc);
			}
			if(x->_lc)
			{
				stack.push(x->_lc);
			}

			visit(x);
		}
	}
	
};

int buildTree(RangeNode<station_type>** arr, int n)
{
	int treeSize = n;
	int size = n;
	while(1 < size)
	{
		int count = 0;
		for(int i = 0; i < size; i += 2)
		{
			station_type sta = { arr[i]->_largestBelow->_data.x,  arr[i]->_largestBelow->_data.y, arr[i]->_largestBelow->_data.temp };
			if(i + 1 < size)
			{
				int sizeBelow = arr[i]->_sizeBelow + arr[i + 1]->_sizeBelow;
				RangeNode<station_type> *newNode = new RangeNode<station_type>(sta, sizeBelow, NULL, arr[i], arr[i + 1], arr[i + 1]->_largestBelow);
				newNode->_lc->_parent = newNode;
				newNode->_rc->_parent = newNode;
				count++;
				arr[i + 1] = NULL;
				arr[i] = newNode;
			}
			else
			{
				int sizeBelow = arr[i]->_sizeBelow;
				RangeNode<station_type> *newNode = new RangeNode<station_type>(sta, sizeBelow, NULL, arr[i], NULL, arr[i]->_largestBelow);
				newNode->_lc->_parent = newNode;
				arr[i] = newNode;
			}
		}

		int j = 0;
		for(int i = 0; i < size - count; i++)
		{
			arr[i] = arr[j];
			if(0 < j)
			{
				arr[j] = NULL;
			}
			j += 2;
		}

		size = size - count;
		treeSize += size;
	}

	return treeSize;
}

struct AssociatedTree
{
	void operator() (RangeNode<station_type>* node)
	{
		int count = node->_sizeBelow;
		Queue<RangeNode<station_type> *> que;
		que.enqueue(node);

		while(que.front()->_lc)
		{
			RangeNode<station_type> *currentNode = que.dequeue();
			que.enqueue(currentNode->_lc);
			if(currentNode->_rc)
			{
				que.enqueue(currentNode->_rc);
			}
		}

		RangeNode<station_type> **arr = new RangeNode<station_type> *[count];

		for(int i = 0; i < count; i++)
		{
			RangeNode<station_type> *copyNode = new RangeNode<station_type>(que.dequeue()->_data);
			copyNode->_sizeBelow = 1;
			copyNode->_largestBelow = copyNode;
			arr[i] = copyNode;
		}

		heapSort(arr, 0, count, Y);
		//ע�⣬���������֮���ٶ�arr�еĽڵ����_succ�����á�
		for(int i = 0; i < count; i++)
		{
			if(i > 0)
			{
				arr[i - 1]->_succ = arr[i];
			}
		}

		int treeSize = buildTree(arr, count);

		node->_ptr = new RangeTree<station_type>;
		node->_ptr->_root = arr[0];
		node->_ptr->_size = treeSize;
		arr[0] = NULL;
		delete[] arr;
	}
};

RangeTree<station_type> *createTree(int n)
{
	RangeNode<station_type> **arr = new RangeNode<station_type>*[n];
	int x, y, temp;
	for(int i = 0; i < n; i++)
	{
		GetStationInfo(i, &x, &y, &temp);
		arr[i] = new RangeNode<station_type>({ x, y, temp });
		arr[i]->_largestBelow = arr[i];
		arr[i]->_sizeBelow = 1;
		if(i > 0)
		{
			arr[i - 1]->_succ = arr[i];
		}
	}
	heapSort(arr, 0, n, X);
	
	int treeSize = buildTree(arr, n);

	//��������Ĺ��졣
	RangeTree<station_type> *tree = new RangeTree<station_type>;
	tree->_root = arr[0];
	tree->_size = treeSize;
	arr[0] = NULL;
	delete[] arr;

	//������Ҫ���й������Ľ��졣

	tree->travPre(tree->_root, AssociatedTree());

	return tree;
}

struct DeleteTree
{
	void operator() (RangeNode<station_type> *node)
	{
		RangeTree<station_type> *ptr = node->_ptr;

		Queue<RangeNode<station_type> *> que;
		que.enqueue(ptr->_root);

		while(que.front()->_lc)
		{
			RangeNode<station_type> *currentNode = que.dequeue();
			que.enqueue(currentNode->_lc);
			if(currentNode->_rc)
			{
				que.enqueue(currentNode->_rc);
			}
			//currentNode->_largestBelow = NULL;
			delete currentNode;
		}
		while(!que.empty())
		{
			RangeNode<station_type> *currentNode = que.dequeue();			
			//currentNode->_largestBelow = NULL;
			delete currentNode;
		}

		ptr = NULL;
		delete ptr;
	}
};

RangeNode<station_type>* searchInTree(int e, RangeTree<station_type>* tree, SORT sort)
{
	RangeNode<station_type> *root = tree->_root;
	RangeNode<station_type> *key = root;
	
	switch(sort)
	{
		case X:
			while(key->_lc)
			{
				if(e <= key->_data.x)
				{
					key = key->_lc;
				}
				else
				{
					key = key->_rc;
				}
			}
			break;
		case Y:
			while(key->_lc)
			{
				if(e <= key->_data.y)
				{
					key = key->_lc;
				}
				else
				{
					key = key->_rc;
				}
			}
			break;		
	}
	
	if(!key)
	{
		key = root->_largestBelow;
	}
	return key;
}

long long calAvr(int &x1, int &y1, int &x2, int &y2, RangeTree<station_type>* tree)
{
	RangeNode<station_type> *left = searchInTree(x1, tree, X);
	RangeNode<station_type> *right = searchInTree(x2, tree, X);

	RangeNode<station_type> *divide;

	RangeNode<station_type> *lSeek = left;
	RangeNode<station_type> *rSeek = right;
	while(lSeek != rSeek)
	{
		lSeek = lSeek->_parent;
		rSeek = rSeek->_parent;
	}
	divide = lSeek;

	int count = tree->_root->_sizeBelow;
	RangeTree<station_type> **arr = new RangeTree<station_type> *[count];

	int index = 0;

	//��������ֻ��һ��Ԫ�ص������
	if(!divide->_lc)
	{
		arr[index++] = divide->_ptr;		
	}
	else
	{
		RangeNode<station_type> *lDivide = divide->_lc;
		RangeNode<station_type> *rDivide = divide->_rc;

		//�������������Ҫ�Ĺ���������arr��
		while(lDivide != left)
		{
			if(left->_data.x <= lDivide->_data.x)
			{
				arr[index++] = lDivide->_rc->_ptr;
				lDivide = lDivide->_lc;
			}			
			else
			{
				lDivide = lDivide->_rc;
			}
		}
		arr[index++] = lDivide->_ptr;

		//��������Ҳ����Ҫ�Ĺ���������arr��
		while(rDivide != right)
		{
			if(rDivide->_data.x < right->_data.x)
			{
				arr[index++] = rDivide->_lc->_ptr;
				rDivide = rDivide->_rc;
			}
			else
			{
				rDivide = rDivide->_lc;
			}
		}
		if(rDivide->_data.x <= x2)
		{
			arr[index++] = rDivide->_ptr;
		}
	}

	long long totalTemp = 0;
	long long totalStations = 0;

	for(int i = 0; i < index; i++)
	{
		RangeNode<station_type> *down = searchInTree(y1, arr[i], Y);
		RangeNode<station_type> *up = searchInTree(y2, arr[i], Y);

		if(down == up)
		{
			if(up->_data.y <= y2 && y1 <= down->_data.y)
			{
				totalTemp += up->_data.temp;
				totalStations++;
			}
		}
		else
		{
			RangeNode<station_type> *start = down;
			while(down != up)
			{
				totalTemp += down->_data.temp;
				totalStations++;
				down = down->_succ;
			}
			if(up->_data.y <= y2)
			{
				totalTemp += up->_data.temp;
				totalStations++;
			}
		}
		/*
		Queue<RangeNode<station_type> *> que;
		que.enqueue(arr[i]->_root);

		while(que.front()->_lc)
		{
			RangeNode<station_type> *currentNode = que.dequeue();
			que.enqueue(currentNode->_lc);
			if(currentNode->_rc)
			{
				que.enqueue(currentNode->_rc);
			}						
		}

		while(!que.empty())
		{
			RangeNode<station_type> *currentNode = que.dequeue();

			if(currentNode == down)
			{
				while(!que.empty() && currentNode != up)
				{
					totalTemp += currentNode->_data.temp;
					totalStations++;
					currentNode = que.dequeue();
				}

				if(up->)
				{

				}
			}			
		}
		*/
	}

	return totalStations == 0 ? 0 : totalTemp / totalStations;
}

int main()
{	
	int n;
	n = GetNumOfStation();
	int x1, x2, y1, y2;

	if(n == 0)
	{
		while(GetQuery(&x1, &y1, &x2, &y2))
		{
			Response(0);
		}
		return 0;
	}

	RangeTree<station_type> *tree = createTree(n);

	while(GetQuery(&x1, &y1, &x2, &y2))
	{
		int temp = calAvr(x1, y1, x2, y2, tree);
		Response(temp);
	}

	tree->travPre(tree->_root, DeleteTree());
}