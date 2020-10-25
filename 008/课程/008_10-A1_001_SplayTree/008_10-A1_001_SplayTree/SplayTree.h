#pragma once
#include "BST.h"

template<typename T>
struct Splay: public BST<T>
{
	protected:
	BinNodePosi(T) splay(BinNodePosi(T) v);

	public:
	BinNodePosi(T) &search(const T &e);
	BinNodePosi(T) insert(const T &e);
	bool remove(const T &e);
};

template<typename T>
BinNodePosi(T) Splay<T>::splay(BinNodePosi(T) v)
{
	//����û�д���v����tree�ڵ��������Ȼͨ��Ҳ���������Ľڵ��õ����ڵ�������...
	if(!v)
	{
		return NULL;
	}

	//��չ����������������չ���ģ���������ǲ��Ǽ�һ����vΪrootʱ��ֱ�ӷ��ص��жϻ�ȽϺã�
	/*
	if(v == this->_root)
	{
		return v;
	}
	*/

	BinNodePosi(T) p;
	BinNodePosi(T) g;

	while((p = v->_parent) && (g = p->_parent))
	{
		BinNodePosi(T) gg = g->_parent;
		if(isLChild(v))
		{
			//zigzig
			if(isLChild(p))
			{				
				attachAsLChild(g, p->_rc);
				attachAsLChild(p, v->_rc);
				attachAsRChild(p, g);
				attachAsRChild(v, p);
			}
			//zigzag
			else
			{
				attachAsLChild(p, v->_rc);
				attachAsRChild(g, v->_lc);
				attachAsLChild(v, g);
				attachAsRChild(v, p);
			}
		}
		else
		{
			//zagzig
			if(isLChild(p))
			{
				attachAsRChild(p, v->_lc);
				attachAsLChild(g, v->_rc);
				attachAsLChild(v, p);
				attachAsLChild(v, g);
			}
			//zagzag
			else
			{
				attachAsRChild(g, p->_lc);
				attachAsRChild(p, v->_lc);
				attachAsLChild(p, g);
				attachAsLChild(v, p);
			}
		}
		
		if(!gg)
		{
			v->_parent = NULL;
		}
		else
		{
			(g == gg->_lc) ? attachAsLChild(gg, v) : attachAsRChild(gg, v);
		}
		
		this->updateHeight(g);
		this->updateHeight(p);
		this->updateHeight(v);
	}

	if(p = v->_parent)
	{
		if(isLChild(v))
		{
			attachAsLChild(p, v->_rc);
			attachAsRChild(v, p);
		}
		else
		{
			attachAsRChild(p, v->_rc);
			attachAsLChild(v, p);
		}
	}
	v->_parent = NULL;
	this->_root = v;
	return v;
}

template<typename T>
BinNodePosi(T) &Splay<T>::search(const T& e)
{
	BinNodePosi(T) &v = BST<T>::searchIn(this->_root, e, this->_hot = NULL);
	splay(v ? v : this->_hot);
	return this->_root;
}

template<typename T>
BinNodePosi(T) Splay<T>::insert(const T &e)
{
	if(!this->_root)
	{
		return BST<T>::insert(e);
	}

	BinNodePosi(T) p = search(e);
	
	if(p->_data == e)
	{
		return this->_root;
	}

	BinNodePosi(T) newNode = new BinNode<T>(e);
	//��������ڣ���splay()�Ὣ_hotת�Ƶ�_rootλ�á���_hot�ڵ��Ԫ��Ҫô�Ǵ���e����С����Ҫô��С��e���������
	//��ˣ�����������e�Ĵ�С����ֱ�ӽ�_root���������������ӵ��´����ڵ�����ӽڵ�����ӽڵ�λ�ã�Ȼ�󽫵�ǰ��_root�ӵ��´����ڵ����һ�ӽڵ㴦��������_root��
	if(p->_data <= e)
	{
		attachAsRChild(newNode, p->_rc);
		p->_rc = NULL;
		attachAsLChild(newNode, p);
	}
	else
	{
		attachAsLChild(newNode, p->_lc);
		p->_lc = NULL;
		attachAsRChild(newNode, p);
	}
	this->_root = newNode;

	this->updateHeightAbove(p);
	this->_size++;
	return this->_root;
}

template<typename T>
bool Splay<T>::remove(const T &e)
{
	BinNodePosi(T) p = search(e);

	if(!this->_root || p->_data != e)
	{
		return false;
	}

	if(!p->_lc)
	{
		this->_root = p->_rc;
		if(this->_root)
		{
			this->_root->_parent = NULL;
		}		
	}
	else if(!p->_rc)
	{
		this->_root = p->_lc;
		if(this->_root)
		{
			this->_root->_parent = NULL;
		}
	}
	else
	{
		BinNodePosi(T) lTree = p->_lc;
		//��ʱ���г������������������p����������
		this->_root = p->_rc;
		//�ٴν���һ��e����������ʱ���Ƶ���λ�õ�һ���Ǵ���p����С�Ľڵ㡣���Ǹ��ڵ�һ��û����ڵ㡣
		search(e);
		attachAsLChild(this->_root, lTree);		
	}

	release(p->_data);
	release(p);
	this->updateHeightAbove(this->_root);
	this->_size--;
	return true;
}
