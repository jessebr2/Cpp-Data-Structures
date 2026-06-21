#pragma once

#include <algorithm>
#include <cstddef>
#include <stdexcept>

/**
* UnionFind/Disjoint Set data structure implementation
* This code is an C++ inspired modification of the Java union find implementation of 
* William Fiset, which, in turn, was inspired by Robert Sedgewick and Kevin Wayne in
* 'Algoritms Fourth Edition'.
* 
* @author Jesse Andrade, jesse.berto.andrade@gmail.com
*/

class UnionFind
{
private:
	// The number of elements in this union find
	std::size_t elementCount;

	// Used to track the size of each of the components
	std::size_t* sz;

	// parent of each node i, if parent[i] = i then i is a root node
	std::size_t* parent;

	// Tracks the number of components in the union find
	std::size_t numComponents;

public:
	UnionFind(std::size_t inElements)
		: elementCount(inElements)
	{
		if (elementCount == 0) throw std::invalid_argument("The size must be greater than 0");

		numComponents = elementCount;
		sz = new std::size_t[elementCount];
		parent = new std::size_t[elementCount];

		std::fill_n(sz, numComponents, 1); // Each component is originally of size one

		// Each component is originally linked to itself
		for (std::size_t i = 0; i < elementCount; ++i)
		{
			parent[i] = i;
		}
	}

	~UnionFind()
	{
		delete[] sz;
		delete[] parent;
	}

	UnionFind(const UnionFind&) = delete;
	UnionFind& operator=(const UnionFind&) = delete;

	// Find which component/set 'p' belongs to, takes amortized constant time.
	std::size_t find(std::size_t p)
	{
		validateIndex(p);
		// Find the root of the component/set
		std::size_t root = p;
		while (root != parent[root])
			root = parent[root];

		/**
		* Compress the path leading back to the root.
		* Doing this operation is called "path compression"
		* and is what gives us amortized constant time complexity
		*/
		while (p != root)
		{
			std::size_t next = parent[p];
			parent[p] = root;
			p = next;
		}
		return root;
	}

	// Return whether or not the elements 'p' and 'q' are in the same components/set.
	bool connected(std::size_t p, std::size_t q)
	{
		return find(p) == find(q);
	}

	// Return the size of the components/set 'p' belongs to
	std::size_t componentSize(std::size_t p)
	{
		return sz[find(p)];
	}

	// Return the number of elements in this UnionFind/Disjoint set
	std::size_t size() const { return elementCount; }

	// Return the number of remaining components/sets
	std::size_t components() { return numComponents; }

	void unify(std::size_t p, std::size_t q)
	{
		const std::size_t root1 = find(p);
		const std::size_t root2 = find(q);

		// These elements are already in the same group!
		if (root1 == root2) return;

		// Merge two components/sets together. Merge smaller into the large one.
		if (sz[root1] < sz[root2])
		{
			sz[root2] += sz[root1];
			parent[root1] = root2;
		}
		else
		{
			sz[root1] += sz[root2];
			parent[root2] = root1;
		}

		// Since the roots found are different we know that the number
		// number of components/sets has decreased by one
		--numComponents;
	}

private:
	void validateIndex(std::size_t p) const
	{
		if (p >= size())
		{
			throw std::out_of_range("UnionFind index outo of range");
		}
	}
};