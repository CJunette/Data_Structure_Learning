// 002_Homework_2-36.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <iomanip>
using namespace std;

class Bitmap
{
    private:
    int *F, N;
    int *T, top;

    protected:
    bool valid(int k)
    {
        return (0 <= k) && (k < top);
    }
    bool erased(int k)
    {
        return valid(F[k]) && (T[F[k]] + 1 + k == 0);
    }
    void expand(int k)
    {
        if(k > N - 1)
        {
            int *tempF = F;
            F = new int[k * 2]();
            for(int i = 0; i < N; i++)
            {
                F[i] = tempF[i];
            }
            int *tempT = T;
            T = new int[k * 2]();
            for(int i = 0; i < N; i++)
            {
                T[i] = tempT[i];
            }
            N = 2 * k;
            delete[] tempF, tempT;
        }
    }

    public:
    Bitmap(int n = 8)
    {
        N = n;
        F = new int[N]();
        T = new int[N]();
        top = 0;
    }
    ~Bitmap()
    {
        delete[] F;
        delete[] T;
    }

    void set(int k)
    {
        if(k == 0)
        {
            goto excute;
        }
        expand(k);        
        if(test(k) || (k < 0))
        {
            return;
        }
        excute:
        if(!erased(k))
        {
            F[k] = top++;
        }
        T[F[k]] = k;
    }
    void clear(int k)
    {
        if(test(k))
        {
            T[F[k]] = -1 - k;
        }
    }
    bool test(int k)
    {
        return valid(F[k]) && (T[F[k]] == k);
    }
    void show() const
    {
        cout << "F: ";
        for(int i = 0; i < N; i++)
        {
            cout << setw(4) << F[i] << " ";
        }
        cout << endl;
        cout << "T: ";
        for(int i = 0; i < N; i++)
        {
            cout << setw(4) << T[i] << " ";
        }
        cout << endl << endl;
    }
};

void calcEratosthenes(int n)
{
    Bitmap b;
    b.set(0);
    b.set(1);
    for(int i = 2; i < sqrt(n); i++)
    {
        if(!b.test(i))
        {
            for(int j = i * i; j < n; j += i)
            {
                b.set(j);
            }
        }
    }
    b.show();

    cout << "Prime number under " << n << " : ";
    for(int i = 0; i < n; i++)
    {
        if(!b.test(i))
        {
            cout << i << " ";
        }
    }
    cout << endl << endl;
}

int main()
{
    calcEratosthenes(30);

    
}