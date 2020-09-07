// 005_Homework_5-25.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "BinTree.h"
using namespace std;

template<typename T>
struct MaxSwitch
{
    void operator () (BinNode<T> *n)
    {
        T left, right;
        n->_lc ? left = n->_lc->_data : left = 0;
        n->_rc ? right = n->_rc->_data : right = 0;

        if(!n->_lc && !n->_rc)
        {
            return;
        }
        else
        {
            T max = left < right ? right : left;
            n->_data < max ? n->_data = max : n->_data;
        }
        
    }
};

int main()
{
    BinNode<int> *i = new BinNode<int>(9);
    BinTree<int> tree;
    tree._root = i;
    BinNode<int> *d = tree.insertAsLc(i, 4);
    BinNode<int> *c = tree.insertAsLc(d, 3);
    BinNode<int> *a = tree.insertAsLc(c, 1);
    BinNode<int> *b = tree.insertAsRc(a, 2);

    BinNode<int> *h = tree.insertAsRc(d, 8);
    BinNode<int> *f = tree.insertAsLc(h, 6);
    BinNode<int> *e = tree.insertAsLc(f, 5);
    BinNode<int> *g = tree.insertAsRc(f, 7);

    BinNode<int> *l = tree.insertAsRc(i, 12);
    BinNode<int> *k = tree.insertAsLc(l, 11);
    BinNode<int> *j = tree.insertAsLc(k, 10);
    BinNode<int> *n = tree.insertAsRc(l, 14);
    BinNode<int> *m = tree.insertAsLc(n, 13);
    BinNode<int> *p = tree.insertAsRc(n, 16);
    BinNode<int> *o = tree.insertAsLc(p, 15);

    tree.travPost_I1(i, MaxSwitch<int>());
}
