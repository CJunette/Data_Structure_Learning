// 004_4-4_002_Maze.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

const int LABY_MAX = 8;

enum Status
{
    AVAILABLE, ROUTE, BACKTRACED, WALL, STARTOREND
};

enum ESWN
{
    UNKNOWN, EAST, SOUTH, WEST, NORTH, NOWAY
};

ESWN next(ESWN eswn)
{
    return (ESWN)(eswn + 1);
}

struct Cell
{
    int _x, _y;
    Status _status;
    ESWN _incoming, _outgoing;

    Cell(int x = -1, int y = -1, Status status = AVAILABLE, ESWN incoming = UNKNOWN, ESWN outgoing = UNKNOWN):
        _x(x), _y(y), _status(status), _incoming(incoming), _outgoing(outgoing)
    {}
};

template<typename T>
struct Stack
{
    int _index, _size;
    T stack[LABY_MAX * LABY_MAX];
    //T *stack;

    Stack(int capacity = 100):
        _index(-1), _size(0)
    {
        //stack = new T[capacity];
    }

    void push(T const &e)
    {
        stack[++_index] = e;
        _size++;
    }

    T pop()
    {
        T e = stack[_index];
        stack[_index--] = T();
        _size--;
        return e;
    }

    T &top()
    {
        return stack[_index];
    }

    bool empty() const
    {
        return _size == 0;
    }
};

Cell *neighbor(Cell *cell)
{
    switch(cell->_outgoing)
    {
        case EAST:
            return cell + 1;
            break;
        case SOUTH:
            return cell + LABY_MAX;
            break;
        case WEST:
            return cell - 1;
            break;
        case NORTH:
            return cell - LABY_MAX;
            break;
        default:
            break;
    }
}

void printMaze(Cell maze[LABY_MAX][LABY_MAX])
{
    for(int i = 0; i < LABY_MAX; i++)
    {
        for(int j = 0; j < LABY_MAX; j++)
        {
            switch(maze[i][j]._status)
            {
                case WALL:
                    cout << " ||";
                    break;
                case AVAILABLE:
                    cout << " __";
                    break;
                case ROUTE:
                    cout << " ++";
                    break;
                case BACKTRACED:
                    cout << " **";
                    break;
                case STARTOREND:
                    cout << " --";
                    break;
                default:
                    break;
            }
        }
        cout << endl;
    }
    cout << endl;
}

bool findWayOut(Cell laby[LABY_MAX][LABY_MAX], Cell *start, Cell *terminal)
{
    if(start->_status == WALL || terminal->_status == WALL)
    {
        return false;
    }

    //如果入口点即出口点，则无需继续，直接返回。
    if(start == terminal)
    {
        return true;
    }
    
    Stack<Cell*> path;
    Cell *p= start;
    //p->_outgoing = next(p->_outgoing);    
    p->_status = ROUTE;
    //path.push(p);
    //p = neighbor(p);
    printMaze(laby);

search:
    do
    {
        if(p == terminal)
        {
            return true;
        }

        p->_outgoing = next(p->_outgoing);
        
        while(p->_outgoing < NOWAY)
        {
            Cell *neighborCell = neighbor(p);
            if((neighborCell->_status == AVAILABLE) || (neighborCell->_status == STARTOREND))
            {
                path.push(p);                
                p->_status = ROUTE;

                p = neighborCell;
                printMaze(laby);
                goto search;
            }
            else
            {
                p->_outgoing = next(p->_outgoing);
            }
        }
        p->_status = BACKTRACED;
        if(!path.empty())
        {
            p = path.pop();
        }
        printMaze(laby);
    } while(!path.empty() || (start->_outgoing < NOWAY));
}

int main()
{
    //周围造墙
    Cell maze[LABY_MAX][LABY_MAX];
    for(int i = 0; i < LABY_MAX; i++)
    {
        maze[0][i]._status = WALL;
        maze[i][0]._status = WALL;
        maze[LABY_MAX - 1][i]._status = WALL;
        maze[i][LABY_MAX - 1]._status = WALL;
    }

    for(int i = 0; i < LABY_MAX; i++)
    {
        for(int j = 0; j < LABY_MAX; j++)
        {
            maze[i][j]._x = i;
            maze[i][j]._y = j;

        }
    }

    //LABY_MAX = 5。
    /*
    maze[2][2]._status = WALL;
    maze[2][3]._status = WALL;

    Cell *start = &maze[3][1];
    Cell *terminal = &maze[1][3];
    start->_status = STARTOREND;
    terminal->_status = STARTOREND;
    */

    //下面对maze[1][2]的修改是为了使堵死出路用的。
    maze[1][2]._status = WALL;
    maze[2][2]._status = WALL;
    maze[3][2]._status = WALL;
    maze[4][2]._status = WALL;
    //下面对maze[5][1]的修改是为了增加迷宫复杂度的。
    //maze[5][1]._status = WALL;
    maze[5][2]._status = WALL;
    maze[5][4]._status = WALL;
    maze[4][4]._status = WALL;
    maze[3][4]._status = WALL;
    maze[3][5]._status = WALL;
    maze[2][5]._status = WALL;
    maze[6][5]._status = WALL;

    Cell *start = &maze[1][1];
    Cell *terminal = &maze[6][6];
    start->_status = STARTOREND;
    terminal->_status = STARTOREND;

    printMaze(maze);

    bool result = findWayOut(maze, start, terminal);
    (result == true) ? cout << "Found path out!" << endl : cout << "There is no way out!" << endl;
}
