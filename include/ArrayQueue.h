#pragma once

#include <cstddef>
#include <ostream>
#include <stdexcept>
#include <utility>

template <typename T>
class ArrayQueue
{
public:
	explicit ArrayQueue(std::size_t InCapacity)
		: Capacity(InCapacity)
	{
		if (Capacity == 0)
		{
			throw std::invalid_argument("Capacity must be greater than 0.");
		}
		Data = new T[Capacity];
	}

	~ArrayQueue()
	{
		delete[] Data;
	}

	ArrayQueue(const ArrayQueue&) = delete;
	ArrayQueue& operator=(const ArrayQueue&) = delete;

	std::size_t Size() const
	{
		return CurrentSize;
	}

	std::size_t GetCapacity() const
	{
		return Capacity;
	}

	bool IsEmpty() const
	{
		return CurrentSize == 0;
	}

	bool IsFull() const
	{
		return CurrentSize == Capacity;
	}

	const T& Peek() const
	{
		if (IsEmpty())
		{
			throw std::out_of_range("ArrayQueue is empty.");
		}
		return Data[FrontIndex];
	}

	void Enqueue(const T& Element)
	{
		if (IsFull())
		{
			throw std::out_of_range("ArrayQueue is full.");
		}
		Data[BackIndex] = Element;
		BackIndex = (BackIndex + 1) % Capacity;
		++CurrentSize;
	}

	T Dequeue()
	{
		if(IsEmpty())
		{
			throw std::out_of_range("ArrayQueue is empty.");
		}
		T DequeuedElement = std::move(Data[FrontIndex]);
		FrontIndex = (FrontIndex + 1) % Capacity;
		--CurrentSize;

		return DequeuedElement;
	}

	friend std::ostream& operator<<(std::ostream& OutputStream, const ArrayQueue& InQueue)
	{
		OutputStream << "[";
		for (std::size_t i = 0; i < InQueue.CurrentSize; ++i)
		{
			OutputStream << InQueue.Data[(InQueue.FrontIndex + i) % InQueue.Capacity];
			if (i < InQueue.CurrentSize - 1)
			{
				OutputStream << ", ";
			}
		}
		OutputStream << "]";
		return OutputStream;
	}

private:
	T* Data = nullptr;
	std::size_t CurrentSize = 0;
	std::size_t Capacity = 0;

	std::size_t FrontIndex = 0; // Index of the front element in the queue
	std::size_t BackIndex = 0;  // Index where the next element will be enqueued
};