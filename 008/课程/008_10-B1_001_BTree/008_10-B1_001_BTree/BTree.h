#pragma once
#include "Vector.h"
#include <queue>
#define BTNodePosi(T) BTNode<T>*

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

/*----------------------------------------------------------------------------*/

template<typename T>
struct BTNode
{
	BTNodePosi(T) _parent;
	Vector<T> key;
	Vector<BTNodePosi(T)> child;

	BTNode();
	BTNode(T e, BTNodePosi(T) lc = NULL, BTNodePosi(T) rc = NULL);
};

template<typename T>
BTNode<T>::BTNode()
{
	_parent = NULL;
	child.insert(0, NULL);
}

template<typename T>
BTNode<T>::BTNode(T e, BTNodePosi(T) lc, BTNodePosi(T) rc)
{
	_parent = NULL;
	key.insert(0, e);
	child.push_back(lc);
	child.push_back(rc);

	if(lc)
	{
		lc->_parent = this;
	}
	if(rc)
	{
		rc->_parent = this;
	}
}

/*----------------------------------------------------------------------------*/

template<typename T>
struct BTree
{
	//_order代表阶数，也就是BTNode中的分支个数。
	int _size, _order;
	BTNodePosi(T) _root;
	BTNodePosi(T) _hot;

	BTree(int order_ = 3);
	~BTree();
	BTNodePosi(T) search(const T &e);
	bool insert(const T &e);
	bool remove(const T &e);
	
	void show();

	protected:
	void solveOverflow(BTNodePosi(T) v);
	void solveUnderflow(BTNodePosi(T) v);
	void clear(BTNodePosi(T) v);
};

template<typename T>
BTree<T>::BTree(int order_):
	_size(0), _order(order_), _hot(NULL)
{
	_root = new BTNode<T>;
}

template<typename T>
void BTree<T>::clear(BTNodePosi(T) v)
{
	if(!v)
	{
		release(v);
		return;
	}	
	for(int i = 0; i < v->child.getSize(); i++)
	{
		clear(v->child[i++]);

	}
	return;
}

template<typename T>
BTree<T>::~BTree()
{
	clear(_root);
}

template<typename T>
BTNodePosi(T) BTree<T>::search(const T &e)
{
	BTNodePosi(T) v = _root;
	_hot = NULL;

	while(v)
	{
		int r = v->key.searchLinear(e);
		if(0 <= r && v->key[r] == e)
		{
			return v;
		}

		_hot = v;
		v = v->child[r + 1];
	}
	
	return v;
}

template<typename T>
bool BTree<T>::insert(const T &e)
{
	BTNodePosi(T) v = search(e);
	
	if(v)
	{
		return false;
	}

	int r = _hot->key.searchLinear(e);
	_hot->key.insert(r + 1, e);
	_hot->child.insert(r + 2, NULL);
	_size++;
	solveOverflow(_hot);
	return true;
}

template<typename T>
void BTree<T>::solveOverflow(BTNodePosi(T) v)
{
	if(v->child.getSize() <= _order)
	{
		return;
	}

	int s = _order / 2;

	//将节点的右半部分数据单独分离出来。

	//注意，rt在生成时就会自带一个空孩子。
	BTNodePosi(T) rt = new BTNode<T>;

	for(int i = 0; i < _order - s - 1; i++)
	{		
		rt->key.insert(i, v->key.remove(s + 1));
		rt->child.insert(i, v->child.remove(s + 1));
	}
	rt->child[_order - s - 1] = v->child.remove(s + 1);


	//如果rt的child中的元素不为NULL，则对新生成的右树中的child元素，确定新的parent（这种情况发生在叶节点的分裂带来的祖先节点的solveOverflow中）。
	if(rt->child[0])
	{
		for(int i = 0; i < rt->child.getSize(); i++)
		{
			rt->child[i]->_parent = rt;
		}
	}	

	//确定上一级节点。
	BTNodePosi(T) p = v->_parent;
	//如果上一级节点不存在，则新创建一个，并把现有的左树v添加到其中的child向量中。
	if(!p)
	{
		p = new BTNode<T>;
		p->child[0] = v;
		v->_parent = p;
		_root = p;
	}

	//将中间位置的元素移动到上一级节点，并将右树移动到上级节点的child向量中。
	rt->_parent = p;

	//我之前是在搜索的时候是在p->child中搜索v，但由于child中的地址不是按大小排列的，所以还是要用关键码进行搜索。
	int r = p->key.searchLinear(v->key[0]) + 1;
	p->key.insert(r, v->key.remove(s));
	p->child.insert(r + 1, rt);

	solveOverflow(p);
}

template<typename T>
bool BTree<T>::remove(const T &e)
{
	BTNodePosi(T) v = search(e);

	if(!v)
	{
		return false;
	}

	int r = v->key.searchLinear(e);
	//如果当前的v不是一个叶节点，那么我们希望将v[r]与它直接后继的叶节点中的元素进行交换。
	if(v->child[0])
	{
		BTNodePosi(T) u = v->child[r + 1];
		while(u->child[0])
		{
			u = u->child[0];
		}

		//此时将v->key[r]所指元素与u->key[0]所指元素进行“交换”。虽然是交换，但由于v->key[r]是要被删除的元素，所以具体数值并不重要。因此只要把u->key[0]交换过来即可。
		v->key[r] = u->key[0];
		v = u;
		r = 0;
	}

	v->key.remove(r);
	v->child.remove(r + 1);
	_size--;
	solveUnderflow(v);
	return true;
}

template<typename T>
void BTree<T>::solveUnderflow(BTNodePosi(T) v)
{
	if((_order + 1) / 2 <= v->child.getSize())
	{
		return;
	}

	BTNodePosi(T) p = v->_parent;
	if(!p)
	{
		if(v->key.getSize() == 0 && v->child[0])
		{
			_root = v->child[0];
			_root->_parent = NULL;
			v->child[0] = NULL;
			release(v);
		}
		//由于根节点可以最少只有2阶，即1个元素，所以除了根节点元素为0的情况，其他情况都可以直接返回。
		return;
	}

	//这里不能用v->key[0]去搜索，因为这个key已经被删除了。
	/*
	int r = p->key.searchLinear(v->key[0]) + 1;
	*/
	int r = 0;
	while(p->child[r] != v)
	{
		r++;
	}

	//旋转
	//当存在左兄弟，可以向左兄弟借时。
	if(0 < r)
	{
		BTNodePosi(T) ls = p->child[r - 1];
		if((_order + 1) / 2 < ls->child.getSize())
		{
			//将p中的元素转移到v的头部。
			v->key.insert(0, p->key[r - 1]);
			//将ls的child的最后一个元素移动到v的child的开头位置。
			v->child.insert(0, ls->child.remove(ls->child.getSize() - 1));			
			//将ls的key的最后一个元素移动到p中的合适位置。
			p->key[r - 1] = ls->key.remove(ls->key.getSize() - 1);
			if(v->child[0])			
			{
				v->child[0]->_parent = v;
			}
			//旋转操作不会造成进一步的下溢，因此可以直接返回。
			return;
		}		
	}
	//当存在右兄弟，可以向右兄弟借时。（如果左兄弟大小太小，且v不在最左端，也会进入这个条件。）
	else if(r < p->child.getSize() - 1)
	{
		BTNodePosi(T) rs = p->child[r + 1];
		if((_order + 1) / 2 < rs->child.getSize())
		{			
			v->key.insert(v->key.getSize(), p->key[r]);
			v->child.insert(v->child.getSize(), rs->child.remove(0));
			p->key[r] = rs->key.remove(0);
			if(v->child[v->child.getSize() - 1])
			{
				v->child[v->child.getSize() - 1]->_parent = v;
			}
			return;
		}
	}

	//合并

	//与左兄弟合并时。
	if(0 < r)
	{
		BTNodePosi(T) ls = p->child[r - 1];

		//将p中key[r-1]元素移动到ls中。
		ls->key.insert(ls->key.getSize(), p->key.remove(r - 1));
		//删除p中的child[r]。
		p->child.remove(r);

		//为了方便下面用循环，先将一个v中的child[0]移动到ls的child中。
		ls->child.insert(ls->child.getSize(), v->child.remove(0));
		if(ls->child[ls->child.getSize() - 1])
		{
			ls->child[ls->child.getSize() - 1]->_parent = ls;
		}

		//批量将v的key和child合并到ls中。
		while(!v->key.empty())
		{
			ls->key.insert(ls->key.getSize(), v->key.remove(0));
			ls->child.insert(ls->child.getSize(), v->child.remove(0));

			if(ls->child[ls->child.getSize() - 1])
			{
				ls->child[ls->child.getSize() - 1]->_parent = ls;
			}
		}
	}
	//与右兄弟合并时。
	else
	{
		BTNodePosi(T) rs = p->child[r + 1];

		//注意，这里是p->key.remove(r)，而不是p->key.remove(r - 1)。拼合是按照大小关系来的。
		rs->key.insert(0, p->key.remove(r));
		p->child.remove(r);

		rs->child.insert(0, v->child.remove(v->child.getSize() - 1));
		if(rs->child[0])
		{
			rs->child[0]->_parent = rs;
		}

		while(!v->key.empty())
		{
			rs->key.insert(0, v->key.remove(v->key.getSize() - 1));
			rs->child.insert(0, v->child.remove(v->child.getSize() - 1));

			if(rs->child[0])
			{
				rs->child[0]->_parent = rs;
			}
		}
	}
	release(v);
	//合并操作可能带来进一步的下溢。
	solveUnderflow(p);
}

template<typename T>
void BTree<T>::show()
{
	std::queue<BTNodePosi(T)> q;
	BTNodePosi(T) v = _root;
	int i;
	for(i = 0; i < v->key.getSize(); i++)
	{
		std::cout << v->key[i] << " ";
		if(v->child[i])
		{
			q.push(v->child[i]);
		}
	}
	if(v->child[i])
	{
		q.push(v->child[i]);
	}

	std::cout << std::endl;
	int count = v->child.getSize();

	
	while(!q.empty())
	{
		int newCount = 0;
		for(int j = 0; j < count; j++)
		{
			v = q.front();
			q.pop();

			int i;
			for(i = 0; i < v->key.getSize(); i++)
			{
				std::cout << v->key[i] << " ";
				if(v->child[i])
				{
					q.push(v->child[i]);
				}
			}
			if(v->child[i])
			{
				q.push(v->child[i]);
			}

			std::cout << "\t";
			newCount += v->child.getSize();
		}		
		std::cout << std::endl;
		count = newCount;
	}	
}