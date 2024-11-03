#pragma once
#include "GameMath.h"

using namespace std;

namespace Engine
{

#pragma region Dynamic Array

	template<typename T>
	class DArray
	{
		T* data;
		unsigned int size;

	public:
		// constructor
		DArray()
		{
			data = nullptr;
			size = 0;
		}

		// destructor
		~DArray()
		{
			if (size != 0)
			{
				delete[] data;
				data = nullptr;
				size = 0;
			}
		}

		// get current size of array
		int get_size()
		{
			return size;
		}

		// check if element is in the array or not
		bool has(T value)
		{
			for (unsigned int i = 0; i < size; i++)
				if (data[i] == value)
					return true;
			return false;
		}

		// get index of the element if present else return -1
		int get_index(T value)
		{
			for (unsigned int i = 0; i < size; i++)
				if (data[i] == value)
					return i;
			return -1;
		}

		// add new element to the array
		void add(T value)
		{
			if (size == 0)
			{
				size += 1;
				data = new T[size];
				data[size - 1] = value;
			}
			else
			{
				T* newData = new T[size + 1];
				memcpy(newData, data, size * sizeof(T));
				newData[size++] = value;
				delete[] data;
				data = newData;
				newData = nullptr;
			}
		}

		// remove element at given index
		bool remove_at(unsigned int index)
		{
			if (index < size)
			{
				if (size == 1)
				{
					delete[] data;
					size = 0;
				}
				else
				{
					T* newData = new T[--size];
					memcpy(newData, data, index * sizeof(T));

					if (index < size)
						memcpy(&newData[index], &data[index + 1], (size - index) * sizeof(T));

					delete[] data;
					data = newData;
					newData = nullptr;
				}

				return true;
			}
			return false;
		}

		// remove perticular element
		bool remove(T value)
		{
			int index = get_index(value);
			return  index == -1 ? false : remove_at(index);
		}

		// clear array
		void clear()
		{
			if (data != nullptr)
			{
				delete[] data;
				size = 0;
			}
		}

		T& operator[](int index)
		{
			return data[index];
		}

		friend ostream& operator<<(ostream& os, DArray& value)
		{
			os << "[ ";
			for (unsigned int i = 0; i < value.size; i++)
				os << value.data[i] << " ";
			os << "] ";
			return os;
		}
	};

#pragma endregion

#pragma region Dictionary

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
		DArray<Pair<T1, T2>> data;
		unsigned int size = 0;
	public:

		// returns current size of data
		unsigned int get_size()
		{
			return size;
		}

		// checks if data has given key or not
		bool has(T1 _key)
		{
			for (unsigned int i = 0; i < size; i++)
				if (data[i].key == _key)
					return true;
			return false;
		}

		// returns index of the key (if not present returns -1)
		int index_of(T1 _key)
		{
			for (unsigned int i = 0; i < size; i++)
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

		// get value at index
		T1 get_key(unsigned int index)
		{
			if (index < size)
				return data[index].key;
			return data[0].key;
		}

		// adds pair to data
		bool add(Pair<T1, T2>& _pair, bool _override = true)
		{
			int index = index_of(_pair.key);

			if (index != -1)
			{
				if (_override)
				{
					if (data[index].value != _pair.value)
					{
						data[index].value = _pair.value;
						return true;
					}
				}
			}
			else
			{
				data.add(_pair);
				size = data.get_size();
				return true;
			}
			return false;
		}

		// remove pair at index
		bool remove(unsigned int index)
		{
			if (data.remove_at(index))
			{
				size = data.get_size();
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

		// clear all data of dictionary
		void clear()
		{
			data.clear();
			size = data.get_size();
		}

		Pair<T1, T2>& operator[](unsigned int index)
		{
			if (index < size)
				return data[index];
			return data[0];
		}

		friend ostream& operator<<(ostream& os, Dictionary& _dict)
		{
			Pair<T1, T2> pair;
			os << " [";
			for (unsigned int i = 0; i < _dict.size; i++)
			{
				pair = _dict[i];
				os << " (" << pair.key << "," << pair.value << ") ";
			}
			os << "] ";
			return os;
		}
	};

#pragma endregion

}

