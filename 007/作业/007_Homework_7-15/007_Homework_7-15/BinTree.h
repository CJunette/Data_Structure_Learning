#pragma once
#include <algorithm>
#include <stack>
#include <queue>
using namespace std;

#define BinNodePosi(T) BinNode<T>*

/*
template<typename T>
struct Cleaner
{
	static void clean(T t)
	{
		return;
	}

	static void clean(T *t)
	{
		if(t)
		{
			delete t;
		}
	}
};
*/

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
	{
		_height = 0;
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
	//若当前节点存在右子节点，则右子节点的左侧链最深处就是后继节点。
	if(s->_rc)
	{
		s = s->_rc;
		while(s->_lc)
		{
			s = s->_lc;
		}
		return s;
	}
	//否则，如果当前节点是父节点的左子节点，则返回父节点。如果当前节点不是父节点的左子节点，则一直向上，直到找到是父节点的左子节点的祖先为止。
	else
	{
		while(s->_parent && (s != s->_parent->_lc))
		{
			s = s->_parent;
		}
		//如果遍历后发现不再有父节点，说明已经抵达根位置，此时目标节点为二叉树的最后一个元素，其的后继结点为空。
		//后来我发现，这样的情况也可以通过s->_parent输出，因为s->_parent此时为NULL。
		/*
		if(!s->_parent)
		{
			return NULL;
		}
		*/
		s = s->_parent;
		return s;
	}
}

template<typename T>
BinNode<T> *BinNode<T>::pred()
{
	BinNodePosi(T) s = this;

	//当当前对象存在左子节点时，前驱必然在左子节点中。
	//且前驱必然在左子节点的最深右子节点中。若不存在右子节点，则前驱就是左子节点。
	if(s->_lc)
	{
		s = s->_lc;
		while(s->_rc)
		{
			s = s->_rc;
		}
		return s;
	}
	//否则，向上查找直至找到某个祖先是其父节点的右子节点或到达根节点。
	//若查找到某个祖先是该祖先的父节点的右子节点，则祖先的父节点就是我们要查找的前驱节点。
	//若查找到根仍未找到，则说明当前节点就是第一个节点，不存在前驱（或前驱为空），因此返回NULL。
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

template<typename T>
bool isLChild(BinNodePosi(T) x)
{
	return x->_parent && x->_parent->_lc == x;
}

template<typename T>
bool isRChild(BinNodePosi(T) x)
{
	return x->_parent && x->_parent->_rc == x;
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


	protected:

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
		x->_height = updateHeight(x);
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

		//这里课件上用的是visit(x->data)，即让visit()访问x的具体数据。
		//我为了保证traverse有更好的适用性，所以换成了visit(x)，即访问指针。		

		//注意，这里的visit()的执行顺序和课上讲的不一样。
		//之所以放到了压栈之后，是因为我在用travPre_I1()调用Delete类执行删除操作的时候发现，如果先执行delete就会导致x的_rc和_lc都为空，进而导致stack为空，后续元素无法删除。
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
		//对于stack而言，如果x->_rc为NULL，也一样会被压入栈中，只是下次执行时会直接在while处跳过。
		stack.push(x->_rc);
		visit(x);
		x = lc;
	}
}

template<typename T>
template<typename VST>
void BinTree<T>::travMid_Cunstomed_I1(BinNodePosi(T) x, VST visit)
{
	//如果按照先序排列版本一的设计思路，在没有对节点引入确认自身左侧节点是否已经完成遍历的属性的情况下，是会导致压入栈操作不断重复压入自身、执行左链的操作的。
	/*
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

		if(x)
		{
			stack.push(x);
		}
		else
		{
			visit(x);
		}
	}
	*/

	stack<BinNode<T> *> stack;
	while(true)
	{
		//一直想左下方延伸直到左下方没有子节点为止。
		visitAlongVineComstumedForMid_I1(x, visit, stack);
		if(stack.empty())
		{
			break;
		}
		//此时弹出的节点是当前节点。
		x = stack.top();
		stack.pop();
		//执行操作。
		visit(x);
		//此时弹出的节点是当前节点的右子节点。
		x = stack.top();
		stack.pop();
	}
}

template<typename T>
template<typename VST>
void BinTree<T>::visitAlongVineComstumedForMid_I1(BinNodePosi(T) x, VST visit, stack<BinNodePosi(T)> &stack)
{
	//因为可能传入NULL指针，所以要进行一次非空判断。
	if(x)
	{
		//一直让x深入到不存在左子节点的那个节点。
		while(x->_lc)
		{
			//对于stack而言，如果x->_rc为NULL，也一样会被压入栈中，只是下次执行时会直接在while处跳过。		
			stack.push(x->_rc);
			stack.push(x);
			//stack.push(x);
			x = x->_lc;
		}
		//将右节点压入栈。
		stack.push(x->_rc);
		//将自身压入栈。
		stack.push(x);
	}
}

template<typename T>
void BinTree<T>::goAlongLeftBranch(BinNodePosi(T) x, stack<BinNodePosi(T)> &stack)
{
	while(x)
	{
		//这里只在栈中压入了当前元素，之后对右侧子树的访问是通过当前元素的_rc来实现的。
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
	//当当前节点既没有左子节点有没有右子节点时，跳出循环。
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

		//注意这次的跳出循环的位置与之前的有所不同，之前都是在pop()之前检验是否需要跳出循环，而此处由于是后序遍历，栈顶元素被访问时一定是其子树已经都完成访问了。因此当最后一个栈顶元素被取出时，不论它有没有右子树，后续都将不再有操作了。
		//而中序遍历和前序遍历则不同，他们的栈顶元素被访问时，至少还有一个子树没有被访问。也就是说，后续依然可能会有操作，将新的节点压入栈中。
		if(stack.empty())
		{
			break;
		}

		//如果自身是当前节点的父节点的右子节点，则返回NULL，使程序调用栈中的下一个节点。
		if(x == x->_parent->_rc)
		{
			x = NULL;
		}
		//如果自身是当前节点的父节点的左子节点，则返回当前节点的父节点的右子节点。
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
			//这里本来应该判断是否存在x->_rc，然后在执行push()操作的。
			//但是这里的循环终止条件就是栈顶元素为空，由于最后查找到的节点一定是没有子节点的，即子节点都为空，因此push()它的_rc会导致向栈中压入一个NULL，从而在下次条件判断中退出循环。
			stack.push(x->_rc);
		}
	}
	//弹出刚刚压入的NULL。
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

	//下面这段是我自己写的。
	/*
	while(true)
	{
		gotoLeftMostLeaf(stack);
		x = stack.top();
		stack.pop();
		visit(x);
		if(stack.empty())
		{
			break;
		}
		if(stack.top() == x->_parent)
		{
			stack.push(NULL);
		}
	}
	*/

	while(!stack.empty())
	{
		//当x所代表的子节点与栈顶的节点间具有父子关系时，说明此时栈顶节点的子节点都已经完成了遍历。
		//此时不希望程序继续对栈顶节点进行一次gotoLeftMostLeaf()操作，因此跳过。
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