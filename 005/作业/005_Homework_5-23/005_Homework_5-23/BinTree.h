#pragma once
#include <algorithm>
#include <stack>
#include <queue>
using namespace std;

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
	//RBColor color;

	BinNode(T e, BinNodePosi(T) parent = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL):
		_data(e), _parent(parent), _lc(lc), _rc(rc)
	{}

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


	private:

	int stature(BinNodePosi(T) x)
	{
		//对于NULL，需要返回高度为-1。
		return x ? x->_height : -1;
	}
	virtual int updateHeight(BinNodePosi(T) x);
	void updateHeightAbove(BinNodePosi(T) x);

	public:

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
};

template<typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x)
{
	return 1 + max(stature(x->_lc), stature(x->_rc));
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
		//这里交换了一下运行的顺序，先执行visit()，在取获取左子节点和右子节点。
		visit(x);
		BinNodePosi(T) lc = x->_lc;
		stack.push(x->_rc);
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