#pragma once
#include <iostream>
#include "Dictionary.h"
#include "Entry.h"
#include "Bitmap.h"

template<typename T>
void release(T t)
{
	return;
}

template<typename T>
void release(T *t)
{
	delete t;
}

/*----------------------------------------------------------------------*/

int hashCode(char c) { return (int)c; }
int hashCode(int k) { return (int)k; }
int hashCode(long long i) { return (i >> 32) + (int)i; }
int hashCode(char s[])
{
	unsigned int h = 0;
	for(int i = 0; i < strlen(s); i++)
	{
		h = (h << 5) | (h >> 27);
		h += (int)s[i];
	}
	return h;
}

/*----------------------------------------------------------------------*/

template<typename K, typename V>
struct Hashtable: public Dictionary<K, V>
{
	Entry<K, V> **ht;
	int M;
	int N;
	Bitmap *lazyRemoval;


	bool lazilyRemoved(int x)
	{
		return lazyRemoval->test(x);
	}
	void markedAsRemoved(int x)
	{
		lazyRemoval->set(x);
	}

	protected:
	int probe4Hit(const K &k);
	int probe4Free(const K &k);
	void rehash();

	public:
	Hashtable(int c = 5);
	~Hashtable();
	int size() { return N; }
	bool put(K k, V v);
	V *get(K k);
	bool remove(K k);
};

template<typename K, typename V>
Hashtable<K, V>::Hashtable(int c)
{
	char filename[] = "prime-1048576-bitmap.txt";
	M = primeNLT(c, 1048576, filename);
	N = 0;
	ht = new Entry<K, V> * [M];
	memset(ht, 0, sizeof(Entry<K, V> *) * M);
	lazyRemoval = new Bitmap(M);
}

template<typename K, typename V>
Hashtable<K, V>::~Hashtable()
{
	for(int i = 0; i < M; i++)
	{
		if(ht[i])
		{
			release(ht[i]);
		}
	}
	delete[] ht;
	delete lazyRemoval;
}

template<typename K, typename V>
V *Hashtable<K, V>::get(K k)
{
	int r = probe4Hit(k);
	return ht[r] ? &(ht[r]->_value) : NULL;
}

template<typename K, typename V>
int Hashtable<K, V>::probe4Hit(const K &k)
{
	int r = hashCode(k) % M;
	//int i = 1;
	while((ht[r] && ht[r]->_key != k) || (!ht[r] && lazilyRemoved(r)))
	{
		//r = (r + i * i) % M;
		r = (r + 1) % M;
		//i++;
	}

	return r;
}

template<typename K, typename V>
int Hashtable<K, V>::probe4Free(const K &k)
{
	int r = hashCode(k) % M;
	//int i = 1;
	while(ht[r])
	{
		//r = (r + i * i) % M;
		r = (r + 1) % M;
		//i++;
	}
	return r;
}

template<typename K, typename V>
bool Hashtable<K, V>::put(K k, V v)
{
	if(ht[probe4Hit(k)])
	{
		return false;
	}

	Entry<K, V> *e = new Entry<K, V>(k, v);
	int r = probe4Free(k);
	ht[r] = e;
	++N;

	if(M < N * 2)
	{
		rehash();
	}
}

template<typename K, typename V>
bool Hashtable<K, V>::remove(K k)
{
	int r = probe4Hit(k);
	if(!ht[r])
	{
		return false;
	}
	release(ht[r]);
	ht[r] = NULL;
	markedAsRemoved(r);
	N--;
	return true;
}

template<typename K, typename V>
void Hashtable<K, V>::rehash()
{
	int oldCapacity = M;
	Entry<K, V> **oldHt = ht;
	
	char filename[] = "prime-1048576-bitmap.txt";

	M = primeNLT(M * 2, 1048576, filename);
	N = 0;
	ht = new Entry<K, V> * [M];
	memset(ht, 0, sizeof(Entry<K, V> *) * M);
	delete lazyRemoval;
	lazyRemoval = new Bitmap(M);

	for(int i = 0; i < oldCapacity; i++)
	{
		if(oldHt[i])
		{
			put(oldHt[i]->_key, oldHt[i]->_value);
			//����ֱ����put()������ܵ��²��������ִ�����϶����Ϊ����ԭ���Ǿ�������ͻ������ķ�ʽ����ɢ�е�Ͱ�еĶ�����ԣ����ǵ�_key����Ӧ��λ����M�ӱ����������ǰ��λ������Զ��
			//�磬��������MΪ5��ɢ�б����Ȳ���1���ٲ���6����6������ڵڶ���λ�á�������������ݣ�M��Ϊ10��6�ͻ��ƶ���������λ�á��м�����˴����Ŀսڵ㣬ʹ���������ѡ�
			//�����ø�ֱ�ӵķ������в��룬�Ա�����������Ĵ��롣
			/*
			Entry<K, V> *e = new Entry<K, V>(oldHt[i]->_key, oldHt[i]->_value);			
			ht[i] = e;
			++N;
			*/
		}
	}
	delete oldHt;	
}