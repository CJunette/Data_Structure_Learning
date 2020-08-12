// Zuma_Refined_2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ctime>
using namespace std;

struct Node;
Node *judgeElimi_iter;
Node *judgeElimi_head;

struct Node
{
    char _elem;
    bool _elimi;
    Node *_pred;
    Node *_succ;

    Node(const char &color = '-', bool elimi = false, Node *pred = NULL, Node *succ = NULL):
        _elem(color), _elimi(elimi), _pred(pred), _succ(succ)
    {}

    Node *judgeElimi()
    {
        if(_pred == NULL)
        {
            return this;
        }

        judgeElimi_iter = this;
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
        return judgeElimi_head;
    }

    Node *insertAsPred(const char &color);
    Node *insertAsSucc(const char &color);
};

Node Nodeptr[20004];
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
    Node *eliminate(Node *p);

    void show() const;
};

List::List()
{
    header = &Nodeptr[index++];
    tailer = &Nodeptr[index++];
    header->_pred = NULL;
    header->_succ = tailer;
    header->_elem = ' ';
    tailer->_pred = header;
    tailer->_succ = NULL;
    tailer->_elem = ' ';
    _size = 0;
}

List::~List()
{}


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
    judge_head = p->judgeElimi();
    while(judge_head->_elimi)
    {
        judge_head = eliminate(judge_head);
        judge_head = judge_head->judgeElimi();
    }
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

Node *show_p;
void List::show() const
{
    //单个输出元素会及其费时，因此最好用一个数组数据都集合起来，再输出。
    char s[20004];

    show_p = header->_succ;   
    int i;
    for(i = 0; i < _size; i++)
    {
        //timespec_get(&start, 1);
        //char c = show_p->_elem;
        //timespec_get(&end, 1);
        //time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
        //cout << "time4: " << time << endl;

        //printf("%c", c);
        //putchar(c);        
        
        s[i] = show_p->_elem;

        //timespec_get(&start, 1);
        show_p = show_p->_succ;
        //timespec_get(&end, 1);
        //time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
        //cout << "time6: " << time << endl;
    }
    s[i] = '\0';

    if(_size == 0)
    {
        printf("-\n");
        //cout << "-" << endl;
        return;
    }

    //timespec start, end;
    //double time;
    //timespec_get(&start, 1);
    printf("%s\n", s);
    //timespec_get(&end, 1);
    //time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
    //printf("%f\n", time);
    //printf("\n");
    //cout << endl;
}

int main()
{
    timespec start, end;

    List list;
    char c;   
    /*
    for(scanf("%c", &c); c != '\n' ;)
    {
        list.insertAsLast(c);
        scanf("%c", &c);
    }
    */
    double time = 0;
    do
    {
        //cin.get(c);
        scanf("%c", &c);
        //timespec_get(&start, 1);        
        list.insertAsLast(c);
        //timespec_get(&end, 1);
        //time += (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
    } while(c != '\n');
    //timespec_get(&start, 1);
    list.remove(list.tailer->_pred);
    //timespec_get(&end, 1);
    //time += (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);

    //cout << "time1: " << time << endl;

    int n;
    scanf("%d", &n);

    int loc;
    char color;
    //char sperator;

    for(int i = 0; i < n; i++)
    {

        cin >> loc >> color;

        //timespec_get(&start, 1);
        list.insertWithRank(loc, color);
        //timespec_get(&end, 1);
        //time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
        //cout << "time2: " << time << endl;

        //timespec_get(&start, 1);
        list.show();
        //timespec_get(&end, 1);
        //time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
        //cout << "time3: " << time << endl;
        //cout << endl;
    }
    /*
    while(n > 0)
    {
        //cin >> loc >> color;
        scanf("%d%c%c", &loc, &sperator, &color);
        list.insertWithRank(loc, color);
        list.show();
        n--;
    }
    */
}