// Temperature_2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//�޷�ͨ�����ԣ����ݽṹ�ƺ�̫���ӡ�

#include <iostream>
#include "temperature.h"
using namespace std;

enum SORT { X, Y, NEITHER };

/*--------------------------------------------------------*/

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

template<typename T>
int bigger(const T *PQ, int i, int j, SORT sort)
{
	switch(sort)
	{
		case X:
			return PQ[i]._x < PQ[j]._x ? j : i;
		case Y:
			return PQ[i]._y < PQ[j]._y ? j : i;
	}
}

template<typename T>
int properParent(const T *PQ, int n, int i, SORT sort)
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

template<typename T>
int percolateUp(T *A, int i)
{
	T temp = A[i];
	while(0 < i)
	{
		int j = parent(i);
		if(A[j] < temp)
		{
			A[i] = A[j];
			i = j;
		}
		else
		{
			break;
		}
	}
	A[i] = temp;
	return i;
}

template<typename T>
int percolateDown(T *A, int n, int i, SORT sort)
{
	int j;
	while(i != (j = properParent(A, n, i, sort)))
	{
		//������԰�percolateUp()�е�˼·��΢����һ���޸ġ�����Ϊ�����漰��properParent()�ĸĶ��������Ҿ���ʱ���������ˡ�
		T temp = A[i];
		A[i] = A[j];
		A[j] = temp;
		i = j;
	}
	return i;
}

template<typename T>
void heapify(T *A, int n, SORT sort)
{
	for(int i = n / 2 - 1; -1 < i; i--)
	{
		percolateDown(A, n, i, sort);
	}
}

template<typename T>
void heapSort(T *_elem, int lo, int hi, SORT sort)
{
	int n = hi - lo;
	heapify(_elem + lo, n, sort);
	while(0 < --n)
	{
		T temp = _elem[n];
		_elem[n] = _elem[0];
		_elem[0] = temp;
		percolateDown(_elem, n, 0, sort);
	}
}

template<typename T>
void release(T)
{}

template<typename T>
void release(T *e)
{
	delete e;
}

/*--------------------------------------------------------*/

const int DEFAULT_CAPACITY = 1;

template<typename T>
struct Vector;

//��copyElement()�����������
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
//�����������������޸ġ�
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

	Vector(const Vector<T> & v)
	{
		_capacity = DEFAULT_CAPACITY;
		_size = 0;
		_elem = new T[_capacity];
		copy(v);
	}

	virtual ~Vector()
	{
		for(int i = 0; i < _size; i++)
		{
			release(_elem[i]);
		}
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
		/*
		if(_size - 1 < i)
		{
			throw std::out_of_range("Index Out of Range!");
		}
		*/
		return _elem[i];
	}

	virtual Vector<T> &operator = (const Vector<T> & v)		
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
				//ע��ʹ�������
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
				//ע��ʹ�������
				//temp[i] = elem[i];
				copyElement(temp, _elem, i);
			}
			delete[] _elem;
			_elem = temp;
		}
	}
	void copy(const Vector<T> & v)
	{
		_size = v._size;

		//����ǰ��������ʱ�����������ݣ���Ȼ�����Խ�������ġ�Խ�绹����ɺ������ظ�delete�����⡣
		if(_capacity != v._capacity)
		{
			_capacity = v._capacity;
			delete[] _elem;
			_elem = new T[_capacity];
		}
		for(int i = 0; i < v._size; i++)
		{
			//ע��ʹ�������
			copyElement(_elem, v._elem, i);
			//elem[i] = v.elem[i];
		}
	}
};

/*--------------------------------------------------------*/

template<typename T>
struct Queue
{
	int size;
	int capacity;
	int _front, _end;
	T *queue;

	Queue(int capacity_):
		capacity(capacity_), size(0), _front(0), _end(0)
	{
		queue = new T[capacity_];
	}
	~Queue()
	{
		delete[] queue;
	}

	bool empty() const { return size == 0; }

	void enqueue(const T &e)
	{
		queue[_end++] = e;
		size++;
	}

	T dequeue()
	{
		T temp = queue[_front];
		queue[_front++] = 0;
		size--;
		return temp;
	}

	T &front()
	{
		return queue[front];
	}
};

/*--------------------------------------------------------*/

//������Ҫ����n��Ԫ��ΪҶ�ڵ����ȫ�������Ĵ�С��ָ��ֵ��
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
struct RangeNode
{
	T _data;
	bool _isInternal;
	Vector<RangeNode<T>> *_ptr;
	RangeNode *_largestBelow;
	int _sizeBelow;

	RangeNode(T const &data_ = 0, bool isInternal_ = false, Vector<RangeNode<T>> *ptr_ = NULL)
	{
		_data = data_;
		_isInternal = isInternal_;
		_ptr = ptr_;
		_largestBelow = NULL;
		_sizeBelow = 0;
	}
};

struct Station
{
	int _x, _y;
	int _temp;
	Station(int x_ = 0, int y_ = 0, int temp_ = 0):
		_x(x_), _y(y_), _temp(temp_) {}
	Station(const Station &sta):
		_x(sta._x), _y(sta._y), _temp(sta._temp) {}
};

Vector<RangeNode<Station>> *createNewTree(Station *elem, int n)
{
	int e = calTreeSizeExponent(n);
	int size = facOfTwo(e) - 1;
	//��v�е�Ԫ�ع��췶Χ��������
	Vector<RangeNode<Station>> *searchV = new Vector<RangeNode<Station>>(size, size);
	int start = facOfTwo(e - 1) - 1;

	for(int i = 0; i < n; i++)
	{
		searchV->_elem[start + i]._data = elem[i];
		searchV->_elem[start + i]._sizeBelow = 1;
		searchV->_elem[start + i]._largestBelow = &searchV->_elem[start + i];
		searchV->_elem[start + i]._isInternal = true;
	}

	//����Χ��������ײ��ȱ�ڵ��isInternal��Ϊfalse��
	for(int i = start + n; i < size; i++)
	{
		searchV->_elem[i]._data = 0;
		searchV->_elem[i]._isInternal = false;
	}

	//�Է�Χ����������ײ����ϵĲ��ֽ��г�ʼ����
	for(int i = start - 1; -1 < i; i--)
	{
		//������Һ��Ӷ������ڲ���㣬��ǰ�ڵ�Ҳ�����ڲ���㡣
		if(!searchV->_elem[lChild(i)]._isInternal && !searchV->_elem[rChild(i)]._isInternal)
		{
			searchV->_elem[i]._data = 0;
			searchV->_elem[i]._isInternal = false;
			continue;
		}

		searchV->_elem[i]._isInternal = true;
		int lc = lChild(i);
		int rc = rChild(i);
		//ʹ��_largestBelow��Ϊ�˷������ϴ��ݵ�ǰ�ڵ��µ����ڵ�ֵ���Ա���ײ�����ڵ��_data��
		//����Һ������ڲ���㣬��ǰ�ڵ��_largestBelowһ�����Һ��ӵ�_largestBelow����Ϊ�ڵ��С����С�Ҵ�
		if(searchV->_elem[rc]._isInternal)
		{
			searchV->_elem[i]._largestBelow = searchV->_elem[rc]._largestBelow;
		}
		//����Һ������ⲿ�ڵ㣬ֻ���������ڲ���㣬�������ӵ�_largestBelow��Ϊ��ǰ�ڵ��_largestBelow��
		else
		{
			searchV->_elem[i]._largestBelow = searchV->_elem[lc]._largestBelow;
		}

		//��ǰ�ڵ��_dataһ�������ӵ�_largestBelow��
		//ͬʱ���½ڵ��_sizeBelow��
		searchV->_elem[i]._data = searchV->_elem[lc]._largestBelow->_data;
		searchV->_elem[i]._sizeBelow = searchV->_elem[lc]._sizeBelow + searchV->_elem[rc]._sizeBelow;
	}
	//���ˣ������Ĺ����Ѿ���ɡ���������Ҫ�����������
	return searchV;
}

Vector<RangeNode<Station>> *createSearchVector(Station *elem, int n)
{
	//��elem�е�Ԫ���ƶ���v�У�������	
	heapSort(elem, 0, n, X);
	//����������
	Vector<RangeNode<Station>> *mainTree = createNewTree(elem, n);

	//�����������
	int e = calTreeSizeExponent(n);
	int size = facOfTwo(e) - 1;
	int start = facOfTwo(e - 1) - 1;

	Station *newArr = new Station[size];

	for(int i = 0; i < size; i++)
	{
		if(!mainTree->_elem[i]._isInternal)
		{
			continue;
		}

		int sizeBelow = mainTree->_elem[i]._sizeBelow;

		if(sizeBelow == 1)
		{
			newArr[0] = mainTree->_elem[i]._data;
		}
		else
		{
			int lc = lChild(i);

			while(mainTree->_elem[lChild(lc)]._isInternal && lChild(lc) < size)
			{
				lc = lChild(lc);
			}
			/*
			while(mainTree->_elem[rc]._isInternal)
			{
				rc = rChild(i);
			}
			if(start + n < rc)
			{
				rc = start + n;
			}
			*/
			for(int j = 0; j < sizeBelow; j++)
			{
				newArr[j] = elem[lc - start + j];
			}
		}

		heapSort(newArr, 0, sizeBelow, Y);
		Vector<RangeNode<Station>> *associatedTree = createNewTree(newArr, sizeBelow);
		mainTree->_elem[i]._ptr = associatedTree;

		/*
		Queue<int> que(sizeBelow);
		que.enqueue(i);
		for(int j = 0; j < sizeBelow; j++)
		{
			int index = que.dequeue();
			newArr[j] = mainTree->_elem[index]._data;

			if(mainTree->_elem[lChild(i)]._isInternal)
			{
				que.enqueue(lChild(i));
			}
			if(mainTree->_elem[rChild(i)]._isInternal)
			{
				que.enqueue(rChild(i));
			}
		}
		*/

	}

	return mainTree;
}

int searchInMaintree(int e, int size, Vector<RangeNode<Station>> *tree)
{
	int p = 0;

	while(!isLeaf(size, p) && tree->_elem[p]._isInternal)
	{
		if(e <= tree->_elem[p]._data._x)
		{
			p = lChild(p);
		}
		else
		{
			p = rChild(p);
		}
	}

	if(!tree->_elem[p]._isInternal)
	{
		//���������ֵ���������ֵ��Ҫ���򷵻�-1��
		return -1;
	}
	else
	{
		return p;
	}
}

int searchInAssociatedTree(int e, int size, Vector<RangeNode<Station>> *tree)
{
	int p = 0;

	while(!isLeaf(size, p) && tree->_elem[p]._isInternal)
	{
		if(e <= tree->_elem[p]._data._y)
		{
			p = lChild(p);
		}
		else
		{
			p = rChild(p);
		}
	}

	if(!tree->_elem[p]._isInternal)
	{
		//���������ֵ���������ֵ��Ҫ���򷵻�-1��
		return -1;
	}
	else
	{
		return p;
	}
}

long long calAvr(int &x1, int &y1, int &x2, int &y2, int n, int size, Vector<RangeNode<Station>> *mainTree)
{
	long long totalTemp = 0;
	long long totalCount = 0;

	int left = searchInMaintree(x1, size, mainTree);

	int right = searchInMaintree(x2, size, mainTree);
	if(right == -1)
	{
		right = (size - 1) / 2 + n - 1;
	}

	int lParent = parent(left);
	int rParent = parent(right);

	while(lParent != rParent)
	{
		lParent = parent(lParent);
		rParent = parent(rParent);
	}

	int divide = lParent;
		
	int lDivide = lChild(divide);
	int rDivide = rChild(divide);

	Vector<Vector<RangeNode<Station>> *> subTree;

	//�����Ĺ���������subTree����
	if(isLeaf(size, lDivide))
	{
		subTree.push_back(mainTree->_elem[lDivide]._ptr);
	}
	else
	{		
		while(!isLeaf(size, lDivide))
		{
			if(mainTree->_elem[left]._data._x <= mainTree->_elem[lDivide]._data._x)
			{
				subTree.push_back(mainTree->_elem[rChild(lDivide)]._ptr);

				if(isLeaf(size, lChild(lDivide)))
				{
					subTree.push_back(mainTree->_elem[lChild(lDivide)]._ptr);
				}
				lDivide = lChild(lDivide);
			}
			else
			{
				if(isLeaf(size, rChild(lDivide)))
				{
					subTree.push_back(mainTree->_elem[rChild(lDivide)]._ptr);
				}
				lDivide = rChild(lDivide);
			}
		}
	}
	
	//���Ҳ�Ĺ���������subTree������
	if(isLeaf(size, rDivide))
	{
		subTree.push_back(mainTree->_elem[rDivide]._ptr);
	}
	else
	{
		while(!isLeaf(size, rDivide))
		{
			if(mainTree->_elem[rDivide]._data._x < mainTree->_elem[right]._data._x)
			{
				subTree.push_back(mainTree->_elem[lChild(rDivide)]._ptr);
				if(isLeaf(size, rChild(rDivide)) && (mainTree->_elem[rChild(rDivide)]._data._x <= x2))
				{
					subTree.push_back(mainTree->_elem[rChild(rDivide)]._ptr);
				}
				rDivide = rChild(rDivide);
			}
			else
			{
				if(isLeaf(size, rChild(rDivide)))
				{
					subTree.push_back(mainTree->_elem[lChild(rDivide)]._ptr);
				}
				rDivide = lChild(rDivide);
			}
		}
	}

	//����subTree�����������еĹ���������ִ����������ͳ�����¶Ⱥ��ܼ��վ������
	for(int i = 0; i < subTree._size; i++)
	{
		int down = searchInAssociatedTree(y1, subTree[i]->_elem[0]._sizeBelow, subTree[i]);

		int up = searchInAssociatedTree(y2, subTree[i]->_elem[0]._sizeBelow, subTree[i]);
		if(up == -1)
		{
			up = (size - 1) / 2 + n - 1;
		}

		for(int j = down; j < up; j++)
		{
			totalTemp += subTree._elem[i]->_elem[j]._data._temp;
			totalCount++;
		}
		if(subTree._elem[i]->_elem[up]._data._y <= y2)
		{
			totalTemp += subTree._elem[i]->_elem[up]._data._temp;
			totalCount++;
		}
		//�ǵ�һ��Ҫ��subTreeָ��NULL����Ȼ֮�������������������delete���ġ�
		subTree[i] = NULL;
	}

	return totalCount == 0 ? 0 : totalTemp / totalCount;
}

/*--------------------------------------------------------*/

int main()
{
	
	GetNumOfStation();

	int n = GetNumOfStation();

	int e = calTreeSizeExponent(n);
	int size = facOfTwo(e) - 1;

	Station *sta = new Station[n];
	for(int i = 0; i < n; i++)
	{
		GetStationInfo(i, &sta[i]._x, &sta[i]._y, &sta[i]._temp);
	}
	
	Vector<RangeNode<Station>> *tree = createSearchVector(sta, n);
	delete[] sta;

	int x1, x2, y1, y2;


	while(GetQuery(&x1, &y1, &x2, &y2))
	{
		long long temp = calAvr(x1, y1, x2, y2, n, size, tree);
		Response(temp);
	}
	
	for(int i = 0; i < tree->_size; i++)
	{
		delete tree->_elem[i]._ptr;
	}

	delete tree;

	/*
	Station rn[] = { {4, 0, 4}, {3, 1, 3}, {2, 2, 2}, {1, 3, 1}, {0, 4, 0} };

	Vector<RangeNode<Station>> *v = createSearchVector(rn, 5);

	int e = calTreeSizeExponent(5);
	int size = facOfTwo(e) - 1;

	int i;
	int x1 = 0;
	int y1 = 0;
	int x2 = 4;
	int y2 = 4;
	i = calAvr(x1, y1, x2, y2, 5, 15, v);
	*/
}