#pragma once

#include "List.h"
#include "Entry.h"
#include "Quadlist.h"
#include "Dictionary.h"
#include <iostream>

template<typename K, typename V>
//我个人十分不认同这里让List变成基类的做法。按正常思路应该是吧List变成一个数据成员。
struct Skiplist: public Dictionary<K, V>, public List<Quadlist<Entry<K, V>>*>
{
	protected:
	bool skipSearch(Node<Quadlist<Entry<K, V>> *> *&qlist, QuadlistNode<Entry<K, V>> *&p, K &k);

	public:
	int size() { return this->empty() ? 0 : this->last()->data()->size(); }
	//level()反映层高。
	int level() { return List<Quadlist<Entry<K, V>> *>::size(); }
	bool put(K, V);
	V *get(K k);
	bool remove(K k);
	void show();
};

template<typename K, typename V>
bool Skiplist<K, V>::skipSearch(Node<Quadlist<Entry<K, V>> *> *&qlist, QuadlistNode<Entry<K, V>> *&p, K &k)
{
	while(true)
	{
		while(p->_succ && p->_entry._key <= k)
		{
			p = p->_succ;
		}
		p = p->_pred;
		if(p->_pred && p->_entry._key == k)
		{
			return true;
		}
		else
		{
			if(qlist->succ() == this->last()->succ())
			{
				return false;
			}
			qlist = qlist->succ();
			p = p->_pred ? p->_below : qlist->data()->first();
		}
	}
}

template<typename K, typename V>
bool Skiplist<K, V>::put(K k, V v)
{
	Entry<K, V> e = Entry<K, V>(k, v);

	if(this->empty())
	{
		this->insertAsFirst(new Quadlist<Entry<K, V>>);
	}

	Node<Quadlist<Entry<K, V>> *> *qlist = this->first();
	QuadlistNode<Entry<K, V>> *p = qlist->data()->first();

	if(skipSearch(qlist, p, k))
	{
		while(p->_below)
		{
			p = p->_below;
			qlist = qlist->succ();
		}
	}

	//直接写p->insertAsSuccAbove()不好吗？
	QuadlistNode<Entry<K, V>> *b = qlist->data()->insertAfterAbove(e, p);

	while(rand() & 1)
	{
		while(qlist->data()->valid(p) && !p->_above)
		{
			p = p->_pred;
		}

		if(!qlist->data()->valid(p))
		{
			if(qlist == this->first())
			{
				this->insertAsFirst(new Quadlist<Entry<K, V>>);
			}
			p = qlist->pred()->data()->first()->_pred;
		}
		else
		{
			p = p->_above;
		}
		qlist = qlist->pred();
		b = qlist->data()->insertAfterAbove(e, p, b);
	}

	return true;
}


template<typename K, typename V>
V *Skiplist<K, V>::get(K k)
{
	if(this->empty())
	{
		return NULL;
	}
	Node<Quadlist<Entry<K, V>> *> *qlist = this->first();
	QuadlistNode<Entry<K, V>> *p = qlist->data()->first();
	return skipSearch(qlist, p, k) ? &p->_entry._value : NULL;	
}

template<typename K, typename V>
bool Skiplist<K, V>::remove(K k)
{
	if(this->empty())
	{
		return false;
	}

	Node<Quadlist<Entry<K, V>> *> *qlist = this->first();
	QuadlistNode<Entry<K, V>> *p = qlist->data()->first();

	if(!skipSearch(qlist, p, k))
	{
		return false;
	}

	do
	{
		QuadlistNode<Entry<K, V>> *next = p->_below;
		qlist->data()->remove(p);
		p = next;
		qlist = qlist->succ();

	} while(qlist != this->last()->succ());

	while(!this->empty() && this->first()->data()->empty())
	{
		List<Quadlist<Entry<K, V>> *>::remove(this->first());
	}
	return true;
}

template<typename K, typename V>
void Skiplist<K, V>::show()
{
	if(size() == 0)
	{
		return;
	}
	Node<Quadlist<Entry<K, V>> *> *qlist = this->last();
	QuadlistNode<Entry<K, V>> *p = qlist->data()->first();
	int *hasSucc = new int[size()];
	for(int i = 0; i < size(); i++)
	{
		hasSucc[i] = 1;
	}

	std::cout << "输出为从底到顶，从右到左（即上下左右都颠倒）：" << std::endl;

	while(qlist != this->first()->pred())
	{
		int i = 0;
		QuadlistNode<Entry<K, V>> *last = qlist->data()->last()->_succ;

		while(i < size())
		{
			if(hasSucc[i])
			{
				std::cout << p->_entry._value << "\t";

				if(p->_above)
				{
					hasSucc[i] = 1;
				}
				else
				{
					hasSucc[i] = 0;
				}

				p = p->_succ;
			}
			else
			{
				hasSucc[i] = 0;
				std::cout << "-" << "\t";
			}

			i++;
		}
		std::cout << std::endl;
		qlist = qlist->pred();
		if(qlist->data())
		{
			p = qlist->data()->first();
		}
	}
	delete[] hasSucc;
}