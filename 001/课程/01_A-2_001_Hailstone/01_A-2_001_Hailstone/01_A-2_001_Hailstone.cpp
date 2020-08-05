// 01_A-2_001_Hailstone.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <iterator>
using namespace std;

int hailstone(int n)
{
	vector<int> v;
	v.push_back(n);
	while(n > 1)
	{
		n = n % 2 ? 3 * n + 1 : n / 2;
		v.push_back(n);
	}
	copy(v.begin(), v.end(), ostream_iterator<int>(cout, "\t"));
	return v.size();
}

int main()
{
	int n;
	cout << "Please input an integer number: ";
	cin >> n;
	//是否存在n，使得hailstone(n)无法返回一个确定的值（出现死循环）？即，是否存在n，使hailstone()算法对于n是无穷的？
	//答案是，不确定。既不能肯定，也不能否定。
	int steps = hailstone(n);
	cout << endl << "Step: " << steps << endl;
}
