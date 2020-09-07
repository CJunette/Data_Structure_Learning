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
	//_hot������ʾ���нڵ�ĸ��ס�
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

//�����ظ�Ԫ��ʱ��search�����׸�Ԫ�ء�
template<typename T>
BinNodePosi(T) &BST<T>::search(const T &e)
{
	return searchIn(this->_root, e, _hot = NULL);
}

template<typename T>
BinNodePosi(T) &BST<T>::searchIn(BinNodePosi(T) &x, const T &e, BinNodePosi(T) &hot)
{
	//����֮ǰ���������������Ķ��ֲ������������Ķ��ֲ��ҵģ��������ڹ����ϻ���������ͬ�������Ķ��ֲ���˼·��������������
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
			//������Ԫ���ظ�ʱ�����ص�һ��Ԫ�ء�
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
		//��cָ�����һ����e��ͬ�Ľڵ㡣
		while(c->succ() && c->succ()->_data == e)
		{
			_hot = c;
			c = c->succ();
		}
		//���c�������ҽڵ㣬��ֱ�����ҽڵ�λ�ò����½ڵ㡣
		if(!c->_rc)
		{
			_hot = c;
			c->_rc = new BinNode<T>(e, _hot);
			c = c->_rc;
		}
		//���c�����ҽڵ㣬������ҵ�c��succ()����succ()����ڵ���롣
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
	//����search���ص�����ͬԪ�ص���Ԫ�ص�λ�ã����������ڶ��Ԫ�أ�ֻ��Ҫ��succ()���������Ҽ��ɡ�

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