#pragma once

template<typename T>
struct PQ
{
	virtual void insert(const T& e) = 0;
	virtual T getMax() = 0;
	virtual T delMax() = 0;
};