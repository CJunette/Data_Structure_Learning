#pragma once

template<typename K, typename V>
struct Entry
{
	K _key;
	V _value;

	Entry(K key_ = K(), V value_ = V()):
		_key(key_), _value(value_) {}
	Entry(Entry<K, V> const &e):
		_key(e._key), _value(e._value) {}

	bool operator < (Entry<K, V> const &e) { return _key < e._key; }
	bool operator > (Entry<K, V> const &e) { return _key > e._key; }
	bool operator == (Entry<K, V> const &e) { return _key == e._key; }
	bool operator != (Entry<K, V> const &e) { return _key != e._key; }
};