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
		//ƽ���������Ϊ1����СΪ-1��
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
			//��������������ɾ���ĵ����п��ܳ��ֵ������
			//�����������ȸ�ʱ��ѡ���븸�ڵ�ͬ���������
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
	//���ﻹ���ֶ���insert�Ĵ���������
	//��Ϊinsert���Զ�ִ��updateHeightAbove()�������ﲢ��ϣ���ڼ��ʧ��ǰ�Ͷ�height���е�����
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


	//�Ӹ��׿�ʼ���ʧ�⣨�������Ǵ��游��ʼ�Ż�ʧ��ģ������ڸ��׿��ܻ��и߶ȱ仯�����ѭ���Ӹ��׿�ʼ����
	for(BinNodePosi(T) g = this->_hot; g ; g = g->_parent)
	{
		if(!AvlBalanced(*g))
		{
			BinNodePosi(T) x;		
			//��Ϊ��rotateAt֮��g��parent�ᷢ���ı䣬����ڴ�֮ǰ�ȶ�����б��ݣ����ں���������
			//�ǵ�����Ҫ�����á�
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
	//����remove����Ͷ����Ƚ�����εĸ߶ȸ��£�����������ֱ�ӵ���remove()��
	bool x = BST<T>::remove(e);
	if(!x)
	{
		return x;
	}

	//�Ӹ��׿�ʼ�������Ƿ�ʧ�⡣
	for(BinNodePosi(T) g = this->_hot; g; g = g->_parent)
	{
		if(!AvlBalanced(*g))
		{
			BinNodePosi(T) x;
			BinNodePosi(T) &fromParent = fromParentTo(g);

			//��������ÿ��remove�����ܵ��¸߶ȱ仯����˱������α����������ȡ�
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