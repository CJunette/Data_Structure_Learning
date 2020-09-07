// 005_D-1_001_BinaryTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "BinTree.h"

template<typename T>
struct Show
{
    void operator() (T *t)
    {
        cout << t->_data << " ";
    }
};

int main()
{
    
    {
        BinNode<char> *root1 = new BinNode<char>('a');
        BinTree<char> tree1;
        tree1._root = root1;
        tree1.insertAsLc(root1, 'b');
        BinNode<char> *c = tree1.insertAsRc(root1, 'c');
        BinNode<char> *d = tree1.insertAsLc(c, 'd');
        BinNode<char> *e = tree1.insertAsRc(d, 'e');
        BinNode<char> *f = tree1.insertAsRc(c, 'f');

        //tree1.travPre_I1(root1, Show<BinNode<char>>());
    }
    

    {
        BinNode<char> *root2 = new BinNode<char>('i');
        BinTree<char> tree2;
        tree2._root = root2;
        BinNode<char> *d = tree2.insertAsLc(root2, 'd');
        BinNode<char> *c = tree2.insertAsLc(d, 'c');
        BinNode<char> *a = tree2.insertAsLc(c, 'a');
        BinNode<char> *b = tree2.insertAsRc(a, 'b');

        BinNode<char> *h = tree2.insertAsRc(d, 'h');
        BinNode<char> *f = tree2.insertAsLc(h, 'f');
        BinNode<char> *e = tree2.insertAsLc(f, 'e');
        BinNode<char> *g = tree2.insertAsRc(f, 'g');

        BinNode<char> *l = tree2.insertAsRc(root2, 'l');
        BinNode<char> *k = tree2.insertAsLc(l, 'k');
        BinNode<char> *j = tree2.insertAsLc(k, 'j');
        BinNode<char> *n = tree2.insertAsRc(l, 'n');
        BinNode<char> *m = tree2.insertAsLc(n, 'm');
        BinNode<char> *p = tree2.insertAsRc(n, 'p');
        BinNode<char> *o = tree2.insertAsLc(p, 'o');

        cout << "travMid_I1:\t\t";
        tree2.travMid_I1(root2, Show<BinNode<char>>());
        cout << endl;

        //当x指向的目标是a时，其前驱为空，因此下面的输出语句需要单独列出一种情况。
        BinNode<char> *x = p;
        cout << "pred of " << x->_data << " : ";        
        x->pred() ? cout << x->pred()->_data << endl : cout << "NULL" << endl;
        

        cout << "travPost_Customed_I1:\t";
        tree2.travPost_Customed_I1(root2, Show<BinNode<char>>());
        cout << endl;

        cout << "travPost_Customed_I2:\t";
        tree2.travPost_Customed_I2(root2, Show<BinNode<char>>());
        cout << endl;

        cout << "travPost_I1:\t\t";
        tree2.travPost_I1(root2, Show<BinNode<char>>());
        cout << endl;

        cout << "travLevelCustomed:\t";
        tree2.travLevel_Custom(Show<BinNode<char>>());
        cout << endl;

        cout << "travLevel:\t\t";
        tree2.travLevel(Show<BinNode<char>>());
        cout << endl;
    }
    
    cout << endl;
}
