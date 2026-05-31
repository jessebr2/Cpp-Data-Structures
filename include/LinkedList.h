#pragma once
#include <cstddef>
#include <ostream>
#include <stdexcept>
#include <utility>

template <typename T>
class DoublyLinkedList
{

public:
	DoublyLinkedList() = default;

	~DoublyLinkedList()
	{
		Clear();
	}

	DoublyLinkedList(const DoublyLinkedList&) = delete;
	DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;

private:
	struct Node
	{
		T Data;
		Node* Previous;
		Node* Next;

		explicit Node(const T& Value)
			: Data(Value), Previous(nullptr), Next(nullptr)
		{
		}

		Node(const T& Value, Node* PreviousNode, Node* NextNode)
			: Data(Value), Previous(PreviousNode), Next(NextNode)
		{
		}

		friend std::ostream& operator<<(std::ostream& OutputStream, const Node& Node)
		{
			OutputStream << Node.Data;
			return OutputStream;
		}
	};

public:
	void Clear()
	{
		Node* Trav = Head;
		while (Trav != nullptr)
		{
			Node* Next = Trav->Next;
			delete Trav;
			Trav = Next;
		}
		CurrentSize = 0;
		Head = nullptr;
		Tail = nullptr;
	}

	std::size_t Size() const
	{
		return CurrentSize;
	}

	bool IsEmpty() const
	{
		return CurrentSize == 0;
	}

	void Add(const T& Element)
	{
		AddLast(Element);
	}

	void AddFirst(const T& Element)
	{
		if (IsEmpty())
		{
			Head = Tail = new Node(Element);
		}
		else
		{
			Head->Previous = new Node(Element, nullptr, Head);
			Head = Head->Previous;
		}
		++CurrentSize;
	}
	
	void AddLast(const T& Element)
	{
		if (IsEmpty())
		{
			Head = Tail = new Node(Element);
		}
		else
		{
			Tail->Next = new Node(Element, Tail, nullptr);
			Tail = Tail->Next;
		}
		++CurrentSize;
	}

	const T& PeekFirst() const
	{
		if (IsEmpty())
		{
			throw std::out_of_range("List is empty.");
		}
		return Head->Data;
	}

	const T& PeekLast() const
	{
		if (IsEmpty())
		{
			throw std::out_of_range("List is empty.");
		}
		return Tail->Data;
	}

	T RemoveFirst()
	{
		if(IsEmpty())
		{
			throw std::out_of_range("List is empty.");
		}

		Node* NodeToRemove = Head;
		T RemovedData = std::move(Head->Data);

		Head = Head->Next;
		--CurrentSize;

		if (IsEmpty())
		{
			Tail = nullptr;
		}
		else
		{
			Head->Previous = nullptr;
		}

		delete NodeToRemove;

		return RemovedData;
	}

	T RemoveLast()
	{
		if (IsEmpty())
		{
			throw std::out_of_range("List is empty.");
		}

		Node* NodeToRemove = Tail;
		T RemovedData = std::move(Tail->Data);

		Tail = Tail->Previous;
		--CurrentSize;

		if (IsEmpty())
		{
			Head = nullptr;
		}
		else
		{
			Tail->Next = nullptr;
		}

		delete NodeToRemove;

		return RemovedData;
	}

	T RemoveAt(std::size_t Index)
	{
		if (Index >= CurrentSize)
		{
			throw std::out_of_range("Index out of range.");
		}

		Node* Trav;

		if (Index < CurrentSize / 2) // Search from the head
		{
			Trav = Head;
			for (std::size_t i = 0; i != Index; ++i)
			{
				Trav = Trav->Next;
			}
		}
		else // Search from the tail
		{
			Trav = Tail;
			for (std::size_t i = CurrentSize - 1; i != Index; --i)
			{
				Trav = Trav->Previous;
			}
		}
		return RemoveNode(Trav);
	}

	bool Remove(const T& Value) //O(n)
	{
		Node* Trav = Head;
		while (Trav != nullptr)
		{
			if (Trav->Data == Value)
			{
				RemoveNode(Trav);
				return true;
			}
			Trav = Trav->Next;
		}
		return false;
	}

	int IndexOf(const T& Value) const
	{
		int Index = 0;
		Node* Trav = Head;

		while (Trav != nullptr)
		{
			if (Trav->Data == Value)
			{
				return Index;
			}
			Trav = Trav->Next;
			++Index;
		}
		return -1;
	}

	bool Contains(const T& Value) const
	{
		return IndexOf(Value) != -1;
	}

	friend std::ostream& operator<<(std::ostream& OutputStream, const DoublyLinkedList& List)
	{
		Node* Trav = List.Head;
		OutputStream << "[";
		while (Trav != nullptr)
		{
			OutputStream << Trav->Data;
			if (Trav->Next != nullptr)
			{
				OutputStream << ", ";
			}
			Trav = Trav->Next;
		}
		OutputStream << "]";
		return OutputStream;
	}

private:
	T RemoveNode(Node* RemovedNode)
	{
		if (RemovedNode == nullptr)
		{
			throw std::invalid_argument("Node to remove cannot be null.");
		}
		if (RemovedNode->Previous == nullptr) // RemovedNode is the head
		{
			return RemoveFirst();
		}
		if (RemovedNode->Next == nullptr) // RemovedNode is the tail
		{
			return RemoveLast();
		}
		
		// Make the pointers of the adjacent nodes skip over 'RemovedNode'
		RemovedNode->Previous->Next = RemovedNode->Next;
		RemovedNode->Next->Previous = RemovedNode->Previous;

		T RemovedData = std::move(RemovedNode->Data);

		delete RemovedNode;
		--CurrentSize;

		return RemovedData;
	}

	std::size_t CurrentSize = 0;
	Node* Head = nullptr;
	Node* Tail = nullptr;

	
};