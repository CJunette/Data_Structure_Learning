// 004_E-1_001_StackShuffling.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

const int length = 5;

bool stackShuffleCheck(int (&A)[length], int (&B)[length])
{
    Stack<int> S;
    int ia = 0, ib = 0; 
    
    while((ib < length) && (ia < length))
    {
        //跳过两序列开头若干个相同的数。
        if(A[ia] == B[ib])
        {
            ia++;
            ib++;
            continue;
        }
        else
        {
            //当出现不同的数时，B的迭代器不变，A的迭代器后移。同时把每次不同的A中的数据都压入栈中。
            S.push(A[ia++]);
            while(ia < length)
            {
                //如果在A的迭代器后移的过程中遇到了与B相同的数，则同时后移两个迭代器。
                //此时，B的移动后的迭代器所指向的值可能在A迭代器的位置，也可能在栈顶。有且仅有这两种可能（通过模拟移动过程就可以清楚的知道）。
                if(A[ia] == B[ib])
                {
                    ia++;
                    ib++;
                    break;
                }
                else
                {
                    S.push(A[ia]);
                    ia++;
                }
            }

            //如果在A迭代器移动后的位置，可以通过外层循环的if语句去判断。
            //因此此处需要对在栈顶的情况进行判断。
            while((ib < length) && B[ib] == S.top())
            {
                ib++;
                S.pop();                
            }            
        }        
    }

    //当A的迭代器提前到末尾时，栈中可能仍然有未弹出的元素。因此逐一检查。
    while((ib < length) && B[ib] == S.top())
    {
        ib++;
        S.pop();
    }

    //如果检查结束后栈中仍有元素，说明B必然不是A的栈混洗。
    return S.empty();
}

int main()
{
    //A为标准栈。B为混洗栈。
    int A[length] = { 1, 2, 3, 4, 5 };
    int B[length] = { 5, 3, 4, 1, 2 };

    stackShuffleCheck(A, B) ? (cout << "true" << endl) : (cout << "false" << endl);
}