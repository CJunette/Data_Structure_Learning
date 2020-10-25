#pragma once
#include "Vector.h"
#include <queue>
#define BTNodePosi(T) BTNode<T>*

template<typename T>
void release(T t)
{
	return;
}

template<typename T>
void release(T *t)
{
	delete t;
}

/*----------------------------------------------------------------------------*/

template<typename T>
struct BTNode
{
	BTNodePosi(T) _parent;
	Vector<T> key;
	Vector<BTNodePosi(T)> child;

	BTNode();
	BTNode(T e, BTNodePosi(T) lc = NULL, BTNodePosi(T) rc = NULL);
};

template<typename T>
BTNode<T>::BTNode()
{
	_parent = NULL;
	child.insert(0, NULL);
}

template<typename T>
BTNode<T>::BTNode(T e, BTNodePosi(T) lc, BTNodePosi(T) rc)
{
	_parent = NULL;
	key.insert(0, e);
	child.push_back(lc);
	child.push_back(rc);

	if(lc)
	{
		lc->_parent = this;
	}
	if(rc)
	{
		rc->_parent = this;
	}
}

/*----------------------------------------------------------------------------*/

template<typename T>
struct BTree
{
	//_order���������Ҳ����BTNode�еķ�֧������
	int _size, _order;
	BTNodePosi(T) _root;
	BTNodePosi(T) _hot;

	BTree(int order_ = 3);
	~BTree();
	BTNodePosi(T) search(const T &e);
	bool insert(const T &e);
	bool remove(const T &e);
	
	void show();

	protected:
	void solveOverflow(BTNodePosi(T) v);
	void solveUnderflow(BTNodePosi(T) v);
	void clear(BTNodePosi(T) v);
};

template<typename T>
BTree<T>::BTree(int order_):
	_size(0), _order(order_), _hot(NULL)
{
	_root = new BTNode<T>;
}

template<typename T>
void BTree<T>::clear(BTNodePosi(T) v)
{
	if(!v)
	{
		release(v);
		return;
	}	
	for(int i = 0; i < v->child.getSize(); i++)
	{
		clear(v->child[i++]);

	}
	return;
}

template<typename T>
BTree<T>::~BTree()
{
	clear(_root);
}

template<typename T>
BTNodePosi(T) BTree<T>::search(const T &e)
{
	BTNodePosi(T) v = _root;
	_hot = NULL;

	while(v)
	{
		int r = v->key.searchLinear(e);
		if(0 <= r && v->key[r] == e)
		{
			return v;
		}

		_hot = v;
		v = v->child[r + 1];
	}
	
	return v;
}

template<typename T>
bool BTree<T>::insert(const T &e)
{
	BTNodePosi(T) v = search(e);
	
	if(v)
	{
		return false;
	}

	int r = _hot->key.searchLinear(e);
	_hot->key.insert(r + 1, e);
	_hot->child.insert(r + 2, NULL);
	_size++;
	solveOverflow(_hot);
	return true;
}

template<typename T>
void BTree<T>::solveOverflow(BTNodePosi(T) v)
{
	if(v->child.getSize() <= _order)
	{
		return;
	}

	int s = _order / 2;

	//���ڵ���Ұ벿�����ݵ������������

	//ע�⣬rt������ʱ�ͻ��Դ�һ���պ��ӡ�
	BTNodePosi(T) rt = new BTNode<T>;

	for(int i = 0; i < _order - s - 1; i++)
	{		
		rt->key.insert(i, v->key.remove(s + 1));
		rt->child.insert(i, v->child.remove(s + 1));
	}
	rt->child[_order - s - 1] = v->child.remove(s + 1);


	//���rt��child�е�Ԫ�ز�ΪNULL����������ɵ������е�childԪ�أ�ȷ���µ�parent���������������Ҷ�ڵ�ķ��Ѵ��������Ƚڵ��solveOverflow�У���
	if(rt->child[0])
	{
		for(int i = 0; i < rt->child.getSize(); i++)
		{
			rt->child[i]->_parent = rt;
		}
	}	

	//ȷ����һ���ڵ㡣
	BTNodePosi(T) p = v->_parent;
	//�����һ���ڵ㲻���ڣ����´���һ�����������е�����v��ӵ����е�child�����С�
	if(!p)
	{
		p = new BTNode<T>;
		p->child[0] = v;
		v->_parent = p;
		_root = p;
	}

	//���м�λ�õ�Ԫ���ƶ�����һ���ڵ㣬���������ƶ����ϼ��ڵ��child�����С�
	rt->_parent = p;

	//��֮ǰ����������ʱ������p->child������v��������child�еĵ�ַ���ǰ���С���еģ����Ի���Ҫ�ùؼ������������
	int r = p->key.searchLinear(v->key[0]) + 1;
	p->key.insert(r, v->key.remove(s));
	p->child.insert(r + 1, rt);

	solveOverflow(p);
}

template<typename T>
bool BTree<T>::remove(const T &e)
{
	BTNodePosi(T) v = search(e);

	if(!v)
	{
		return false;
	}

	int r = v->key.searchLinear(e);
	//�����ǰ��v����һ��Ҷ�ڵ㣬��ô����ϣ����v[r]����ֱ�Ӻ�̵�Ҷ�ڵ��е�Ԫ�ؽ��н�����
	if(v->child[0])
	{
		BTNodePosi(T) u = v->child[r + 1];
		while(u->child[0])
		{
			u = u->child[0];
		}

		//��ʱ��v->key[r]��ָԪ����u->key[0]��ָԪ�ؽ��С�����������Ȼ�ǽ�����������v->key[r]��Ҫ��ɾ����Ԫ�أ����Ծ�����ֵ������Ҫ�����ֻҪ��u->key[0]�����������ɡ�
		v->key[r] = u->key[0];
		v = u;
		r = 0;
	}

	v->key.remove(r);
	v->child.remove(r + 1);
	_size--;
	solveUnderflow(v);
	return true;
}

template<typename T>
void BTree<T>::solveUnderflow(BTNodePosi(T) v)
{
	if((_order + 1) / 2 <= v->child.getSize())
	{
		return;
	}

	BTNodePosi(T) p = v->_parent;
	if(!p)
	{
		if(v->key.getSize() == 0 && v->child[0])
		{
			_root = v->child[0];
			_root->_parent = NULL;
			v->child[0] = NULL;
			release(v);
		}
		//���ڸ��ڵ��������ֻ��2�ף���1��Ԫ�أ����Գ��˸��ڵ�Ԫ��Ϊ0��������������������ֱ�ӷ��ء�
		return;
	}

	//���ﲻ����v->key[0]ȥ��������Ϊ���key�Ѿ���ɾ���ˡ�
	/*
	int r = p->key.searchLinear(v->key[0]) + 1;
	*/
	int r = 0;
	while(p->child[r] != v)
	{
		r++;
	}

	//��ת
	//���������ֵܣ����������ֵܽ�ʱ��
	if(0 < r)
	{
		BTNodePosi(T) ls = p->child[r - 1];
		if((_order + 1) / 2 < ls->child.getSize())
		{
			//��p�е�Ԫ��ת�Ƶ�v��ͷ����
			v->key.insert(0, p->key[r - 1]);
			//��ls��child�����һ��Ԫ���ƶ���v��child�Ŀ�ͷλ�á�
			v->child.insert(0, ls->child.remove(ls->child.getSize() - 1));			
			//��ls��key�����һ��Ԫ���ƶ���p�еĺ���λ�á�
			p->key[r - 1] = ls->key.remove(ls->key.getSize() - 1);
			if(v->child[0])			
			{
				v->child[0]->_parent = v;
			}
			//��ת����������ɽ�һ�������磬��˿���ֱ�ӷ��ء�
			return;
		}		
	}
	//���������ֵܣ����������ֵܽ�ʱ����������ֵܴ�С̫С����v��������ˣ�Ҳ����������������
	else if(r < p->child.getSize() - 1)
	{
		BTNodePosi(T) rs = p->child[r + 1];
		if((_order + 1) / 2 < rs->child.getSize())
		{			
			v->key.insert(v->key.getSize(), p->key[r]);
			v->child.insert(v->child.getSize(), rs->child.remove(0));
			p->key[r] = rs->key.remove(0);
			if(v->child[v->child.getSize() - 1])
			{
				v->child[v->child.getSize() - 1]->_parent = v;
			}
			return;
		}
	}

	//�ϲ�

	//�����ֵܺϲ�ʱ��
	if(0 < r)
	{
		BTNodePosi(T) ls = p->child[r - 1];

		//��p��key[r-1]Ԫ���ƶ���ls�С�
		ls->key.insert(ls->key.getSize(), p->key.remove(r - 1));
		//ɾ��p�е�child[r]��
		p->child.remove(r);

		//Ϊ�˷���������ѭ�����Ƚ�һ��v�е�child[0]�ƶ���ls��child�С�
		ls->child.insert(ls->child.getSize(), v->child.remove(0));
		if(ls->child[ls->child.getSize() - 1])
		{
			ls->child[ls->child.getSize() - 1]->_parent = ls;
		}

		//������v��key��child�ϲ���ls�С�
		while(!v->key.empty())
		{
			ls->key.insert(ls->key.getSize(), v->key.remove(0));
			ls->child.insert(ls->child.getSize(), v->child.remove(0));

			if(ls->child[ls->child.getSize() - 1])
			{
				ls->child[ls->child.getSize() - 1]->_parent = ls;
			}
		}
	}
	//�����ֵܺϲ�ʱ��
	else
	{
		BTNodePosi(T) rs = p->child[r + 1];

		//ע�⣬������p->key.remove(r)��������p->key.remove(r - 1)��ƴ���ǰ��մ�С��ϵ���ġ�
		rs->key.insert(0, p->key.remove(r));
		p->child.remove(r);

		rs->child.insert(0, v->child.remove(v->child.getSize() - 1));
		if(rs->child[0])
		{
			rs->child[0]->_parent = rs;
		}

		while(!v->key.empty())
		{
			rs->key.insert(0, v->key.remove(v->key.getSize() - 1));
			rs->child.insert(0, v->child.remove(v->child.getSize() - 1));

			if(rs->child[0])
			{
				rs->child[0]->_parent = rs;
			}
		}
	}
	release(v);
	//�ϲ��������ܴ�����һ�������硣
	solveUnderflow(p);
}

template<typename T>
void BTree<T>::show()
{
	std::queue<BTNodePosi(T)> q;
	BTNodePosi(T) v = _root;
	int i;
	for(i = 0; i < v->key.getSize(); i++)
	{
		std::cout << v->key[i] << " ";
		if(v->child[i])
		{
			q.push(v->child[i]);
		}
	}
	if(v->child[i])
	{
		q.push(v->child[i]);
	}

	std::cout << std::endl;
	int count = v->child.getSize();

	
	while(!q.empty())
	{
		int newCount = 0;
		for(int j = 0; j < count; j++)
		{
			v = q.front();
			q.pop();

			int i;
			for(i = 0; i < v->key.getSize(); i++)
			{
				std::cout << v->key[i] << " ";
				if(v->child[i])
				{
					q.push(v->child[i]);
				}
			}
			if(v->child[i])
			{
				q.push(v->child[i]);
			}

			std::cout << "\t";
			newCount += v->child.getSize();
		}		
		std::cout << std::endl;
		count = newCount;
	}	
}