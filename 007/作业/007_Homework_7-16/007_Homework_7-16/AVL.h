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
	BinNodePosi(T) &p = this->search(e);
	BinNodePosi(T) c;

	if(!p)
	{
		p = new BinNode<T>(e, this->_hot);
		c = p;
		this->_size++;		
	}
	else
	{
		c = p;
		//让c指向最后一个与e相同的节点。
		while(c->succ() && c->succ()->_data == e)
		{
			this->_hot = c;
			c = c->succ();
		}
		//如果c不存在右节点，则直接在右节点位置插入新节点。
		if(!c->_rc)
		{
			this->_hot = c;
			c->_rc = new BinNode<T>(e, this->_hot);
			c = c->_rc;
		}
		//如果c存在右节点，则继续找到c的succ()，在succ()的左节点插入。
		else
		{
			c = c->succ();
			this->_hot = c;
			c->_lc = new BinNode<T>(e, this->_hot);
			c = c->_lc;
		}
		this->_size++;		
	}
	this->updateHeight(c);


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
	return c;
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
