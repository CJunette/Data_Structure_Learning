#pragma once

#include "BinTree.h"

template<typename T>
struct BST: public BinTree<T>
{
	public:
	BST();

	//由于BST本身可能还存在更多的变种，因此这里将这些函数都写成虚函数，便于以后的修改。

	//search()函数返回值为引用的原因在于，后续其他的函数可能还会需要使用它。
	virtual BinNodePosi(T) &search(const T &);
	virtual BinNodePosi(T) insert(const T &);
	virtual bool remove(const T &);
	virtual void leftMostPath();

	protected:
	//_hot用来表示命中节点的父亲。
	BinNodePosi(T) _hot;
	virtual BinNodePosi(T) connect34(BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c, BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3);
	virtual BinNodePosi(T) rotateAt(BinNodePosi(T) v);

	private:
	static BinNodePosi(T) &searchIn(BinNodePosi(T) &x, const T &e, BinNodePosi(T) &hot);
	static BinNodePosi(T) &removeAt(BinNodePosi(T) &x, BinNodePosi(T) &hot);
	BinNodePosi(T) zagForTwo(BinNodePosi(T) &x);
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
		//注意，这里必须要令c为一个指针引用，不然会导致返回的只是左子节点或右子节点内的地址值，而没有返回左子节点或右子节点的引用。
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
	//注意，这里也必须用指针引用，不然无法实现指向变化。
	BinNodePosi(T) &p = search(e);
	/*
	std::cout << &p << std::endl;
	std::cout << &this->_root->_rc << std::endl;
	std::cout << &this->_root->_lc << std::endl;
	*/
	if(!p)
	{
		p = new BinNode<T>(e, _hot);

		/*
		if(this->_size == 0)
		{}
		else
		{
			(e < _hot->_data) ? _hot->_lc = p : _hot->_rc = p;
		}
		*/
		this->_size++;
		this->updateHeightAbove(p);
	}
	return p;
}

template<typename T>
BinNodePosi(T) &BST<T>::removeAt(BinNodePosi(T) &x, BinNodePosi(T) &hot)
{
	BinNodePosi(T) succ = NULL;

	//如果x只有单分支，则将succ指向非空的那一个分支，以备后续的替换操作。
	if(!x->_lc)
	{
		succ = x->_rc;
	}
	else if(!x->_rc)
	{
		succ = x->_lc;
	}
	//如果x存在双分支，则交换x的数值与x的中序遍历意义下的直接后继的数值。此时只需要删除这个“直接后继”接口。由于“直接后继”必然只有右子树，没有左子树，因此此时问题退化为单分支问题。
	//令succ等于x的中序遍历意义下的后一节点。然后交换x与succ的数值。接着令x指向succ，succ指向succ的右子节点，hot指向此时x的parent。
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

	//hot始终保持x的parent的身份。
	//此处确定x是hot的左孩子还是右孩子。由于x在之后将被删除，因此需要建立hot与succ的父子关系。
	if(hot->_lc == x)
	{
		hot->_lc = succ;
	}
	else
	{
		hot->_rc = succ;
	}

	//如果succ不为空，则还需保证将succ的_parent也进行对应的设置。
	if(succ)
	{
		succ->_parent = hot;
	}

	//接下来执行删除x节点的操作。
	release(x->_data);
	release(x);
	//由于x是引用，之后还需要进行updateHeightAbove(x)的操作，因此需要把x进行重新指向。
	//由于无法确定succ是否为空，因此将x指向hot。
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
	//实际上，旋转操作并不需要真的进行那样复杂的指针交换。
	//只需要根据a、b、c及T0、T1、T2、T3间的相对位置关系，进行组合即可。
	//至于a、b、c与g、p、v的对应关系，在输入参数时进行确定。

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

template<typename T>
BinNodePosi(T) BST<T>::zagForTwo(BinNodePosi(T) &x)
{
	BinNodePosi(T) y = x->_rc;
	//BinNodePosi(T) T0 = x->_lc;
	BinNodePosi(T) T1 = y->_lc;
	//BinNodePosi(T) T2 = y->_rc;

	if(x->_parent)
	{
		BinNodePosi(T) &fromParent = isLChild(x) ? x->_parent->_lc : x->_parent->_rc;
		fromParent = y;
	}
	else
	{
		this->_root = y;
	}

	x->_rc = T1;
	
	y->_parent = x->_parent;

	y->_lc = x;
	x->_parent = y;

	return x;
}

template<typename T>
void BST<T>::leftMostPath()
{
	BinNodePosi(T) p = this->_root;

	while(p->_lc)
	{
		p = p->_lc;
	}

	while(p->_rc || p->_parent)
	{
		if(!p->_rc)
		{
			p = p->_parent;			
		}

		if(p->_rc)
		{
			p = zagForTwo(p);
		}		
	}
}