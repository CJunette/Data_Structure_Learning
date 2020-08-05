// 002_Homework_2-34.cpp : This file contains the 'main' function. Program execution begins and ends there.
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
        //3.所以这里不得不使用goto语句，让当k==0的情况跳过检查，保证输入能够进行。
        //4.或者另一种可能可行的做法是在初始化的时候单独对F[0]和T[0]进行设置，使其成为负值。
        if(k == 0)
        {
            goto excute;
        }
        expand(k);
        //2.给创建动态数组赋初始值0会造成一个问题，即第一个对象如果是0的话，会默认其已存在。
        //2.进而导致第一个数如果是0，会无法输入，top不会发生变化。
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
        /*
        if(!valid(F[k]))
        {
            return false;
        }
        */
        //1.如果创建动态对象时使用的int[]()的方法的话，创建出来的动态数组的默认值都是0。
        //1.因此此时不需要valid(F[k])也可以。
        //1.后面我又想了想，这里本身下面都是用&&操作的，根本没有必要用if语句单独判断。
        return valid(F[k]) && (T[F[k]] == k);
    }
    void show() const
    {
        cout << "F: ";
        for(int i = 0; i < N; i++)
        {
            cout << setw(4) <<  F[i] << " ";
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

int main()
{
    Bitmap b;
    b.set(1);
    b.set(3);
    b.set(5);
    b.set(7);
    b.show();    

    b.clear(3);
    b.show();
    cout << b.test(3) << endl;

    b.set(9);
    b.show();
}
