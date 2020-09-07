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
	//这里的numOfVertexes和下面的edges分别对应以前的n和e。
	//之所以这么改是因为模板类继承时，派生模板无法直接读取基模板中的数据成员；但派生模板中又不能出现与基模板中名称一样的数据成员。
	//因此，为了便于派生类的访问，将基模板中的参数名都进行修改。
	//n为顶点总数。
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

	//e为边总数。
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
		//注意，完成一轮循环后要把当前出队的元素改为VISITED状态。
		status(v) = VISITED;
	}
}

template<typename Tv, typename Te>
void Graph<Tv, Te>::bfs(int s)
{
	//开头不要忘记reset()。
	reset();
	int clock = 0;
	int v = s;
	//若图中存在多个连通域，且不同连通域间不一定相连时，需要让v在n的范围内遍历。
	//一旦遇到UNDISCOVERED的顶点，就执行广度优先搜索，查找到与其相连的所有顶点。。
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
	//最开始别忘记reset()。
	reset();
	Stack<Tv> *stack = new Stack<Tv>;
	int clock = 0;
	int v = s;

	do
	{
		if(status(v) == UNDISCOVERED)
		{
			//若任一连通域中存在环路，则清空栈。
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
	//fTime此时用来记录当前节点的前一个关节点所在的位置。如果存在返回路径，则返回路径所指向的终点就是那个关节点。
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
				//如果邻接顶点的fTime小于当前顶点的fTime，说明邻接顶点或邻接顶点的构成的子树中必然出现了返回路径，且返回顶点在当前顶点之前。
				//此时将当前顶点的fTime也调整为返回顶点的fTime，即当前顶点的“关节点”也应该在那个返回节点处。
				if(fTime(u) < fTime(v))
				{
					fTime(v) = fTime(u);
				}
				//否则，说明当前顶点的邻接顶点递归中不存在返回路径，换言之，当前顶点对应的子树中，关节点还是在当前节点。因此，清空栈中的当前顶点上方的那些邻接顶点。
				else
				{
					//这里书上的代码比较简单。但是我为了结果看起来更直观，所以稍微修改了一下输入方式。
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
				//当邻接顶点指向DISCOVERED顶点时，说明必然出现了返回路径。
				type(v, u) = BACKWARD;
				if(u != parent(v))
				{
					//这里是可能存在fTime(v) < dTime(u)的情况的。即在存在多条返回路径的时候，第一条返回路径的返回点较高，第二条返回路径的返回点较低。
					//此时，希望保存较高的那个点，作为关节点。
					fTime(v) = fTime(v) < dTime(u) ? fTime(v) : dTime(u);
				}
				break;
			case VISITED:
				//出现VISITED的情况不是考虑的重点，因此这里就不再通过fTime的大小做区分了。
				type(v, u) = CROSS;
				break;
		}
	}
	//最后不要忘记把状态改为VISITED。
	status(v) = VISITED;
}

template<typename Tv, typename Te>
void Graph<Tv, Te>::bcc(int s)
{
	Stack<Tv> stack;
	int clock = 0;
	int v = s;
	//这里我多加了一个Queue，用来记录双连通域。
	Queue<Tv> q;

	do
	{
		if(status(v) == UNDISCOVERED)
		{
			BCC(s, clock, stack, q);
			//完成后，stack中还会留存一个元素。将其弹出。
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
	//数字越小，优先级越大。
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
				//每次从UNDISCOVERED的顶点中，选出权重最小（优先级最大）的那一个。
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
	//dTime，表示被发现的时间；fTime，表示访问完毕的时间。
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
	//E就相当于邻接矩阵。每一行代表对应秩的顶点指向其他顶点的边。
	Vector<Vector<Edge<Te> *>> E;

	//这里的n和e分别对应基类中的numOfVertexes和edges。注意，这里需要使用引用，使得派生类的数据成员在对值进行修改时，修改能传递回基模板的数据成员。
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
	//nextNbr是从后向前寻找的。这里是从E[i][j]开始向前搜索（不包括j），寻找第一个值为非空的元素。即点i在点j之前的、第一个产生连接的点。
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
	//输出顶行。
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

	//输出下列各行。
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
	//在先前的每个vector的后面加入一列。
	for(int i = 0; i < n; i++)
	{
		E[i].push_back(NULL);
	}
	//增加总点数。
	n++;

	//在E中新添加一行。
	Vector<Edge<Te> *> vectorEdge(n, n, NULL);
	E.push_back(vectorEdge);

	//在V中添加一个元素。
	V.push_back(Vertex<Tv>(d));

	return n;
}

template<typename Tv, typename Te>
Tv GraphMatrix<Tv, Te>::remove(int index)
{
	Tv temp = V[index].data;

	//这里我自己写的做法和ppt中的做法略有不同，但思路一致。
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

	//下面是ppt中给出的原版算法。
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
	//不要忘记对e、outDegree和inDegree的添加。
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
	//记得delete后把对应元素还原为NULL。
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