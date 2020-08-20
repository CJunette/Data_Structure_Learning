// 004_Homework_4-18.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

template<typename T>
struct Stack
{
    int _index, _size;
    T stack[4];
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
        stack[_index--] = -1;
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

void FL(int n)
{
    Stack<int> nums;        
    int sum = 0;

    for(int i = 0; i < 4; i++)
    {
        nums.push(0);
    }

    //nums.top()++;
    int i;

    while(!nums.empty())
    {
        i = nums.top();
        sum += i * i;
        if((sum < n) && nums._size == 4)
        {
            sum -= i * i;
            nums.top() = ++i;
            continue;
        }

        if((sum < n) && (nums._size < 4))
        {
            i = nums.top();
            nums.push(i);
            continue;
        }

        if((sum == n) && (nums._size == 4))
        {
            int i1 = nums.pop();
            int i2 = nums.pop();
            int i3 = nums.pop();
            int i4 = nums.pop();
            cout << n << " = " << i4 << "^2 + " << i3 << "^2 + " << i2 << "^2 + " << i1 << "^2" << endl;
            nums.push(i4);
            nums.push(i3);
            nums.push(i2);
            nums.push(i1);
        }

        if(n <= sum)
        {
            sum -= i * i;
            nums.pop();
            if(nums.empty())
            {
                continue;
            }
            else
            {
                i = nums.top()++;
                sum -= i * i;
            }            
        }
    }
    /*
    while(!nums.empty())
    {
        i = nums.top();
        sum += i * i;
        if((sum < n) && (nums._size == 4))
        {
            sum -= i * i;
            nums.top() = ++i;
            continue;
        }

        if((sum < n) && (nums._size < 4))
        {
            sum -= i * i;
            i = ++nums.top();
            sum += i * i;
            nums.push(i);            
            //不要进入到下面的“n <= sum”的判断中，不然会重复减去i * i。
            continue;
        }

        if((sum == n) && (nums._size  == 4))
        {
            int i1 = nums.pop();
            int i2 = nums.pop();
            int i3 = nums.pop();
            int i4 = nums.pop();
            cout << n << " = " << i4 << "^2 + " << i3 << "^2 + " << i2 << "^2 + " << i1 << "^2" << endl;
            nums.push(i4);
            nums.push(i3);
            nums.push(i2);
            nums.push(i1);
        }

        if(n <= sum)
        {
            sum -= i * i;
            nums.pop();
            i = nums.top();
            //由于下次循环开头还要进行一次平方和，因此在此处直接把top()元素的平方和减掉。
            sum -= i * i;
        }
    }
    */
        
}

void FLForEach(int n)
{
    for(int i = 1; i <= n; i++)
    {
        FL(i);
        cout << endl;
    }
}

int main()
{
    FLForEach(30);
}