#pragma once
//2020.9.22 对BinTree的析构及对应的删除函数进行了调整。

template<typename T>
void release(T t)
{
	return;
}

template<typename T>
void release(T *t)
{
	delete t;
}

/*------------------------------------------------------------------------------------------*/

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

/*------------------------------------------------------------------------------------------*/

#define BinNodePosi(T) BinNode<T>*

template<typename T>
struct BinNode
{
	T _data;

	BinNodePosi(T) _parent;
	BinNodePosi(T) _lc;
	BinNodePosi(T) _rc;

	BinNodePosi(T) _from = NULL;

	int _height;
	int npl;
	//RBColor color;

	BinNode(T e, BinNodePosi(T) parent = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL):
		_data(e), _parent(parent), _lc(lc), _rc(rc)
	{
		_height = -1;
		npl = 1;
	}

	int size();
	BinNodePosi(T) insertAsLc(T const &e);
	BinNodePosi(T) insertAsRc(T const &e);
	BinNodePosi(T) succ();
	BinNodePosi(T) pred();
};

template<typename T>
int BinNode<T>::size()
{
	int s = 1;
	if(_lc)
	{
		s += _lc->size();
	}
	if(_rc)
	{
		s += _rc->size();
	}
	return s;
}

template<typename T>
BinNodePosi(T) BinNode<T>::insertAsLc(T const &e)
{
	return _lc = new BinNode(e, this);
}

template<typename T>
BinNodePosi(T) BinNode<T>::insertAsRc(T const &e)
{
	return _rc = new BinNode(e, this);
}

template<typename T>
struct Delete
{
	void operator ()(T *&x)
	{
		//增加了x为空的判定。
		if(x)
		{
			delete x;
			x = NULL;
		}		
	}
};

template<typename T>
BinNode<T> *BinNode<T>::succ()
{
	BinNodePosi(T) s = this;
	if(s->_rc)
	{
		s = s->_rc;
		while(s->_lc)
		{
			s = s->_lc;
		}
		return s;
	}

	else
	{
		while(s->_parent && (s != s->_parent->_lc))
		{
			s = s->_parent;
		}

		s = s->_parent;
		return s;
	}
}

template<typename T>
BinNode<T> *BinNode<T>::pred()
{
	BinNodePosi(T) s = this;

	if(s->_lc)
	{
		s = s->_lc;
		while(s->_rc)
		{
			s = s->_rc;
		}
		return s;
	}

	else
	{
		while(s->_parent && s == s->_parent->_lc)
		{
			s = s->_parent;
		}
		s = s->_parent;
		return s;
	}
}

/*------------------------------------------------------------------------------------------*/
//用于树节点处理的一些全局函数。

template<typename T>
bool isLChild(BinNodePosi(T) x)
{
	return x->_parent->_lc == x;
}

template<typename T>
bool isRChild(BinNodePosi(T) x)
{
	return x->_parent->_rc == x;
}

template<typename T>
BinNodePosi(T) &fromParent(BinNodePosi(T) x)
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

template<typename T>
inline void attachAsLChild(BinNodePosi(T) p, BinNodePosi(T) lc)
{
	p->_lc = lc;
	if(lc)
	{
		lc->_parent = p;
	}
}

template<typename T>
inline void attachAsRChild(BinNodePosi(T) p, BinNodePosi(T) rc)
{
	p->_rc = rc;
	if(rc)
	{
		rc->_parent = p;
	}
}

template<typename T>
int stature(BinNodePosi(T) x)
{
	return x ? x->_height : -1;
}

/*------------------------------------------------------------------------------------------*/

template<typename T>
struct BinTree
{
	BinNodePosi(T) _root;
	int _size;
	BinTree():
		_root(NULL), _size(0)
	{}

	~BinTree()
	{
		travPre_I1(_root, Delete<BinNode<T>>());
	}

	virtual int updateHeight(BinNodePosi(T) x);
	virtual void updateHeightAbove(BinNodePosi(T) x);

	int size() const { return _size; }
	int empty() const { return !_root; }
	BinNodePosi(T) root() const { return _root; }

	//节点插入。
	BinNodePosi(T) insertAsLc(BinNodePosi(T) x, T const &e);
	BinNodePosi(T) insertAsRc(BinNodePosi(T) x, T const &e);
	//子树接入。
	BinNodePosi(T) attchAsLc(BinNodePosi(T) x, BinTree<T> *&s);
	BinNodePosi(T) attchAsRc(BinNodePosi(T) x, BinTree<T> *&s);
	//子树删除。
	int remove(BinNodePosi(T) x);
	//子树分离。
	BinTree<T> *secede(BinNodePosi(T) x);

	//先序遍历，递归版本一。
	template<typename VST>
	void travPre_I1(BinNodePosi(T) x, VST visist);

	//先序遍历，递归版本二。
	template<typename VST>
	void travPre_I2(BinNodePosi(T) x, VST visist);
};

template<typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x)
{
	return x->_height = 1 + (stature(x->_lc) > stature(x->_rc)) ? stature(x->_lc) : stature(x->_rc);
}

template<typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x)
{
	while(x)
	{
		updateHeight(x);
		x = x->_parent;
	}
}

template<typename T>
BinNodePosi(T) BinTree<T>::insertAsLc(BinNodePosi(T) x, T const &e)
{
	_size++;
	x->insertAsLc(e);
	updateHeightAbove(x);
	return x->_lc;
}

template<typename T>
BinNodePosi(T) BinTree<T>::insertAsRc(BinNodePosi(T) x, T const &e)
{
	_size++;
	x->insertAsRc(e);
	updateHeightAbove(x);
	return x->_rc;
}

template<typename T>
BinNodePosi(T) BinTree<T>::attchAsLc(BinNodePosi(T) x, BinTree<T> *&s)
{
	if(s)
	{
		x->_lc = s->_root;
		s->_root->_parent = x;
		_size += s->_size;
		updateHeightAbove(x);

		delete s;
		s = NULL;
	}
	return x;
}

template<typename T>
BinNodePosi(T) BinTree<T>::attchAsRc(BinNodePosi(T) x, BinTree<T> *&s)
{
	if(s)
	{
		x->_rc = s->_root;
		s->_root->_parent = x;
		_size += s->_size;
		updateHeightAbove(x);

		delete s;
		s = NULL;
	}
	return x;
}

template<typename T>
int BinTree<T>::remove(BinNodePosi(T) x)
{
	BinNodePosi(T) parent = x->_parent;
	if(parent)
	{
		if(parent->_lc == x)
		{
			parent->_lc == NULL;
		}
		else
		{
			parent->_rc == NULL;
		}
	}
	int n = x->size();
	_size -= n;
	updateHeightAbove(x);

	travPre_I1(x, Delete<BinNode<T>>());
	return n;
}

template<typename T>
BinTree<T> *BinTree<T>::secede(BinNodePosi(T) x)
{
	BinNodePosi(T) parent = x->_parent;
	if(parent)
	{
		if(parent->_lc == x)
		{
			parent->_lc == NULL;
		}
		else
		{
			parent->_rc == NULL;
		}
	}
	int n = x->size();
	_size -= n;
	updateHeightAbove(x);

	BinTree<T> *S = BinTree<T>(x, n);
	x->_parent = NULL;
}

template<typename T>
template<typename VST>
void BinTree<T>::travPre_I1(BinNodePosi(T) x, VST visit)
{
	Stack<BinNode<T> *> stack;
	stack.push(x);
	while(!stack.empty())
	{
		x = stack.top();
		stack.pop();

		//增加了对x的判定。
		if(x && x->_rc)
		{
			stack.push(x->_rc);
		}
		if(x && x->_lc)
		{
			stack.push(x->_lc);
		}

		visit(x);
	}
}

template<typename T>
template<typename VST>
void BinTree<T>::travPre_I2(BinNodePosi(T) x, VST visit)
{
	Stack<BinNodePosi(T)> stack;

	while(true)
	{
		visitAlongVine(x, visit, stack);
		if(!stack.empty())
		{
			x = stack.top();
			stack.pop();
		}
		else
		{
			break;
		}
	}
}