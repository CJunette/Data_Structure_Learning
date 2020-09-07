// Train.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

template<typename T>
struct Stack
{
    int size;
    int topPtr;
    int capacity;
    T *stack;

    Stack(int capacity_):
        size(0), topPtr(-1), capacity(capacity_)
    {
        stack = new T[capacity_];
    }
    ~Stack()
    {
        delete[] stack;
    }

    bool empty() const { return size == 0; }
    bool full() const { return size == capacity; }

    bool push(const T &e)
    {
        if(full())
        {
            return false;
        }
        else
        {
            stack[++topPtr] = e;
            ++size;
            return true;
        }
    }

    T pop()
    {
        if(!empty())
        {
            T temp = stack[topPtr];
            stack[topPtr--] = 0;
            size--;
            return temp;
        }        
    }

    T &top()
    {
        return stack[topPtr];
    }
};

template<typename T>
struct Queue
{
    int size;
    int capacity;
    int _front, _end;
    T *queue;

    Queue(int capacity_):
        capacity(capacity_), size(0), _front(0), _end(0)
    {
        queue = new T[capacity_];
    }
    ~Queue()
    {
        delete[] queue;
    }

    bool empty() const { return size == 0; }
    
    void enqueue(const T &e)
    {
        queue[_end++] = e;
        size++;
    }

    T dequeue()
    {
        T temp = queue[_front];
        queue[_front++] = 0;
        size--;
        return temp;
    }

    T &front()
    {
        return queue[front];
    }
};

int main()
{
    //n代表列车车厢数，m代表栈的容量。
    int m, n;

    cin >> n >> m;

    int *test = new int[n];
    Stack<int> origin(n);
    Queue<char *> operation(2 * n);

    char push[] = "push";
    char pop[] = "pop";

    for(int i = 0; i < n; i++)
    {        
        //scanf("%d", &test[i]);   
        cin >> test[i];

        //用于比较的数需要逆向压入栈。
        origin.push(n - i);        
    }

    Stack<int> store(m);

    int i = 0;    
    bool fail = false;
    while(!origin.empty())
    {
        //test的第i位数字可能与origin中的顶端数字重合。
        if((test[i] == origin.top()) && !store.full())
        {
            i++;            
            operation.enqueue(push);
            operation.enqueue(pop);
            origin.pop();
        }
        //test的第i位数字也可能与store顶端的数字重合。
        else if(!store.empty() && store.top() == test[i])
        {
            i++;
            operation.enqueue(pop);
            store.pop();
        }
        //如果test的第i为数字及不与origin的顶端数字重合，也不予store的顶端数字重合，则说明该数字可能还在origin的更后方，因此将origin的顶端数字压入栈。
        else
        {
            if(store.full())
            {
                fail = true;
                break;
            }
            else
            {
                store.push(origin.pop());
                operation.enqueue(push);                
            }
        }
    }
    
    if(fail)
    {
        printf("No\n");
        delete[] test;
        return 0;
    }

    while(!store.empty())
    {
        if(test[i++] == store.pop())
        {
            operation.enqueue(pop);
        }
        else
        {
            fail = true;
            delete[] test;
            break;
        }
    }

    if(fail)
    {
        printf("No\n");        
        return 0;
    }

    while(!operation.empty())
    {
        printf("%s\n", operation.dequeue());
        //cout << operation.dequeue() << endl;
    }
    delete[] test;
}
