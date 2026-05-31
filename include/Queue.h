#pragma once

#include <cstddef>
#include <stdexcept>
#include <ostream>
#include "LinkedList.h"

template <typename T>
class Queue
{
private:
	DoublyLinkedList<T> List;

public:
	std::size_t Size() const
	{
		return List.Size();
	}

	bool IsEmpty() const
	{
		return List.IsEmpty();
	}

	const T& Peek() const
	{
		if (IsEmpty())
		{
			throw std::out_of_range("Queue is empty.");
		}
		return List.PeekFirst();
	}

	void Enqueue(const T& Element)
	{
		List.AddLast(Element);
	}

	T Dequeue()
	{
		if (IsEmpty())
		{
			throw std::out_of_range("Queue is empty.");
		}
		return List.RemoveFirst();
	}

	friend std::ostream& operator<<(std::ostream& OutputStream, const Queue& InQueue)
	{
		OutputStream << InQueue.List;
		return OutputStream;
	}
};