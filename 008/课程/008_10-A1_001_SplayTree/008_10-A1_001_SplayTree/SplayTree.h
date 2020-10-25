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
	//好像没有处理v不在tree内的情况。虽然通常也不会把树外的节点用到树内的搜索中...
	if(!v)
	{
		return NULL;
	}

	//伸展树本身是以区域性展开的，因此这里是不是加一个当v为root时的直接返回的判断会比较好？
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
	//如果不存在，则splay()会将_hot转移到_root位置。而_hot节点的元素要么是大于e的最小数，要么是小于e的最大数。
	//因此，接下来根据e的大小，可直接将_root的左树或是右树接到新创建节点的左子节点或右子节点位置，然后将当前的_root接到新创建节点的另一子节点处，最后更新_root。
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
		//暂时先切除左子树，并令根等于p的右子树。
		this->_root = p->_rc;
		//再次进行一次e的搜索，此时被移到根位置的一定是大于p的最小的节点。而那个节点一定没有左节点。
		search(e);
		attachAsLChild(this->_root, lTree);		
	}

	release(p->_data);
	release(p);
	this->updateHeightAbove(this->_root);
	this->_size--;
	return true;
}
