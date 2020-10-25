#pragma once
#include "Entry.h"
#include <stdexcept>

#define QlistNodePosi(T) QuadlistNode<T>*

template<typename T>
struct QuadlistNode
{
	T _entry;
	QlistNodePosi(T) _pred;
	QlistNodePosi(T) _succ;
	QlistNodePosi(T) _above;
	QlistNodePosi(T) _below;
	
	QuadlistNode(T e = T(), QlistNodePosi(T) pred_ = NULL, QlistNodePosi(T) succ_ = NULL, QlistNodePosi(T) above_ = NULL, QlistNodePosi(T) below_ = NULL):
		_entry(e), _pred(pred_), _succ(succ_), _above(above_), _below(below_)	{}

	QlistNodePosi(T) insertAsSuccAbove(T const &e, QlistNodePosi(T) b = NULL);
};

template<typename T>
QlistNodePosi(T) QuadlistNode<T>::insertAsSuccAbove(T const &e, QlistNodePosi(T) b)
{
	QlistNodePosi(T) n = new QuadlistNode<T>(e, this, _succ, NULL, b);
	if(_succ)
	{
		_succ->_pred = n;
	}
	this->_succ = n;
	if(b)
	{
		b->_above = n;
	}
	return n;
}

/*----------------------------------------------------------------------------------------*/

template<typename T>
struct Quadlist
{
	private:
	int _size;
	QlistNodePosi(T) _header;
	QlistNodePosi(T) _trailer;

	protected:
	void init();
	int clear();

	public:
	Quadlist() { init(); }
	~Quadlist() { clear(); delete _header; delete _trailer; }
	int size() { return _size; }
	bool empty() { return _size == 0; }
	QlistNodePosi(T) first() const { return _header->_succ; }
	QlistNodePosi(T) last() const { return _trailer->_pred; }
	bool valid(QlistNodePosi(T) p) { return p && (p != _header) && (p != _trailer); }
	T remove(QlistNodePosi(T) p);
	QlistNodePosi(T) insertAfterAbove(T const &e, QlistNodePosi(T) p, QlistNodePosi(T) b = NULL);

	void traverse(void(*) (T &e));
	template<typename VST>
	void traverse(VST &);
};

template<typename T>
void Quadlist<T>::init()
{
	_header = new QuadlistNode<T>(0);
	_trailer = new QuadlistNode<T>(0);

	_header->_succ = _trailer;
	_trailer->_pred = _header;
	_size = 0;
}

template<typename T>
int Quadlist<T>::clear()
{
	int oldsize = _size;
	while(0 < _size)
	{
		remove(_header->_succ);
	}
	return oldsize;
}

template<typename T>
T Quadlist<T>::remove(QlistNodePosi(T) p)
{
	if(valid(p))
	{
		T temp = p->_entry;
		p->_pred->_succ = p->_succ;
		p->_succ->_pred = p->_pred;
		_size--;
		delete p;
		return temp;
	}
	else
	{
		throw std::out_of_range("OutOfRange");
	}
}

template<typename T>
QlistNodePosi(T) Quadlist<T>::insertAfterAbove(T const &e, QlistNodePosi(T) p, QlistNodePosi(T) b)
{
	_size++;
	return p->insertAsSuccAbove(e, b);
}

template<typename T>
void Quadlist<T>::traverse(void(*e)(T &))
{
	QlistNodePosi(T) p = _header->_succ;
	while(p != _trailer)
	{
		e(p->_entry);
	}
}

template<typename T>
template<typename VST>
void Quadlist<T>::traverse(VST& visit)
{
	QlistNodePosi(T) p = _header->_succ;
	while(p != _trailer)
	{
		visit(p->_entry);
	}
}