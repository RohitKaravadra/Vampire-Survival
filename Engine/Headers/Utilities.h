#pragma once

#pragma region Dictionary

#include <ostream>
#include <iostream>

using namespace std;

template<typename T1, typename T2>
struct Pair
{
	T1 key;
	T2 value;

	Pair() {};

	Pair(T1 _key, T2 _value)
	{
		key = _key;
		value = _value;
	}
};

template<typename T1, typename T2>
class Dictionary
{
	Pair<T1, T2>* data;
	unsigned int maxSize;
	unsigned int curIndex;
public:
	// constructor
	Dictionary(unsigned int _maxSize = 1)
	{
		_maxSize = _maxSize > 0 ? _maxSize : 1; // makesure dict always have a size > 0
		curIndex = 0;
		maxSize = _maxSize;
		data = new Pair<T1, T2>[_maxSize];
	}

	// destructor
	~Dictionary()
	{
		delete[] data;
	}
	// returns last current size of data
	unsigned int get_size()
	{
		return curIndex;
	}

	// checks if data has given key or not
	bool has(T1 _key)
	{
		for (unsigned int i = 0; i < curIndex; i++)
			if (data[i].key == _key)
				return true;
		return false;
	}

	// returns index of the key (if not present returns -1)
	int index_of(T1 _key)
	{
		for (unsigned int i = 0; i < curIndex; i++)
			if (data[i].key == _key)
				return i;
		return -1;
	}

	// get value at key
	T2 get(T1 _key)
	{
		int index = index_of(_key);
		if (index == -1)
			return data[0].value;
		return data[index].value;
	}

	// adds pair to data
	bool add(Pair<T1, T2>& _pair, bool _override = true)
	{
		int index = index_of(_pair.key);

		if (index != -1)
		{
			if (_override)
			{
				data[index].value = _pair.value;
				return true;
			}
			return false;
		}

		if (curIndex < maxSize)
		{
			data[curIndex++] = _pair;
			return true;
		}

		return false;
	}

	// remove pair at index
	bool remove(unsigned int index)
	{
		if (index < curIndex)
		{
			curIndex--;
			if (index != curIndex)
				memcpy(&data[index], &data[index + 1], sizeof(Pair<T1, T2>) * (curIndex - index));
			return true;
		}
		return false;
	}

	// removes pair with given key from data
	bool remove_key(T1 _key)
	{
		int index = index_of(_key);
		if (index == -1)
			return false;
		return remove(index);
	}

	Pair<T1, T2>& operator[](unsigned int index)
	{
		if (index < curIndex)
			return data[index];
		return data[0];
	}

	friend ostream& operator<<(ostream& os, Dictionary& _dict)
	{
		Pair<T1, T2> pair;
		os << " [";
		for (unsigned int i = 0; i < _dict.curIndex; i++)
		{
			pair = _dict[i];
			os << " (" << pair.key << "," << pair.value << ") ";
		}
		os << "] ";
		return os;
	}
};

#pragma endregion

#pragma region Dynamic Array

template<typename T>
class DArray
{
	T* data;
	unsigned int curIndex;
	unsigned int maxSize;

public:
	DArray(int _maxSize)
	{
		data = new T[_maxSize];
		maxSize = _maxSize;
		curIndex = 0;
	}

	~DArray()
	{
		delete[] data;
		data = nullptr;
		curIndex = 0;
		maxSize = 0;
	}

	int get_size()
	{
		return curIndex;
	}

	bool has(T* value)
	{
		for (unsigned int i = 0; i < curIndex; i++)
			if (data[i] == &value)
				return true;
		return false;
	}

	int get_index(T* value)
	{
		for (unsigned int i = 0; i < curIndex; i++)
			if (data[i] == *value)
				return i;
		return -1;
	}

	bool add(T* value)
	{
		if (curIndex < maxSize)
		{
			data[curIndex++] = *value;
			return true;
		}
		return false;
	}

	bool remove_at(int index)
	{
		if (index < curIndex)
		{
			delete data[index];
			if (index != --curIndex)
				data[index] = data[curIndex];
		}
		return false;
	}

	bool remove(T* value)
	{
		int index = get_index(value);
		return  index == -1 ? false : remove_at(index);
	}

	T operator[](int index)
	{
		return data[index];
	}

	friend ostream& operator<<(ostream& os, DArray& value)
	{
		os << "[ ";
		for (unsigned int i = 0; i < value.curIndex; i++)
			os << value.data[i] << ",";
		os << "] ";
		return os;
	}
};

#pragma endregion

