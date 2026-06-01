#pragma once
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <ostream>
#include <stdexcept>
#include <unordered_map>
#include <set>
#include <utility>
#include "DynamicArray.h"

template <typename T, typename Compare = std::less<T>>
class PriorityQueue
{
private:
	DynamicArray<T> Heap;
	Compare Comparator;
	std::unordered_map<T, std::set<std::size_t>> Map;

public:
	PriorityQueue() = default;

	PriorityQueue(std::initializer_list<T> Elements)
	{
		// Place all elements in Heap
		for (const T& Element : Elements)
		{
			MapAdd(Element, Heap.Size());
			Heap.Add(Element);
		}

		// Heapify process, O(n)
		if (Heap.Size() > 1)
		{
			for (std::size_t i = Heap.Size() / 2; i > 0; --i)
			{
				Sink(i - 1);
			}
		}
	}

	PriorityQueue(const PriorityQueue&) = delete;
	PriorityQueue& operator=(const PriorityQueue&) = delete;

	bool IsEmpty() const
	{
		return Heap.IsEmpty();
	}

	std::size_t Size() const
	{
		return Heap.Size();
	}

	void Clear()
	{
		Heap.Clear();
		Map.clear();
	}

	const T& Peek() const
	{
		if(IsEmpty())
		{
			throw std::out_of_range("PriorityQueue is empty.");
		}
		return Heap.Get(0);
	}

	T Poll()
	{
		if(IsEmpty())
		{
			throw std::out_of_range("PriorityQueue is empty.");
		}
		return RemoveAt(0);
	}

	bool Contains(const T& Element) const
	{
		auto It = Map.find(Element);

		return It != Map.end() && !It->second.empty();
	}

	void Add(const T& Element)
	{
		MapAdd(Element, Heap.Size());
		Heap.Add(Element);
		Swim(Heap.Size() - 1);
	}

	bool Remove(const T& Element)
	{
		if (!Contains(Element)) return false;

		// Logarithmic removal with map, O(log(n))
		std::size_t Index = MapGet(Element); // Get an index of the element to remove
		RemoveAt(Index);
		return true;
	}

	/**
	* Recursively checks if this heap is a min heap.
	* This method is just for testing purposes to make sure the heap property is maintained after operations.
	* Call this method with index 0 to check the entire heap.
	*/
	bool IsMinHeap(std::size_t Index = 0) const
	{
		//If we are outside the bounds of the tree, return true since we have not found any violations of the min heap property
		if (Index >= Heap.Size()) return true;

		std::size_t Left = 2 * Index + 1;
		std::size_t Right = 2 * Index + 2;

		/**
		 * Make sure that the current node Index is less than its left and right children, if they exist. Then recursively check the left and right subtrees.
		 * If any of these checks fail, return false to indicate that this is not a min heap.
		 *
		*/
		if (Left < Heap.Size() && Less(Left, Index)) return false;
		if (Right < Heap.Size() && Less(Right, Index)) return false;

		// Recurse on both children to make sure they are also min heaps
		return IsMinHeap(Left) && IsMinHeap(Right);
	}

	friend std::ostream& operator<<(std::ostream& OutputStream, const PriorityQueue& PQ)
	{
		OutputStream << PQ.Heap;
		return OutputStream;
	}

private:
	bool Less(std::size_t i, std::size_t j) const
	{
		return Comparator(Heap.Get(i), Heap.Get(j));
	}

	// Bottom up node swim, O(log n)
	void Swim(std::size_t Index)
	{
		// Grab the index of the next parent node to compare with
		std::size_t ParentIndex = (Index - 1) / 2;

		// Keep swimming whule we have not reached the root and while we're less than our parent
		while (Index > 0 && Less(Index, ParentIndex))
		{
			Swap(ParentIndex, Index);
			Index = ParentIndex;
			ParentIndex = (Index - 1) / 2; // New parent
		}
	}

	// Top down node sink, O(log n)
	void Sink(std::size_t Index)
	{
		std::size_t Size = Heap.Size();
		while (true)
		{
			std::size_t Left = 2 * Index + 1; // Left child index
			std::size_t Right = 2 * Index + 2; // Right child index
			std::size_t Smallest = Left; // Assume Left is the smallest node of the two children

			// Find which is smaller between Left and Right
			// If Right is smaller set smallest to Right
			if (Right < Size && Less(Right, Left))
			{
				Smallest = Right;
			}

			// Stop if we're outside the bounds of the tree or stop early if we cannot sink Index any further
			if (Left >= Size || !Less(Smallest, Index))
			{
				break;
			}

			// Move down the tree following the smallest node
			Swap(Smallest, Index);
			Index = Smallest;
		}
	}

	void Swap(std::size_t i, std::size_t j)
	{
		MapSwap(Heap.Get(i), Heap.Get(j), i, j); //*

		T Temp = std::move(Heap.Get(i));
		Heap.Set(i, std::move(Heap.Get(j)));
		Heap.Set(j, std::move(Temp));
	}

	T RemoveAt(std::size_t Index)
	{
		if (IsEmpty())
		{
			throw std::out_of_range("PriorityQueue is empty.");
		}

		std::size_t LastIndex = Heap.Size() - 1;
		const bool IsLastElement = Index == LastIndex;

		if (!IsLastElement)
		{
			// Swap the element to remove with the last element
			Swap(Index, LastIndex);
		}

		// Remove the last element (the one we want to remove)
		T RemovedElement = Heap.RemoveAt(LastIndex);

		// Remove from Map
		MapRemove(RemovedElement, LastIndex);

		// If we removed the last element, we're done
		if (IsLastElement)
		{
			return RemovedElement;
		}

		T Element = Heap.Get(Index);

		// Try sinking first
		Sink(Index);

		// If that doesn't work, try swimming
		if (Heap.Get(Index) == Element)
		{
			Swim(Index);
		}
		return RemovedElement;
	}

	// Add a value and its index to the Map
	void MapAdd(const T& Element, std::size_t Index)
	{
		Map[Element].insert(Index);
	}

	// Remove the Index at a given value, O(log(n))
	void MapRemove(const T& Element, std::size_t Index)
	{
		auto It = Map.find(Element);

		if (It == Map.end()) return;

		It->second.erase(Index);

		if (It->second.empty())
		{
			Map.erase(It);
		}
	}

	/**
	* Extract an index position for the given value
	* Note: If a value exists multiple times in the heap the highest index is returned (this has arbitrarily been chosen)
	*/
	std::size_t MapGet(const T& Element)
	{
		auto It = Map.find(Element);
		if (It == Map.end() || It->second.empty())
		{
			throw std::out_of_range("Element does not exist in the heap.");
		}
		return *It->second.rbegin();
	}

	// Exchange the index of two nodes internally within the map
	void MapSwap(const T& Element1, const T& Element2, std::size_t Index1, std::size_t Index2)
	{
		if (Element1 == Element2) return; 

		auto It1 = Map.find(Element1);
		auto It2 = Map.find(Element2);

		if (It1 != Map.end())
		{
			It1->second.erase(Index1);
			It1->second.insert(Index2);
		}

		if (It2 != Map.end())
		{
			It2->second.erase(Index2);
			It2->second.insert(Index1);
		}
	}
};