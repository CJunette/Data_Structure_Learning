// 002_D-3_001_FibonacciSearch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
using namespace std;

class Fib
{
    private:
    int g, f;

    public:
    Fib(int n = 1):
        g(1), f(0)
    {
        int count = 1;
        while(count++ < n)
        {
            next();
        }
    }
    void next()
    {
        g = g + f;
        f = g - f;
    }
    void prev()
    {
        f = g - f;
        g = g - f;
    }
    int get() const
    {
        return g;
    }
};

typedef int Rank;

template <typename T>
Rank fibSearch(T &S, typename T::value_type e, Rank lo, Rank hi)
{    
    Fib f(hi - lo);
    //这里判断条件可以是lo < hi，不用是hi - lo < 1。
    while(lo < hi)
    {
        while(hi - lo < f.get())
        {
            f.prev();
        }
        Rank mid = lo + f.get() - 1;
        if(e < S[mid])
        {
            hi = mid;
        }
        else
        {
            if(S[mid] < e)
            {
                lo = mid;
            }
            else
            {
                return mid;
            }
        }
    }
    return -1;
}

int main()
{
    vector<int> a;
    a.push_back(1);
    a.push_back(1);
    a.push_back(1);
    a.push_back(2);
    a.push_back(4);
    a.push_back(5);
    a.push_back(6);
    a.push_back(7);
    a.push_back(8);
    a.push_back(9);    

    cout << fibSearch(a, 2, 0, 10) << endl;    
}
