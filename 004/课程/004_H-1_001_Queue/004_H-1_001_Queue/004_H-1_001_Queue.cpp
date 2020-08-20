// 004_H-1_001_Queue.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <list>
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

int main()
{
    Queue<int> q;
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);

    cout << q.front() << endl;
    q.front() = 0;
    cout << endl;

    cout << q.dequeue() << endl;
    cout << q.dequeue() << endl;
    cout << q.dequeue() << endl;
}