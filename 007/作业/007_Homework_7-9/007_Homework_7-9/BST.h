#pragma once

#include "BinTree.h"
#include <ctime>

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
	BinNodePosi(T) w = x;

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

	//hotʼ�ձ���x��parent����ݡ�
	//�˴�ȷ��x��hot�����ӻ����Һ��ӡ�����x��֮�󽫱�ɾ���������Ҫ����hot��succ�ĸ��ӹ�ϵ��
	if(hot->_lc == w)
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
	release(w->_data);
	release(w);
	//����x�����ã�֮����Ҫ����updateHeightAbove(x)�Ĳ����������Ҫ��x��������ָ��
	//�����޷�ȷ��succ�Ƿ�Ϊ�գ���˽�xָ��hot��
	//x = hot;
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

