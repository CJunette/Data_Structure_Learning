// 004_I-1_001_BankSimulation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <list>
#include <ctime>
using namespace std;

template<typename T>
struct Queue: public list<T>
{
    void enqueue(T const &e) { this->push_back(e); }

    T dequeue()
    {
        typename list<T>::iterator iter;
        iter = this->begin();
        T t = *iter;
        this->erase(iter);
        return t;
    }

    T &front() { return *(this->begin()); }
};

template<typename T>
struct Stack
{
    int _index, _size;
    T stack[20];
    //T *stack;

    Stack(int capacity = 100):
        _index(-1), _size(0)
    {
        //stack = new T[capacity];
    }

    void push(T const &e)
    {
        stack[++_index] = e;
        _size++;
    }

    T pop()
    {
        T e = stack[_index];
        stack[_index--] = 0;
        _size--;
        return e;
    }

    T &top()
    {
        return stack[_index];
    }

    bool empty() const
    {
        return _size == 0;
    }
};

struct Customer
{
    int window;
    unsigned int time;
};

//服务总时间。
int servTime = 240;
//银行窗口数。
int nWin = 3;

int selectMin(Queue<Customer> *windows)
{    
    timespec start, end;
    double time;
    timespec_get(&start, 1);

    //这里弄这么复杂是为了在存在多个相同人数的队列时，程序能随机选择其中一个，而不是固定选择序号较小的。
    Stack<int> stack;
    int min = 0;
    stack.push(min);

    //注意这里i从1开始。
    for(int i = 1; i < nWin; i++)
    {
        if(windows[i].size() < windows[min].size())
        {
            min = i;
            while(!stack.empty())
            {
                stack.pop();
            }
            stack.push(min);
        }
        if(windows[min].size() == windows[i].size())
        {
            stack.push(i);
        }
    }

    int *arr = new int[stack._size];
    //这里随着元素不断pop()，stack的_size会不断缩小。
    int size = stack._size;
    for(int i = 0; i < size; i++)
    {
        arr[i] = stack.pop();
    }
    if(size == 1)
    {
        int r = arr[0];
        delete[] arr;
        return r;
    }
    else
    {        
        timespec_get(&end, 1);
        time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
        srand(time * 100000000000 + rand());
        int i = rand() % size;
        int r = arr[i];
        delete[] arr;
        return r;
    }    
}

void simulation()
{
    Queue<Customer> *windows = new Queue<Customer>[nWin];
    int *EmptyTime = new int[nWin]();

    timespec start, end;
    double time;

    timespec_get(&start, 1);
    for(int i = 0; i < servTime; i++)
    {
        //决定多久来一个客户。
        if(i % 5 == 0)
        {
            timespec_get(&end, 1);
            time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
            srand(time * 100000000000);

            Customer customer;
            //决定客户服务时间的区间。
            customer.time = 1 + rand() % 20;
            customer.window = selectMin(windows);
            windows[customer.window].enqueue(customer);
        }

        cout << "Time: " << i << "." << endl;
        for(int k = 0; k < nWin; k++)
        {
            if(!windows[k].empty())
            {
                printf("Window [%d] : %d left(s). Front needs another %d minutes.\n", k, windows[k].size(), windows[k].front().time);
                //cout << "Window [" << k << "] : " << windows[k].size() << " left(s). Front needs another " << windows[k].front().time << " minutes." << endl;
            }
            else
            {
                printf("Window [%d] : empty now.\n", k);
                //cout << "Window [" << k << "] : empty now." << endl;
                EmptyTime[k]++;
            }
        }

        for(int j = 0; j < nWin; j++)
        {
            if(!windows[j].empty())
            {
                if(--windows[j].front().time <= 0)
                {
                    windows[j].dequeue();
                }
            }
        }

        cout << endl;
    }

    cout << "Empty Statistics: " << endl;
    for(int i = 0; i < nWin; i++)
    {
        printf("Window [%d] : %f\n", i, (float)EmptyTime[i] / (float)servTime);
        //cout << "Window [" << i << "] : " << (float)EmptyTime[i] / (float)servTime << endl;
    }
    delete[] windows;
    delete[] EmptyTime;
}

int main()
{    
    simulation();
}