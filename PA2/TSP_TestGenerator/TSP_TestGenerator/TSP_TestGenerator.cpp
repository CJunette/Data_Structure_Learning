// TSP_TestGenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	int count = 0;
	int n = 1;
	ofstream fout("connect.txt");

	for(int i = 0; i < 1000000 - 1; i++)
	{
		for(int j = 0; j < n; j++)
		{
			for(int k = 0; k < n; k++)
			{
				fout << i * n + j + 1 << " " << n * (i + 1) + k + 1 << " ";
				count++;
				fout << endl;
			}
		}
		//cout << endl;
	}
	cout << count << endl;
}
