// 004_Homework_4-23.cpp : This file contains the 'main' function. Program execution begins and ends there.
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
void merge(Queue<T> &left, Queue<T> &right, Queue<T> &queue)
{
    int size = left.size() + right.size();
    while(queue.size() < size)
    {
        if((right.size() <= 0) || (left.front() <= right.front()))
        {
            queue.enqueue(left.dequeue());
        }
        else
        {
            queue.enqueue(right.dequeue());
        }
    }
}

template<typename T>
void mergeSearch(Queue<T> &queue)
{
    if(queue.size() == 1)
    {
        return;
    }

    int mi = queue.size() / 2;
    Queue<T> left;
    Queue<T> right;
    for(int i = 0; i < mi; i++)
    {
        left.enqueue(queue.dequeue());
    }
    mi = queue.size();
    for(int i = 0; i < mi; i++)
    {
        right.enqueue(queue.dequeue());
    }

    mergeSearch(left);
    mergeSearch(right);
    merge(left, right, queue);
}

int main()
{
    Queue<int> q;
    for(int i = 0; i < 10; i++)
    {
        q.enqueue(10 - i);
    }
    mergeSearch(q);
    for(int i = 0; i < 10; i++)
    {
        cout << q.dequeue() << " ";
    }
    cout << endl;
}
