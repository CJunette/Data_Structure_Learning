// Zuma.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

struct Node
{
    char _elem;
    bool _elimi;
    Node *_pred;
    Node *_succ;    

    Node(const char& color, bool elimi = false, Node* pred = NULL, Node* succ = NULL):
        _elem(color), _elimi(elimi), _pred(pred), _succ(succ)
    {}

    Node* judgeElimi()
    {
        if(_pred == NULL)
        {
            return this;
        }

        Node *iter = this;
        while(iter->_pred->_elem == iter->_elem)
        {
            iter = iter->_pred;
        }
        int count = 1;
        Node *head = iter;
        while(iter->_succ->_elem == iter->_elem)
        {
            iter = iter->_succ;
            count++;
        }
        if(3 <= count)
        {
            iter = head;
            iter->_elimi = true;
            while(iter->_succ->_elem == iter->_elem)
            {
                iter->_succ->_elimi = true;
                iter = iter->_succ;                
            }
        }
        return head;
    }

    Node *insertAsPred(const char &color)
    {
        Node *p = new Node(color, false, _pred, this);
        _pred->_succ = p;
        _pred = p;
        return p;
    }

    Node *insertAsSucc(const char &color)
    {
        Node *p = new Node(color, false, this, _succ);
        _succ->_pred = p;
        _succ = p;
        return p;
    }
};

struct List
{
    Node *header, *tailer;
    int _size;
    int _continuity;

    List();
    ~List();
    int clear();
    char remove(Node *p);

    /*
    void insertB(Node* p, const char &color);
    void insertA(Node *p, const char &color);
    void insertAsFirst(const char &color);
    */
    void insertAsLast(const char &color);
    void insertWithRank(int r, const char &color);

    void judge(Node *p);
    Node *eliminate(Node *p);
    /*
    Node *first() const;
    Node *last() const;
    */
    void show() const;
};

List::List()
{
    header = new Node(' ', false, NULL, tailer);
    tailer = new Node(' ', false, header, NULL);
    _size = 0;
    _continuity = 1;
}

List::~List()
{
    clear();
    delete header;
    delete tailer;
}

int List::clear()
{
    int oldSize = _size;
    while(header->_succ != tailer)
    {
        remove(header->_succ);
    }
    return oldSize - _size;
}

char List::remove(Node *p)
{
    char c = p->_elem;
    p->_pred->_succ = p->_succ;
    p->_succ->_pred = p->_pred;
    delete p;
    _size--;
    return c;
}
/*
void List::insertB(Node *p, const char &color)
{
    _size++;
    Node *n = p->insertAsPred(color);
    judge(n);
    //return p->insertAsPred(color);
}

void List::insertA(Node *p, const char &color)
{
    _size++;
    Node *n = p->insertAsSucc(color);
    judge(n);
    //return p->insertAsSucc(color);
}

void List::insertAsFirst(const char &color)
{
    _size++;
    Node *p = header->insertAsSucc(color);
    judge(p);
    //return header->insertAsSucc(color);
}
*/
void List::insertAsLast(const char &color)
{
    _size++;    
    Node *p = tailer->insertAsPred(color);
    //任意三个相邻珠子不同色，因此不用考虑。
    /*
    if(p->_elem == p->_pred->_elem)
    {
        _continuity++;
    }
    else
    {
        if(3 <= _continuity)
        {
            judge(p->_pred);
        }
        _continuity = 1;
    }
    */
    //return tailer->insertAsPred(color);
}

void List::insertWithRank(int r, const char &color)
{
    Node *p;
    if(r > _size / 2)
    {
        r = _size - r;
        p = tailer;
        while(r-- > 0)
        {
            p = p->_pred;
        }
    }
    else
    {
        p = header->_succ;
        while(r-- > 0)
        {
            p = p->_succ;
        }
    }

    _size++;

    p = p->insertAsPred(color);
    judge(p);
}

void List::judge(Node *p)
{
    Node *head = p->judgeElimi();
    while(head->_elimi)
    {
        head = eliminate(head);        
        head = head->judgeElimi();
    }
    //解决“AAAAAABBBBBB”的输入情况。
    //只在输入时发生。通过主程序中的“list.judge(list.header);”代替。
    //似乎输入中不会出现这个情况。题干中提到“其中任意三个相邻的珠子不会完全同色”。
    /*
    if((head == header) && (0 < _size))
    {
        judge(head->_succ);
    }
    */
}

//返回相同子序列之前的节点。
Node *List::eliminate(Node *p)
{
    while(p->_elimi == true)
    {
        p = p->_succ;
        remove(p->_pred);
    }
    return p->_pred;
}
/*
Node *List::first() const
{
    return header->_succ;
}

Node *List::last() const
{
    return tailer->_pred;
}
*/
void List::show() const
{
    if(_size == 0)
    {
        //printf("-\n");
        cout << "-" << endl;
        return;
    }
    Node *p = header->_succ;
    for(int i = 0; i < _size; i++)
    {
        char c = p->_elem;
        //printf("%c", c);
        cout << c;
        p = p->_succ;
    }
    //printf("\n");
    cout << endl;
}

int main()
{
    List list;
    //如果开始的输入为空，则scanf("%s")的方法是无效的。
    //char s[10001];
    /*
    scanf("%s", s);
    */
    char c;
    int i = 0;
    do
    {        
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