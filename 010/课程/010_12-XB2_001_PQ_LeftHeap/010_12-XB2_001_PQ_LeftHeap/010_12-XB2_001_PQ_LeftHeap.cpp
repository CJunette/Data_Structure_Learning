// 010_12-XB2_001_PQ_LeftHeap.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "PQ_LeftHeap.h"
using namespace std;

int main()
{
    //对合并算法的测试。
    {
        BinTree<int> a;
        BinTree<int> b;

        BinNode<int> *nodeA = new BinNode<int>(17);
        BinNode<int> *nodeB = new BinNode<int>(15);

        a._root = nodeA;
        b._root = nodeB;

        a.insertAsLc(a._root, 13);
        a.insertAsRc(a._root, 12);
        a.insertAsLc(a._root->_lc, 6);

        b.insertAsLc(b._root, 10);
        b.insertAsRc(b._root, 8);

        a._root->npl = 2;
        a._root->_lc->npl = 1;
        a._root->_rc->npl = 1;
        a._root->_lc->_lc->npl = 1;

        b._root->npl = 2;
        b._root->_lc->npl = 1;
        b._root->_rc->npl = 1;

        merge(a._root, b._root);    
    }

    //对基于合并算法的左式堆相关算法的测试。
    {
        PQ_LeftHeap<int> pq1;
        pq1.insert(13);
        pq1.insert(6);
        pq1.insert(17);
        pq1.insert(12);

        PQ_LeftHeap<int> pq2;
        pq2.insert(15);
        pq2.insert(10);
        pq2.insert(8);

        merge(pq1._root, pq2._root);
        pq1.delMax();
    }    
}