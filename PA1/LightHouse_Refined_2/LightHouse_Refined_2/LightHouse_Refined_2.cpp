// LightHouse_Refined_2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//这个文件没啥实际意义。
//本来我想合并merge和count两个过程的，但这两个过程其实是无法合并的。
//因此这个程序没有意义。

#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

struct LightHouse
{
    int _x, _y;
    LightHouse(int x = 0, int y = 0):
        _x(x), _y(y)
    {}
};

/*
void merge(LightHouse *lh, int lo, int mi, int hi)
{
    int lenB = mi - lo;
    int lenC = hi - mi;
    LightHouse *B = new LightHouse[lenB];
    for(int i = 0; i < lenB; i++)
    {
        B[i] = lh[lo + i];
    }
    LightHouse *C = &lh[mi];

    int iterA = 0;
    int iterB = 0;
    int iterC = 0;

    while(iterB < lenB)
    {
        lh[lo + iterA++] = ((B[iterB]._x <= C[iterC]._x) || (lenC <= iterC)) ? B[iterB++] : C[iterC++];
    }
    delete[] B;
}

void mergeSort(LightHouse *lh, int lo, int hi)
{
    if(hi - lo <= 1)
    {
        return;
    }
    int mi = (lo + hi) >> 1;

    mergeSort(lh, lo, mi);
    mergeSort(lh, mi, hi);
    merge(lh, lo, mi, hi);
}

long long inversionBetween(LightHouse *lh, int lo, int mi, int hi)
{
    int lenB = mi - lo;
    int lenC = hi - mi;
    LightHouse *B = new LightHouse[lenB];
    for(int i = 0; i < lenB; i++)
    {
        B[i] = lh[lo + i];
    }
    LightHouse *C = &lh[mi];

    int iterA = 0;
    int iterB = 0;
    int iterC = 0;

    long long count = 0;

    while(iterB < lenB)
    {
        //这里用复杂的三目表达式会造成错误，所以我写回条件判断了。
        if((B[iterB]._y <= C[iterC]._y) || (lenC <= iterC))
        {
            lh[lo + iterA++] = B[iterB++];
        }
        else
        {
            count += (long long)(lenB - iterB);
            lh[lo + iterA++] = C[iterC++];
        }
    }
    delete[] B;

    return count;
}

long long inversionInside(LightHouse *lh, int lo, int hi)
{
    if(hi - lo <= 1)
    {
        return 0;
    }
    int mi = (lo + hi) >> 1;

    long long i, j, k;
    i = inversionInside(lh, lo, mi);
    j = inversionInside(lh, mi, hi);
    k = inversionBetween(lh, lo, mi, hi);
    return i + j + k;
}
*/

long long mergeAndCount(LightHouse (&lh)[100], LightHouse(&countlh)[100], int lo, int mi, int hi)
{
    //Merge.
    int lenB = mi - lo;
    int lenC = hi - mi;
    LightHouse *B = new LightHouse[lenB];
    for(int i = 0; i < lenB; i++)
    {
        B[i] = lh[lo + i];        
    }
    
    LightHouse *C = &lh[mi];

    int iterA = 0;
    int iterB = 0;
    int iterC = 0;

    while(iterB < lenB)
    {
        lh[lo + iterA++] = ((B[iterB]._x <= C[iterC]._x) || (lenC <= iterC)) ? B[iterB++] : C[iterC++];
    }

    //Count.

    //将lh的元素复制到countlh中。
    for(int i = lo; i < hi; i++)
    {
        countlh[i] = lh[i];
    }

    iterA = 0;
    iterB = 0;
    iterC = 0;

    //更新元素。
    //会导致_y的排序无法在countlh中保留。这个算法思路不成立。
    for(int i = 0; i < lenB; i++)
    {
        B[i] = countlh[lo + i];
    }

    long long count = 0;
    while(iterB < lenB)
    {
        //这里用复杂的三目表达式会造成错误，所以我写回条件判断了。
        if((B[iterB]._y <= C[iterC]._y) || (lenC <= iterC))
        {
            countlh[lo + iterA++] = B[iterB++];
        }
        else
        {
            count += (long long)(lenB - iterB);
            countlh[lo + iterA++] = C[iterC++];
        }
    }

    delete[] B;
    return count;
}

long long sortAndCount(LightHouse (&lh)[100], LightHouse(&countlh)[100], int lo, int hi)
{
    if(hi - lo <= 1)
    {
        return 0;
    }
    int mi = (lo + hi) >> 1;

    long long i, j, k;
    i = sortAndCount(lh, countlh, lo, mi);
    j = sortAndCount(lh, countlh, mi, hi);
    k = mergeAndCount(lh, countlh, lo, mi, hi);
    return i + j + k;
}

int main()
{
    
    int n;
    cin >> n;

    if(n == 1)
    {
        cout << 0 << endl;
        return 0;
    }
    
    /*
    timespec start, end;
    double time;
    int n = 100000;
    ifstream fin("file.txt");
    */

    //LightHouse *pLH = new LightHouse[n];
    LightHouse pLH[100];
    LightHouse countLH[100];

    int x, y;
    for(int i = 0; i < n; i++)
    {
        cin >> x >> y;
        //fin >> x >> y;
        pLH[i]._x = x;
        pLH[i]._y = y;
    }

    //timespec_get(&start, 1);

    long long count = sortAndCount(pLH, countLH, 0, n);
    /*
    timespec_get(&end, 1);
    time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
    cout << "time2: " << time << endl;
    */

    long long acc = (long long)(n) * (long long)(n - 1);
    //组合数为3-n的阶乘除以2的阶乘。
    acc = acc / 2;

    //输出总组合数减去逆序数。
    printf("%lld\n", acc - count);
    //delete[] pLH;

    /*
    timespec_get(&start, 1);

    mergeSort(pLH, 0, n);

    timespec_get(&end, 1);
    time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
    cout << "time1: " << time << endl;

    timespec_get(&start, 1);

    //完成排序后，由于_x已有序，用同样的归并思路去寻找_y的逆序对。
    long long count = inversionInside(pLH, 0, n);

    timespec_get(&end, 1);
    time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
    cout << "time2: " << time << endl;



    long long acc = (long long)(n) * (long long)(n - 1);

    //组合数为3-n的阶乘除以2的阶乘。
    acc = acc / 2;
    //输出总组合数减去逆序数。
    printf("%lld\n", acc - count);

    timespec_get(&start, 1);

    delete[] pLH;

    timespec_get(&end, 1);
    time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
    cout << "time3: " << time << endl;
    //fin.close();
    */
}
