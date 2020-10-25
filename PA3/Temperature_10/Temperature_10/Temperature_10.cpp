// Temperature_10.cpp : This file contains the 'main' function. Program execution begins and ends there.
//把用于排序的array放到原来的_ptr里去。
//将数组用Vecotor代替。
//修改无效。

//下面注释掉的这段代码用于进行内存泄漏检测。
/*
#include <crtdbg.h>
#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif // _DEBUG
*/

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

const int DEFAULT_CAPACITY = 1;

template<typename T>
struct Vector;

//用copyElement()来进行深拷贝。
template<typename S>
void copyElement(S *temp, S *elem, int i)
{
	temp[i] = elem[i];
};
template<typename S>
void copyElement(Vector<S *> *temp, Vector<S *> *elem, int i)
{
	temp[i] = Vector<S *>(elem->_capacity, elem->_size);
	for(int j = 0; j < elem->_size; j++)
	{
		copyElement(temp->_elem, elem->_elem, j);
	}
};
//对这里的深拷贝进行了修改。
template<typename S>
void copyElement(S **temp, S **elem, int i)
{
	if(elem[i])
	{
		temp[i] = elem[i];
		elem[i] = NULL;
	}
	else
	{
		temp[i] = NULL;
	}
};

template<typename T>
struct Vector
{
	T *_elem;
	int _size, _capacity;

	Vector(int capacity_ = DEFAULT_CAPACITY, int size_ = 0)
	{
		_capacity = capacity_;
		_size = size_;
		_elem = new T[_capacity];
	}

	Vector(T const *e, int start, int end)
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

	Vector(const Vector<T> &v)
	{
		_capacity = DEFAULT_CAPACITY;
		_size = 0;
		_elem = new T[_capacity];
		copy(v);
	}

	virtual ~Vector()
	{
		/*
		for(int i = 0; i < _size; i++)
		{
			release(_elem[i]);
		}
		*/
		delete[] _elem;
	}

	virtual void push_back(T const &e)
	{
		expand();
		_elem[_size] = e;
		_size++;
	}

	virtual void insert(int i, T const &e)
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

	virtual T remove(int i)
	{
		T temp = _elem[i];
		for(int j = i; j < _size - 1; j++)
		{
			_elem[j] = _elem[j + 1];
		}
		_size--;
		//shrink();
		return temp;
	}

	virtual T &operator [] (int i)
	{

		if(_size - 1 < i)
		{
			throw "Index Out of Range!";
		}

		return _elem[i];
	}

	virtual Vector<T> &operator = (const Vector<T> &v)
	{
		copy(v);
		return *this;
	}


	virtual int size() const { return _size; }
	virtual bool empty() const { return _size == 0; }

	protected:
	virtual void expand()
	{
		if(_capacity <= _size)
		{
			_capacity *= 2;
			T *temp = new T[_capacity];
			for(int i = 0; i < _size; i++)
			{
				//注意使用深拷贝。
				//temp[i] = _elem[i];
				copyElement(temp, _elem, i);
			}
			delete[] _elem;
			_elem = temp;
		}
	}
	virtual void shrink()
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
	void copy(const Vector<T> &v)
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
};

/*------------------------------------------------------------------------------------*/

template<typename T> struct RangeNode;

int bigger(Vector<RangeNode<station_type> *> &PQ, int i, int j, SORT sort);

int properParent(Vector<RangeNode<station_type> *> &PQ, int n, int i, SORT sort)
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

int percolateDown(Vector<RangeNode<station_type> *> &A, int n, int i, SORT sort)
{
	int j;
	while(i != (j = properParent(A, n, i, sort)))
	{
		RangeNode<station_type> *temp = A[i];
		A[i] = A[j];
		A[j] = temp;
		i = j;
	}
	return i;
}

void heapify(Vector<RangeNode<station_type> *> &A, int n, SORT sort)
{
	for(int i = n / 2 - 1; -1 < i; i--)
	{
		percolateDown(A, n, i, sort);
	}
}

void heapSort(Vector<RangeNode<station_type> *> &_elem, int lo, int hi, SORT sort)
{
	int n = hi - lo;
	heapify(_elem, n, sort);
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
struct RangeNode
{
	T _data;
	int _sizeBelow;
	RangeNode<T> *_parent;
	RangeNode<T> *_lc;
	RangeNode<T> *_rc;
	RangeNode<T> *_largestBelow;
	Vector<RangeNode<T> *>_ptr;
	RangeNode<T> *_succ;

	RangeNode(T data_ = { 0, 0, 0 }, int sizeBelow_ = 0, RangeNode *parent_ = NULL, RangeNode *lc_ = NULL, RangeNode *rc_ = NULL, RangeNode *largestBelow_ = NULL):
		_data(data_), _sizeBelow(sizeBelow_), _parent(parent_), _lc(lc_), _rc(rc_), _largestBelow(largestBelow_), _ptr(), _succ(NULL) {}

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

int bigger(Vector<RangeNode<station_type> *> &PQ, int i, int j, SORT sort)
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
bool isLChild(RangeNode<T> *x)
{
	return x->_parent->_lc == x;
}

template<typename T>
bool isRChild(RangeNode<T> *x)
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

int log(int n)
{
	int count = 0;
	while(1 < n)
	{
		n /= 2;
		count++;
	}
	return count;
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
		_root(NULL), _size(0) {}

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

int buildTree(Vector<RangeNode<station_type> *>& arr, int n)
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
				if(!newNode)
				{
					throw "Memory Exceed in buildTree-newNode";
				}
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
				if(!newNode)
				{
					throw "Memory Exceed in buildTree-newNode";
				}
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

	//返回值为树中非空节点的个数。
	return treeSize;
}

struct AssociatedTree
{
	void operator() (RangeNode<station_type> *node)
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
		
		for(int i = 0; i < count; i++)
		{
			RangeNode<station_type> *copyNode = new RangeNode<station_type>(que.dequeue()->_data);
			if(!copyNode)
			{
				throw "Memory Exceed in AssociatedTree-copyNode";
			}
			copyNode->_sizeBelow = 1;
			copyNode->_largestBelow = copyNode;
			node->_ptr.push_back(copyNode);
		}

		heapSort(node->_ptr, 0, count, Y);
		//注意，等完成排序之后再对arr中的节点进行_succ的设置。
		for(int i = 0; i < count; i++)
		{
			if(i > 0)
			{
				node->_ptr[i - 1]->_succ = node->_ptr[i];
			}
		}
	}
};

RangeTree<station_type> *createTree(int n)
{
	Vector<RangeNode<station_type> *> arr(n, n);
	if(!arr._elem)
	{
		throw "Memory Exceed in createTree-arr";
	}
	int x, y, temp;
	for(int i = 0; i < n; i++)
	{
		GetStationInfo(i, &x, &y, &temp);
		arr[i] = new RangeNode<station_type>({ x, y, temp });
		if(!arr[i])
		{
			throw "Memory Exceed in createTree-arr[i]";
		}
		arr[i]->_largestBelow = arr[i];
		arr[i]->_sizeBelow = 1;
	}
	heapSort(arr, 0, n, X);

	for(int i = 0; i < n; i++)
	{
		if(i > 0)
		{
			arr[i - 1]->_succ = arr[i];
		}
	}

	int treeSize = buildTree(arr, n);

	//完成主树的构造。
	RangeTree<station_type> *tree = new RangeTree<station_type>;
	if(!tree)
	{
		throw "Memory Exceed in createTree-tree";
	}
	tree->_root = arr[0];
	tree->_size = treeSize;
	arr[0] = NULL;

	//下面需要进行关联树的建造。

	tree->travPre(tree->_root, AssociatedTree());

	return tree;
}

struct DeleteTree
{
	void operator() (RangeNode<station_type> *node)
	{
		RangeNode<station_type> *start = node->_ptr[0];

		while(start)
		{
			RangeNode<station_type> *next = start->_succ;
			delete start;
			start = next;
		}		
	}
};

int searchInArray(int y, Vector<RangeNode<station_type> *> &arr, int size)
{
	int left = 0, right = size;

	while(left < right)
	{
		int med = (right + left) >> 1;
		if(y < arr[med]->_data.y)
		{
			right = med;
		}
		else
		{
			left = med + 1;
		}
	}
	if(left == size || ((0 <= left - 1) && arr[left - 1]->_data.y == y))
	{
		return left - 1;
	}
	else
	{
		return left;
	}
}

RangeNode<station_type> *searchInTree(int e, RangeTree<station_type> *tree, SORT sort)
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

long long calAvr(int &x1, int &y1, int &x2, int &y2, RangeTree<station_type> *tree)
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

	int e = calTreeSizeExponent(count);
	int size = facOfTwo(e) - 1;
	int height = log(size) * 2 + 1;

	Vector<Vector<RangeNode<station_type> *> *> arr;

	//RangeNode<station_type> ***arr = new RangeNode<station_type> * *[count];
	if(!arr._elem)
	{
		throw "Memory Exceed in searchInTree-arr";
	}

	/*
	int index = 0;
	int *arrSize = new int[height];
	if(!arrSize)
	{
		throw "Memory Exceed in searchInTree-arrSize";
	}
	*/

	//处理树中只有一个元素的情况。
	if(!divide->_lc)
	{
		arr.push_back(&divide->_ptr);
	}
	else
	{
		RangeNode<station_type> *lDivide = divide->_lc;
		RangeNode<station_type> *rDivide = divide->_rc;

		//将分歧点左侧的需要的关联树纳入arr。
		while(lDivide != left)
		{
			if(left->_data.x <= lDivide->_data.x)
			{
				arr.push_back(&lDivide->_rc->_ptr);
				lDivide = lDivide->_lc;
			}
			else
			{
				lDivide = lDivide->_rc;
			}
		}
		arr.push_back(&lDivide->_ptr);

		//将分歧点右侧的需要的关联树纳入arr。
		while(rDivide != right)
		{
			if(rDivide->_data.x < right->_data.x)
			{
				arr.push_back(&rDivide->_lc->_ptr);
				rDivide = rDivide->_rc;
			}
			else
			{
				rDivide = rDivide->_lc;
			}
		}
		if(rDivide->_data.x <= x2)
		{
			arr.push_back(&rDivide->_ptr);
		}
	}

	long long totalTemp = 0;
	long long totalStations = 0;

	for(int i = 0; i < arr._size; i++)
	{
		int down = searchInArray(y1, *arr[i], (*arr[i])._size);
		int up = searchInArray(y2, *arr[i], (*arr[i])._size);

		if(down == up)
		{
			if((*arr[i])[up]->_data.y <= y2 && y1 <= (*arr[i])[down]->_data.y)
			{
				totalTemp += (*arr[i])[up]->_data.temp;
				totalStations++;
			}
		}
		else
		{
			int start = down;
			while(start < up)
			{
				totalTemp += (*arr[i])[start]->_data.temp;
				totalStations++;
				start++;
			}
			if((*arr[i])[up]->_data.y <= y2)
			{
				totalTemp += (*arr[i])[up]->_data.temp;
				totalStations++;
			}
		}
	}

	for(int i = 0; i < arr._size; i++)
	{
		arr[i] = NULL;
	}

	return totalStations == 0 ? 0 : totalTemp / totalStations;
}

int main()
{
	//_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

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
	delete tree;
}