#pragma once

#include <algorithm>
#include <cstddef>
#include <ostream>
#include "ArrayQueue.h"
#include "DynamicArray.h"

enum class TraversalOrder
{
	InOrder,
	PreOrder,
	PostOrder,
	LevelOrder
};

template <typename T>
class BinarySearchTree
{
private:
	struct Node
	{
		T Data;
		Node* Left;
		Node* Right;

		Node(const T& Value, Node* LeftNode, Node* RightNode)
			: Data(Value), Left(LeftNode), Right(RightNode)
		{
		}
	};

	

public:
	BinarySearchTree() = default;

	~BinarySearchTree()
	{
		Clear();
	}

	BinarySearchTree(const BinarySearchTree&) = delete;
	BinarySearchTree& operator=(const BinarySearchTree&) = delete;

	bool IsEmpty() const
	{
		return Root == nullptr;
	}

	bool Add(const T& Value)
	{
		// If Value already exists, do not add it again.
		if (Contains(Value)) return false;
		Root = Add(Root, Value);
		++NodeCount;
		return true;
	}

	bool Contains(const T& Value) const
	{
		return Contains(Root, Value);
	}

	void Clear()
	{
		Clear(Root);
		Root = nullptr;
		NodeCount = 0;
	}

	bool Remove(const T& Value)
	{
		// Make sure the node to remove actually exist before remove it
		if (Contains(Value))
		{
			Root = Remove(Root, Value);
			--NodeCount;
			return true;
		}
		return false;
	}

	int Height() const
	{
		return Height(Root);
	}

	std::size_t Size() const
	{
		return NodeCount;
	}

	DynamicArray<T> Traverse(TraversalOrder Order) const
	{
		DynamicArray<T> Result;
		switch (Order)
		{
			case TraversalOrder::InOrder:
				InOrderTraversal(Root, Result);
				break;
			case TraversalOrder::PreOrder:
				PreOrderTraversal(Root, Result);
				break;
			case TraversalOrder::PostOrder:
				PostOrderTraversal(Root, Result);
				break;
			case TraversalOrder::LevelOrder:
				LevelOrderTraversal(Root, Result);
				break;
		}
		return Result;
	}

	friend std::ostream& operator<<(std::ostream& OutputStream, const BinarySearchTree& Tree)
	{
		OutputStream << Tree.Traverse(TraversalOrder::InOrder);
		return OutputStream;
	}

private:
	// Private recursive method to add a value to the tree. It returns the new root of the subtree.
	Node* Add(Node* CurrentNode, const T& Value)
	{
		// Base case: found a leaf node
		if(CurrentNode == nullptr)
		{
			return new Node(Value, nullptr, nullptr);
		}
		
		// Place lower elements values in the left subtree
		if (Value < CurrentNode->Data)
		{
			CurrentNode->Left = Add(CurrentNode->Left, Value);
		}
		else// Place higher elements values in the right subtree
		{
			CurrentNode->Right = Add(CurrentNode->Right, Value);
		}
		return CurrentNode;
	}

	Node* Remove(Node* CurrentNode, const T& Value)
	{
		if (CurrentNode == nullptr) return nullptr;

		if (Value < CurrentNode->Data) // Dig into left subtree
		{
			CurrentNode->Left = Remove(CurrentNode->Left, Value);
		}
		else if (CurrentNode->Data < Value) // Dig into right subtree
		{
			CurrentNode->Right = Remove(CurrentNode->Right, Value);
		}
		else // Found the node to remove
		{
			if (CurrentNode->Left == nullptr) 
			{
				// This is the case with only a right subtree or no subtree at all.
				// In this situation just swap the node to remove with its right child.
				Node* RightChild = CurrentNode->Right;
				delete CurrentNode;
				return RightChild;
			}
			else if (CurrentNode->Right == nullptr)
			{
				// This is the case with only a left subtree.
				// In this situation just swap the node to remove with its left child.
				Node* LeftChild = CurrentNode->Left;
				delete CurrentNode;
				return LeftChild;
			}
			else
			{
				// When remove a node from a binary tree with two links, the sucessor of
				// the node being removed can either be the largest value in the left subtree
				// or the smallest value in the right subtree. In this implementation we will find 
				// the smallest value in the right subtree witch can be found by traversing as far
				// left as possible in the right subtree.

				// Find the leftmost node in the right subtree
				Node* Temp = DigLeft(CurrentNode->Right);

				// Swap the data
				CurrentNode->Data = Temp->Data;

				// Go into the right subtree and remove the leftmost node founded
				// and swapped data with. This prevent the tree from has two nodes
				// with the same value.
				CurrentNode->Right = Remove(CurrentNode->Right, Temp->Data);
			}
		}
		return CurrentNode;
	}

	Node* DigLeft(Node* InNode)
	{
		Node* CurrentNode = InNode;
		while (CurrentNode->Left != nullptr)
		{
			CurrentNode = CurrentNode->Left;
		}
		return CurrentNode;
	}

	// Private recursive method to check if a value exists in the tree.
	bool Contains(Node* CurrentNode, const T& Value) const
	{
		// Base case: reached a leaf node without finding the value
		if (CurrentNode == nullptr) return false;

		if (Value < CurrentNode->Data) // Dig into left subtree
		{
			return Contains(CurrentNode->Left, Value);
		}
		else if (CurrentNode->Data < Value) // Dig into right subtree
		{
			return Contains(CurrentNode->Right, Value);
		}
		else // If not less not greater, found the node with the value
		{
			return true;
		}
	}
	
	int Height(Node* CurrentNode) const
	{
		if (CurrentNode == nullptr) return -1;
		return std::max(Height(CurrentNode->Left), Height(CurrentNode->Right)) + 1;
	}

	void PreOrderTraversal(Node* CurrentNode, DynamicArray<T>& Result) const
	{
		if (CurrentNode == nullptr) return;
		Result.Add(CurrentNode->Data);
		PreOrderTraversal(CurrentNode->Left, Result);
		PreOrderTraversal(CurrentNode->Right, Result);
	}

	void InOrderTraversal(Node* CurrentNode, DynamicArray<T>& Result) const
	{
		if (CurrentNode == nullptr) return;
		InOrderTraversal(CurrentNode->Left, Result);
		Result.Add(CurrentNode->Data);
		InOrderTraversal(CurrentNode->Right, Result);
	}

	void PostOrderTraversal(Node* CurrentNode, DynamicArray<T>& Result) const
	{
		if (CurrentNode == nullptr) return;
		PostOrderTraversal(CurrentNode->Left, Result);
		PostOrderTraversal(CurrentNode->Right, Result);
		Result.Add(CurrentNode->Data);
	}

	void LevelOrderTraversal(Node* CurrentNode, DynamicArray<T>& Result) const
	{
		if (CurrentNode == nullptr) return;
		ArrayQueue<Node*> Queue(NodeCount);
		Queue.Enqueue(CurrentNode);
		while (!Queue.IsEmpty())
		{
			Node* Current = Queue.Dequeue();
			if (Current == nullptr) continue;

			Result.Add(Current->Data);

			if (Current->Left != nullptr)
			{
				Queue.Enqueue(Current->Left);
			}
			if (Current->Right != nullptr)
			{
				Queue.Enqueue(Current->Right);
			}
		}
	}

	void Clear(Node* CurrentNode)
	{
		if (CurrentNode == nullptr) return;
		Clear(CurrentNode->Left);
		Clear(CurrentNode->Right);
		delete CurrentNode;
	}

private:
	std::size_t NodeCount = 0;
	Node* Root = nullptr;
};