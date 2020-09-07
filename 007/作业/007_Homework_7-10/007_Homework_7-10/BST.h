#pragma once

#include "BinTree.h"
#include <ctime>

template<typename T>
struct BST: public BinTree<T>
{
	public:
	BST();

	virtual BinNodePosi(T) &search(const T &);
	virtual BinNodePosi(T) insert(const T &);
	virtual bool remove(const T &);
	virtual int searchAll(const T &e);

	protected:
	//_hot用来表示命中节点的父亲。
	BinNodePosi(T) _hot;
	BinNodePosi(T) connect34(BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T));
	BinNodePosi(T) rotateAt(BinNodePosi(T));

	private:
	static BinNodePosi(T) &searchIn(BinNodePosi(T) &x, const T &e, BinNodePosi(T) &hot);
	static BinNodePosi(T) &removeAt(BinNodePosi(T) &x, BinNodePosi(T) &hot);
};

template<typename T>
BST<T>::BST():
	_hot(NULL)
{}

//存在重复元素时，search返回首个元素。
template<typename T>
BinNodePosi(T) &BST<T>::search(const T &e)
{
	return searchIn(this->_root, e, _hot = NULL);
}

template<typename T>
BinNodePosi(T) &BST<T>::searchIn(BinNodePosi(T) &x, const T &e, BinNodePosi(T) &hot)
{
	//这里之前我想用类似向量的二分查找来处理树的二分查找的，但两者在构造上还是有所不同，向量的二分查找思路并不适用于树。
	if(!x)
	{
		return x;
	}
	else if(x->_data == e)
	{		
		while(x->pred() && x->pred()->_data == e)
		{
			x = x->pred();
		}
		return x;
	}	
	hot = x;

	while(true)
	{		
		BinNodePosi(T) &c = (e < hot->_data) ? hot->_lc : hot->_rc;
		if(!c)
		{
			return c;
		}
		else if(c->_data == e)
		{
			//当存在元素重复时，返回第一个元素。
			while(c->pred() && c->pred()->_data == e)
			{
				c = c->pred();
			}
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
		this->updateHeightAbove(_hot);
		return p;
	}
	else
	{
		BinNodePosi(T) c = p;
		//让c指向最后一个与e相同的节点。
		while(c->succ() && c->succ()->_data == e)
		{
			_hot = c;
			c = c->succ();
		}
		//如果c不存在右节点，则直接在右节点位置插入新节点。
		if(!c->_rc)
		{
			_hot = c;
			c->_rc = new BinNode<T>(e, _hot);
			c = c->_rc;
		}
		//如果c存在右节点，则继续找到c的succ()，在succ()的左节点插入。
		else
		{
			c = c->succ();
			_hot = c;
			c->_lc = new BinNode<T>(e, _hot);			
			c = c->_lc;
		}
		this->_size++;
		this->updateHeightAbove(_hot);
		return c;
	}
}

template<typename T>
BinNodePosi(T) &BST<T>::removeAt(BinNodePosi(T) &x, BinNodePosi(T) &hot)
{
	BinNodePosi(T) succ = NULL;
	BinNodePosi(T) w = x;

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
		srand(time(0));
		int choose = rand() % 2;

		T temp;
		switch(choose)
		{
			case 0:
				succ = w->succ();

				temp = w->_data;
				w->_data = succ->_data;
				succ->_data = temp;

				w = succ;
				succ = succ->_rc;
				hot = w->_parent;
				break;
			case 1:
				succ = w->pred();

				temp = w->_data;
				w->_data = succ->_data;
				succ->_data = temp;

				w = succ;
				succ = succ->_lc;
				hot = w->_parent;
		}
	}

	if(hot->_lc == w)
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

	release(w->_data);
	release(w);

	return succ;
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
	this->updateHeightAbove(_hot);
	return true;
}

template<typename T>
int BST<T>::searchAll(const T &e)
{
	BinNodePosi(T) p = search(e);
	int count = 0;
	//由于search返回的是相同元素的首元素的位置，因此如果存在多个元素，只需要用succ()不断向后查找即可。

	if(!p)
	{
		return count;
	}
	else
	{
		count++;
		while(p = p->succ(), p->_data == e)
		{			
			count++;
		}
		return count;
	}
}