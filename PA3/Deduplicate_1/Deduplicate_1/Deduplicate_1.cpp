// Deduplicate_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

unsigned int Hash(char S[])
{
	unsigned int h = 0;
	
	for(int i = 0; S[i] != '\0'; i++)
	{
		h = (h << 5) | (h >> 17);
		h += S[i];
	}
	return (unsigned int) h;
}

bool Check(char s1[], char s2[])
{
	int i;
	for(i = 0; s1[i] != '\0' && s2[i] != '\0'; i++)
	{
		if(s1[i] != s2[i])
		{
			return false;
		}
	}

	if(s1[i] == '\0' && s2[i] == '\0')
	{
		return true;
	}
	else
	{
		return false;
	}
}

int main()
{
    unsigned int n;
    scanf("%d", &n);
	
	const unsigned int N = n;


	char **names = new char *[N];
	char **duplicate = new char *[N];
	char **duplicateInOrder = new char *[N];
	
	int duplicateCount = 0;

	for(int i = 0; i < N; i++)
	{
		names[i] = NULL;
		duplicate[i] = NULL;
	}
	
	for(int i = 0; i < n; i++)
	{
		char *name = new char[41];
		scanf("%s", name);	
		
		unsigned int index = Hash(name) % N;

		if(!names[index])
		{
			names[index] = name;
		}
		else
		{
			if(!Check(name, names[index]))
			{
				while(names[index] && !Check(name, names[index]))
				{
					index = (index + 1) % N;
				}
				if(!names[index])
				{
					names[index] = name;
					continue;
				}
			}	

			/*
			while(duplicate[index] && !Check(duplicate[index], name))
			{
				index++;
			}
			*/

			if(!duplicate[index])
			{
				duplicate[index] = name;
				duplicateInOrder[duplicateCount++] = name;
			}
		}		
	}
	
	for(int i = 0; i < duplicateCount; i++)
	{
		printf("%s\n", duplicateInOrder[i]);
	}

	for(int i = 0; i < N; i++)
	{
		if(names[i])
		{
			delete names[i];
		}
	}


	delete[] names;
	delete[] duplicate;
	delete[] duplicateInOrder;
}