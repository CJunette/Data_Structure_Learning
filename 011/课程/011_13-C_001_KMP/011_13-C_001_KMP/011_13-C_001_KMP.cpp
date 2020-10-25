// 011_13-C_001_KMP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

int *buildNext1(char *P)
{
    int t = -1, j = 0;    
    int m = strlen(P);

    int *N = new int[m];    
    N[0] = -1;
    
    while(j < m - 1)
    {
        if(t < 0 || P[t] == P[j])
        {
            N[++j] = ++t;
        }
        else
        {
            t = N[t];
        }
    }

    return N;
}

int *buildNext2(char *P)
{
    int t = -1, j = 0;
    int m = strlen(P);
    int *N = new int[m];
    N[0] = -1;

    while(j < m - 1)
    {
        if(t < 0 || P[j] == P[t])
        {
            ++j;
            ++t;
            N[j] = P[j] == P[t] ? N[t] : t;
        }
        else
        {
            j = N[j];
        }
    }
    return N;
}

int KMP(char *P, char *T)
{
    int *next = buildNext2(P);
    int i = 0, j = 0;
    int n = strlen(T), m = strlen(P);

    while(i < n && j < m)
    {
        if(j < 0 || T[i] == P[j])
        {
            i++;
            j++;
        }
        else
        {
            j = next[j];
        }
    }

    delete[] next;
    return i - j;
}


int main()
{
    /*
    char T[] = "MAMIAMAMAMIA";    
    char P[] = "MAMAMIA";
    */
    char T[] = "000100001";
    char P[] = "00001";
    int *N = buildNext1(P);
    delete[] N;

    int n = KMP(P, T);
    if(n == strlen(T) - strlen(P))
    {
        cout << "match!" << endl;
    }
    else
    {
        cout << "dismatch!" << endl;
    }
}