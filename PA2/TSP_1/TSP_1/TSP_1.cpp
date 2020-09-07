// TSP_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

enum VStatus
{
    UNDISCOVERED, DISCOVERED, VISITED
};

enum EType
{
    UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD
};

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
            return false;
        }
        else
        {
            stack[++topPtr] = e;
            ++size;
            return true;
        }
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
};

struct Vertex
{
    int data;
    int inDegree, outDegree;
    VStatus status;
    int parent;
    int height;

    Vertex(const int &e = 0):
        data(0), height(e), inDegree(0), outDegree(0), status(UNDISCOVERED), parent(-1)
    {}
};

struct GraphMatrix
{
    Vertex *V;
    int **E;

    int n;
    int e;

    GraphMatrix(int n_ = 0):
        n(n_), e(0)
    {
        V = new Vertex[n]();
        E = new int *[n];
        for(int i = 0; i < n; i++)
        {
            V[i].data = i + 1;
            E[i] = new int[n]();
        }
    }
    ~GraphMatrix()
    {
        delete[] V;
        for(int i = 0; i < n; i++)
        {
            delete[] E[i];
        }
        delete[] E;
    }

    void insert(int i, int j)
    {
        if(E[i][j])
        {
            return;
        }
        e++;
        E[i][j] = 1;
        V[i].outDegree++;
        V[j].inDegree++;
    }

    int lastNbr(int i, int j)
    {
        --j;
        while((-1 < j) && !E[j][i])
        {
            --j;
        }
        return j;
    }

    int firstNbr(int i)
    {
        return lastNbr(i, n);
    }

    void longest()
    {
        int i = 0;
        Stack<int> store(n);

        while(i < n && V[i].status == UNDISCOVERED)
        {
            if(V[i].outDegree == 0)
            {
                V[i].status = DISCOVERED;
                store.push(i);

                while(!store.empty())
                {
                    int k = store.pop();

                    for(int j = firstNbr(k); -1 < j; j = lastNbr(k, j))
                    {
                        V[j].height = (V[j].height < V[k].height + 1) ? V[k].height + 1 : V[j].height;
                        V[j].status = DISCOVERED;
                        store.push(j);
                    }
                }
            }
            i++;
        }
    }
};


int main()
{
    int n, m;
    scanf("%d%d", &n, &m);

    GraphMatrix graph(n);

    for(int k = 0; k < m; k++)
    {
        int i, j;
        scanf("%d%d", &i, &j);

        graph.insert(i - 1, j - 1);
    }

    graph.longest();

    int max = 0;
    for(int i = 0; i < n; i++)
    {
        max = max < graph.V[i].height ? graph.V[i].height : max;
    }

    printf("%d", max + 1);
}
