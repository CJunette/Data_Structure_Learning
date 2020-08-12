// LightHouse.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ctime>
using namespace std;

struct Node
{
	int _x, _y;
	Node *_pred, *_succ;

	Node(int x = 0, int y = 0, Node* pred = NULL, Node* succ= NULL):
		_x(x), _y(y), _pred(pred), _succ(succ)
	{}

	Node *insertAsPred(int x, int y)
	{
		Node *p = new Node(x, y, _pred, this);
		_pred->_succ = p;
		_pred = p;
		return p;
	}

	Node *insertAsSucc(int x, int y)
	{
		Node *p = new Node(x, y, this, _succ);
		_succ->_pred = p;
		_succ = p;
		return p;
	}
};

struct List
{
	int _size;
	Node *header, *tailer;

	List();
	~List();
	void remove(Node* p);

	Node *search(int x);
	Node *insertWithOrder(int x, int y);

	long long countPair();
};

List::List()
{
	_size = 0;
	header = new Node(0, 0, NULL, NULL);
	tailer = new Node(0, 0, NULL, NULL);
	header->_succ = tailer;
	tailer->_pred = header;
}

List::~List()
{
	while(header->_succ != tailer)
	{
		remove(header->_succ);
	}
	delete header;
	delete tailer;
}

void List::remove(Node *p)
{
	p->_pred->_succ = p->_succ;
	p->_succ->_pred = p->_pred;
	delete p;
	_size--;
}

Node *List::search(int x)
{
	//查找返回小于x的第一个数（因为x间均互异）。
	Node *p = tailer->_pred;
	while(p != header)
	{
		if(x < p->_x)
		{
			p = p->_pred;
		}
		else
		{
			break;
		}
	}	
	return p;
}

//能否在insert的过程中就确定好插入的数量？
//改为从前向后查找，并在节点中添加count参数。
//每次比较先比较x，再比较y。
Node *List::insertWithOrder(int x, int y)
{
	Node *p = search(x);
	p->insertAsSucc(x, y);
	_size++;
	return p;
}

long long List::countPair()
{
	long long count = 0;
	//花费n^2时间，太慢。
	/*
	Node *p = header->_succ;
	while(p != tailer)
	{
		Node *q = p->_succ;
		while(q != tailer)
		{
			if(p->_y < q->_y)
			{
				count++;
			}
			q = q->_succ;
		}
		p = p->_succ;
	}*/


	return count;
}

int main()
{
	int n;
	cin >> n;

	List locs;

	int x, y;

	timespec start, end;
	double time;

	for(int i = 0; i < n; i++)
	{
		cin >> x;
		cin >> y;

		timespec_get(&start, 1);

		locs.insertWithOrder(x, y);	

		timespec_get(&end, 1);
		time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
		cout << "time1: " << time << endl;
	}

	timespec_get(&start, 1);

	cout << locs.countPair() << endl;

	timespec_get(&end, 1);
	time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
	cout << "time2: " << time << endl;
}