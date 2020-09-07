// TSP_4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//邻接矩阵占据空间过大，做出修改。将E转为用类列表节点的结构构成的数组。
//用基于动态内存分配的栈取代之前基于列表的队列，作为数据中转的结构。
//将E转为向量构成的数组（希望通过内存连续分布的机制加快速度）。

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
struct Vector
{
    int data;
    int size;
    int capacity;
    T *elem;

    Vector(int capacity_ = 8):
        capacity(capacity_), size(0)
    {
        elem = new T[capacity];
        data = 0;
    }
    ~Vector()
    {
        delete[] elem;
    }

    bool full() const { return capacity == size; }
    bool empty() const { return size == 0; }

    void push_back(const T &e)
    {
        if(full())
        {
            expand();
        }
        elem[size++] = e;
    }

    T &operator[] (int i)
    {
        return elem[i];
    }
        
    private:
    void expand()
    {
        T *newArea = new T[capacity * 2];
        for(int i = 0; i < size; i++)
        {
            newArea[i] = elem[i];
        }
        delete[] elem;
        elem = newArea;
        capacity *= 2;
    }
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
    Vector<int> *E;

    int n;
    int e;

    GraphMatrix(int n_ = 0):
        n(n_), e(0)
    {
        V = new Vertex[n]();
        E = new Vector<int>[n];
        for(int i = 0; i < n; i++)
        {
            V[i].data = i + 1;
            E[i].data = i + 1;
        }
    }
    ~GraphMatrix()
    {
        delete[] V;
        delete[] E;
    }

    void insert(int i, int j)
    {        
        e++;

        E[j].push_back(i);

        V[i].outDegree++;
        V[j].inDegree++;
    }

    int lastNbr(int i, int j)
    {
        --j;
        while((-1 < j) && !E[i][j])
        {
            --j;
        }
        return j;
    }

    int firstNbr(int i)
    {
        return lastNbr(i, E[i].size);
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

                    for(int j = E[k].size - 1; -1 < j; j--)
                    {
                        int l = E[k][j];
                        int heightL = V[l].height;
                        int heightK = V[k].height;
                        V[l].height = (heightL < heightK + 1) ? heightK + 1 : heightL;
                        store.push(l);
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

    int max = 0;
    for(int i = 0; i < n; i++)
    {
        max = max < graph.V[i].height ? graph.V[i].height : max;
    }

    printf("%d", max + 1);



}

