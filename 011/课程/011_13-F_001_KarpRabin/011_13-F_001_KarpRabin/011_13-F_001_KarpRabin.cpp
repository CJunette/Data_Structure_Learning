// 011_13-F_001_KarpRabin.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

const int M = 93;
const int R = 10;

int digit(char *s, int i)
{
    return s[i] - '0';
}

bool check1by1(char *P, char *T, int i)
{
    int length = strlen(P);
    for(int j = 0; j < length; j++)
    {
        if(P[j] != T[i + j])
        {
            return false;
        }
    }
    return true;
}

long long prepareDM(int m)
{
    long long dm = 1;
    for(int i = 1; i < m; i++)
    {
        dm = (dm * R) % M;
    }
    return dm;
}

void updateHash(long long &hashT, char *T, int k, int m, long long dm)
{
    hashT = (hashT - dm * digit(T, k)) % M;
    hashT = (hashT * R + digit(T, k + m)) % M;
    hashT = hashT < 0 ? hashT + M : hashT;
}

int match(char *P, char *T)
{
    int m = strlen(P);
    int n = strlen(T);
    long long dm = prepareDM(m);
    long long hashP = 0, hashT = 0;

    for(int i = 0; i < m; i++)
    {
        hashP = (hashP * R + digit(P, i)) % M;
        hashT = (hashT * R + digit(T, i)) % M;
    }

    int k;

    for(k = 0; k <= n - m; k++)
    {
        if(hashP == hashT)
        {
            break;
        }
        else
        {
            if(k + 1 <= n - m)
            {
                updateHash(hashT, T, k, m, dm);
            }
        }
    }

    return k;
}

int main()
{
    char P1[] = "23456";
    char T1[] = "123456";

    //P2与T2对应的M为11，记得修改。
    char P2[] = "101";
    char T2[] = "9101";

    int n = match(P2, T2);
    if(n <= strlen(T2) - strlen(P2))
    {
        cout << "match!" << endl;
    }
    else
    {
        cout << "dismatch!" << endl;
    }
}