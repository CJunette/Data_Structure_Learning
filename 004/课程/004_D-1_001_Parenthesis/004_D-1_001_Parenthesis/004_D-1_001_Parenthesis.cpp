// 004_D-1_001_Parenthesis.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <list>
#include <string>
#include <set>
#include <map>
using namespace std;

template<typename T>
struct Stack: public list<T>
{
    void push(T const &e) 
    { 
        this->push_back(e); 
    }

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

bool paren(const char* str, int lo, int hi)
{
    Stack<char> c;
    set<char> left;
    map<char, char> right;
    left.insert('(');
    left.insert('[');
    left.insert('{');
    right.insert(make_pair(')', '('));
    right.insert(make_pair(']', '['));
    right.insert(make_pair('}', '{'));

    for(int i = lo; i < hi; i++)
    {
        if(left.count(str[i]))
        {
            c.push(str[i]);
        }
        else
        {
            char r = (*right.find(str[i])).second;
            char l = c.top();
            if(r != l)
            {
                return false;
            }
            else
            {
                c.pop();
            }
        }
    }
    return c.empty();
}

int main()
{
    string str;
    getline(cin, str);

    if(paren(str.c_str(), 0, str.size()))
    {
        cout << "true" << endl;
    }
    else
    {
        cout << "false" << endl;
    }
}