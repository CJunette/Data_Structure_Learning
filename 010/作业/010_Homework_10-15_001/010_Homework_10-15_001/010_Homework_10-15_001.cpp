// 010_Homework_10-15_001.cpp : This file contains the 'main' function. Program execution begins and ends there.
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
}
