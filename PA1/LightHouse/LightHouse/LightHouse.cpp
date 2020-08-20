// LightHouse.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

struct Node
{
	int _x, _y;
	long long _count;
	Node *_pred, *_succ;

	Node(int x = 0, int y = 0, Node* pred = NULL, Node* succ= NULL):
		_x(x), _y(y), _count(0), _pred(pred), _succ(succ)
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

	//Node *search(int x);
	void insertWithOrder(int x, int y);

	void countPair();
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

/*
Node *List::search(int x)
{
	//���ҷ���С��x�ĵ�һ��������Ϊx������죩��
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
*/


void List::insertWithOrder(int x, int y)
{
	Node *p = tailer;
	long long count = 0;
	
	//��Ҫ���������������Ԫ�ء�
	//����_x����x�ĵ�һ��Ԫ�ص�λ�á�
	while(p->_pred != header)
	{
		if(x < p->_pred->_x)
		{
			if(y < p->_pred->_y)
			{
				count++;
			}
			p = p->_pred;
		}
		else
		{
			break;
		}
	}
	p = p->insertAsPred(x, y);
	p->_count = count;

	_size++;

	while((p = p->_pred) != header)
	{
		if(p->_y < y)
		{
			p->_count++;
		}
	}

	//�㷨���󣬵�����{(3, 3), (2, 2), (1, 1)}��˳�����ʱ���ᵼ���޷�ͳ�ơ�
	/*
	//�ܷ���insert�Ĺ����о�ȷ���ò����������
	//��Ϊ��ǰ�����ң����ڽڵ������count������
	//ÿ�αȽ��ȱȽ�x���ٱȽ�y��
	Node *p = header->_succ;
	int count = header->_count;
	
	while(p != tailer)
	{
		if((p->_x < x) && (p->_y < y))
		{
			p->_count++;
		}

		//���ش���x�ĵ�һ��Ԫ�صĵ�ַ����Ϊx������죩��
		if(x < p->_succ->_x)
		{
			p = p->_succ;
			break;
		}
		p = p->_succ;
	}

	p = p->insertAsPred(x, y);
	p->_count = count;

	header->_count++;
	_size++;
	return p;
	*/

}

void List::countPair()
{	
	//����n^2ʱ�䣬̫����
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


	long long count = 0;
	
	timespec start, end;
	double time;
	timespec_get(&start, 1);
	
	Node *p = header->_succ;
	while(p != tailer)
	{
		count += p->_count;
		p = p->_succ;
	}
	
	timespec_get(&end, 1);
	time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
	cout << "time3: " << time << endl;
	
	printf("%lld\n", count);

}

int main()
{
	
	int n;
	cin >> n;
	
	/*
	int n = 100000;
	*/
	List locs;
	int x, y;
	/*
	timespec start, end;
	double time;
	
	ifstream fin("file.txt");
	*/


	for(int i = 0; i < n; i++)
	{
		
		cin >> x;
		cin >> y;
		
		/*
		fin >> x >> y;
		
		timespec_get(&start, 1);
		*/
		
		locs.insertWithOrder(x, y);	
		/*
		timespec_get(&end, 1);
		time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
		cout << i << "\ttime1: " << time << endl;
		*/
	}

	//timespec_get(&start, 1);

	locs.countPair();
	/*
	timespec_get(&end, 1);
	time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
	cout << "time2: " << time << endl;
	*/
}