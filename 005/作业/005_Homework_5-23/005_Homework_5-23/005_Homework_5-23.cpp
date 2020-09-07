// 005_Homework_5-23.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "BinTree.h"
using namespace std;

template<typename T>
struct Swap
{
    void operator () (BinNode<T> *node)
    {
        BinNode<T> *temp;
        temp = node->_lc;
        node->_lc = node->_rc;
        node->_rc = temp;
    }
};

int main()
{
    BinNode<char> *root = new BinNode<char>('i');
    BinTree<char> tree;
    tree._root = root;
    BinNode<char> *d = tree.insertAsLc(root, 'd');
    BinNode<char> *c = tree.insertAsLc(d, 'c');
    BinNode<char> *a = tree.insertAsLc(c, 'a');
    BinNode<char> *b = tree.insertAsRc(a, 'b');

    BinNode<char> *h = tree.insertAsRc(d, 'h');
    BinNode<char> *f = tree.insertAsLc(h, 'f');
    BinNode<char> *e = tree.insertAsLc(f, 'e');
    BinNode<char> *g = tree.insertAsRc(f, 'g');

    BinNode<char> *l = tree.insertAsRc(root, 'l');
    BinNode<char> *k = tree.insertAsLc(l, 'k');
    BinNode<char> *j = tree.insertAsLc(k, 'j');
    BinNode<char> *n = tree.insertAsRc(l, 'n');
    BinNode<char> *m = tree.insertAsLc(n, 'm');
    BinNode<char> *p = tree.insertAsRc(n, 'p');
    BinNode<char> *o = tree.insertAsLc(p, 'o');

    tree.travPre_I2(root, Swap<char>());
}
