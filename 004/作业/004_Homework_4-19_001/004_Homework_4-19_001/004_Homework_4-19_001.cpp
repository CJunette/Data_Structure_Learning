// 004_Homework_4-19_001.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <list>
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

const char prior[N_OPTR][N_OPTR] =
{
    //这里根据练习册p95-4-10，对阶乘与阶乘的优先级进行了修改。
    /*            当        前        运         算        符    */
    /*            +    -    *    /    ^    !    (    )    \0    */
    /*  栈  +  */'>', '>', '<', '<', '<', '<', '<', '>', '>',
    /*      -  */'>', '>', '<', '<', '<', '<', '<', '>', '>',
    /*  顶  *  */'>', '>', '>', '>', '<', '<', '<', '>', '>',
    /*      /  */'>', '>', '>', '>', '<', '<', '<', '>', '>',
    /*  运  ^  */'>', '>', '>', '>', '<', '<', '<', '>', '>',
    /*      !  */'>', '>', '>', '>', '>', '>', ' ', '>', '>',
    /*  算  (  */'<', '<', '<', '<', '<', '<', '<', '=', ' ',
    /*      )  */' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    /*  符  \0 */'<', '<', '<', '<', '<', '<', '<', ' ', '='
};

void append(char *&rpn, double opnd)
{
    char buf[64];
    if(opnd - (int)opnd == 0)
    {
        //输出时有空格分割。
        sprintf(buf, "%d \0", (int)opnd);
    }
    else
    {
        sprintf(buf, "%f \0", opnd);
    }
    rpn = (char *)realloc(rpn, sizeof(char) * (strlen(rpn) + strlen(buf) + 1));
    strcat(rpn, buf);
    //cout << rpn << endl;
}

void append(char *&rpn, char optr)
{
    int n = strlen(rpn);
    rpn = (char *)realloc(rpn, sizeof(char) * (n + 3));
    rpn[n] = optr;
    rpn[n + 1] = ' ';
    rpn[n + 2] = '\0';
}


double evaluate(char *s, char *&RPN)
{
    Stack<double> opnd;
    Stack<char> optr;
    optr.push('\0');

    while(!optr.empty())
    {
        if(isdigit(*s))
        {
            readNum(s, opnd);
            append(RPN, opnd.top());
        }
        {
            switch(prior[opToNum(optr.top())][opToNum(*s)])
            {
                case '>':
                    append(RPN, optr.top());
                    if(optr.top() == '!')
                    {
                        double op = opnd.pop();
                        optr.pop();
                        op = factorial(op);
                        opnd.push(op);
                    }
                    else
                    {
                        double op2 = opnd.pop();
                        double op1 = opnd.pop();
                        char op = optr.pop();
                        double result = calc(op1, op2, op);
                        opnd.push(result);
                    }
                    break;
                case '<':
                    optr.push(*s++);
                    break;
                case '=':
                    optr.pop();
                    s++;
                    break;
                default:
                    break;
            }
        }
    }
    return opnd.pop();
}

char nextOperator(char c)
{
    switch(c)
    {
        case '+':
            return '-';
        case '-':
            return '*';
        case '*':
            //'n'表示null，即两数字间没有运算符。
            return '/';
        case '/':
            return '^';
        case '^':
            return 'n';
        case 'n':
            //'e'表示end，即运算符循环的终点。
            return 'e';
        default:
            break;
    }
}

void showExpression(int n)
{
    char a[] = "0+1+2+3+4+5+6+7+8+9";
    char *s = a;
    char *RPN = new char();

    Stack<char> optr;
    for(int i = 0; i < 9; i++)
    {
        optr.push('+');
    }

    while(!optr.empty())
    {
        if((optr._size < 9) && (optr.top() != 'e'))
        {
            if(optr.top() == 'e')
            {
                continue;
            }

            s[optr._size * 2 - 1] = optr.top();
            optr.push('+');
            s[optr._size * 2 - 1] = '+';
            continue;
        }

        if(optr.top() == 'e')
        {
            s[optr._size * 2 - 1] = 'e';
            optr.pop();
            optr.top() = nextOperator(optr.top());
            continue;
        }

        double result = evaluate(s, RPN);
        if((result != n) && (optr._size == 9))
        {
            char c = nextOperator(optr.top());
            if(c != 'e')
            {
                optr.top() = c;
                s[optr._size * 2 - 1] = c;
                continue;
            }
            else
            {
                optr.top() = c;
                continue;
            }
        }

        if((result == n) && (optr._size == 9))
        {
            for(int i = 0; i < strlen(s); i++)
            {
                if(s[i] == 'n')
                {
                    continue;
                }
                if(!isdigit(s[i]))
                {
                    cout << " " << s[i] << " ";
                }
                else
                {
                    cout << s[i];
                }
            }
            cout << endl;

            char c = nextOperator(optr.top());
            s[optr._size * 2 - 1] = c;
            optr.top() = c;
        }
    }
}

int main()
{
    showExpression(100);
}