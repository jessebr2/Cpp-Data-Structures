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

	std::size_t size() const
	{
		return current_size_;
	}

	bool empty() const
	{
		return current_size_ == 0;
	}

	std::size_t capacity() const
	{
		return Capacity;
	}

	bool full() const
	{
		return current_size_ == Capacity;
	}

	const T& peek() const
	{
		if (empty())
		{
			throw std::out_of_range("ArrayQueue is empty.");
		}
		return Data[front_index_];
	}

	void enqueue(const T& Element)
	{
		if (full())
		{
			throw std::out_of_range("ArrayQueue is full.");
		}
		Data[back_index_] = Element;
		back_index_ = (back_index_ + 1) % Capacity;
		++current_size_;
	}

	T dequeue()
	{
		if(empty())
		{
			throw std::out_of_range("ArrayQueue is empty.");
		}
		T DequeuedElement = std::move(Data[front_index_]);
		front_index_ = (front_index_ + 1) % Capacity;
		--current_size_;

		return DequeuedElement;
	}

	friend std::ostream& operator<<(std::ostream& OutputStream, const ArrayQueue& InQueue)
	{
		OutputStream << "[";
		for (std::size_t i = 0; i < InQueue.current_size_; ++i)
		{
			OutputStream << InQueue.Data[(InQueue.front_index_ + i) % InQueue.Capacity];
			if (i < InQueue.current_size_ - 1)
			{
				OutputStream << ", ";
			}
		}
		OutputStream << "]";
		return OutputStream;
	}

private:
	// Cache line 1: Ptr 
	T* Data = nullptr;
	std::size_t current_size_ = 0;
	std::size_t Capacity = 0;

	std::size_t front_index_ = 0; // Index of the front element in the queue
	std::size_t back_index_ = 0;  // Index where the next element will be enqueued
};