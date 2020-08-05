// 002_F-2_001_MergeSearch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
using namespace std;

typedef int Rank;

template<typename T>
void merge1(vector<T> &A, Rank lo, Rank mi, Rank hi)
{
	//这里，认为合并后的向量为A，合并前的子向量为B和C。
	int lengthB = mi - lo;
	int lengthC = hi - mi;
	int lengthA = hi - lo;

	//由于重新排列会导致前半段的元素被覆盖，因此需要将前半段元素复制出来。
	T *B = new T[lengthB];
	for(int i = 0; i < lengthB; i++)
	{
		//注意，这里A的移动必须是以lo为起点的。下面不能直接写A[i]。
		B[i] = A[lo + i];
	}
	//这里如果处于代码易读性的角度，其实是可以把mi到hi的元素也移动到一个C指向的数组中的。
	//但是由于实际中那部分元素时不会出现被覆盖的问题的，因此这里就直接利用A向量，不再单独创建数组了。

	int iterB = 0;
	int iterC = 0;
	int iterA = 0;

	while(iterA < lengthA)
	{
		//注意，虽然这里没有创建数组C，但如上面所说的，可以假象[mi, hi)中的元素在一个C数组中，这样会便于说明。
		//当iterB未超出B时，如果iterC已经越界（C中的元素已经都被排入A了）；或虽然iterC没有越界，但B的首项小于C的首项。此时取B的首项插入A中。
		if((iterB < lengthB) && ((iterC >= lengthC) || (B[iterB] <= A[mi + iterC])))
		{
			A[lo + iterA++] = B[iterB++];
		}
		//要注意，这两个if语句是可能同时执行的，如当B[0] < C[0] < B[1]时，上一个语句会把B[0]加入A，下一个语句会把C[0]加入A。
		if((iterC < lengthC) && ((iterB >= lengthB) || (A[mi + iterC] < B[iterB])))
		{
			A[lo + iterA++] = A[mi + iterC++];
		}
	}
	delete[] B;
}

//merge2是课件上给出的代码，更加精简一些。
template<typename T>
void merge2(vector<T> &a, Rank lo, Rank mi, Rank hi)
{
	int LengthA = hi - lo;
	int LengthB = mi - lo;
	int LengthC = mi - lo;

	T *A = &a[0] + lo;
	T *B = new T[LengthB];
	for(int i = 0; i < LengthB; i++)
	{
		B[i] = A[i];
	}
	T *C = &a[0] + mi;

	int IterA = 0;
	int IterB = 0;
	int IterC = 0;

	//这里关于终止条件为“IterB < LengthB”，因为并没有给C单独创建数组，换言之C本身就是在A中的。
	//如果B中元素已经全部被转移到A中了，而C中还有剩余，就应该把C中剩余的元素排在B的最后一项的后面，但由于C本身就在A中，所以C中的剩余项在一开始就已经排列在了正确的位置上。因此不需要额外的循环操作。
	//但如果C中元素被全部排列到A中，而B中还有剩余，则应该把B中元素在逐个转移到A中，此时还是需要“IterB < LengthB”这样的条件保证循环的结束。
	//综上，可以看出，实际上用来判断循环结束的条件只有“IterB < LengthB”就够了。
	while(IterB < LengthB)
	{
		//这里由于IterB < LengthB已经变为循环条件，因此不需要在这里的条件语句中再次声明。
		//条件语句也因此可以简化为三目运算符的形式。
		A[IterA++] = ((IterC >= LengthC) || (B[IterB] <= C[IterC])) ? B[IterB++] : C[IterC++];
	}
	delete[] B;
}

template<typename T>
void mergeSearch(vector<T> &A, Rank lo, Rank hi)
{
	//递归基的情况。
	if(hi - lo ==1)
	{
		return;
	}
    Rank mi = (lo + hi) >> 1;
	mergeSearch(A, lo, mi);
	mergeSearch(A, mi, hi);
	merge2(A, lo, mi, hi);
}

int main()
{
	vector<int> a;
	a.push_back(6);
	a.push_back(3);
	a.push_back(2);
	a.push_back(7);
	a.push_back(1);
	a.push_back(5);
	a.push_back(8);
	a.push_back(4);

	mergeSearch(a, 0, 8);
	
	for(int i = 0; i < a.size(); i++)
	{
		cout << a[i] << " ";
	}
	cout << endl;
}
