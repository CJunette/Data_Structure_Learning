// Zuma_Refined_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

struct Node;
Node *judgeElimi_iter;
Node *judgeElimi_head;
Node *judgeElimi_tail;

struct Node
{
    char _elem;
    bool _elimi;
    Node *_pred;
    Node *_succ;

    Node(const char &color = '-', bool elimi = false, Node *pred = NULL, Node *succ = NULL):
        _elem(color), _elimi(elimi), _pred(pred), _succ(succ)
    {}
    
    Node *judgeElimi(int &size)
    {
        if(_pred == NULL)
        {
            return NULL;
        }
                
        judgeElimi_head = this;
        judgeElimi_tail = this;
        
        int count = 1;

        while(judgeElimi_head->_pred->_elem == judgeElimi_head->_elem)
        {
            judgeElimi_head = judgeElimi_head->_pred;
            count++;
        }
        while(judgeElimi_tail->_succ->_elem == judgeElimi_tail->_elem)
        {
            judgeElimi_tail = judgeElimi_tail->_succ;
            count++;
        }
        if(3 <= count)
        {
            size -= count;
            //删除中间节点。
            judgeElimi_head->_pred->_succ = judgeElimi_tail->_succ;
            judgeElimi_tail->_succ->_pred = judgeElimi_head->_pred;
            return judgeElimi_head->_pred;
        }
        else
        {
            return NULL;
        }

        /*
        while(judgeElimi_iter->_pred->_elem == judgeElimi_iter->_elem)
        {
            judgeElimi_iter = judgeElimi_iter->_pred;            
        }
        int count = 1;
        judgeElimi_head = judgeElimi_iter;
        while(judgeElimi_iter->_succ->_elem == judgeElimi_iter->_elem)
        {
            judgeElimi_iter = judgeElimi_iter->_succ;
            count++;
        }
        if(3 <= count)
        {
            judgeElimi_iter = judgeElimi_head;
            judgeElimi_iter->_elimi = true;
            while(judgeElimi_iter->_succ->_elem == judgeElimi_iter->_elem)
            {
                judgeElimi_iter->_succ->_elimi = true;
                judgeElimi_iter = judgeElimi_iter->_succ;
            }
        }
        */
        return judgeElimi_head;
    }

    Node *insertAsPred(const char &color);
    Node *insertAsSucc(const char &color);
};

Node Nodeptr[100001];
int index = 0;

Node *Node::insertAsPred(const char &color)
{
    Node *p = &Nodeptr[index++];
    p->_pred = _pred;
    p->_succ = this;
    p->_elem = color;

    _pred->_succ = p;
    _pred = p;
    return p;
}

Node *Node::insertAsSucc(const char &color)
{
    Node *p = &Nodeptr[index++];
    p->_pred = this;
    p->_succ = _succ;
    p->_elem = color;

    _succ->_pred = p;
    _succ = p;
    return p;
}

struct List
{
    Node *header, *tailer;
    int _size;
    
    List();
    ~List();
    char remove(Node *p);

    void insertAsLast(const char &color);
    void insertWithRank(int r, const char &color);

    void judge(Node *p);
    //Node *eliminate(Node *p);

    void show() const;
};

List::List()
{
    header = new Node(' ', false, NULL, tailer);
    tailer = new Node(' ', false, header, NULL);
    _size = 0;
}

List::~List()
{
    
    int oldSize = _size;
    while(header->_succ != tailer)
    {
        remove(header->_succ);
    }    
    delete header;
    delete tailer;
}


char List::remove(Node *p)
{
    char c = p->_elem;
    p->_pred->_succ = p->_succ;
    p->_succ->_pred = p->_pred;
    //delete p;
    _size--;
    return c;
}

void List::insertAsLast(const char &color)
{
    _size++;
    tailer->insertAsPred(color);
}

Node *insertWithRank_p;
void List::insertWithRank(int r, const char &color)
{    
    if(r > _size / 2)
    {
        r = _size - r;
        insertWithRank_p = tailer;
        while(r-- > 0)
        {
            insertWithRank_p = insertWithRank_p->_pred;
        }
    }
    else
    {
        insertWithRank_p = header->_succ;
        while(r-- > 0)
        {
            insertWithRank_p = insertWithRank_p->_succ;
        }
    }
    _size++;

    insertWithRank_p = insertWithRank_p->insertAsPred(color);    

    judge(insertWithRank_p);    
}

Node *judge_head;
void List::judge(Node *p)
{
    judge_head = p->judgeElimi(_size);
    while(judge_head)
    {
        judge_head = judge_head->judgeElimi(_size);
    }
    /*
    while(judge_head->_elimi)
    {
        judge_head = eliminate(judge_head);
        judge_head = judge_head->judgeElimi();
    }
    */
}

//返回相同子序列之前的节点。
/*
Node *List::eliminate(Node *p)
{
    while(p->_elimi == true)
    {
        p = p->_succ;
        remove(p->_pred);
    }
    return p->_pred;
}
*/

Node *show_p;
void List::show() const
{
    if(_size == 0)
    {
        //printf("-\n");
        cout << "-" << endl;
        return;
    }
    show_p = header->_succ;
    for(int i = 0; i < _size; i++)
    {
        char c = show_p->_elem;
        //printf("%c", c);
        cout << c;
        show_p = show_p->_succ;
    }
    //printf("\n");
    cout << endl;
}

int main()
{
    List list;

    char c;
    int i = 0;
    do
    {
        //cin.get(c);
        scanf("%c", &c);
        list.insertAsLast(c);
    } while(c != '\n');
    list.remove(list.tailer->_pred);

    int n;
    scanf("%d", &n);

    int loc;
    char color;
    //char sperator;
    for(i = 0; i < n; i++)
    {
        cin >> loc >> color;
        //scanf("%d%c%c", &loc, &sperator, &color);
        list.insertWithRank(loc, color);
        list.show();
    }
}