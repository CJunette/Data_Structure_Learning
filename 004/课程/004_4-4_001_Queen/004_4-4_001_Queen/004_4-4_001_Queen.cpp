// 004_4-4_001_Queen.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

const int N = 4;

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
        stack[_index--] = {-1, -1};
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

    bool find(T const &e)
    {
        for(int i = 0; i < _size; i++)
        {
            if(stack[i] == e)
            {
                return true;
            }
        }
        return false;
    }
    
    void show() const
    {
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < N; j++)
            {
                if(stack[i]._y == j)
                {
                    cout << "* ";
                }
                else
                {
                    cout << "_ ";
                }
            }
            cout << endl;
        }
        cout << endl;
    }

};

struct Queen
{
    //x表示行，y表示列。
    int _x, _y;
    Queen(int x = -1, int y = -1):
        _x(x), _y(y)
    {}

    bool operator == (Queen const &q)
    {
        return (_x == q._x) ||
            (_y == q._y) ||
            (_x - q._x == _y - q._y) ||
            (q._x - _x == _y - q._y);
    }

    bool operator != (Queen const &q)
    {
        return !(*this == q);
    }
};

void placeQueen(int N)
{
    Queen q(0, 0);
    Stack<Queen> stack;
    while(q._x < N)
    {        
        if(stack.find(q) && (q._y < N))
        {            
            q._y++;
            continue;
            
        }
        else if(q._y >= N)
        {
            q = stack.pop();
            q._y++;
        }
        else
        {
            stack.push(q);
            q._x++;
            q._y = 0;
        }
        stack.show();
    }
}


int main()
{
    placeQueen(N);
}