#pragma once
#include <cstddef>
#include <stdexcept>
#include <ostream>
#include "DynamicArray.h"

template <typename T>
class ArrayStack
{
private:
	DynamicArray<T> Array;

public:
	std::size_t Size() const
	{
		return Array.Size();
	}

	bool IsEmpty() const
	{
		return Array.IsEmpty();
	}

	void  Push(const T& Element)
	{
		Array.Add(Element);
	}

	T Pop()
	{
		if (IsEmpty())
		{
			throw std::out_of_range("Stack is empty.");
		}
		return Array.RemoveAt(Array.Size() - 1);
	}

	const T& Peek() const
	{
		if (IsEmpty())
		{
			throw std::out_of_range("Stack is empty.");
		}
		return Array.Get(Array.Size() - 1);
	}

	friend std::ostream& operator<<(std::ostream& OutputStream, const ArrayStack& Stack)
	{
		OutputStream << Stack.Array;
		return OutputStream;
	}

};