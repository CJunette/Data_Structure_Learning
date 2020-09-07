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
		//��cָ�����һ����e��ͬ�Ľڵ㡣
		while(c->succ() && c->succ()->_data == e)
		{
			this->_hot = c;
			c = c->succ();
		}
		//���c�������ҽڵ㣬��ֱ�����ҽڵ�λ�ò����½ڵ㡣
		if(!c->_rc)
		{
			this->_hot = c;
			c->_rc = new BinNode<T>(e, this->_hot);
			c = c->_rc;
		}
		//���c�����ҽڵ㣬������ҵ�c��succ()����succ()����ڵ���롣
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


	//�Ӹ��׿�ʼ���ʧ�⣨�������Ǵ��游��ʼ�Ż�ʧ��ģ������ڸ��׿��ܻ��и߶ȱ仯�����ѭ���Ӹ��׿�ʼ����
	for(BinNodePosi(T) g = this->_hot; g; g = g->_parent)
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
	return c;
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
