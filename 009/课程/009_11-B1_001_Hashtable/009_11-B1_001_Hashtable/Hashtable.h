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
			//这里直接用put()放入可能导致查找链出现大量空隙。因为对于原先是经过“冲突解决”的方式进入散列的桶中的对象而言，他们的_key所对应的位置在M加倍后可能与先前的位置相差很远。
			//如，假设我在M为5的散列表中先插入1，再插入6，则6会出现在第二个位置。但如果经过扩容，M变为10，6就会移动到第六个位置。中间造成了大量的空节点，使查找链断裂。
			//下面用更直接的方法进行插入，以避免上述情况的代码。
			/*
			Entry<K, V> *e = new Entry<K, V>(oldHt[i]->_key, oldHt[i]->_value);			
			ht[i] = e;
			++N;
			*/
		}
	}
	delete oldHt;	
}