// 005_Homework_5-30_Trie.cpp : This file contains the 'main' function. Program execution begins and ends there.
//感觉用vector或list搭建trie都必然造成空间或时间复杂度的极大增加。
//这里的程序由于vector的接口没有办法直接返回正确的插入顺序，导致代码复杂了很多。
//且由于输入的最后一个字符必须要设置_end属性，导致同样的代码重复了好多遍。

#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct TrieNode
{
    char _data;
    bool _end;
    vector<TrieNode *> *_next;

    TrieNode(char data, vector<TrieNode *> *next = NULL, bool end = false):
        _data(data), _end(end), _next(next)
    {}
};

void deleteVector(vector<TrieNode *> *v)
{
    if(v->size() == 0)
    {
        delete v;
        return;
    }

    //先利用递归逐渐向下直至删除最底端的空向量。
    for(int i = 0; i < v->size(); i++)
    {
        deleteVector((*v)[i]->_next);
        (*v)[i]->_next = NULL;        
    }    
    //然后在注意删除当前层的向量。
    for(int i = 0; i < v->size(); i++)
    {
        delete (*v)[i];
    }
}

/*
template<typename T>
void initVector(vector<TrieNode<T>>* v)
{
    for(int i = 0; i < 26; i++)
    {
        v.push_back(new TrieNode<T>('a' + i));
    }
}
*/

int main()
{
    vector<TrieNode *> root;

    int n;
    cout << "How many words is there going to be?" << endl;
    cin >> n;
    //这里需要用cin.ignore()来保证之前输入的回车不会影响到后面的getline()。
    cin.ignore();
    for(int i = 0; i < n; i++)
    {
        string word;
        getline(cin, word);

        vector<TrieNode *> *p = &root;

        //bool exist = false;

        //按字符串长度，注意录入。
        for(int j = 0; j < word.size(); j++)
        {
            //如果当前向量为空。
            if(p->size() == 0)
            {
                TrieNode *character = new TrieNode(word[j], new vector<TrieNode *>);
                p->push_back(character);

                //这里的对最后一个字符的_end设置在下面的程序中也有出现好几次，都是复制的。
                //但上面这一个略有点不同。
                if(j == word.size() - 1)
                {
                    //这里用的是(*p)[0}，而不是(*p)[k]。
                    (*p)[0]->_end = true;
                    break;
                }
                p = character->_next;
            }
            //若当前向量不为空。
            else
            {       
                bool inserted = false;
                int k;
                vector<TrieNode *>::iterator iter = p->begin();
                for(k = 0; k < p->size(); k++, iter++)
                {
                    //如果向量中已存在与当前字符相同的元素。
                    if(tolower((*p)[k]->_data) == tolower(word[j]))
                    {
                        if(j == word.size() - 1)
                        {
                            (*p)[k]->_end == true;
                            break;
                        }

                        inserted = true;
                        p = (*p)[k]->_next;
                        break;
                    }
                    else
                    {
                        //如果找到大于当前字符的元素，则在大于当前字符的元素前插入。
                        if(tolower(word[j]) < tolower((*p)[k]->_data))
                        {
                            TrieNode *character = new TrieNode(word[j], new vector<TrieNode *>);
                            p->insert(iter, character);

                            if(j == word.size() - 1)
                            {
                                (*p)[k]->_end == true;
                                break;
                            }

                            inserted = true;
                            p = character->_next;
                            break;
                        }
                    }
                }
                //如果一直到最后也没有插入，则在末尾插入。
                if(k == p->size() && !inserted)
                {
                    TrieNode *character = new TrieNode(word[j], new vector<TrieNode *>);
                    p->insert(iter, character);

                    if(j == word.size() - 1)
                    {
                        (*p)[k]->_end == true;                        
                    }
                    p = character->_next;
                }
            }            
        }
    }

    cout << endl;
    cout << "Input the text you want to find (input words start with * to exit): ";

    string check;

    //验证某字符是否存在于Trie中。
    while(getline(cin, check), check.c_str()[0] != '*')
    {
        vector<TrieNode *> *q = &root;

        bool found = false;
        //以输入字符长度为循环长度进行逐一比对。
        for(int i = 0; i < check.size(); i++)
        {
            //在每一级的Trie中进行逐一比对。
            for(int j = 0; j < q->size(); j++)
            {
                if(tolower((*q)[j]->_data) == tolower(check[i]))
                {
                    if(i == check.size() - 1)
                    {
                        (*q)[j]->_end == true ? cout << "Exist" : cout << "Do Not Exist";
                        cout << endl;
                        found = true;
                        break;
                    }

                    q = (*q)[j]->_next;
                    break;
                }                
            }
        }
        if(!found)
        {
            cout << "Do Not Exist" << endl;
        }
    }

    //删除动态内存分配对象。
    deleteVector(&root);
}
