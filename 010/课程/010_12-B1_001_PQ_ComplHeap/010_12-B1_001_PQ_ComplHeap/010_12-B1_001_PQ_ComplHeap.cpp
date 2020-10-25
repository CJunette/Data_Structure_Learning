// 010_12-B1_001_PQ_ComplHeap.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "PQ_ComplHeap.h"
using namespace std;

int main()
{
    //PQ_ComplHeap一般接口的测试。
    {
        int a1[6] = { 0, 1, 2, 3, 4, 5 };
        PQ_ComplHeap<int> pq1(a1, 6);

        pq1.increase(5, 7);
        pq1.decrease(1, 4);

        int a2[6] = { 10, 3, 9, 1, 2, 8 };
        PQ_ComplHeap<int> pq2(a2, 6);
        pq2.remove(4);
    }

    //对基于完全二叉堆的heapSort()的测试。
    {
        int a[6] = { 5, 1, 2, 4, 3, 0 };
        Vector<int> v(a, 0, 6);
        v.heapSort(0, 6);
    }    
}