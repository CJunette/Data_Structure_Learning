// ProperRebuild.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

template<typename T>
struct Stack
{
    int size;
    int topPtr;
    int capacity;
    T *stack;

    Stack(int capacity_):
        size(0), topPtr(-1), capacity(capacity_)
    {
        stack = new T[capacity_];
    }
    ~Stack()
    {
        delete[] stack;
    }

    bool empty() const { return size == 0; }
    bool full() const { return size == capacity; }

    bool push(const T &e)
    {
        if(full())
        {
            return false;
        }
        else
        {
            stack[++topPtr] = e;
            ++size;
            return true;
        }
    }

    T pop()
    {
        if(!empty())
        {
            T temp = stack[topPtr];
            stack[topPtr--] = 0;
            size--;
            return temp;
        }
    }

    T &top()
    {
        return stack[topPtr];
    }
};

struct Sequence
{
    int left;
    int right;
    int offset;
    int *seque;
    Sequence(int left_ = 0, int right_ = 0, int offset_ = 0, int *seque_ = NULL):
        left(left_), right(right_), offset(offset_), seque(seque_)
    {}
    int &operator[] (int n)
    {
        return seque[n];
    }
    int size() const
    {
        return right - left;
    }
};

void toMid(Sequence &first, Sequence &last, int *result, int size)
{
    Stack<Sequence> firstStack(size);
    Stack<Sequence> lastStack(size);

    firstStack.push(first);
    lastStack.push(last);

    while(!firstStack.empty())
    {
        Sequence fr = firstStack.pop();
        Sequence ls = lastStack.pop();

        if(fr.size() == 1)
        {
            result[fr.offset] = fr[0];
            continue;
        }
        else
        {
            int Node = fr[0];
            int NodeTL = fr[1];
            int NodeTR = ls[ls.size() - 2];

            int i = 0;
            int size = fr.right - fr.left;
            for(i; i < size; i++)
            {
                if(fr[i] == NodeTR)
                {
                    break;
                }
            }

            int j = 0;
            for( j; j < size; j++)
            {
                if(ls[j] == NodeTL)
                {
                    break;
                }
            }

            int pos = fr.offset + fr.left + i - (fr.left + 1);
            result[pos] = Node;

            Sequence frTL(fr.left + 1, fr.left + i, fr.offset, &fr[1]);
            Sequence frTR(fr.left + i, fr.right, pos + 1, &fr[i]);

            Sequence lsTL(ls.left, ls.left + j + 1, fr.offset, &ls[0]);
            Sequence lsTR(ls.left + j + 1, ls.right - 1, pos + 1, &ls[j + 1]);

            firstStack.push(frTR);
            firstStack.push(frTL);
            lastStack.push(lsTR);
            lastStack.push(lsTL);
        }        
    }
}

int main()
{
    int n;
    cin >> n;

    int *first = new int[n];
    int *last = new int[n];

    for(int i = 0; i < n; i++)
    {
        scanf("%d", &first[i]);
    }
    for(int i = 0; i < n; i++)
    {
        scanf("%d", &last[i]);
    }

    int *result = new int[n]();    

    Sequence firstSeq(0, n, 0, first);
    Sequence lastSeq(0, n, 0, last);

    toMid(firstSeq, lastSeq, result, n);
    delete[] first;
    delete[] last;

    for(int i = 0; i < n; i++)
    {
        printf("%d ", result[i]);
    }
    delete[] result;
}