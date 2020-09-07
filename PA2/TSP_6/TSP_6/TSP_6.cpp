// TSP_6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//邻接矩阵占据空间过大，做出修改。将E转为用类列表节点的结构构成的数组。
//用基于动态内存分配的栈取代之前基于列表的队列，作为数据中转的结构。
//将E转为向量构成的数组（希望通过内存连续分布的机制加快速度）。
//利用getchar()编写read()函数，加快数据读取速度。
//上个版本在longest中的遍历消耗n^2时间，这里做出修改。用另一个stack承接第一轮outDegree为0的节点的祖先中在“删除”第一轮节点后，outDegree为0的那些祖先节点（利用遍历保证stack中的数据不重复，不然可能会再次造成n^2的空间消耗）。

#include <iostream>
#include <ctime>
#include <fstream>
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

    void unrepeatedPush(const T &e)
    {
        if(full())
        {
            expand();
        }

        for(int i = 0; i < size; i++)
        {
            if(stack[i] == e)
            {
                return;
            }
        }
        stack[++topPtr] = e;
        ++size;
        return;
    }

    T pop()
    {
        if(!empty())
        {
            T temp = stack[topPtr];
            stack[topPtr--] = -1;
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
    int heightCount;
    int tailCount;

    GraphMatrix(int n_ = 0):
        n(n_), heightCount(0), tailCount(0)
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
        //e++;

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
        Stack<int> transit(n);
        /*
        timespec start, end;
        double time1 = 0;
        double time2 = 0;
        */

        while(tailCount < n)
        {
            //timespec_get(&start, 1);
            while(i < n)
            {
                if(V[i].outDegree == 0)
                {
                    store.push(i);
                    tailCount++;
                }
                i++;
            }
            /*
            timespec_get(&end, 1);
            time1 += (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
            */
            heightCount++;

            //timespec_get(&start, 1);

            //用transit栈来做当前outDegree为0的节点的祖先中，在“删除”当前节点后，outDegree变为0的那些节点。
            //以此来避免对全图的重复遍历。
            while(!store.empty())
            {
                int num = store.pop();
                V[num].outDegree = -1;

                int k = E[num].size - 1;
                for(int j = E[num][k]; -1 < k; j = E[num][--k])
                {
                    V[j].outDegree--;
                    if(V[j].outDegree == 0)
                    {
                        transit.unrepeatedPush(j);
                    }
                }

                if(store.empty())
                {
                    while(!transit.empty())
                    {
                        store.push(transit.pop());
                        tailCount++;
                    }   
                    //如果经过转以后store仍为空，说明当前已到最后节点。此时不再添加heightCount。
                    if(!store.empty())
                    {
                        heightCount++;
                    }
                }
            }
            /*
            timespec_get(&end, 1);
            time2 += (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
            */
            i = 0;
        }
        /*
        std::cout << "time1: " << time1 << endl;
        std::cout << "time2: " << time2 << endl;
        */
    }
};

inline int read()
{
    int x = 0;
    char c = getchar();

    while(c == ' ' || c == '\n')
    {
        c = getchar();
    }

    while('0' <= c && c <= '9')
    {
        x = x * 10 + c - '0';
        c = getchar();
    }

    return x;
}

int main()
{
    timespec start, end;
    timespec start1, end1;
    double time = 0;

    int n, m;
    scanf("%d%d", &n, &m);

    GraphMatrix graph(n);

    int i, j;
    double time0 = 0;
    double time1 = 0;
    //char c;
    //ifstream fin("connect.txt");
    int *num = new int[2 * m];

    //timespec_get(&start, 1);

    for(int k = 0; k < 2 * m; k++)
    {
        //scanf("%d%d", &num[k], &num[k + 1]);
        //std::cin >> num[k] >> num[k + 1];
        num[k] = read();
        //fin >> num[k];
    }

    /*
    timespec_get(&end, 1);
    time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
    std::cout << "time0: " << time << endl;
    //printf("time0: %f\n", time);
    */

    for(int k = 0; k < 2 * m; k += 2)
    {
        timespec_get(&start, 1);

        /*
        i = 0;
        j = 0;
        c = getchar();
        while(c == ' ' || c == '\n')
        {
            c = getchar();
        }
        while(c != ' ')
        {
            i = i * 10 + c - '0';
            c = getchar();
        }
        c = getchar();
        while(c != ' ' && c != '\n')
        {
            j = j * 10 + c - '0';
            c = getchar();
        }
        */
        //scanf("%d%d", &i, &j);      
        //cin >> i >> j;

        /*
        timespec_get(&end, 1);
        time0 += (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
        */
        graph.insert(num[k] - 1, num[k + 1] - 1);
        //graph.insert(i - 1, j - 1);
        /*
        timespec_get(&start1, 1);
        timespec_get(&end1, 1);
        time1 += (end1.tv_sec - start1.tv_sec) + 0.000000001 * (end1.tv_nsec - start1.tv_nsec);
        */
    }
    /*
    std::cout << "time1: " << time0 << endl;
    std::cout << "time2: " << time1 << endl;
    */
    /*
    printf("time1: %f\n", time0);
    printf("time2: %f\n", time1);
    */
    //timespec_get(&start, 1);

    graph.longest();
    /*
    timespec_get(&end, 1);
    time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
    //printf("time3: %f\n", time);

    std::cout << "time2: " << time << endl;
    */

    delete[] num;

    //std::printf("%d", graph.heightCount);
    std::cout << graph.heightCount << endl;
}


