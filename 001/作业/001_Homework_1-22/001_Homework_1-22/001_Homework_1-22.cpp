// 001_Homework_1-22.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

class Fib
{
    public:
    Fib(int n):
        f(0), g(1)
    {
        if(n > 1)
        {
            int i = 1;
            while(i < n--)
            {
                next();
            }
        }       
    }
    int next()
    {
        g += f;
        f = g - f;
        return g;
    }
    int prve()
    {
        if(f == 0)
        {
            cout << "No previous!" << endl;
            return 0;
        }
        else
        {
            f = g - f;
            g = g - f;
            return g;
        }
    }
    int get()
    {
        return g;
    }

    private:
    int f, g;
};

int main()
{
    Fib fib(5);
    cout << fib.get() << endl;
}