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
			//std::cout << "what" << std::endl;

			data = new T[capacity];
			this->capacity = capacity;
			size = 0;
		}

		List(std::initializer_list<T> values)
		{	
			//std::cout << "carrot" << std::endl;

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
			//std::cout << "sammich" << std::endl;

			if (data != nullptr)
			{
				delete[] data;
			}
		}

		List(const List& source)
		{
			//std::cout << "crazy" << std::endl;

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
			//std::cout << "pee" << std::endl;

			if (this == &source) { return; }

			//std::cout << "1" << std::endl;
			if (data != nullptr) { delete[] data; }
			//std::cout << "2" << std::endl;

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
			//std::cout << "adalee" << std::endl;

			return size;
		}

		void Add(T value)
		{
			//std::cout << "draco" << std::endl;

			if (size == capacity)
			{
				IncreaseCapacity();
			}

			//std::cout << "Cap: " << capacity << std::endl;
			//std::cout << "Siz: " << size << std::endl;

			data[size] = value;
			size += 1;
		}

		void Remove(T value)
		{
			//std::cout << "lucas" << std::endl;

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
			//std::cout << "sorry" << std::endl;

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
			//std::cout << "teehee" << std::endl;

			if (index < 0 || index >= size)
			{
				throw std::invalid_argument("Index out of range");
			}

			return data[index];
		}
};

#endif