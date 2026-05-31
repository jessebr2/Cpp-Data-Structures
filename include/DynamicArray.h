#pragma once

#include <cstddef>
#include <stdexcept>
#include <ostream>
#include <utility>

template <typename T>
class DynamicArray
{
public:
	static constexpr std::size_t DefaultCapacity = 16;

	DynamicArray()
		: DynamicArray(DefaultCapacity)
	{
	}

	explicit DynamicArray(std::size_t InitialCapacity)
		: Data(nullptr), CurrentSize(0), TotalCapacity(InitialCapacity)
	{
		if (TotalCapacity > 0)
		{
			Data = new T[TotalCapacity];
		}
	}

	~DynamicArray()
	{
		delete[] Data;
	}

	std::size_t Size() const
	{
		return CurrentSize;
	}

	std::size_t Capacity() const
	{
		return TotalCapacity;
	}

	bool IsEmpty() const
	{
		return CurrentSize == 0;
	}
	
	T& Get(std::size_t Index)
	{
		CheckIndex(Index);
		return Data[Index];
	}

	const T& Get(std::size_t Index) const
	{
		CheckIndex(Index);
		return Data[Index];
	}

	void Set(std::size_t Index, const T& Value)
	{
		CheckIndex(Index);
		Data[Index] = Value;
	}

	void Clear()
	{
		CurrentSize = 0;
	}

	void Add(const T& Value)
	{
		EnsureCapacityForOneMoreElement();

		Data[CurrentSize] = Value;
		++CurrentSize;
	}

	void Add(T&& Value)
	{
		EnsureCapacityForOneMoreElement();

		Data[CurrentSize] = std::move(Value);
		++CurrentSize;
	}

	T RemoveAt(std::size_t Index)
	{
		CheckIndex(Index);

		T RemovedValue = std::move(Data[Index]);

		for (std::size_t CurrentIndex = Index; CurrentIndex < CurrentSize - 1; ++CurrentIndex)
		{
			Data[CurrentIndex] = std::move(Data[CurrentIndex + 1]);
		}
		--CurrentSize;
		return RemovedValue;
	}

	bool Remove(const T& Value)
	{
		const int FoundIndex = IndexOf(Value);
		if (FoundIndex == -1) return false;
		RemoveAt(static_cast<std::size_t>(FoundIndex));
		return true;
	}

	int IndexOf(const T& Value)const
	{
		for (std::size_t Index = 0; Index < CurrentSize; ++Index)
		{
			if (Data[Index] == Value)
			{
				return static_cast<int>(Index);
			}
		}
		return -1;
	}

	bool Contains(const T& Value) const
	{
		return IndexOf(Value) != -1;
	}

	T& operator[](std::size_t Index)
	{
		return Get(Index);
	}

	const T& operator[](std::size_t Index) const
	{
		return Get(Index);
	}

	T* begin()
	{
		return Data;
	}

	const T* begin() const
	{
		return Data;
	}

	T* end()
	{
		return Data + CurrentSize;
	}

	const T* end() const
	{
		return Data + Size;
	}

	friend std::ostream& operator<<(std::ostream& OutputStream, const DynamicArray& Array)
	{
		OutputStream << "[";
		
		for (std::size_t Index = 0; Index < Array.CurrentSize; ++Index)
		{
			OutputStream << Array.Data[Index];
			if (Index + 1 < Array.CurrentSize)
			{
				OutputStream << ", ";
			}
		}

		OutputStream << "]";

		return OutputStream;
	}

private:
	void CheckIndex(std::size_t Index) const
	{
		if (Index >= CurrentSize)
		{
			throw std::out_of_range("DynamicArray index out of range.");
		}
	}

	void EnsureCapacityForOneMoreElement()
	{
		if (CurrentSize < TotalCapacity) return;

		const std::size_t NewCapacity = TotalCapacity == 0 ? DefaultCapacity : TotalCapacity * 2;
		Resize(NewCapacity);
	}

	void Resize(std::size_t NewCapacity)
	{
		T* NewData = new T[NewCapacity];
		for (std::size_t Index = 0; Index < CurrentSize; ++Index)
		{
			NewData[Index] = std::move(Data[Index]);
		}

		delete[] Data;
		Data = NewData;
		TotalCapacity = NewCapacity;
	}

private:
	T* Data;
	std::size_t CurrentSize;
	std::size_t TotalCapacity;
};