// 004_XC-1_001_FindLargestRectangle.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stack>
using namespace std;

template<size_t N>
int maxArea1(int (&H)[N])
{
    int left[N];
    int right[N];

    //栈用于保存当前矩形之前的可能的左界或右界（以秩来确定）。
    //这里统一认为矩形的左侧边为秩。
    stack<int> stack;

    //寻找左界。
    for(int i = 0; i < N; i++)
    {
        //当遇见当前矩形的高度比栈顶矩形的高度低时，弹出栈顶，直到栈为空或栈顶矩形的高度小于当前矩形的高度。
        //此时说明栈顶矩形（若存在）或最左端零点位置，就是当前矩形计算面积时的左界。
        while(!stack.empty() && H[i] <= H[stack.top()])
        {
            stack.pop();
        }

        if(!stack.empty() && H[stack.top()] < H[i])
        {
            left[i] = stack.top() + 1;
            stack.push(i);
            continue;
        }        

        if(stack.empty())
        {
            stack.push(i);
            left[i] = 0;
            continue;
        }
    }
    
    while(!stack.empty())
    {
        stack.pop();
    }

    //寻找右界。
    //思路与上面寻找左界时相同。
    for(int i = N - 1; 0 <= i; i--)
    {
        while(!stack.empty() && H[i] <= H[stack.top()])
        {
            stack.pop();
        }

        //需要注意的是这里的右界的标记。
        right[i] = stack.empty() ? N : stack.top();
        stack.push(i);
    }

    int max = 0;
    for(int i = 0; i < N; i++)
    {
        int area = H[i] * (right[i] - left[i]);
        if(max < area)
        {
            max = area;
        }
    }
    return max;
}

int maxArea(int left, int right, int hight, int max)
{    
    int area = hight * (right - left);
    if(max < area)
    {
        max = area;
    }
    return max;
}

template<size_t N>
int maxArea2(int (&H)[N])
{
    stack<int> stack;
    int max = 0;

    for(int i = 0; i < N; i++)
    {
        while(!stack.empty() && H[i] < H[stack.top()])
        {
            //当前i代表遇到高度下落的情况时，较低的那个矩形的秩。
            //而栈顶的元素则代表前面的较高矩形的秩。
            int hight = H[stack.top()];
            stack.pop();
            //左界是弹出一个元素后栈中的元素（或0）。
            int left = stack.empty() ? 0 : stack.top() + 1;
            int right = i;
            max = maxArea(left, right, hight, max);
        }
        stack.push(i);
    }

    while(!stack.empty())
    {
        int right = N;
        int hight = H[stack.top()];
        stack.pop();
        int left = stack.empty() ? 0 : stack.top() + 1;
        max = maxArea(left, right, hight, max);
    }

    return max;
}

int main()
{
    int a1[] = { 3, 4, 1, 6, 5, 2 };
    int a2[] = { 2, 1, 5, 6, 2, 3 };
    int a3[] = { 1, 1 };
    int a4[] = { 5, 4, 5, 4, 5 };
    cout << maxArea1(a4) << endl;
    cout << maxArea2(a4) << endl;
}