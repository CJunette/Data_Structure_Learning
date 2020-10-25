#pragma once
#include <algorithm>
#include <stack>
#include <queue>
using namespace std;

enum RBColor
{
	RB_RED, 
	RB_BLACK
};

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
#define BinNodePosi(T) BinNode<T>*

template<typename T>
struct BinNode
{
	T _data;

	BinNodePosi(T) _parent;
	BinNodePosi(T) _lc;
	BinNodePosi(T) _rc;

	int _height;
	//int npl;
	RBColor _color;

	BinNode(T e, BinNodePosi(T) parent = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL):
		_data(e), _parent(parent), _lc(lc), _rc(rc)
	{
		_color = RB_RED;
		_height = -1;
	}

	int size();
	BinNodePosi(T) insertAsLc(T const &e);
	BinNodePosi(T) insertAsRc(T const &e);
	BinNodePosi(T) succ();
	BinNodePosi(T) pred();

	/*
	template<typename VST> void travLevel(VST &);
	template<typename VST> void travPre(VST &);
	template<typename VST> void travIn(VST &);
	template<typename VST> void travPost(VST &);
	*/
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
	if(!x->_parent)
	{
		return x;
	}
	if(isLChild(x))
	{
		return x->_parent->_lc;
	}
	else if(isRChild(x))
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

template<typename T>
BinNodePosi(T) uncle(BinNodePosi(T) x)
{
	BinNodePosi(T) p = x->_parent;
	BinNodePosi(T) g = p->_parent;
	BinNodePosi(T) u = isLChild(p) ? g->_rc : g->_lc;
	return u;
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

	virtual ~BinTree()
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
	template<typename VST>
	void visitAlongVine(BinNodePosi(T) x, VST visit, stack<BinNodePosi(T)> &stack);

	//自己尝试写的中序遍历。
	template<typename VST>
	void travMid_Cunstomed_I1(BinNodePosi(T) x, VST visist);
	template<typename VST>
	void visitAlongVineComstumedForMid_I1(BinNodePosi(T) x, VST visit, stack<BinNodePosi(T)> &stack);

	//课上给出的中序遍历。
	void goAlongLeftBranch(BinNodePosi(T) x, stack<BinNodePosi(T)> &stack);
	template<typename VST>
	void travMid_I1(BinNodePosi(T) x, VST visit);

	//自己尝试写后序遍历。
	void goAlongLeftBranch_ForPost(BinNodePosi(T) x, stack<BinNodePosi(T)> &stack);
	template<typename VST>
	void travPost_Customed_I1(BinNodePosi(T) x, VST visit);
	template<typename VST>
	void travPost_Customed_I2(BinNodePosi(T) x, VST visit);

	//课上给出的后序遍历。
	void gotoLeftMostLeaf(stack<BinNodePosi(T)> &stack);
	template<typename VST>
	void travPost_I1(BinNodePosi(T) x, VST visit);

	//自己尝试写层次遍历。
	template<typename VST>
	void travLevel_Custom(VST visit);

	//课上给出的层次遍历。
	template<typename VST>
	void travLevel(VST visit);

	void show() const;
};

template<typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x)
{
	return x->_height = 1 + max(stature(x->_lc), stature(x->_rc));
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
	stack<BinNode<T> *> stack;
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
	stack<BinNodePosi(T)> stack;

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

template<typename T>
template<typename VST>
void BinTree<T>::visitAlongVine(BinNodePosi(T) x, VST visit, stack<BinNodePosi(T)> &stack)
{
	while(x)
	{
		BinNodePosi(T) lc = x->_lc;
		stack.push(x->_rc);
		visit(x);
		x = lc;
	}
}

template<typename T>
template<typename VST>
void BinTree<T>::travMid_Cunstomed_I1(BinNodePosi(T) x, VST visit)
{
	stack<BinNode<T> *> stack;
	while(true)
	{
		visitAlongVineComstumedForMid_I1(x, visit, stack);
		if(stack.empty())
		{
			break;
		}

		x = stack.top();
		stack.pop();

		visit(x);

		x = stack.top();
		stack.pop();
	}
}

template<typename T>
template<typename VST>
void BinTree<T>::visitAlongVineComstumedForMid_I1(BinNodePosi(T) x, VST visit, stack<BinNodePosi(T)> &stack)
{
	if(x)
	{
		while(x->_lc)
		{
			stack.push(x->_rc);
			stack.push(x);

			x = x->_lc;
		}

		stack.push(x->_rc);
		stack.push(x);
	}
}

template<typename T>
void BinTree<T>::goAlongLeftBranch(BinNodePosi(T) x, stack<BinNodePosi(T)> &stack)
{
	while(x)
	{
		stack.push(x);
		x = x->_lc;
	}
}

template<typename T>
template<typename VST>
void BinTree<T>::travMid_I1(BinNodePosi(T) x, VST visit)
{
	stack<BinNode<T> *> stack;
	while(true)
	{
		goAlongLeftBranch(x, stack);
		if(stack.empty())
		{
			break;
		}
		x = stack.top();
		visit(x);
		stack.pop();
		x = x->_rc;
	}
}

template<typename T>
void BinTree<T>::goAlongLeftBranch_ForPost(BinNodePosi(T) x, stack<BinNodePosi(T)> &stack)
{
	while(x)
	{
		stack.push(x);
		if(x->_lc)
		{
			x = x->_lc;
			continue;
		}
		else if(x->_rc)
		{
			x = x->_rc;
			continue;
		}
		else
		{
			break;
		}
	}
}

template<typename T>
template<typename VST>
void BinTree<T>::travPost_Customed_I1(BinNodePosi(T) x, VST visit)
{
	stack<BinNode<T> *> stack;
	if(!x)
	{
		return;
	}
	while(true)
	{
		goAlongLeftBranch_ForPost(x, stack);

		x = stack.top();
		visit(x);
		stack.pop();

		if(stack.empty())
		{
			break;
		}

		if(x == x->_parent->_rc)
		{
			x = NULL;
		}
		else
		{
			x = x->_parent->_rc;
		}
	}
}

template<typename T>
template<typename VST>
void BinTree<T>::travPost_Customed_I2(BinNodePosi(T) x, VST visit)
{
	stack<BinNode<T> *> stack;

	bool backtracked = false;
	while(true)
	{
		while(x && x->_lc && !backtracked)
		{
			stack.push(x);
			x = x->_lc;
		}
		if(x && x->_rc && !backtracked)
		{
			stack.push(x);
			x = x->_rc;
			continue;
		}

		if(x)
		{
			visit(x);
			if(stack.empty())
			{
				break;
			}
		}
		else
		{
			x = stack.top();
			stack.pop();
			backtracked = true;
			continue;
		}

		if(x == stack.top()->_lc)
		{
			x = stack.top()->_rc;
			backtracked = false;
		}
		else
		{
			x = stack.top();
			stack.pop();
			backtracked = true;
		}

	};
}

template<typename T>
void BinTree<T>::gotoLeftMostLeaf(stack<BinNodePosi(T)> &stack)
{
	BinNodePosi(T) x;
	while(x = stack.top())
	{
		if(x->_lc)
		{
			if(x->_rc)
			{
				stack.push(x->_rc);
			}
			stack.push(x->_lc);
		}
		else
		{
			stack.push(x->_rc);
		}
	}
	stack.pop();
}

template<typename T>
template<typename VST>
void BinTree<T>::travPost_I1(BinNodePosi(T) x, VST visit)
{
	stack<BinNodePosi(T)> stack;
	if(x)
	{
		stack.push(x);
	}

	while(!stack.empty())
	{
		if(stack.top() != x->_parent)
		{
			gotoLeftMostLeaf(stack);
		}
		x = stack.top();
		stack.pop();
		visit(x);
	}
}

template<typename T>
template<typename VST>
void BinTree<T>::travLevel_Custom(VST visit)
{
	deque<BinNodePosi(T)> queue;
	BinNodePosi(T) x;
	queue.push_back(_root);
	while(!queue.empty())
	{
		x = queue.front();
		queue.pop_front();
		if(x)
		{
			visit(x);
			queue.push_back(x->_lc);
			queue.push_back(x->_rc);
		}
	}
}

template<typename T>
template<typename VST>
void BinTree<T>::travLevel(VST visit)
{
	deque<BinNodePosi(T)> queue;
	BinNodePosi(T) x;
	queue.push_back(_root);
	while(!queue.empty())
	{
		x = queue.front();
		queue.pop_front();
		visit(x);

		if(x->_lc)
		{
			queue.push_back(x->_lc);

		}
		if(x->_rc)
		{
			queue.push_back(x->_rc);
		}
	}
}


template<typename T>
void BinTree<T>::show() const
{
	queue<BinNodePosi(T)> q;
	q.push(_root);
	int count = 1;
	
	while(!q.empty())
	{
		int newcount = 0;

		for(int i = 0; i < count; i++)
		{			
			BinNodePosi(T) v = q.front();
			q.pop();
			if(v)
			{
				cout << v->_data << " ";
				switch(v->_color)
				{
					case RB_BLACK:
						cout << "black" << "\t";
						break;
					case RB_RED:
						cout << "red  " << "\t";
						break;
				}
				q.push(v->_lc);
				newcount++;
				q.push(v->_rc);
				newcount++;
			}
			else
			{
				cout << "NULL\t";
			}			
		}
		cout << endl;
		count = newcount;
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


	BinNodePosi(T) &searchIn(BinNodePosi(T) &x, const T &e, BinNodePosi(T) &hot);
	BinNodePosi(T) &removeAt(BinNodePosi(T) &x, BinNodePosi(T) &hot);
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

	//对x指向根节点的情况进行了补充。
	//必须用一个额外的指针指向x。不然在下面重新定向时，由于x就是hot->_lc或hot->_rc，因此修改会导致x失去原先指向的对象，从而无法实现释放。
	BinNodePosi(T) w = x;
	if(hot)
	{
		if(hot->_lc == x)
		{
			hot->_lc = succ;
		}
		else
		{
			hot->_rc = succ;
		}
	}
	else
	{
		this->_root = succ;
	}

	if(succ)
	{
		succ->_parent = hot;
	}

	release(w->_data);
	release(w);

	x = succ;
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
	this->updateHeight(c);
	this->updateHeight(b);
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
