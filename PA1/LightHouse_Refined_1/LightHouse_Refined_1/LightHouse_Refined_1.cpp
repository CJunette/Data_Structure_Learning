// LightHouse_Refined_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//֮ǰ���Ǵ����б����б������η��ʲ�ѯ�ġ���Ҫԭ�������Ҿ��ÿ���������һ�������ݵķ�ʽ���б�����Ч�ʡ�
//Ȼ���б�Ĳ�������Ч�ʲ�û���������������+�鲢�����ٶȿ졣
//��¼�������������������_xΪ��׼��������Ȼ���������������_yΪ��׼�ٴι鲢���򣬲�ͬʱ�������е�����Ը�����
//���ڴ�ʱ��_x������ģ�_y������ʹ������������޷��������䡣

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
        //�����ø��ӵ���Ŀ���ʽ����ɴ���������д�������ж��ˡ�
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
    LightHouse *pLH = new LightHouse[n];
    //LightHouse pLH[100];

    int x, y;

    //timespec_get(&start, 1);

    for(int i = 0; i < n; i++)
    {
        scanf("%d%d", &x, &y);
        //fin >> x >> y;
        pLH[i]._x = x;
        pLH[i]._y = y;
    }
    /*
    timespec_get(&end, 1);
    time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
    cout << "time0: " << time << endl;

    timespec_get(&start, 1);
    */
    mergeSort(pLH, 0, n);
    /*
    timespec_get(&end, 1);
    time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
    cout << "time1: " << time << endl;

    timespec_get(&start, 1);
    */
    //������������_x��������ͬ���Ĺ鲢˼·ȥѰ��_y������ԡ�
    long long count = inversionInside(pLH, 0, n);
    /*
    timespec_get(&end, 1);
    time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
    cout << "time2: " << time << endl;
    */

    long long acc = (long long)(n) * (long long)(n - 1);
    
    //�����Ϊ3-n�Ľ׳˳���2�Ľ׳ˡ�
    acc = acc / 2;
    //������������ȥ��������
    printf("%lld\n", acc - count);

    delete[] pLH;

    //fin.close();
}
