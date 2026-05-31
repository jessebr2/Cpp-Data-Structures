#pragma once
#include <cstddef>
#include <stdexcept>
#include "LinkedList.h"

template <typename T>
class ListStack
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

	void Push(const T& Element)
	{
		List.AddLast(Element);
	}

	T Pop()
	{
		if (IsEmpty())
		{
			throw std::out_of_range("Stack is empty.");
		}
		return List.RemoveLast();
	}

	const T& Peek() const
	{
		if (IsEmpty())
		{
			throw std::out_of_range("Stack is empty.");
		}
		return List.PeekLast();
	}


	friend std::ostream& operator<<(std::ostream& OutputStream, const ListStack& Stack)
	{
		OutputStream << Stack.List;
		return OutputStream;
	}
};