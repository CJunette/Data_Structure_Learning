// 004_A-1_001_StackWithList.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <list>
using namespace std;

template<typename T>
struct Stack: public list<T>
{
    void push(T const &e) { this->push_back(e); }
    T pop()
    {
        typename list<T>::iterator iter;
        iter = --this->end();
        T t = *iter;
        this->erase(iter);
        return t;
    }
    T &top() { return *(--this->end()); }
};

int main()
{
    Stack<int> s;
    s.push(1);
    cout << s.top() << endl;

    s.push(2);
    cout << s.top() << endl;

    s.push(3);
    cout << s.top() << endl;

    cout << endl;

    cout << s.pop() << endl;
    cout << s.top() << endl;

    cout << s.pop() << endl;
    cout << s.top() << endl;

    cout << s.pop() << endl;
}