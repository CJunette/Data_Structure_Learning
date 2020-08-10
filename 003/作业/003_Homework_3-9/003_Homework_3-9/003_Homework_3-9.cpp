// 003_Homework_3-9.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
using namespace std;

template<typename T>
void selectionSort(vector<T> &v)
{
	int r = v.size();
	while(r > 1)
	{
		//1.选取0-r中最大的元素k。复制元素值。
		//2.将k的秩之后的元素都向前移一位。
		//3.将r位置的值变为k。
		//4.r--。
		int max = 0;
		for(int i = 0; i < r; i++)
		{
			if(v[i] >= v[max])
			{
				max = i;
			}
		}
		T temp = v[max];
		/*
		for(int i = max; i < r - 1; i++)
		{
			v[i] = v[i + 1];
		}
		v[--r] = temp;		
		*/

		//将第2步进行修改。我们这里只需要将最大元素和第r位元素交换即可，不用那么复杂的把整体前移。这样可以减少复杂度。
		//2.交换元素k和第a[r]。
		//3.r--。
		v[max] = v[r - 1];
		v[r - 1] = temp;
		r--;
	}
}

int main()
{
    vector<int> v;
    v.push_back(3);
	v.push_back(5);
	v.push_back(1);
	v.push_back(1);
	v.push_back(2);
	v.push_back(8);
	v.push_back(7);
	v.push_back(4);

	selectionSort(v);
	for(int i = 0; i < 8; i++)
	{
		cout << v[i] << " ";
	}
	cout << endl;
}
