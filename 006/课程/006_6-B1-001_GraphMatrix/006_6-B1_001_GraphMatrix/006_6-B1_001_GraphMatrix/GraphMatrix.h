#pragma once
#include "Stack.h"
#include "Vector.h"
#include "Queue.h"
#include <limits>
using namespace std;

enum VStatus
{
	UNDISCOVERED, DISCOVERED, VISITED
};

enum EType
{
	UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD
};

template<typename Tv, typename Te>
struct Graph
{
	protected:
	void reset();
	void BFS(int, int &);
	void DFS(int, int &);
	void DFS_RECURSION(int, int &);
	void BCC(int, int &, Stack<Tv> &, Queue<Tv> &);
	bool TSort(int, int &, Stack<Tv> *);
	template<typename PU> void PFS(int, PU);

	public:
	//�����numOfVertexes�������edges�ֱ��Ӧ��ǰ��n��e��
	//֮������ô������Ϊģ����̳�ʱ������ģ���޷�ֱ�Ӷ�ȡ��ģ���е����ݳ�Ա��������ģ�����ֲ��ܳ������ģ��������һ�������ݳ�Ա��
	//��ˣ�Ϊ�˱���������ķ��ʣ�����ģ���еĲ������������޸ġ�
	//nΪ����������
	int numOfVertexes;

	virtual int insert(Tv const &) = 0;
	virtual Tv remove(int) = 0;
	virtual Tv &vertex(int) = 0;
	virtual int inDegree(int) = 0;
	virtual int outDegree(int) = 0;
	virtual int firstNbr(int) = 0;
	virtual int nextNbr(int, int) = 0;
	virtual VStatus &status(int) = 0;
	virtual int &dTime(int) = 0;
	virtual int &fTime(int) = 0;
	virtual int &parent(int) = 0;
	virtual int &priority(int) = 0;

	//eΪ��������
	int edges;

	virtual bool exist(int, int) = 0;
	virtual void insert(Te const &, int, int, int) = 0;
	virtual Te remove(int, int) = 0;
	virtual EType &type(int, int) = 0;
	virtual Te &edge(int, int) = 0;
	virtual int &weight(int, int) = 0;

	Graph(int n_ = 0, int e_ = 0);

	void bfs(int);
	void dfs(int);
	void dfs_recursion(int);
	void bcc(int);
	Stack<Tv> *tSort(int);
	void prim(int);
	void dijkstra(int);
	template <typename PU> void pfs(int, PU);
};

template<typename Tv, typename Te>
Graph<Tv, Te>::Graph(int n_, int e_):
	numOfVertexes(n_), edges(e_)
{}

template<typename Tv, typename Te>
void Graph<Tv, Te>::reset()
{
	for(int i = 0; i < numOfVertexes; i++)
	{
		status(i) = UNDISCOVERED;
		dTime(i) = -1;
		fTime(i) = -1;
		parent(i) = -1;
		priority(i) = INT_MAX;
		for(int j = 0; j < numOfVertexes; j++)
		{
			if(exist(i, j))
			{
				type(i, j) = UNDETERMINED;
			}
		}
	}
}

template<typename Tv, typename Te>
void Graph<Tv, Te>::BFS(int v, int &clock)
{
	Queue<int> q;
	q.enqueue(v);
	status(v) = DISCOVERED;

	while(!q.empty())
	{
		v = q.dequeue();
		dTime(v) = ++clock;
		for(int u = firstNbr(v); -1 < u; u = nextNbr(v, u))
		{
			if(status(u) == UNDISCOVERED)
			{
				q.enqueue(u);
				status(u) = DISCOVERED;
				parent(u) = v;
				type(v, u) = TREE;
			}
			else
			{
				type(v, u) = CROSS;
			}
		}
		//ע�⣬���һ��ѭ����Ҫ�ѵ�ǰ���ӵ�Ԫ�ظ�ΪVISITED״̬��
		status(v) = VISITED;
	}
}

template<typename Tv, typename Te>
void Graph<Tv, Te>::bfs(int s)
{
	//��ͷ��Ҫ����reset()��
	reset();
	int clock = 0;
	int v = s;
	//��ͼ�д��ڶ����ͨ���Ҳ�ͬ��ͨ��䲻һ������ʱ����Ҫ��v��n�ķ�Χ�ڱ�����
	//һ������UNDISCOVERED�Ķ��㣬��ִ�й���������������ҵ��������������ж��㡣��
	do
	{
		if(status(v) == UNDISCOVERED)
		{
			BFS(v, clock);
		}
		v = (++v) % numOfVertexes;
	} while(s != v);
}

template<typename Tv, typename Te>
void Graph<Tv, Te>::DFS(int v, int &clock)
{
	status(v) = DISCOVERED;
	dTime(v) = ++clock;

	for(int u = firstNbr(v); -1 < u; u = nextNbr(v, u))
	{
		switch(status(u))
		{
			case UNDISCOVERED:
				parent(u) = v;
				type(v, u) = TREE;
				DFS(u, clock);
				break;
			case DISCOVERED:
				type(v, u) = BACKWARD;
				break;
			case VISITED:
				type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
				break;
			default:
				break;
		}
	}
	fTime(v) = ++clock;
	status(v) = VISITED;
}

template<typename Tv, typename Te>
void Graph<Tv, Te>::dfs(int s)
{
	reset();
	int v = s;
	int clock = 0;
	do
	{
		if(status(v) == UNDISCOVERED)
		{
			DFS(v, clock);
		}
		v = ++v % numOfVertexes;
	} while(s != v);
}

template<typename Tv, typename Te>
void Graph<Tv, Te>::DFS_RECURSION(int v, int &clock)
{
	Stack<int> s;
	dTime(s) = ++clock;
	status(v) = DISCOVERED;
	s.push(v);
	while(!s.empty())
	{
		int w = s.pop();
		for(int u = firstNbr(w); -1 < u; u = nextNbr(w, u))
		{
			if(status(u) == UNDISCOVERED)
			{
				s.push(w);
				s.push(u);
				parent(u) = w;
				dTime(u) = ++clock;
				type(w, u) = TREE;
				status(v) = DISCOVERED;
				break;
			}
			else if(status(u) == DISCOVERED)
			{
				type(w, u) = BACKWARD;
			}
			else if(status(u) == VISITED)
			{
				type(w, u) = dTime(u) < dTime(w) ? CROSS : FORWARD;
			}
		}
		status(w) = VISITED;
		fTime(w) = ++clock;
	}
}

template<typename Tv, typename Te>
void Graph<Tv, Te>::dfs_recursion(int s)
{
	reset();
	int v = s;
	int clock = 0;
	do
	{
		if(status(v) == UNDISCOVERED)
		{
			DFS(v, clock);
		}
		v = ++v % numOfVertexes;
	} while(s != v);
}

template<typename Tv, typename Te>
bool Graph<Tv, Te>::TSort(int v, int &clock, Stack<Tv> *stack)
{
	dTime(v) = ++clock;
	status(v) = DISCOVERED;

	/*
	if(outDegree(v) == 0)
	{
		stack->push(vertex(v));
		fTime(v) = ++clock;
		status(v) = VISITED;
		return true;
	}
	*/

	for(int u = firstNbr(v); -1 < u; u = nextNbr(v, u))
	{
		switch(status(u))
		{
			case UNDISCOVERED:
				parent(u) = v;
				type(v, u) = TREE;
				if(!TSort(u, clock, stack))
				{
					return false;
				}
				break;

			case DISCOVERED:
				type(v, u) = BACKWARD;
				return false;

			case VISITED:
				type(v, u) = CROSS;
				break;
			default:
				return false;
				break;
		}
	}

	stack->push(vertex(v));
	fTime(v) = ++clock;
	status(v) = VISITED;
	return true;
}

template<typename Tv, typename Te>
Stack<Tv> *Graph<Tv, Te>::tSort(int s)
{
	//�ʼ������reset()��
	reset();
	Stack<Tv> *stack = new Stack<Tv>;
	int clock = 0;
	int v = s;

	do
	{
		if(status(v) == UNDISCOVERED)
		{
			//����һ��ͨ���д��ڻ�·�������ջ��
			if(!TSort(v, clock, stack))
			{
				while(!stack->empty())
				{
					stack->pop();
				}
				break;
			}
		}
		v = (++v) % numOfVertexes;
	} while(s != v);

	return stack;
}

template<typename Tv, typename Te>
void Graph<Tv, Te>::BCC(int v, int &clock, Stack<Tv> &stack, Queue<Tv> &q)
{
	dTime(v) = ++clock;
	//fTime��ʱ������¼��ǰ�ڵ��ǰһ���ؽڵ����ڵ�λ�á�������ڷ���·�����򷵻�·����ָ����յ�����Ǹ��ؽڵ㡣
	fTime(v) = clock;
	status(v) = DISCOVERED;
	stack.push(vertex(v));

	for(int u = firstNbr(v); -1 < u; u = nextNbr(v, u))
	{
		switch(status(u))
		{
			case UNDISCOVERED:
				parent(u) = v;
				type(v, u) = TREE;
				BCC(u, clock, stack, q);
				//����ڽӶ����fTimeС�ڵ�ǰ�����fTime��˵���ڽӶ�����ڽӶ���Ĺ��ɵ������б�Ȼ�����˷���·�����ҷ��ض����ڵ�ǰ����֮ǰ��
				//��ʱ����ǰ�����fTimeҲ����Ϊ���ض����fTime������ǰ����ġ��ؽڵ㡱ҲӦ�����Ǹ����ؽڵ㴦��
				if(fTime(u) < fTime(v))
				{
					fTime(v) = fTime(u);
				}
				//����˵����ǰ������ڽӶ���ݹ��в����ڷ���·��������֮����ǰ�����Ӧ�������У��ؽڵ㻹���ڵ�ǰ�ڵ㡣��ˣ����ջ�еĵ�ǰ�����Ϸ�����Щ�ڽӶ��㡣
				else
				{
					//�������ϵĴ���Ƚϼ򵥡�������Ϊ�˽����������ֱ�ۣ�������΢�޸���һ�����뷽ʽ��
					/*
					while(vertex(v) != stack.pop())
					{}
					stack.push(vertex(v));
					*/
					while(vertex(v) != stack.top())
					{
						q.enqueue(stack.pop());
					}
					q.enqueue(stack.pop());
					q.enqueue('|');
					stack.push(vertex(v));
				}
				break;
			case DISCOVERED:
				//���ڽӶ���ָ��DISCOVERED����ʱ��˵����Ȼ�����˷���·����
				type(v, u) = BACKWARD;
				if(u != parent(v))
				{
					//�����ǿ��ܴ���fTime(v) < dTime(u)������ġ����ڴ��ڶ�������·����ʱ�򣬵�һ������·���ķ��ص�ϸߣ��ڶ�������·���ķ��ص�ϵ͡�
					//��ʱ��ϣ������ϸߵ��Ǹ��㣬��Ϊ�ؽڵ㡣
					fTime(v) = fTime(v) < dTime(u) ? fTime(v) : dTime(u);
				}
				break;
			case VISITED:
				//����VISITED��������ǿ��ǵ��ص㣬�������Ͳ���ͨ��fTime�Ĵ�С�������ˡ�
				type(v, u) = CROSS;
				break;
		}
	}
	//���Ҫ���ǰ�״̬��ΪVISITED��
	status(v) = VISITED;
}

template<typename Tv, typename Te>
void Graph<Tv, Te>::bcc(int s)
{
	Stack<Tv> stack;
	int clock = 0;
	int v = s;
	//�����Ҷ����һ��Queue��������¼˫��ͨ��
	Queue<Tv> q;

	do
	{
		if(status(v) == UNDISCOVERED)
		{
			BCC(s, clock, stack, q);
			//��ɺ�stack�л�������һ��Ԫ�ء����䵯����
			stack.pop();
		}
		v = (++v) % numOfVertexes;
	} while(v != s);

	while(!q.empty())
	{
		cout << q.dequeue() << " ";
	}
	cout << endl;
}

template<typename Tv, typename Te>
template<typename PU>
void Graph<Tv, Te>::PFS(int v, PU prioUpdater)
{
	status(v) = VISITED;
	parent(v) = -1;
	//����ԽС�����ȼ�Խ��
	priority(v) = 0;

	while(true)
	{
		for(int u = firstNbr(v); -1 < u; u = nextNbr(v, u))
		{
			prioUpdater(this, v, u);
		}

		for(int u = 0, shortest = INT_MAX; u < numOfVertexes; u++)
		{
			if(status(u) == UNDISCOVERED)
			{
				//ÿ�δ�UNDISCOVERED�Ķ����У�ѡ��Ȩ����С�����ȼ���󣩵���һ����
				if(shortest > priority(u))
				{
					shortest = priority(u);
					v = u;
				}
			}
		}
		if(status(v) == VISITED)
		{
			break;
		}
		status(v) = VISITED;
		type(parent(v), v) = TREE;
	}
}

template<typename Tv, typename Te>
template<typename PU>
void Graph<Tv, Te>::pfs(int s, PU prioUpdater)
{
	reset();
	int v = s;
	do
	{
		if(status(v) == UNDISCOVERED)
		{
			PFS(v, prioUpdater);
		}
		v = ++v % numOfVertexes;
	} while(s != v);
}

/*---------------------------------------------------------------------*/

template<typename Tv>
struct Vertex
{
	Tv data;
	int inDegree, outDegree;
	VStatus status;
	//dTime����ʾ�����ֵ�ʱ�䣻fTime����ʾ������ϵ�ʱ�䡣
	int dTime, fTime;
	int parent;
	int priority;

	Vertex(Tv const &d = 0);
	Tv &getData();
};

template<typename Tv>
Vertex<Tv>::Vertex(Tv const &d):
	data(d), inDegree(0), outDegree(0), status(UNDISCOVERED), dTime(-1), fTime(-1), parent(-1), priority(INT_MAX)
{}

template<typename Tv>
Tv &Vertex<Tv>::getData()
{
	return data;
}

/*---------------------------------------------------------------------*/

template<typename Te>
struct Edge
{
	Te data;
	int weight;
	EType type;

	Edge(Te const &d = 0, int w = 0);
	Te &getData();
};

template<typename Te>
Edge<Te>::Edge(Te const &d, int w):
	data(d), weight(w), type(UNDETERMINED)
{}

template<typename Te>
Te &Edge<Te>::getData()
{
	return data;
}

/*---------------------------------------------------------------------*/

template<typename Tv, typename Te>
struct GraphMatrix: public Graph<Tv, Te>
{
	Vector<Vertex<Tv>> V;
	//E���൱���ڽӾ���ÿһ�д����Ӧ�ȵĶ���ָ����������ıߡ�
	Vector<Vector<Edge<Te> *>> E;

	//�����n��e�ֱ��Ӧ�����е�numOfVertexes��edges��ע�⣬������Ҫʹ�����ã�ʹ������������ݳ�Ա�ڶ�ֵ�����޸�ʱ���޸��ܴ��ݻػ�ģ������ݳ�Ա��
	int &n = this->numOfVertexes;
	int &e = this->edges;

	GraphMatrix();
	~GraphMatrix();

	void show();
	virtual int insert(Tv const &data);
	virtual Tv remove(int  index);
	virtual Tv &vertex(int index);
	virtual int inDegree(int index);
	virtual int outDegree(int index);
	virtual int firstNbr(int index);
	//nextNbr�ǴӺ���ǰѰ�ҵġ������Ǵ�E[i][j]��ʼ��ǰ������������j����Ѱ�ҵ�һ��ֵΪ�ǿյ�Ԫ�ء�����i�ڵ�j֮ǰ�ġ���һ���������ӵĵ㡣
	virtual int nextNbr(int i, int j);
	virtual VStatus &status(int index);
	virtual int &dTime(int index);
	virtual int &fTime(int index);
	virtual int &parent(int index);
	virtual int &priority(int index);

	virtual bool exist(int i, int j);
	virtual void insert(Te const &data, int weight, int i, int j);
	virtual Te remove(int i, int j);
	virtual EType &type(int i, int j);
	virtual Te &edge(int i, int j);
	virtual int &weight(int i, int j);

	void TSortUsingIndegree(int v, Vector<Vector<Edge<Te> *>> &vtr);
	void tSortUsingIndegree(int v);
};

template<typename Tv, typename Te>
GraphMatrix<Tv, Te>::GraphMatrix():
	Graph<Tv, Te>(0, 0)
{
}

template<typename Tv, typename Te>
GraphMatrix<Tv, Te>::~GraphMatrix()
{
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			delete E[i][j];
		}
	}
}

template<typename Tv, typename Te>
void GraphMatrix<Tv, Te>::show()
{
	//������С�
	cout << "\t\t";
	for(int i = 0; i < n; i++)
	{
		cout << V[i].data << "\t\t";
	}
	cout << endl;
	cout << "\t\t";
	for(int i = 0; i < n; i++)
	{
		cout << "dTime: " << V[i].dTime << "\t";
	}
	cout << endl;
	cout << "\t\t";
	for(int i = 0; i < n; i++)
	{
		cout << "fTime: " << V[i].fTime << "\t";
	}
	cout << endl;
	cout << "\t\t";
	for(int i = 0; i < n; i++)
	{
		int viParent = V[i].parent;
		if(viParent != -1)
		{
			cout << "parent: " << V[viParent].data << "\t";
		}
		else
		{
			cout << "parent: " << "-1" << "\t";
		}
	}
	cout << endl;
	cout << endl;

	//������и��С�
	for(int i = 0; i < n; i++)
	{
		cout << V[i].data << "\t\t";
		for(int j = 0; j < n; j++)
		{
			if(E[i][j])
			{
				cout << E[i][j]->data << "\t\t";
			}
			else
			{
				cout << "NULL" << "\t\t";
			}
		}
		cout << endl;

		cout << "weight:\t\t";
		for(int j = 0; j < n; j++)
		{
			if(E[i][j])
			{
				cout << E[i][j]->weight << "\t\t";
			}
			else
			{
				cout << "---\t\t";
			}
		}
		cout << endl;

		cout << "type:\t\t";
		for(int j = 0; j < n; j++)
		{
			if(E[i][j])
			{
				switch(E[i][j]->type)
				{
					case UNDETERMINED:
						cout << "UNDETERMINED" << "\t";
						break;
					case TREE:
						cout << "TREE" << "\t\t";
						break;
					case CROSS:
						cout << "CROSS" << "\t\t";
						break;
					case FORWARD:
						cout << "FORWARD" << "\t\t";
						break;
					case BACKWARD:
						cout << "BACKWARD" << "\t";
						break;
					default:
						break;
				}

			}
			else
			{
				cout << "---\t\t";
			}
		}
		cout << endl;
		cout << endl;
	}

}

template<typename Tv, typename Te>
int GraphMatrix<Tv, Te>::insert(Tv const &d)
{
	//����ǰ��ÿ��vector�ĺ������һ�С�
	for(int i = 0; i < n; i++)
	{
		E[i].push_back(NULL);
	}
	//�����ܵ�����
	n++;

	//��E�������һ�С�
	Vector<Edge<Te> *> vectorEdge(n, n, NULL);
	E.push_back(vectorEdge);

	//��V�����һ��Ԫ�ء�
	V.push_back(Vertex<Tv>(d));

	return n;
}

template<typename Tv, typename Te>
Tv GraphMatrix<Tv, Te>::remove(int index)
{
	Tv temp = V[index].data;

	//�������Լ�д��������ppt�е��������в�ͬ����˼·һ�¡�
	for(int i = 0; i < n; i++)
	{
		if(exist(index, i))
		{
			e--;
			V[i].inDegree--;
			delete E[index][i];
		}
	}

	for(int i = 0; i < n; i++)
	{
		if(exist(i, index))
		{
			e--;
			V[i].outDegree--;
			delete E[i][index];
		}
		E[i].remove(index);
	}

	n--;
	E.remove(index);
	V.remove(index);

	return temp;

	//������ppt�и�����ԭ���㷨��
	/*
	for(int i = 0; i < n; i++)
	{
		if(exist(index, i))
		{
			delete E[index][i];
			V[i].inDegree--;
			e--;
		}
	}
	E.remove(i);
	n--;
	Tv vBak = vertex(i);
	v.remove(i);
	for(int i = 0; i < n; i++)
	{
		if(Edge<Te> *x = E[i].remove(index))
		{
			delete x;
			V[i].outDegree--;
			e--;
		}
	}
	return vBak;
	*/
}

template<typename Tv, typename Te>
Tv &GraphMatrix<Tv, Te>::vertex(int index)
{
	return V[index].data;
}

template<typename Tv, typename Te>
int GraphMatrix<Tv, Te>::inDegree(int index)
{
	return V[index].inDegree;
}

template<typename Tv, typename Te>
int GraphMatrix<Tv, Te>::outDegree(int index)
{
	return V[index].outDegree;
}

template<typename Tv, typename Te>
int GraphMatrix<Tv, Te>::firstNbr(int index)
{
	return nextNbr(index, n);
}

template<typename Tv, typename Te>
int GraphMatrix<Tv, Te>::nextNbr(int i, int j)
{
	--j;
	while((-1 < j) && !exist(i, j))
	{
		j--;
	}
	return j;
}

template<typename Tv, typename Te>
VStatus &GraphMatrix<Tv, Te>::status(int index)
{
	return V[index].status;
}

template<typename Tv, typename Te>
int &GraphMatrix<Tv, Te>::dTime(int index)
{
	return V[index].dTime;
}

template<typename Tv, typename Te>
int &GraphMatrix<Tv, Te>::fTime(int index)
{
	return V[index].fTime;
}

template<typename Tv, typename Te>
int &GraphMatrix<Tv, Te>::parent(int index)
{
	return V[index].parent;
}

template<typename Tv, typename Te>
int &GraphMatrix<Tv, Te>::priority(int index)
{
	return V[index].priority;
}

template<typename Tv, typename Te>
bool GraphMatrix<Tv, Te>::exist(int i, int j)
{
	return (0 <= i) && (i < n) && (0 <= j) && (j < n) && (E[i][j] != NULL);
}

template<typename Tv, typename Te>
void GraphMatrix<Tv, Te>::insert(Te const &data, int weight, int i, int j)
{
	if(exist(i, j))
	{
		return;
	}
	Edge<Te> *x = new Edge<Te>(data, weight);
	E[i][j] = x;
	//��Ҫ���Ƕ�e��outDegree��inDegree����ӡ�
	e++;
	V[i].outDegree++;
	V[j].inDegree++;

}

template<typename Tv, typename Te>
Te GraphMatrix<Tv, Te>::remove(int i, int j)
{
	if(!exist(i, j))
	{
		return 0;
	}
	Te temp = E[i][j]->data;
	delete E[i][j];
	//�ǵ�delete��Ѷ�ӦԪ�ػ�ԭΪNULL��
	E[i][j] = NULL;

	e--;
	V[i].outDegree--;
	V[j].inDegree--;

	return temp;
}

template<typename Tv, typename Te>
EType &GraphMatrix<Tv, Te>::type(int i, int j)
{
	return E[i][j]->type;
}

template<typename Tv, typename Te>
Te &GraphMatrix<Tv, Te>::edge(int i, int j)
{
	return E[i][j]->data;
}

template<typename Tv, typename Te>
int &GraphMatrix<Tv, Te>::weight(int i, int j)
{
	return E[i][j]->weight;
}

template<typename Tv, typename Te>
void GraphMatrix<Tv, Te>::TSortUsingIndegree(int v, Vector<Vector<Edge<Te> *>> &vtr)
{

}

template<typename Tv, typename Te>
void GraphMatrix<Tv, Te>::tSortUsingIndegree(int s)
{
	this->reset();
	Queue<char> q;
	int v = s;

	do
	{
		int i = 0;
		int check = 0;
		while(i < n)
		{
			if(V[i].inDegree == 0)
			{
				q.enqueue(V[i].getData());
				remove(i);
				i--;
				check++;
			}
			i++;
		}
		if(!check)
		{
			cout << "Cyclic Route Exists!" << endl;
			return;
		}
	} while(0 < n);

	while(!q.empty())
	{
		cout << q.dequeue() << endl;
	}
}