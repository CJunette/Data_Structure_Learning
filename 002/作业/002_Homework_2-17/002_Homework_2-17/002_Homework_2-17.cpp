// 002_Homework_2-17.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

class MyString
{
    private:
    char *_str;
    mutable int _size;
    
    bool lessThan(char *str1, char*str2, int n)
    {
        if((str1[n] == 0) && (str2[n] != 0))
        {
            return true;
        }
        if((str2[n] == 0) && (str1[n] != 0))
        {
            return false;
        }

        if(str1[n] < str2[n])
        {
            return true;
        }
        if(str1[n] > str2[n])
        {
            return false;
        }

        if(str1[n] == str2[n])
        {
            return lessThan(str1, str2, ++n);
        }
    }

    public:
    MyString(const char *a)
    {
        int size = 0;
        int i = 0;
        while(a[i] != '\0')
        {            
            size++;
            i++;
        }
        size++;
        _str = new char[size];
        i = 0;
        while(i < size)
        {
            _str[i] = a[i];
            i++;
        }
        _size = size;
    }
    MyString(const MyString &mystr)
    {
        _size = mystr._size;
        _str = new char[_size];
        for(int i = 0; i < _size; i++)
        {
            _str[i] = mystr._str[i];
        }
    }


    ~MyString()
    {
        delete _str;
    }

    void show() const
    {
        int i = 0;
        while(i < _size)
        {
            cout << _str[i++];
        }
        cout << endl;
    }
    bool operator < (const MyString &mystr)
    {
        return lessThan(_str, mystr._str, 0);
    }
};

int main()
{
    MyString str1("abcd");
    MyString str2("abd");
    cout << (str1 < str2) << endl;
}