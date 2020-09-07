// 005_Homework_5-26.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "BinTree.h"
using namespace std;

template<typename T>
struct SetValue
{
    void operator() (BinNode<T> *n)
    {
        if(n->_lc)
        {
            n->_lc->_data = n->_data * 2 + 1;
        }
        if(n->_rc)
        {
            n->_rc->_data = n->_data * 2 + 2;
        }
    }
};

int main()
{
    BinNode<int> *i = new BinNode<int>(0);
    BinTree<int> tree;
    tree._root = i;
    BinNode<int> *d = tree.insertAsLc(i, 0);
    BinNode<int> *c = tree.insertAsLc(d, 0);
    BinNode<int> *a = tree.insertAsLc(c, 0);
    BinNode<int> *b = tree.insertAsRc(a, 0);
                                         
    BinNode<int> *h = tree.insertAsRc(d, 0);
    BinNode<int> *f = tree.insertAsLc(h, 0);
    BinNode<int> *e = tree.insertAsLc(f, 0);
    BinNode<int> *g = tree.insertAsRc(f, 0);
                                         
    BinNode<int> *l = tree.insertAsRc(i, 0);
    BinNode<int> *k = tree.insertAsLc(l, 0);
    BinNode<int> *j = tree.insertAsLc(k, 0);
    BinNode<int> *n = tree.insertAsRc(l, 0);
    BinNode<int> *m = tree.insertAsLc(n, 0);
    BinNode<int> *p = tree.insertAsRc(n, 0);
    BinNode<int> *o = tree.insertAsLc(p, 0);

    tree.travPre_I2(i, SetValue<int>());
}