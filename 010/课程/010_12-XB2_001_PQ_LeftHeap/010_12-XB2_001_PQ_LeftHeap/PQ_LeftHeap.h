#pragma once

#include "PQ.h"
#include "BinTree.h"

template<typename T>
struct PQ_LeftHeap: public PQ<T>, public BinTree<T>
{
	//��������ʽ���У�height������һ������Ҫ�Ĳ������������صĺ����ж����ٶ�height����ά����
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
	//�����_parent��ΪNULL�Ĳ���û�б�Ҫ����Ϊ���_root���½ڵ�Ϊ��������������Ҫ��_root��_parent�����޸ģ���������������������ɵĽڵ��_parentҲ��NULL������������á�
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
	//��������΢�Կ��ϵĴ�������һЩ�����������漰�������Ĳ�����
	//��Ҫ��ô������ԭ�����ڣ����ϲ���a��_rc��b֮��b����ʵ��û��ɾ����Ӧ�Ľڵ㡣��͵�������δ������������ʱ���������ظ����������⡣
	//�������ֱ�ӽ�������Ϊ���ã�������ɺϲ����Ѿ�����a��bָ��NULL������֮������ظ����������⡣
	//[2020.9.23����] ������ʵֻ�����������rb��ֵ����ΪNULL��������ģ��ݹ��ڵ�rb��ʵ�����ϲ�ݹ����ɵ���ʱ��������������ΪNULL�����岻�󡣲������յ�Ч���ﵽ��ͨ��ʹrbΪNULL�������䱻�ظ�������Ŀ�ġ�
	rb = NULL;

	//�����ƺ�û�б�Ҫ��a->_rc���жϣ���Ϊִ�е������a->_rcһ�����ڡ��������ڣ�����֮ǰ���˻��������Ѿ��������ˡ�
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