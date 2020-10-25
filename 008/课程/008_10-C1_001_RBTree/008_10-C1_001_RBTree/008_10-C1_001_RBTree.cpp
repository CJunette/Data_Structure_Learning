// 008_10-C1_001_RBTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "RBTree.h"
using namespace std;

int main()
{
    //插入操作
    /*
    {        
        RBTree<int> rbt;
        rbt.insert(5);
        rbt.insert(3);
        rbt.insert(6);
        rbt.insert(2);
        rbt.insert(1);
        rbt.insert(4);
        rbt.show();
    }
    */
    
    //删除非BB情况。
    /*
    {
        RBTree<int> rbt;
        rbt.insert(2);
        rbt.insert(1);

        rbt.remove(1);
        rbt.show();

        rbt.insert(1);
        rbt.remove(2);
        rbt.show();
    }
    */
    //BB1情况1。
    /*
    {
        RBTree<int> rbt;
        rbt.insert(2);
        rbt.insert(1);
        rbt.insert(4);
        rbt.insert(3);
        rbt.show();
        cout << endl;

        rbt.remove(1);
        rbt.show();
    }
    */
    //BB1情况2。
    /*
    {
        RBTree<int> rbt;
        rbt.insert(2);
        rbt.insert(1);
        rbt.insert(3);
        rbt.insert(4);
        rbt.show();
        cout << endl;

        rbt.remove(1);
        rbt.show();
    }
    */

    //BB2-R。
    /*
    {
        RBTree<int> rbt;
        rbt.insert(4);
        rbt.insert(2);
        rbt.insert(5);
        rbt.insert(1);
        rbt.insert(3);
        rbt.search(1)->_color = RB_BLACK;
        rbt.search(2)->_color = RB_RED;
        rbt.search(3)->_color = RB_BLACK;
        rbt.show();
        cout << endl;

        rbt.remove(1);
        rbt.show();
    }
    */
    
    //BB2-B。
    
    {
        RBTree<int> rbt;
        rbt.insert(4);
        rbt.insert(2);
        rbt.insert(6);
        rbt.insert(1);
        rbt.insert(3);
        rbt.insert(5);
        rbt.insert(7);
        rbt.search(2)->_color = RB_BLACK;
        rbt.search(6)->_color = RB_BLACK;
        rbt.search(1)->_color = RB_BLACK;
        rbt.search(3)->_color = RB_BLACK;
        rbt.search(5)->_color = RB_BLACK;
        rbt.search(7)->_color = RB_BLACK;
        rbt.updateHeight(rbt.search(1));
        rbt.updateHeight(rbt.search(3));
        rbt.updateHeight(rbt.search(5));
        rbt.updateHeight(rbt.search(7));
        rbt.updateHeight(rbt.search(2));
        rbt.updateHeight(rbt.search(6));
        rbt.updateHeight(rbt.search(4));
        
        rbt.show();
        cout << endl;

        rbt.remove(1);
        rbt.show();
    }
    

    //BB3。
    /*
    {
        RBTree<int> rbt;
        rbt.insert(2);
        rbt.insert(1);
        rbt.insert(4);
        rbt.insert(3);
        rbt.insert(5);
        rbt.search(1)->_color = RB_BLACK;
        rbt.search(4)->_color = RB_RED;
        rbt.search(3)->_color = RB_BLACK;
        rbt.search(5)->_color = RB_BLACK;
        rbt.updateHeight(rbt.search(3));
        rbt.updateHeight(rbt.search(5));
        rbt.updateHeight(rbt.search(4));
        rbt.updateHeight(rbt.search(1));
        rbt.updateHeight(rbt.search(2));


        rbt.show();
        cout << endl;

        rbt.remove(1);
        rbt.show();
    }
    */
}