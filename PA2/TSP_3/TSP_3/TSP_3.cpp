// TSP_3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//邻接矩阵占据空间过大，做出修改。将E转为用类列表节点的结构构成的数组。
//用基于动态内存分配的栈取代之前基于列表的队列，作为数据中转的结构。

#include <iostream>
#include <ctime>
using namespace std;

template<typename T>
struct Stack
{
    int size;
    int topPtr;
    int capacity;
    T *stack;

    Stack(int capacity_):
        size(0), topPtr(-1), capacity(capacity_)
    {
        stack = new T[capacity_];
    }
    ~Stack()
    {
        delete[] stack;
    }

    bool empty() const { return size == 0; }
    bool full() const { return size == capacity; }

    bool push(const T &e)
    {
        if(full())
        {
            expand();
        }

        stack[++topPtr] = e;
        ++size;
        return true;        
    }

    T pop()
    {
        if(!empty())
        {
            T temp = stack[topPtr];
            stack[topPtr--] = 0;
            size--;
            return temp;
        }
    }

    T &top()
    {
        return stack[topPtr];
    }

    private:
    void expand()
    {
        T *newArea = new T[capacity * 2];
        for(int i = 0; i < size; i++)
        {
            newArea[i] = stack[i];
        }
        delete[] stack;
        stack = newArea;
        capacity *= 2;
    }
};

template<typename T>
struct ListNode
{
    T data;
    ListNode<T> *pred;
    ListNode<T> *succ;

    ListNode<T>(const T &e = 0, ListNode<T> *pred_ = NULL, ListNode<T> *succ_ = NULL) :
        data(e), pred(pred_), succ(succ_)
    {}
};

struct Vertex
{
    int data;
    int inDegree, outDegree;
    int height;
    int parent;

    Vertex(const int &e = 0):
        data(0), height(e), inDegree(0), outDegree(0), parent(-1)
    {}
};

struct GraphMatrix
{
    Vertex *V;
    ListNode<int> *E;

    int n;
    int maxHeight;

    GraphMatrix(int n_ = 0):
        n(n_), maxHeight(0)
    {
        V = new Vertex[n]();
        E = new ListNode<int>[n];
        for(int i = 0; i < n; i++)
        {
            V[i].data = i + 1;
            E[i].data = i + 1;
        }
    }
    ~GraphMatrix()
    {
        delete[] V;
        for(int i = 0; i < n; i++)
        {
            ListNode<int> *p = E[i].succ;
            while(p != NULL)
            {
                ListNode<int> *next = p->succ;
                delete p;
                p = next;
            }
        }
        delete[] E;
    }

    void insert(int i, int j)
    {
        //查重检验暂时搁置。
        //e++;

        ListNode<int> *p = &E[j];
        while(p->succ != NULL)
        {
            p = p->succ;
        }
        p->succ = new ListNode<int>(i);

        V[i].outDegree++;
        V[j].inDegree++;
    }

    ListNode<int> *lastNbr(int i, ListNode<int> *start)
    {
        ListNode<int> *p = start->succ;
        return p;
    }

    ListNode<int> *firstNbr(int i)
    {
        return E[i].succ;
    }

    void longest()
    {
        int i = 0;
        Stack<int> store(n);

        while(i < n)
        {
            if(V[i].outDegree == 0)
            {                
                store.push(i);

                while(!store.empty())
                {
                    int k = store.pop();

                    int num;
                    int height;
                    for(ListNode<int> *j = firstNbr(k); j != NULL; j = lastNbr(k, j))
                    {
                        num = j->data;
                        height = (V[num].height < V[k].height + 1) ? V[k].height + 1 : V[num].height;
                        maxHeight = (maxHeight < height) ? height : maxHeight;
                        V[num].height = height;
                        
                        if(store.top() != num)
                        {
                            store.push(num);
                        }
                    }
                }
            }
            i++;
        }
    }
};

int main()
{
    timespec start, end;
    double time;

    int n, m;
    scanf("%d%d", &n, &m);

    GraphMatrix graph(n);

    timespec_get(&start, 1);

    for(int k = 0; k < m; k++)
    {
        int i, j;
        scanf("%d%d", &i, &j);

        graph.insert(i - 1, j - 1);
    }

    timespec_get(&end, 1);
    time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
    cout << "time1: " << time << endl;

    timespec_get(&start, 1);

    graph.longest();

    timespec_get(&end, 1);
    time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
    cout << "time2: " << time << endl;

    /*
    int max = 0;
    for(int i = 0; i < n; i++)
    {
        max = max < graph.V[i].height ? graph.V[i].height : max;
    }
    */
    printf("%d", graph.maxHeight+ 1);
}

