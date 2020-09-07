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
	//����ǰ�ڵ�������ӽڵ㣬�����ӽڵ�������������Ǻ�̽ڵ㡣
	if(s->_rc)
	{
		s = s->_rc;
		while(s->_lc)
		{
			s = s->_lc;
		}
		return s;
	}
	//���������ǰ�ڵ��Ǹ��ڵ�����ӽڵ㣬�򷵻ظ��ڵ㡣�����ǰ�ڵ㲻�Ǹ��ڵ�����ӽڵ㣬��һֱ���ϣ�ֱ���ҵ��Ǹ��ڵ�����ӽڵ������Ϊֹ��
	else
	{
		while(s->_parent && (s != s->_parent->_lc))
		{
			s = s->_parent;
		}
		//����������ֲ����и��ڵ㣬˵���Ѿ��ִ��λ�ã���ʱĿ��ڵ�Ϊ�����������һ��Ԫ�أ���ĺ�̽��Ϊ�ա�
		//�����ҷ��֣����������Ҳ����ͨ��s->_parent�������Ϊs->_parent��ʱΪNULL��
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

	//����ǰ����������ӽڵ�ʱ��ǰ����Ȼ�����ӽڵ��С�
	//��ǰ����Ȼ�����ӽڵ���������ӽڵ��С������������ӽڵ㣬��ǰ���������ӽڵ㡣
	if(s->_lc)
	{
		s = s->_lc;
		while(s->_rc)
		{
			s = s->_rc;
		}
		return s;
	}
	//�������ϲ���ֱ���ҵ�ĳ���������丸�ڵ�����ӽڵ�򵽴���ڵ㡣
	//�����ҵ�ĳ�������Ǹ����ȵĸ��ڵ�����ӽڵ㣬�����ȵĸ��ڵ��������Ҫ���ҵ�ǰ���ڵ㡣
	//�����ҵ�����δ�ҵ�����˵����ǰ�ڵ���ǵ�һ���ڵ㣬������ǰ������ǰ��Ϊ�գ�����˷���NULL��
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
		//����NULL����Ҫ���ظ߶�Ϊ-1��
		return x ? x->_height : -1;
	}
	virtual int updateHeight(BinNodePosi(T) x);
	void updateHeightAbove(BinNodePosi(T) x);

	public:

	int size() const { return _size; }
	int empty() const { return !_root; }
	BinNodePosi(T) root() const { return _root; }

	//�ڵ���롣
	BinNodePosi(T) insertAsLc(BinNodePosi(T) x, T const &e);
	BinNodePosi(T) insertAsRc(BinNodePosi(T) x, T const &e);
	//�������롣
	BinNodePosi(T) attchAsLc(BinNodePosi(T) x, BinTree<T> *&s);
	BinNodePosi(T) attchAsRc(BinNodePosi(T) x, BinTree<T> *&s);
	//����ɾ����
	int remove(BinNodePosi(T) x);
	//�������롣
	BinTree<T> *secede(BinNodePosi(T) x);

	//����������ݹ�汾һ��
	template<typename VST>
	void travPre_I1(BinNodePosi(T) x, VST visist);

	//����������ݹ�汾����
	template<typename VST>
	void travPre_I2(BinNodePosi(T) x, VST visist);
	template<typename VST>
	void visitAlongVine(BinNodePosi(T) x, VST visit, stack<BinNodePosi(T)> &stack);

	//�Լ�����д�����������
	template<typename VST>
	void travMid_Cunstomed_I1(BinNodePosi(T) x, VST visist);
	template<typename VST>
	void visitAlongVineComstumedForMid_I1(BinNodePosi(T) x, VST visit, stack<BinNodePosi(T)> &stack);

	//���ϸ��������������
	void goAlongLeftBranch(BinNodePosi(T) x, stack<BinNodePosi(T)> &stack);
	template<typename VST>
	void travMid_I1(BinNodePosi(T) x, VST visit);

	//�Լ�����д���������
	void goAlongLeftBranch_ForPost(BinNodePosi(T) x, stack<BinNodePosi(T)> &stack);
	template<typename VST>
	void travPost_Customed_I1(BinNodePosi(T) x, VST visit);
	template<typename VST>
	void travPost_Customed_I2(BinNodePosi(T) x, VST visit);

	//���ϸ����ĺ��������
	void gotoLeftMostLeaf(stack<BinNodePosi(T)> &stack);
	template<typename VST>
	void travPost_I1(BinNodePosi(T) x, VST visit);

	//�Լ�����д��α�����
	template<typename VST>
	void travLevel_Custom(VST visit);

	//���ϸ����Ĳ�α�����
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

		//����μ����õ���visit(x->data)������visit()����x�ľ������ݡ�
		//��Ϊ�˱�֤traverse�и��õ������ԣ����Ի�����visit(x)��������ָ�롣		

		//ע�⣬�����visit()��ִ��˳��Ϳ��Ͻ��Ĳ�һ����
		//֮���Էŵ���ѹջ֮������Ϊ������travPre_I1()����Delete��ִ��ɾ��������ʱ���֣������ִ��delete�ͻᵼ��x��_rc��_lc��Ϊ�գ���������stackΪ�գ�����Ԫ���޷�ɾ����
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
		//����stack���ԣ����x->_rcΪNULL��Ҳһ���ᱻѹ��ջ�У�ֻ���´�ִ��ʱ��ֱ����while��������
		stack.push(x->_rc);
		visit(x);
		x = lc;
	}
}

template<typename T>
template<typename VST>
void BinTree<T>::travMid_Cunstomed_I1(BinNodePosi(T) x, VST visit)
{
	//��������������а汾һ�����˼·����û�жԽڵ�����ȷ���������ڵ��Ƿ��Ѿ���ɱ��������Ե�����£��ǻᵼ��ѹ��ջ���������ظ�ѹ������ִ�������Ĳ����ġ�
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
		//һֱ�����·�����ֱ�����·�û���ӽڵ�Ϊֹ��
		visitAlongVineComstumedForMid_I1(x, visit, stack);
		if(stack.empty())
		{
			break;
		}
		//��ʱ�����Ľڵ��ǵ�ǰ�ڵ㡣
		x = stack.top();
		stack.pop();
		//ִ�в�����
		visit(x);
		//��ʱ�����Ľڵ��ǵ�ǰ�ڵ�����ӽڵ㡣
		x = stack.top();
		stack.pop();
	}
}

template<typename T>
template<typename VST>
void BinTree<T>::visitAlongVineComstumedForMid_I1(BinNodePosi(T) x, VST visit, stack<BinNodePosi(T)> &stack)
{
	//��Ϊ���ܴ���NULLָ�룬����Ҫ����һ�ηǿ��жϡ�
	if(x)
	{
		//һֱ��x���뵽���������ӽڵ���Ǹ��ڵ㡣
		while(x->_lc)
		{
			//����stack���ԣ����x->_rcΪNULL��Ҳһ���ᱻѹ��ջ�У�ֻ���´�ִ��ʱ��ֱ����while��������		
			stack.push(x->_rc);
			stack.push(x);
			//stack.push(x);
			x = x->_lc;
		}
		//���ҽڵ�ѹ��ջ��
		stack.push(x->_rc);
		//������ѹ��ջ��
		stack.push(x);
	}
}

template<typename T>
void BinTree<T>::goAlongLeftBranch(BinNodePosi(T) x, stack<BinNodePosi(T)> &stack)
{
	while(x)
	{
		//����ֻ��ջ��ѹ���˵�ǰԪ�أ�֮����Ҳ������ķ�����ͨ����ǰԪ�ص�_rc��ʵ�ֵġ�
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
	//����ǰ�ڵ��û�����ӽڵ���û�����ӽڵ�ʱ������ѭ����
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

		//ע����ε�����ѭ����λ����֮ǰ��������ͬ��֮ǰ������pop()֮ǰ�����Ƿ���Ҫ����ѭ�������˴������Ǻ��������ջ��Ԫ�ر�����ʱһ�����������Ѿ�����ɷ����ˡ���˵����һ��ջ��Ԫ�ر�ȡ��ʱ����������û�����������������������в����ˡ�
		//�����������ǰ�������ͬ�����ǵ�ջ��Ԫ�ر�����ʱ�����ٻ���һ������û�б����ʡ�Ҳ����˵��������Ȼ���ܻ��в��������µĽڵ�ѹ��ջ�С�
		if(stack.empty())
		{
			break;
		}

		//��������ǵ�ǰ�ڵ�ĸ��ڵ�����ӽڵ㣬�򷵻�NULL��ʹ�������ջ�е���һ���ڵ㡣
		if(x == x->_parent->_rc)
		{
			x = NULL;
		}
		//��������ǵ�ǰ�ڵ�ĸ��ڵ�����ӽڵ㣬�򷵻ص�ǰ�ڵ�ĸ��ڵ�����ӽڵ㡣
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
			//���ﱾ��Ӧ���ж��Ƿ����x->_rc��Ȼ����ִ��push()�����ġ�
			//���������ѭ����ֹ��������ջ��Ԫ��Ϊ�գ����������ҵ��Ľڵ�һ����û���ӽڵ�ģ����ӽڵ㶼Ϊ�գ����push()����_rc�ᵼ����ջ��ѹ��һ��NULL���Ӷ����´������ж����˳�ѭ����
			stack.push(x->_rc);
		}
	}
	//�����ո�ѹ���NULL��
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

	//������������Լ�д�ġ�
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
		//��x��������ӽڵ���ջ���Ľڵ����и��ӹ�ϵʱ��˵����ʱջ���ڵ���ӽڵ㶼�Ѿ�����˱�����
		//��ʱ��ϣ�����������ջ���ڵ����һ��gotoLeftMostLeaf()���������������
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