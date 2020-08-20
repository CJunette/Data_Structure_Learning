// 004_F-2_001_CalcOperator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <list>
using namespace std;

template<typename T>
struct Stack
{
    int _index, _size;
    //T stack[20];
    T *stack;

    Stack(int capacity = 100):
        _index(-1), _size(0)
    {
        stack = new T[capacity];
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
    /*
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
    */
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
    //cout << opnd.top() << endl << endl;

    //这里不需要写“s = ++s”，对指针的自增操作使得指针本身会直接发生变化。
    while(isdigit(*++s))
    {
        T i = opnd.pop() * 10 + (*s - '0');
        opnd.push(i);
        //cout << opnd.top() << endl << endl;

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

//prior[栈顶][当前]
//这里的大于号和小于号是针对栈顶运算符对当前运算符的优先级的。当栈顶运算符优先级大于当前运算符时，用“>”。
//空格说明这种情况不可能发生。
//等号只在左右括号相邻时，或首尾“\0”相邻时出现。
const char prior[N_OPTR][N_OPTR] =
{
    /*            当        前        运         算        符    */
    /*            +    -    *    /    ^    !    (    )    \0    */
    /*  栈  +  */'>', '>', '<', '<', '<', '<', '<', '>', '>',
    /*      -  */'>', '>', '<', '<', '<', '<', '<', '>', '>',
    /*  顶  *  */'>', '>', '>', '>', '<', '<', '<', '>', '>',
    /*      /  */'>', '>', '>', '>', '<', '<', '<', '>', '>',
    /*  运  ^  */'>', '>', '>', '>', '>', '<', '<', '>', '>',
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


//第二个参数RPN在函数中没有被用到，但书上写着这个参数，所以我也照搬了下来。
//后面发现，这个算法还带有将一般的中缀表达式转化为RPN表达式的功能。
//注意，字符串中运算符与数字间不能有空格，因为我这边没有设置空格的处理。
double evaluate(char *s, char *&RPN)
{
    Stack<double> opnd;
    Stack<char> optr;
    optr.push('\0');
    //由于已经给操作符栈optr添加了一个符号“\0”了，因此之后当操作符栈再次为空时，说明栈底的“\0”已经碰到了字符串尾的“\0”，此时表达式的处理已经结束，操作数栈中剩余的元素就是结果。
    while(!optr.empty())
    {
        if(isdigit(*s))
        {
            readNum(s, opnd);
            append(RPN, opnd.top());
        }
        else
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

int main()
{
    char a[] = "(1+2^3!-4)*(5!-(6-(7-(89-0!))))";
    char *s = a;

    char b[] = "(0!+1)*2^(3!+4)-(5!-67-(8+9))";
    char *c = b;

    char *RPN = new char();

    Stack<double> stack;

    double result = evaluate(c, RPN);
    cout << result << endl;

    cout << RPN << endl;
}