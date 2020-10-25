// Broadcast.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;


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
	//int npl;
	//RBColor color;

	BinNode(T e, BinNodePosi(T) parent = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL):
		_data(e), _parent(parent), _lc(lc), _rc(rc)
	{
		_height = -1;
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
		delete x;
		x = NULL;
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

/*------------------------------------------------------------------------------------------*/

template<typename T>
struct BST: public BinTree<T>
{
	public:
	BST();

	virtual BinNodePosi(T) &search(const T &);
	virtual BinNodePosi(T) insert(const T &);
	virtual bool remove(const T &);

	protected:
	//_hot用来表示命中节点的父亲。
	BinNodePosi(T) _hot;
	//注意，这里需要在声明时就把形参也写进去。似乎是因为使用了#define进行全局声明的原因。
	virtual BinNodePosi(T) connect34(BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c, BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3);
	virtual BinNodePosi(T) rotateAt(BinNodePosi(T) v);


	static BinNodePosi(T) &searchIn(BinNodePosi(T) &x, const T &e, BinNodePosi(T) &hot);
	static BinNodePosi(T) &removeAt(BinNodePosi(T) &x, BinNodePosi(T) &hot);
};

template<typename T>
BST<T>::BST():
	_hot(NULL)
{}

template<typename T>
BinNodePosi(T) &BST<T>::search(const T &e)
{
	return searchIn(this->_root, e, _hot = NULL);
}

template<typename T>
BinNodePosi(T) &BST<T>::searchIn(BinNodePosi(T) &x, const T &e, BinNodePosi(T) &hot)
{
	if(!x || x->_data == e)
	{
		return x;
	}
	hot = x;

	while(true)
	{
		BinNodePosi(T) &c = (e < hot->_data) ? hot->_lc : hot->_rc;
		if(!c || c->_data == e)
		{
			return c;
		}
		hot = c;
	}
}

template<typename T>
BinNodePosi(T) BST<T>::insert(const T &e)
{
	BinNodePosi(T) &p = search(e);
	if(!p)
	{
		p = new BinNode<T>(e, _hot);

		this->_size++;
		this->updateHeightAbove(p);
	}
	return p;
}

template<typename T>
BinNodePosi(T) &BST<T>::removeAt(BinNodePosi(T) &x, BinNodePosi(T) &hot)
{
	BinNodePosi(T) succ = NULL;

	if(!x->_lc)
	{
		succ = x->_rc;
	}
	else if(!x->_rc)
	{
		succ = x->_lc;
	}
	else
	{
		succ = x->succ();

		T temp = x->_data;
		x->_data = succ->_data;
		succ->_data = temp;

		x = succ;
		succ = succ->_rc;
		hot = x->_parent;
	}

	if(hot->_lc == x)
	{
		hot->_lc = succ;
	}
	else
	{
		hot->_rc = succ;
	}

	if(succ)
	{
		succ->_parent = hot;
	}

	release(x->_data);
	release(x);

	x = hot;
	return x;
}

template<typename T>
bool BST<T>::remove(const T &e)
{
	BinNodePosi(T) x = search(e);

	if(!x)
	{
		return false;
	}
	removeAt(x, _hot);
	this->_size--;
	this->updateHeightAbove(x);
	return true;
}

template<typename T>
BinNodePosi(T) BST<T>::connect34(BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c, BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3)
{
	a->_lc = T0;
	if(T0)
	{
		T0->_parent = a;
	}

	a->_rc = T1;
	if(T1)
	{
		T1->_parent = a;
	}

	c->_lc = T2;
	if(T2)
	{
		T2->_parent = c;
	}

	c->_rc = T3;
	if(T3)
	{
		T3->_parent = c;
	}

	b->_lc = a;
	b->_rc = c;
	a->_parent = b;
	c->_parent = b;
	this->updateHeight(a);
	this->updateHeight(b);
	this->updateHeight(c);
	return b;
}
template<typename T>
BinNodePosi(T) BST<T>::rotateAt(BinNodePosi(T) v)
{
	BinNodePosi(T) p = v->_parent;
	BinNodePosi(T) g = p->_parent;

	if(isLChild(p))
	{
		if(isLChild(v))
		{
			//zigzig
			p->_parent = g->_parent;
			return connect34(v, p, g, v->_lc, v->_rc, p->_rc, g->_rc);
		}
		else
		{
			//zagzig
			v->_parent = g->_parent;
			return connect34(p, v, g, p->_lc, v->_lc, v->_rc, g->_rc);
		}
	}
	else
	{
		if(isLChild(v))
		{
			//zigzag
			v->_parent = g->_parent;
			return connect34(g, v, p, g->_lc, v->_lc, v->_rc, p->_rc);
		}
		else
		{
			//zagzag
			p->_parent = g->_parent;
			return connect34(g, p, v, g->_lc, p->_lc, v->_lc, v->_rc);
		}
	}
}

/*------------------------------------------------------------------------------------------*/

template<typename T>
bool Balanced(const BinNode<T> &x)
{
	return stature(x._lc) == stature(x._rc);
}

template<typename T>
int BalFac(const BinNode<T> &x)
{
	return stature(x._lc) - stature(x._rc);
}

template<typename T>
bool AvlBalanced(BinNode<T> &x)
{
	//平衡因子最大为1，最小为-1。
	return (-2 < BalFac(x)) && (BalFac(x) < 2);
}

template<typename T>
BinNodePosi(T) &tallerChild(BinNodePosi(T) &g)
{
	if(stature(g->_rc) < stature(g->_lc))
	{
		return g->_lc;
	}
	else if(stature(g->_lc) < stature(g->_rc))
	{
		return g->_rc;
	}
	else
	{
		//下面这个情况是在删除的单旋中可能出现的情况。
		//即，当子树等高时，选择与父节点同侧的子树。
		if(g == g->_parent->_lc)
		{
			return g->_lc;
		}
		else
		{
			return g->_rc;
		}
	}
}

template<typename T>
BinNodePosi(T) &fromParentTo(BinNodePosi(T) &x)
{
	if(x->_parent == NULL)
	{
		return x;
	}
	else
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
}

/*------------------------------------------------------------------------------------------*/

template<typename T>
struct AVL: public BST<T>
{
	BinNodePosi(T) insert(const T &);
	bool remove(const T &);

	bool radioCheck(const T &from, const T &to);
};

template<typename T>
BinNodePosi(T) AVL<T>::insert(const T &e)
{
	//这里还是手动把insert的代码搬过来。
	//因为insert会自动执行updateHeightAbove()，但这里并不希望在检查失衡前就对height进行调整。
	BinNodePosi(T) &p = this->search(e);
	if(!p)
	{
		p = new BinNode<T>(e, this->_hot);

		/*
		if(this->_size == 0)
		{
		}
		else
		{
			(e < this->_hot->_data) ? this->_hot->_lc = p : this->_hot->_rc = p;
		}
		*/

		this->_size++;
	}
	this->updateHeight(p);


	//从父亲开始检查失衡（按道理是从祖父开始才会失衡的，但由于父亲可能会有高度变化，因此循环从父亲开始）。
	for(BinNodePosi(T) g = this->_hot; g; g = g->_parent)
	{
		if(!AvlBalanced(*g))
		{
			BinNodePosi(T) x;
			//因为在rotateAt之后，g的parent会发生改变，因此在此之前先对其进行备份，便于后续操作。
			//记得这里要用引用。
			BinNodePosi(T) &fromParent = fromParentTo(g);

			x = this->rotateAt(tallerChild(tallerChild(g)));

			if(g == this->_root)
			{
				this->_root = x;
			}
			else
			{
				fromParent = x;
			}
			break;
		}
		else
		{
			this->updateHeight(g);
		}
	}	
	return p;
}

template<typename T>
bool AVL<T>::remove(const T &e)
{
	//由于remove本身就对祖先进行逐次的高度更新，因此这里可以直接调用remove()。
	bool x = BST<T>::remove(e);
	if(!x)
	{
		return x;
	}

	//从父亲开始，考察是否失衡。
	for(BinNodePosi(T) g = this->_hot; g; g = g->_parent)
	{
		if(!AvlBalanced(*g))
		{
			BinNodePosi(T) x;
			BinNodePosi(T) &fromParent = fromParentTo(g);

			//这里由于每次remove都可能导致高度变化，因此必须依次遍历所有祖先。
			x = this->rotateAt(tallerChild(tallerChild(g)));

			if(g == this->_root)
			{
				this->_root = x;
			}
			else
			{
				fromParent = x;
			}
		}
		this->updateHeight(g);
	}

	return x;
}

template<typename T>
bool AVL<T>::radioCheck(const T &from, const T &to)
{
	BinNodePosi(T) fromNode = this->search(from);
	BinNodePosi(T) toNode = this->search(to);
	if(fromNode->_from && toNode->_from && fromNode->_from == toNode->_from)
	{
		return false;
	}
	else
	{
		toNode->_from = fromNode;
		return true;
	}
}

int main()
{
	
	int n, m;
	scanf("%d%d", &n, &m);
	int *from = new int[m];
	int *to = new int[m];

	for(int i = 0; i < m; i++)
	{
		scanf("%d%d", &from[i], &to[i]);
	}

	AVL<int> avl;

	for(int i = 0; i < n; i++)
	{
		avl.insert(i + 1);
	}

	for(int i = 0; i < m; i++)
	{
		if(!avl.radioCheck(from[i], to[i]))
		{
			printf("-1");
			return 0;
		}
	}

	printf("1");
}