#pragma once
#include "BST.h"

namespace
{
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
}

/*------------------------------------------------------------------------------------------*/

template<typename T>
struct AVL: public BST<T>
{
	BinNodePosi(T) insert(const T &);
	bool remove(const T &);
	
	//virtual BinNodePosi(T) connect34(BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T));
	//virtual BinNodePosi(T) rotateAt(BinNodePosi(T));
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

		if(this->_size == 0)
		{
		}
		else
		{
			(e < this->_hot->_data) ? this->_hot->_lc = p : this->_hot->_rc = p;
		}
		this->_size++;		
	}
	this->updateHeight(p);


	//从父亲开始检查失衡（按道理是从祖父开始才会失衡的，但由于父亲可能会有高度变化，因此循环从父亲开始）。
	for(BinNodePosi(T) g = this->_hot; g ; g = g->_parent)
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

/*
template<typename T>
BinNodePosi(T) AVL<T>::connect34(BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c, BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3)
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
BinNodePosi(T) AVL<T>::rotateAt(BinNodePosi(T) v)
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
*/