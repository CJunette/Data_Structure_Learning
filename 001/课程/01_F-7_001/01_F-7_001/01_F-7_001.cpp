// 01_F-7_001.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
using namespace std;

int countLCS(string str1, string str2)
{
    if(str1 == "" | str2 == "")
    {
        return 0;
    }
    if(str1[str1.size() - 1] == str2[str2.size() - 1])
    {
        //注意这里取子串的时候，大小要用str.size() - 1，而非str.size() - 2。这个后边界是不被包括的。
        return 1 + countLCS(str1.substr(0, str1.size() - 1), str2.substr(0, str2.size() - 1));
    }
    else
    {
        int n = countLCS(str1.substr(0, str1.size() - 1), str2);
        int m = countLCS(str1, str2.substr(0, str2.size() - 1));
        return n > m ? n : m;
    }
}

int main()
{
    cout << "Please input 2 string: " << endl;
    string str1, str2;
    getline(cin, str1);
    getline(cin, str2);
    cout << "Common text number: " << countLCS(str1, str2) << endl;
}
