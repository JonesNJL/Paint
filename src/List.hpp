#ifndef LIST_H
#define LIST_H

#include <stdexcept>
#include <initializer_list>

template <typename T>
class List
{
	private:
		T* data = nullptr;
		int capacity = 0;
		int size = 0;

	public:
		List() = default;

		List(int capacity)
		{
			data = new T[capacity];
			this->capacity = capacity;
			size = 0;
		}

		List(std::initializer_list<T> values)
		{	
			data = new T[values.size()];
			capacity = values.size();
			size = values.size();

			int i = 0;
			for (T value : values)
			{
				data[i] = value;
				i++;
			}
		}

		~List()
		{
			if (data != nullptr)
			{
				delete[] data;
			}
		}

		List(const List& source)
		{
			capacity = source.capacity;
			size = source.size;

			data = new T[capacity];
			for (int i = 0; i < size; i++)
			{
				data[i] = source.data[i];
			}
		}

		void operator=(const List& source)
		{
			if (this == &source) { return; }

			if (data != nullptr) { delete[] data; }

			capacity = source.capacity;
			size = source.size;

			data = new T[capacity];
			for (int i = 0; i < size; i++)
			{
				data[i] = source.data[i];
			}
		}

		int Size()
		{
			return size;
		}

		void Add(T value)
		{
			if (size == capacity)
			{
				IncreaseCapacity();
			}

			data[size] = value;
			size += 1;
		}

		void Remove(T value)
		{
			int valueIndex = -1;
			for (int i = 0; i < size; i++)
			{
				if (data[i] == value)
				{
					valueIndex = i;
					break;
				}
			}

			if (valueIndex == -1) { return; }

			T* newData = new T[capacity];

			int sourceIndex = 0;
			for (int i = 0; i < size - 1; i++)
			{
				if (sourceIndex == valueIndex) { sourceIndex++; }
				
				newData[i] = data[sourceIndex];
				sourceIndex++;
			}

			if (data != nullptr) { delete[] data; }

			data = newData;
			size -= 1;
		}

		void IncreaseCapacity()
		{
			if (capacity == 0) { capacity = 1; }
			capacity *= 2;

			T* newData = new T[capacity];

			for (int i = 0; i < size; i++)
			{
				newData[i] = data[i];
			}

			if (data != nullptr) { delete[] data; }

			data = newData;
		}

		T& operator[] (int index) const
		{
			if (index < 0 || index >= size)
			{
				throw std::invalid_argument("Index out of range");
			}

			return data[index];
		}
};

#endif