#pragma once

#include "PQ.h"
#include "BinTree.h"

template<typename T>
struct PQ_LeftHeap: public PQ<T>, public BinTree<T>
{
	//由于在左式堆中，height并不是一个很重要的参数，因此在相关的函数中都不再对height进行维护。
	virtual T getMax() { return this->_root->_data; }
	virtual void insert(const T &e);
	virtual T delMax();
};

template<typename T>
void PQ_LeftHeap<T>::insert(const T &e)
{
	BinNodePosi(T) node = new BinNode<T>(e);
	this->_root = merge(this->_root, node);
	this->_size++;
	//这个将_parent设为NULL的操作没有必要，因为如果_root与新节点为发生交换，则不需要对_root的_parent进行修改；如果发生交换，则新生成的节点的_parent也是NULL，无需进行设置。
	//this->_root->_parent = NULL;
}

template<typename T>
T PQ_LeftHeap<T>::delMax()
{
	T temp = this->_root->_data;
	BinNodePosi(T) left = this->_root->_lc;
	BinNodePosi(T) right = this->_root->_rc;

	delete this->_root;

	this->_root = merge(left, right);
	if(this->_root)
	{
		this->_root->_parent = NULL;
	}
	this->_size--;
	return temp;
}

template<typename T>
static BinNodePosi(T) merge(BinNodePosi(T)& ra, BinNodePosi(T)& rb)
{
	BinNodePosi(T) a = ra;
	BinNodePosi(T) b = rb;

	if(!a)
	{
		return b;
	}
	if(!b)
	{
		return a;
	}

	if(a->_data < b->_data)
	{
		BinNodePosi(T) temp = a;
		a = b;
		b = temp;
	}

	a->_rc = merge(a->_rc, b);
	//这里我稍微对课上的代码做了一些调整。调整涉及到函数的参数。
	//需要这么调整的原因在于，当合并了a的_rc和b之后，b中其实并没有删除对应的节点。这就导致了在未来进行析构的时候可能造成重复析构的问题。
	//因此这里直接将参数改为引用，并在完成合并后将已经并入a的b指向NULL。避免之后出现重复析构的问题。
	//[2020.9.23补充] 这里其实只有最初进来的rb的值被设为NULL是有意义的，递归内的rb其实都是上层递归生成的临时变量，将他们设为NULL的意义不大。不过最终的效果达到了通过使rb为NULL，避免其被重复析构的目的。
	rb = NULL;

	//这里似乎没有必要做a->_rc的判断，因为执行到这里的a->_rc一定存在。若不存在，则在之前的退化条件中已经被返回了。
	if(a->_rc)
	{
		a->_rc->_parent = a;
	}

	if(!a->_lc || a->_lc->npl < a->_rc->npl)
	{
		BinNodePosi(T) temp = a->_lc;
		a->_lc = a->_rc;
		a->_rc = temp;
	}

	a->npl = a->_rc ? a->_rc->npl + 1 : 1;

	return a;
}