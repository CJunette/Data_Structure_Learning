// 004_Homework_4-19_002.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <list>
#include "List.h"
using namespace std;

template<typename T>
struct Stack
{
    int _index, _size;
    T stack[9];
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
        stack[_index--] = 0;
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

//N_OPTR代表操作符的个数。
const int N_OPTR = 9;

int factorial(int n)
{
    if(n == 0)
    {
        return 1;
    }
    int fac = 1;
    for(int i = 1; i <= n; i++)
    {
        fac = fac * i;
    }
    return fac;
}

template<typename T>
void readNum(char *&s, Stack<T> &opnd)
{
    opnd.push(*s - '0');

integer:
    while(isdigit(*++s))
    {
        T i = opnd.pop() * 10 + (*s - '0');
        opnd.push(i);
        //cout << opnd.top() << endl << endl;

    }
    if(*s == 'n')
    {
        goto integer;
    }
    if(*s == '.')
    {
        double fraction = 1;
        while(isdigit(*++s))
        {
            fraction /= 10;
            T i = opnd.pop() + (*s - '0') * fraction;
            opnd.push(i);
            //cout << setprecision(10) << opnd.top() << endl << endl;

        }
    }
    if(*s == ' ')
    {
        return;
    }
}

int opToNum(char optr)
{
    switch(optr)
    {
        case '+':
            return 0;
            break;
        case '-':
            return 1;
            break;
        case '*':
            return 2;
            break;
        case '/':
            return 3;
            break;
        case '^':
            return 4;
            break;
        case '!':
            return 5;
            break;
        case '(':
            return 6;
            break;
        case ')':
            return 7;
            break;
        case '\0':
            return 8;
            break;
        default:
            return -1;
            break;
    }
}

double calc(double op1, double op2, char op)
{
    switch(op)
    {
        case '+':
            return op1 + op2;
            break;
        case '-':
            return op1 - op2;
            break;
        case '*':
            return op1 * op2;
            break;
        case '/':
            return op1 / op2;
            break;
        case '^':
            return pow(op1, op2);
            break;
        default:
            return 0;
            break;
    }
}

double calcRPN(char *rpn)
{
    Stack<double> nums;
    for(int i = 0; i < strlen(rpn); i++)
    {
        if(isdigit(rpn[i]))
        {
            nums.push(rpn[i] - '0');
        }
        else
        {
            if(rpn[i] == ' ')
            {
                continue;
            }

            if(rpn[i] == '!')
            {
                double opnd = nums.pop();
                opnd = factorial(opnd);
                nums.push(opnd);
            }
            else
            {
                double opnd2 = nums.pop();
                double opnd1 = nums.pop();
                double result = calc(opnd1, opnd2, rpn[i]);
                nums.push(result);
            }
        }
    }    
    return nums.pop();
}

//对“+”和“-”的优先度稍作了调整。
//[栈顶][当前]
const char priorForTrans[N_OPTR][N_OPTR] =
{
    //这里根据练习册p95-4-10，对阶乘与阶乘的优先级进行了修改。
    /*            当        前        运         算        符    */
    /*            +    -    *    /    ^    !    (    )    \0    */
    /*  栈  +  */'<', '<', '<', '<', '<', '<', '<', '>', '>',
    /*      -  */'<', '<', '<', '<', '<', '<', '<', '>', '>',
    /*  顶  *  */'>', '>', '>', '>', '<', '<', '<', '>', '>',
    /*      /  */'>', '>', '>', '>', '<', '<', '<', '>', '>',
    /*  运  ^  */'>', '>', '>', '>', '<', '<', '<', '>', '>',
    /*      !  */'>', '>', '>', '>', '>', '>', ' ', '>', '>',
    /*  算  (  */'<', '<', '<', '<', '<', '<', '<', '=', ' ',
    /*      )  */' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    /*  符  \0 */'<', '<', '<', '<', '<', '<', '<', ' ', '='
};

struct Equation
{
    int _value;
    char _equa[128];
    char _lastOptr;
    Equation(double v = -1):
        _value(v)
    {
        _lastOptr = '!';
        sprintf(_equa, "%d\0", _value);
    }
    /*
    Equation(int v = 0):
        _value(v)
    {
        sprintf(_equa, "%d\0", (int)_value);
    }
    */
};

void postfixToInfix(char *rpn)
{
    Stack<int> nums;
    Stack<Equation> equation;

    int i = 0;
    while(i < strlen(rpn))    
    {
        char c = rpn[i];
        if(isdigit(rpn[i]))
        {
            char *p = &rpn[i];
            char *oldP = p;
            readNum(p, nums);
            i += p - oldP;
            int num = nums.top();
            equation.push(Equation(num));
        }
        else
        {
            if(rpn[i] == ' ')
            {
                i++;
                continue;
            }

            if(rpn[i] == '!')
            {
                double opnd = nums.pop();
                opnd = factorial(opnd);
                nums.push(opnd);

                Equation e = equation.pop();
                e._value = opnd;                
                int len = strlen(e._equa);
                e._equa[len] = '!';
                e._equa[len + 1] = '\0';
                e._lastOptr = '!';
                equation.push(e);
            }
            else
            {
                double opnd2 = nums.pop();
                double opnd1 = nums.pop();
                double result = calc(opnd1, opnd2, rpn[i]);
                nums.push(result);

                Equation e2 = equation.pop();
                Equation e1 = equation.pop();
                e1._value = result;

                int len1 = strlen(e1._equa);
                int len2 = strlen(e2._equa);

                int offset = 0;

                if(priorForTrans[opToNum(e1._lastOptr)][opToNum(rpn[i])] == '<')
                {
                    for(int i = len1; 0 < i; i--)
                    {
                        e1._equa[i] = e1._equa[i - 1];
                    }
                    e1._equa[0] = '(';
                    e1._equa[len1 + 1] = ')';
                    len1 += 2;

                }
                if(priorForTrans[opToNum(e2._lastOptr)][opToNum(rpn[i])] == '<')
                {
                    for(int i = len2; 0 < i; i--)
                    {
                        e2._equa[i] = e2._equa[i - 1];
                    }
                    e2._equa[0] = '(';
                    e2._equa[len2 + 1] = ')';
                    len2 += 2;
                }

                if(rpn[i] == '^')
                {
                    e1._equa[len1] = rpn[i];
                    offset += 1;
                }
                else
                {
                    e1._equa[len1] = ' ';
                    e1._equa[len1 + 1] = rpn[i];
                    e1._equa[len1 + 2] = ' ';
                    offset += 3;
                }
                for(int i = 0; i < len2; i++)
                {
                    e1._equa[len1 + offset + i] = e2._equa[i];
                }

                e1._equa[len1 + offset + len2] = '\0';
                e1._lastOptr = rpn[i];
                equation.push(e1);
            }
            //cout << equation.top()._equa << endl;
        }
        i++;
    }
    cout << equation.top()._equa << endl;
}

int main()
{
    //char rpn[] = "1 2 + 3 4 ^ *";
    char rpn[] = "0 ! 1 + 2 3 ! 4 + ^ * 5 ! 67 - 8 9 + - -";
    char *r = rpn;
    calcRPN(r);
    postfixToInfix(r);
}
