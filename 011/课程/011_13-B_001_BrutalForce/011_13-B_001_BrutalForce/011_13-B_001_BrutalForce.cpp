// 011_13-B_001_BrutalForce.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

int match1(char *P, char *T)
{
    int i = 0, j = 0;
    int n = strlen(T), m = strlen(P);

    while(j < m && i < n)
    {
        if(P[j] == T[i])
        {
            i++;
            j++;
        }
        else
        {
            i = i - j + 1;
            j = 0;
        }
    }

    return i - j;
}

int match2(char *P, char *T)
{
    int i, j;
    int n = strlen(T), m = strlen(P);

    for(i = 0; i < n - m + 1; i++)
    {
        for(j = 0; j < m; j++)
        {
            if(P[j] != T[i + j])
            {
                break;
            }
        }
        if(j == m)
        {
            break;
        }
    }
    return n - m;
}

int main()
{
    char T[] = "abcd";
    char P[] = "cd";

    int n = match2(P, T);

    if(n == strlen(T) - strlen(P))
    {
        cout << "match!" << endl;
    }
    else
    {
        cout << "dismatch!" << endl;
    }
}