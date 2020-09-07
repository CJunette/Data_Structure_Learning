#pragma once

#include "BinTree.h"

template<typename T>
struct BST: public BinTree<T>
{
	public:
	BST();

	//����BST������ܻ����ڸ���ı��֣�������ｫ��Щ������д���麯���������Ժ���޸ġ�

	//search()��������ֵΪ���õ�ԭ�����ڣ����������ĺ������ܻ�����Ҫʹ������
	virtual BinNodePosi(T) &search(const T &);
	virtual BinNodePosi(T) insert(const T &);
	virtual bool remove(const T &);
	virtual void leftMostPath();

	protected:
	//_hot������ʾ���нڵ�ĸ��ס�
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
		//ע�⣬�������Ҫ��cΪһ��ָ�����ã���Ȼ�ᵼ�·��ص�ֻ�����ӽڵ�����ӽڵ��ڵĵ�ֵַ����û�з������ӽڵ�����ӽڵ�����á�
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
	//ע�⣬����Ҳ������ָ�����ã���Ȼ�޷�ʵ��ָ��仯��
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

	//���xֻ�е���֧����succָ��ǿյ���һ����֧���Ա��������滻������
	if(!x->_lc)
	{
		succ = x->_rc;
	}
	else if(!x->_rc)
	{
		succ = x->_lc;
	}
	//���x����˫��֧���򽻻�x����ֵ��x��������������µ�ֱ�Ӻ�̵���ֵ����ʱֻ��Ҫɾ�������ֱ�Ӻ�̡��ӿڡ����ڡ�ֱ�Ӻ�̡���Ȼֻ����������û������������˴�ʱ�����˻�Ϊ����֧���⡣
	//��succ����x��������������µĺ�һ�ڵ㡣Ȼ�󽻻�x��succ����ֵ��������xָ��succ��succָ��succ�����ӽڵ㣬hotָ���ʱx��parent��
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

	//hotʼ�ձ���x��parent����ݡ�
	//�˴�ȷ��x��hot�����ӻ����Һ��ӡ�����x��֮�󽫱�ɾ���������Ҫ����hot��succ�ĸ��ӹ�ϵ��
	if(hot->_lc == x)
	{
		hot->_lc = succ;
	}
	else
	{
		hot->_rc = succ;
	}

	//���succ��Ϊ�գ����豣֤��succ��_parentҲ���ж�Ӧ�����á�
	if(succ)
	{
		succ->_parent = hot;
	}

	//������ִ��ɾ��x�ڵ�Ĳ�����
	release(x->_data);
	release(x);
	//����x�����ã�֮����Ҫ����updateHeightAbove(x)�Ĳ����������Ҫ��x��������ָ��
	//�����޷�ȷ��succ�Ƿ�Ϊ�գ���˽�xָ��hot��
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
	//ʵ���ϣ���ת����������Ҫ��Ľ����������ӵ�ָ�뽻����
	//ֻ��Ҫ����a��b��c��T0��T1��T2��T3������λ�ù�ϵ��������ϼ��ɡ�
	//����a��b��c��g��p��v�Ķ�Ӧ��ϵ�����������ʱ����ȷ����

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