#pragma once
#include "BST.h"

template<typename T>
bool isBlack(BinNodePosi(T) p)
{
	return !p || p->_color == RB_BLACK;
}

template<typename T>
bool isRed(BinNodePosi(T) p)
{
	return !isBlack(p);
}

template<typename T>
bool BlackHeightUpdate(BinNodePosi(T) x)
{
	bool b1 = stature(x->_lc) == stature(x->_rc);
	bool b2 = x->_height == (isRed(x) ? stature(x->_lc) : stature(x->_lc) + 1);
	return b1 && b2;
}

/*-----------------------------------------------------------------------*/

template<typename T>
struct RBTree: public BST<T>
{
	protected:
	void solveDoubleRed(BinNodePosi(T) x);
	void solveDoubleBlack(BinNodePosi(T) x);

	public:
	BinNodePosi(T) insert(const T &e);
	bool remove(const T &e);
	int updateHeight(BinNodePosi(T) x);
};


template<typename T>
int RBTree<T>::updateHeight(BinNodePosi(T) x)
{
	if(!x)
	{
		return -1;
	}
	int heightLeft = stature(x->_lc);
	int heightRight = stature(x->_rc);
	//ͨ����x�����Һ��Ӹ߶�Ӧ��һ�£����������˫�ڣ�����ܻ������Һ��Ӹ߶Ȳ�ͬ�����Ρ�
	x->_height = (heightLeft < heightRight) ? heightRight : heightLeft;

	return isBlack(x) ? ++x->_height : x->_height;
}

template<typename T>
BinNodePosi(T) RBTree<T>::insert(const T &e)
{
	BinNodePosi(T) &x = this->search(e);
	if(x)
	{
		return x;
	}

	x = new BinNode<T>(e, this->_hot);
	this->_size++;
	this->updateHeight(x);
	solveDoubleRed(x);
	this->updateHeightAbove(x);
	return x;
}

template<typename T>
void RBTree<T>::solveDoubleRed(BinNodePosi(T) x)
{
	if(x == this->_root)
	{
		x->_color = RB_BLACK;
		updateHeight(x);
		return;
	}

	BinNodePosi(T) p = x->_parent;
	if(p->_color == RB_BLACK)
	{
		return;
	}
	BinNodePosi(T) g = p->_parent;
	BinNodePosi(T) u = uncle(x);

	//uΪ�ڡ�
	if(!u || u->_color == RB_BLACK)
	{
		if(isLChild(p) == isLChild(x))
		{
			p->_color = RB_BLACK;
			updateHeight(p);
		}
		else
		{
			x->_color = RB_BLACK;
			updateHeight(x);
		}
		g->_color = RB_RED;
		updateHeight(g);

		BinNodePosi(T) gg = g->_parent;
		BinNodePosi(T) &fromparent = fromParent(g);
		BinNodePosi(T) b = this->rotateAt(x);

		if(gg)
		{
			fromparent = b;
		}
		else
		{
			this->_root = b;
		}
		return;
	}
	//uΪ�졣
	else
	{
		p->_color = RB_BLACK;		
		u->_color = RB_BLACK;
		g->_color = RB_RED;
		updateHeight(p);
		updateHeight(u);
		updateHeight(g);
		solveDoubleRed(g);
	}
}

template<typename T>
bool RBTree<T>::remove(const T &e)
{
	BinNodePosi(T) &x = this->search(e);
	if(!x)
	{
		return false;
	}
	BinNodePosi(T) r = this->removeAt(x, this->_hot);
	this->_size--;
	//����ղ�ɾ���������һ��Ԫ�أ������������ֱ�ӷ��ء�
	if(!this->_size)
	{
		return true;
	}

	if(!this->_hot)
	{
		r->_color = RB_BLACK;
		updateHeight(r);		
		return true;
	}

	if(BlackHeightUpdate(this->_hot))
	{
		return true;
	}
	if(r && r->_color == RB_RED)
	{
		r->_color = RB_BLACK;
		updateHeight(r);
		//��Ϊ�����BlackHeightUpdate�ж�_hot�ĸ߶Ƚ������޸ģ������rΪ�죬��˵����ɾ����xΪ�ڣ������BlackHeightUpdate_hot�ĸ߶Ȼ��һ���˴���Ҫ�ٴν����޸���
		updateHeight(this->_hot);
		return true;
	}

	solveDoubleBlack(r);
	//this->updateHeightAbove(this->_hot);
	return true;
}

template<typename T>
void RBTree<T>::solveDoubleBlack(BinNodePosi(T) r)
{
	BinNodePosi(T) p = this->_hot;
	if(!p)
	{
		//����ǰ�ڵ�x���Ǹ��ڵ�ʱ��ֱ���˳���
		return;
	}
	//����r����Ϊ�գ��������ﲻ��ֱ����isLChild()��isRChild()��
	BinNodePosi(T) s = (r == p->_lc) ? p->_rc : p->_lc;

	if(isBlack(s))
	{
		BinNodePosi(T) t = NULL;
		if(isRed(s->_rc))
		{
			t = s->_rc;
		}
		if(isRed(s->_lc))
		{
			t = s->_lc;
		}

		//BB-1
		if(t)
		{
			RBColor oldcolor = p->_color;
			BinNodePosi(T) gg = p->_parent;
			BinNodePosi(T) &fromparent = fromParent(p);
						
			BinNodePosi(T) b = this->rotateAt(t);

			if(gg)
			{
				fromparent = b;
			}
			else
			{
				this->_root = b;
			}

			b->_color = oldcolor;
			b->_lc->_color = RB_BLACK;
			b->_rc->_color = RB_BLACK;
			updateHeight(b->_lc);
			updateHeight(b->_rc);
			updateHeight(b);
		}
		else
		{
			//BB-2R
			if(p->_color == RB_RED)
			{
				p->_color = RB_BLACK;
				if(s)
				{
					s->_color = RB_RED;
				}
				updateHeight(s);
				updateHeight(p);
			}
			//BB-2B
			else
			{
				if(s)
				{
					s->_color = RB_RED;
				}
				updateHeight(s);
				updateHeight(p);
				solveDoubleBlack(p);
			}
		}
	}
	//BB-3
	else
	{
		BinNodePosi(T) t = isLChild(s) ? s->_lc : s->_rc;
		BinNodePosi(T) gg = p->_parent;
		BinNodePosi(T) &fromparent = fromParent(p);

		BinNodePosi(T) b = this->rotateAt(t);

		if(gg)
		{
			fromparent = b;
		}
		else
		{
			this->_root = b;
		}

		s->_color = RB_BLACK;
		p->_color = RB_RED;
		updateHeight(p);
		updateHeight(s);
		this->_hot = p->_parent;
		solveDoubleBlack(r);
	}
}