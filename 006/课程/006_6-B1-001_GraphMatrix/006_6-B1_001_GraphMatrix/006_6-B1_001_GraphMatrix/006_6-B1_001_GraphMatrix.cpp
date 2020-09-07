// 006_6-B1_001_GraphMatrix.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "GraphMatrix.h"
#include "Queue.h"
using namespace std;

template<typename Tv, typename Te>
struct BFS_PU
{
	//对于BFS，要保证与当前点v邻接的点u的优先级不会高于当前点v同级的点。
	//即u的优先级应该更低，数值应该更大。因此在处理时使用当前点优先级+1的做法。
	void operator() (Graph<Tv, Te> *grp, int v, int u)
	{
		if(grp->status(u) == UNDISCOVERED)
		{
			//这里之所以要写这样的条件，是为了防止同一优先级的顶点在对各自的邻接节点的优先度赋值时，出现覆盖的情况。
			//假设有两个优先度为2的顶点B、C。B的邻接顶点有D、E，C的邻接顶点有E、F。在B对D、E完成赋值后，D、E的优先度为3，parent是B。
			//此时接下来紧接着需要让C对邻接顶点进行赋值。当给E赋值时，如果条件是“grp->priority(v) < grp->priority(u)”，则会对E进行重新赋值，并修改parent。
			if(grp->priority(v) + 1 < grp->priority(u))
			{
				grp->parent(u) = v;
				grp->priority(u) = grp->priority(v) + 1;
			}
		}
	}
};

template<typename Tv, typename Te>
struct DFS_PU
{
	//对于DFS，要保证与当前点v邻接的点u的优先级大于当前点v，即数值更小。
	//因此在处理时选用当前点优先级-1的做法。
	void operator() (Graph<Tv, Te> *grp, int v, int u)
	{
		if(grp->status(u) == UNDISCOVERED)
		{
			//这里的条件我觉得-1似乎没什么必要。
			if(grp->priority(v) - 1 < grp->priority(u))
			{
				grp->parent(u) = v;
				grp->priority(u) = v - 1;
			}
		}
	}
};

template<typename Tv, typename Te>
struct Prim_PU
{
	void operator() (Graph<Tv, Te> *grp, int v, int u)
	{
		if(grp->status(u) == UNDISCOVERED)
		{
			//若当前节点u在之前未被发现过，则将u的优先度改为边vu的权重。
			//若当前节点u在之前被发现过，且权重为先前边v'u的权重，则比较先前边v'u与当前边vu的权重大小，将其中较小的权重作为u的优先度。
			//这么做的意义在于，当确定割中的点v与割外的点u间边的权重小于割中的另一点v'和割外的点u间的权重时，及时将更小的权重更新到u的优先度上。
			if(grp->weight(v, u) < grp->priority(u))
			{
				grp->priority(u) = grp->weight(v, u);
				grp->parent(u) = v;
			}
		}
	}
};

template<typename Tv, typename Te>
struct Dijkstra_PU
{
	void operator() (Graph<Tv, Te> *grp, int v, int u)
	{
		if(grp->status(u) == UNDISCOVERED)
		{
			//顶点v的权重值实际上就代表了v到初始顶点的距离。
			//对于v的临近顶点u来说，如果u尚未被发现过，则权重为INT_MAX，此时直接调整权重即可。
			//当u曾经被发现过时，权重已经被调整过了，此时只有确定经过v到初始顶点s的距离比经过之前的顶点v'到初始顶点s的距离更近时，才更换u的权重。
			if(grp->priority(v) + grp->weight(v, u) < grp->priority(u))
			{
				grp->priority(u) = grp->priority(v) + grp->weight(v, u);
				grp->parent(u) = v;
			}
		}
	}
};

//修改接口时使用的代码。
/*
template<typename T>
struct A
{
	int* a;
	A(int i_ = 0)
	{
		a = new int[5];
		for(int i = i_; i < i_ + 5; i++)
		{
			a[i] = i;
		}
	}
};

template<typename T>
struct B
{
	Vector<Vector<A<T> *>> E;
	int n, e;

	B()
	{
		n = 5;
		e = 0;
		Vector<A<T> *> v(n, n, NULL);
		E.push_back(v);
	}

	void insert(int d)
	{
		for(int i = 0; i < n; i++)
		{
			E[0].push_back(NULL);
		}

	}
};


template<typename Te>
struct GraphAtrix
{
	Vector<Vector<A<Te> *>> E;
	int n;
	int e;

	GraphAtrix()
	{
		n = 1;
		e = 1;
		Vector<A<Te> *> vectorEdge(n, n, NULL);
		E.push_back(vectorEdge);
	}

	void insert(int i)
	{
		for(int i = 0; i < n; i++)
		{
			E[0].push_back(NULL);
		}
		n++;

	}
};
*/

int main()
{
	//GrpahMatrix，接口测试。
	/*
	{
		GraphMatrix<char, int> gm;

		//图例参考ppt的p517。
		gm.insert('A');
		gm.insert('B');
		gm.insert('C');
		gm.insert('D');

		gm.insert(1, 1, 1, 0);
		gm.insert(1, 1, 2, 0);
		gm.insert(1, 1, 2, 1);
		gm.insert(1, 1, 3, 1);
		gm.insert(1, 1, 0, 1);
		gm.insert(1, 1, 0, 2);
		gm.insert(1, 1, 1, 2);
		gm.insert(1, 1, 1, 3);

		gm.remove(0, 1);
		gm.remove(2, 1);

		//2 2 2 -1 -1
		cout << gm.firstNbr(0) << endl;
		cout << gm.nextNbr(0, 4) << endl;
		cout << gm.nextNbr(0, 3) << endl;
		cout << gm.nextNbr(0, 2) << endl;
		cout << gm.nextNbr(0, 1) << endl;
		cout << endl;
		//3 3 2 0 0
		cout << gm.firstNbr(1) << endl;
		cout << gm.nextNbr(1, 4) << endl;
		cout << gm.nextNbr(1, 3) << endl;
		cout << gm.nextNbr(1, 2) << endl;
		cout << gm.nextNbr(1, 1) << endl;
		cout << endl;
		//0 0 0 0 0
		cout << gm.firstNbr(2) << endl;
		cout << gm.nextNbr(2, 4) << endl;
		cout << gm.nextNbr(2, 3) << endl;
		cout << gm.nextNbr(2, 2) << endl;
		cout << gm.nextNbr(2, 1) << endl;
		cout << endl;
		//1 1 1 1 -1
		cout << gm.firstNbr(3) << endl;
		cout << gm.nextNbr(3, 4) << endl;
		cout << gm.nextNbr(3, 3) << endl;
		cout << gm.nextNbr(3, 2) << endl;
		cout << gm.nextNbr(3, 1) << endl;
		cout << endl;

		//由于顶点数量会变化，所以序号也要随之改变。
		gm.remove(0);
		gm.remove(2);
		gm.remove(0);
		gm.remove(0);
	}
	*/

	//GrpahMatrix，BFS测试。
	/*
	{
		//图形参考ppt的p547，不过这里的搜索顺序与ppt中的顺序似乎正好相反。
		//想要统一的话可以尝试反过来插入顶点。
		GraphMatrix<char, int> gm;
		gm.insert('a');
		gm.insert('b');
		gm.insert('c');
		gm.insert('d');
		gm.insert('e');
		gm.insert('f');
		gm.insert('g');
		gm.insert('s');

		gm.insert(1, 1, 0, 2);
		gm.insert(1, 1, 0, 4);
		gm.insert(1, 1, 0, 7);
		gm.insert(1, 1, 1, 2);
		gm.insert(1, 1, 1, 3);
		gm.insert(1, 1, 1, 6);
		gm.insert(1, 1, 2, 0);
		gm.insert(1, 1, 2, 1);
		gm.insert(1, 1, 2, 7);
		gm.insert(1, 1, 3, 1);
		gm.insert(1, 1, 3, 7);
		gm.insert(1, 1, 4, 0);
		gm.insert(1, 1, 4, 5);
		gm.insert(1, 1, 4, 6);
		gm.insert(1, 1, 5, 4);
		gm.insert(1, 1, 5, 6);
		gm.insert(1, 1, 6, 1);
		gm.insert(1, 1, 6, 4);
		gm.insert(1, 1, 6, 5);
		gm.insert(1, 1, 7, 0);
		gm.insert(1, 1, 7, 2);
		gm.insert(1, 1, 7, 3);

		gm.bfs(7);
		gm.show();
		cout << endl;
	}
	*/

	//GrpahMatrix，DFS测试。
	/*
	{
		GraphMatrix<char, int> gm;

		//图形参考ppt的p572。
		gm.insert('g');
		gm.insert('f');
		gm.insert('e');
		gm.insert('d');
		gm.insert('c');
		gm.insert('b');
		gm.insert('a');

		gm.insert(1, 1, 0, 4);
		gm.insert(1, 1, 0, 6);
		gm.insert(1, 1, 1, 0);
		gm.insert(1, 1, 2, 1);
		gm.insert(1, 1, 3, 2);
		gm.insert(1, 1, 3, 6);
		gm.insert(1, 1, 5, 4);
		gm.insert(1, 1, 6, 1);
		gm.insert(1, 1, 6, 4);
		gm.insert(1, 1, 6, 5);

		gm.show();
		//可以看到这里的dfs()结果与课上的结果仍然有些不同，原因还是在nextNbr()以及dfs()中的先后顺序差异造成的。
		//这里由于我是倒着输入a-g的，因此，在完成第一轮DFS后，随后会在顶点e（序号为2）处再次进入，而非顶点d（序号为3）。
		gm.dfs(6);
		cout << endl;
		gm.show();
		cout << endl;
	}
	*/

	//GrpahMatrix，顺序输出零入度顶点测试。
	//图形参考ppt的p589。
	/*
	{
		GraphMatrix<char, int> gm;

		gm.insert('a');
		gm.insert('b');
		gm.insert('c');
		gm.insert('d');
		gm.insert('e');
		gm.insert('f');

		gm.insert(1, 1, 0, 2);
		gm.insert(1, 1, 0, 3);
		gm.insert(1, 1, 1, 2);
		gm.insert(1, 1, 2, 3);
		gm.insert(1, 1, 2, 4);
		gm.insert(1, 1, 2, 5);
		//下面(3, 0)位置的插入是用来检验环路的。
		//gm.insert(1, 1, 3, 0);
		gm.insert(1, 1, 4, 5);

		gm.tSortUsingIndegree(0);
		gm.show();
	}
	*/

	//GrpahMatrix，tSort测试。
	/*
	{
		GraphMatrix<char, int> gm;

		gm.insert('f');
		gm.insert('e');
		gm.insert('d');
		gm.insert('c');
		gm.insert('b');
		gm.insert('a');

		gm.insert(1, 1, 1, 0);
		gm.insert(1, 1, 3, 0);
		gm.insert(1, 1, 3, 1);
		gm.insert(1, 1, 3, 2);
		gm.insert(1, 1, 4, 3);
		gm.insert(1, 1, 5, 2);
		//下面(3, 0)位置的插入是用来检验环路的。
		//gm.insert(1, 1, 2, 5);
		gm.insert(1, 1, 5, 3);

		Stack<char> *s;
		s = gm.tSort(3);
		while(!s->empty())
		{
			cout << s->pop() << endl;
		}
		delete s;
	}
	*/

	//GrpahMatrix，bcc测试。
	/*
	{
		GraphMatrix<char, int> gm;

		gm.insert('j');
		gm.insert('i');
		gm.insert('h');
		gm.insert('g');
		gm.insert('f');
		gm.insert('e');
		gm.insert('d');
		gm.insert('c');
		gm.insert('b');
		gm.insert('a');

		gm.insert(1, 1, 0, 1);
		gm.insert(1, 1, 0, 9);
		gm.insert(1, 1, 1, 0);
		gm.insert(1, 1, 1, 9);
		gm.insert(1, 1, 2, 7);
		gm.insert(1, 1, 2, 9);
		gm.insert(1, 1, 3, 4);
		gm.insert(1, 1, 3, 5);
		gm.insert(1, 1, 3, 6);
		gm.insert(1, 1, 4, 3);
		gm.insert(1, 1, 5, 3);
		gm.insert(1, 1, 5, 6);
		gm.insert(1, 1, 6, 3);
		gm.insert(1, 1, 6, 5);
		gm.insert(1, 1, 6, 7);
		gm.insert(1, 1, 7, 2);
		gm.insert(1, 1, 7, 6);
		gm.insert(1, 1, 7, 8);
		gm.insert(1, 1, 8, 7);
		gm.insert(1, 1, 8, 9);
		gm.insert(1, 1, 9, 0);
		gm.insert(1, 1, 9, 1);
		gm.insert(1, 1, 9, 2);
		gm.insert(1, 1, 9, 8);

		gm.bcc(9);
	}
	*/

	//GrpahMatrix，pfs（及配合的bfs、dfs算法）测试。
	/*
	{
		//这里的图用的是和之前BFS测试一样的图，参考ppt的p547。
		GraphMatrix<char, int> gm;
		gm.insert('a');
		gm.insert('b');
		gm.insert('c');
		gm.insert('d');
		gm.insert('e');
		gm.insert('f');
		gm.insert('g');
		gm.insert('s');

		gm.insert(1, 1, 0, 2);
		gm.insert(1, 1, 0, 4);
		gm.insert(1, 1, 0, 7);
		gm.insert(1, 1, 1, 2);
		gm.insert(1, 1, 1, 3);
		gm.insert(1, 1, 1, 6);
		gm.insert(1, 1, 2, 0);
		gm.insert(1, 1, 2, 1);
		gm.insert(1, 1, 2, 7);
		gm.insert(1, 1, 3, 1);
		gm.insert(1, 1, 3, 7);
		gm.insert(1, 1, 4, 0);
		gm.insert(1, 1, 4, 5);
		gm.insert(1, 1, 4, 6);
		gm.insert(1, 1, 5, 4);
		gm.insert(1, 1, 5, 6);
		gm.insert(1, 1, 6, 1);
		gm.insert(1, 1, 6, 4);
		gm.insert(1, 1, 6, 5);
		gm.insert(1, 1, 7, 0);
		gm.insert(1, 1, 7, 2);
		gm.insert(1, 1, 7, 3);

		//gm.pfs(7, BFS_PU<char, int>());
		//gm.show();

		gm.pfs(7, DFS_PU<char, int>());
		gm.show();
	}
	*/

	//GrpahMatrix，Prim算法测试。
	/*
	{
		//图形参考课本p176。
		GraphMatrix<char, int> gm;
		gm.insert('a');
		gm.insert('b');
		gm.insert('c');
		gm.insert('d');
		gm.insert('e');
		gm.insert('f');
		gm.insert('g');
		gm.insert('h');

		gm.insert(1, 4, 0, 1);
		gm.insert(1, 6, 0, 3);
		gm.insert(1, 7, 0, 6);
		gm.insert(1, 4, 1, 0);
		gm.insert(1, 12, 1, 2);
		gm.insert(1, 12, 2, 1);
		gm.insert(1, 9, 2, 3);
		gm.insert(1, 1, 2, 4);
		gm.insert(1, 2, 2, 5);
		gm.insert(1, 10, 2, 7);
		gm.insert(1, 6, 3, 0);
		gm.insert(1, 9, 3, 2);
		gm.insert(1, 13, 3, 4);
		gm.insert(1, 2, 3, 6);
		gm.insert(1, 1, 4, 2);
		gm.insert(1, 13, 4, 3);
		gm.insert(1, 5, 4, 5);
		gm.insert(1, 11, 4, 6);
		gm.insert(1, 8, 4, 7);
		gm.insert(1, 2, 5, 2);
		gm.insert(1, 5, 5, 4);
		gm.insert(1, 7, 5, 7);
		gm.insert(1, 7, 6, 0);
		gm.insert(1, 2, 6, 3);
		gm.insert(1, 11, 6, 4);
		gm.insert(1, 14, 6, 7);
		gm.insert(1, 10, 7, 2);
		gm.insert(1, 8, 7, 4);
		gm.insert(1, 7, 7, 5);
		gm.insert(1, 14, 7, 6);

		gm.pfs(0, Prim_PU<char, int>());
		gm.show();
	}
	*/

	//GrpahMatrix，Dijkstra算法测试。
	/*
	{
		//图形参考课本p180。
		GraphMatrix<char, int> gm;
		gm.insert('a');
		gm.insert('b');
		gm.insert('c');
		gm.insert('d');
		gm.insert('e');
		gm.insert('f');
		gm.insert('g');
		gm.insert('s');

		gm.insert(1, 25, 0, 1);
		gm.insert(1, 2, 1, 3);
		gm.insert(1, 18, 2, 1);
		gm.insert(1, 30, 2, 3);
		gm.insert(1, 5, 2, 5);
		gm.insert(1, 11, 3, 4);
		gm.insert(1, 16, 3, 6);
		gm.insert(1, 6, 4, 1);
		gm.insert(1, 6, 4, 6);
		gm.insert(1, 20, 5, 3);
		gm.insert(1, 40, 5, 6);
		gm.insert(1, 9, 7, 0);
		gm.insert(1, 14, 7, 2);
		gm.insert(1, 15, 7, 5);

		gm.pfs(7, Dijkstra_PU<char, int>());
		gm.show();
	}
	*/

	//GrpahMatrix，迭代DFS测试。
	/*
	{
		GraphMatrix<char, int> gm;

		//图形参考ppt的p572。
		gm.insert('g');
		gm.insert('f');
		gm.insert('e');
		gm.insert('d');
		gm.insert('c');
		gm.insert('b');
		gm.insert('a');

		gm.insert(1, 1, 0, 4);
		gm.insert(1, 1, 0, 6);
		gm.insert(1, 1, 1, 0);
		gm.insert(1, 1, 2, 1);
		gm.insert(1, 1, 3, 2);
		gm.insert(1, 1, 3, 6);
		gm.insert(1, 1, 5, 4);
		gm.insert(1, 1, 6, 1);
		gm.insert(1, 1, 6, 4);
		gm.insert(1, 1, 6, 5);

		gm.dfs_recursion(6);
		cout << endl;
		gm.show();
		cout << endl;
	}
	*/


	//修改接口时使用的代码。
	/*
	int a[5] = { 1, 2, 3, 4, 5 };


	Vector<Vector<A *>> m;
	m.push_back(v);
	for(int i = 0; i < 5; i++)
	{
		m[0].push_back(NULL);
	}
	*/
	/*
	B<int> b;
	b.insert(1);
	b.insert(1);
	b.insert(1);
	b.insert(1);


	GraphAtrix<int> gm;


	gm.insert('A');
	gm.insert('B');
	gm.insert('C');
	gm.insert('D');
	*/
}
