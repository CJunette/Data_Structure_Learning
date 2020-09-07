// 007_8-D1_001_AVLTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "AVL.h"

int main()
{
    //insert.
    //zagzag
    {
        AVL<int> avl;

        avl.insert(2);
        avl.insert(1);
        avl.insert(4);
        //avl.insert(3);
        avl.insert(5);

        avl.insert(6);
    }
    
    //zigzag
    {
        AVL<int> avl;

        avl.insert(2);
        avl.insert(1);
        avl.insert(5);
        avl.insert(4);
        //avl.insert(6);

        avl.insert(3);
    }

    //zigzig
    {
        AVL<int> avl;

        avl.insert(5);
        avl.insert(6);
        avl.insert(3);
        avl.insert(4);
        avl.insert(2);

        avl.insert(1);
    }

    //zagzig
    {
        AVL<int> avl;
        
        avl.insert(5);
        avl.insert(2);
        avl.insert(6);
        avl.insert(1);
        avl.insert(3);

        avl.insert(4);
    }


    //remove.
    //zigzig
    {
        AVL<int> avl;
        
        avl.insert(6);
        avl.insert(3);
        avl.insert(7);
        avl.insert(2);
        avl.insert(4);
        avl.insert(8);
        avl.insert(1);
        avl.insert(5);

        avl.remove(8);
    }

    //zagzag
    {
        AVL<int> avl;

        avl.insert(3);
        avl.insert(2);
        avl.insert(6);
        avl.insert(1);
        avl.insert(5);
        avl.insert(7);
        avl.insert(4);
        avl.insert(8);

        avl.remove(1);
    }

    //zagzig
    {
        AVL<int> avl;

        avl.insert(5);
        avl.insert(2);
        avl.insert(6);
        avl.insert(1);
        avl.insert(4);
        avl.insert(7);
        avl.insert(3);

        avl.remove(7);
    }

    //zigzag
    {
        AVL<int> avl;

        avl.insert(3);
        avl.insert(2);
        avl.insert(6);
        avl.insert(1);
        avl.insert(5);
        avl.insert(7);
        avl.insert(4);

        avl.remove(1);
    }
}