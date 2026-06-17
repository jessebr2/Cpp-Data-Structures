#pragma once

#include <functional>
#include <list>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <vector>

template <typename K, typename V>
class SeparateChainingHashTable
{
private:
	struct Node
	{
		K Key;
		V Value;
		std::size_t Hash;

		Node() = delete;
		Node(const K& InKey, const V& InValue)
			: Key(InKey), Value(InValue), Hash(std::hash<K>{}(InKey)) { }

		bool operator==(const Node& Other) const
		{
			return Hash == Other.Hash && Key == Other.Key;
		}

		friend std::ostream& operator<<(std::ostream& OutputStream, const Node& Node)
		{
			OutputStream << "{" << Node.Key << ": " << Node.Value << "}";
			return OutputStream;
		}
	};

public:
	SeparateChainingHashTable()
		: SeparateChainingHashTable(DefaultCapacity, DefaultLoadFactor)
	{
	}

	SeparateChainingHashTable(std::size_t InCapacity)
		: SeparateChainingHashTable(InCapacity, DefaultLoadFactor)
	{
	}

	SeparateChainingHashTable(std::size_t InCapacity, double InMaxLoadFactor)
		: MaxLoadFactor(InMaxLoadFactor), Capacity(InCapacity), Threshold(static_cast<std::size_t>(InCapacity * InMaxLoadFactor))
	{
		if(InMaxLoadFactor <= 0.0 || InMaxLoadFactor >= 1.0)
		{
			throw std::invalid_argument("Load factor must be between 0 and 1.");
		}
		Table = new std::list<Node>[Capacity];
	}

	SeparateChainingHashTable(const SeparateChainingHashTable&) = delete;
	SeparateChainingHashTable& operator=(const SeparateChainingHashTable&) = delete;

	~SeparateChainingHashTable()
	{
		delete[] Table;
	}

	std::size_t Size() const { return CurrentSize; }

	bool IsEmpty() const { return CurrentSize == 0; }

	void Clear()
	{
		for(std::size_t i = 0; i< Capacity; ++i)
		{
			Table[i].clear();
		}
		CurrentSize = 0;
	}

	bool ContainsKey(const K& InKey) const { return HasKey(InKey); }

	bool HasKey(const K& InKey) const
	{
		std::size_t BucketIndex = NormalizeIndex(std::hash<K>{}(InKey));
		return BucketSeekEntry(BucketIndex, InKey) != nullptr;
	}

	V Put(const K& InKey, const V& InValue) { return Insert(InKey, InValue); }
	V Add(const K& InKey, const V& InValue) { return Insert(InKey, InValue); }

	V Insert(const K& InKey, const V& InValue)
	{
		if(InKey == K{})
		{
			throw std::invalid_argument("Key cannot be default value.");
		}
		Node NewNode(InKey, InValue);
		std::size_t BucketIndex = NormalizeIndex(NewNode.Hash);
		return BucketInsertEntry(BucketIndex, NewNode);
	}

	std::optional<V> Get(const K& InKey) const
	{
		if (InKey != K{})
		{
			std::size_t BucketIndex = NormalizeIndex(std::hash<K>{}(InKey));
			const Node* FoundNode = BucketSeekEntry(BucketIndex, InKey);
			if (FoundNode) return FoundNode->Value;
		}
		return std::nullopt;
	}

	V Remove(const K& InKey)
	{
		if(InKey == K{})
		{
			throw std::invalid_argument("Key cannot be default value.");
		}

		std::size_t BucketIndex = NormalizeIndex(std::hash<K>{}(InKey));
		return BucketRemoveEntry(BucketIndex, InKey);
	}

	std::vector<K> Keys() const
	{
		std::vector<K> KeysArray;
		KeysArray.reserve(CurrentSize);
		for (std::size_t i = 0; i < Capacity; ++i)
		{
			if (Table[i].empty()) continue;
			for (const Node& NodeElement : Table[i])
			{
				KeysArray.push_back(NodeElement.Key);
			}
		}
		return KeysArray;
	}

	std::vector<V> Values() const
	{
		std::vector<V> ValuesArray;
		ValuesArray.reserve(CurrentSize);
		for (std::size_t i = 0; i < Capacity; ++i)
		{
			if (Table[i].empty()) continue;
			for (const Node& NodeElement : Table[i])
			{
				ValuesArray.push_back(NodeElement.Value);
			}
		}
		return ValuesArray;
	}

	friend std::ostream& operator<<(std::ostream& OutputStream, const SeparateChainingHashTable& HashTable)
	{
		OutputStream << "{";
		bool FirstEntry = true;
		for (std::size_t i = 0; i < HashTable.Capacity; ++i)
		{
			if (HashTable.Table[i].empty()) continue;
			for(const auto& NodeElement : HashTable.Table[i])
			{
				if (!FirstEntry)
				{
					OutputStream << ", ";
				}
				OutputStream << NodeElement;
				FirstEntry = false;
			}
		}
		OutputStream << "}";
		return OutputStream;
	}

private:
	std::size_t NormalizeIndex(std::size_t KeyHash) const
	{
		return KeyHash % Capacity;
	}

	V BucketRemoveEntry(std::size_t BucketIndex, const K& InKey)
	{
		Node* NodeToRemove = BucketSeekEntry(BucketIndex, InKey);
		if (NodeToRemove)
		{
			V RemovedValue = std::move(NodeToRemove->Value);
			Table[BucketIndex].remove(*NodeToRemove);
			--CurrentSize;
			return RemovedValue;
		}
		throw std::out_of_range("Key not found.");
	}

	V BucketInsertEntry(std::size_t BucketIndex, const Node& NewNode)
	{
		Node* ExistentNode = BucketSeekEntry(BucketIndex, NewNode.Key);
		if (ExistentNode == nullptr)
		{
			Table[BucketIndex].push_back(NewNode);
			if (++CurrentSize > Threshold) ResizeTable();
			return V(); // Return default value for new entry
		}
		else
		{
			V OldValue = std::move(ExistentNode->Value);
			ExistentNode->Value = NewNode.Value;
			return OldValue; // Return old value for existing entry
		}

	}

	Node* BucketSeekEntry(std::size_t BucketIndex, const K& InKey)
	{
		for (Node& NodeElement : Table[BucketIndex])
		{
			if (NodeElement.Key == InKey)
			{
				return &NodeElement;
			}
		}
		return nullptr;
	}

	const Node* BucketSeekEntry(std::size_t BucketIndex, const K& InKey) const
	{
		for (const Node& NodeElement : Table[BucketIndex])
		{
			if (NodeElement.Key == InKey)
			{
				return &NodeElement;
			}
		}
		return nullptr;
	}

	void ResizeTable()
	{
		const auto OldCapacity = Capacity;
		Capacity *= 2;
		Threshold = static_cast<std::size_t>(Capacity * MaxLoadFactor);
		
		std::list<Node>* NewTable = new std::list<Node>[Capacity];

		for (std::size_t i = 0; i < OldCapacity; ++i)
		{
			if (Table[i].empty()) continue;
			for (const Node& NodeElement : Table[i])
			{
				std::size_t NewBucketIndex = NormalizeIndex(NodeElement.Hash);
				NewTable[NewBucketIndex].push_back(NodeElement);
			}
			Table[i].clear();
		}
		delete[] Table;
		Table = NewTable;
	}

private:
	static constexpr std::size_t DefaultCapacity = 3;
	static constexpr double DefaultLoadFactor = 0.75;

	double MaxLoadFactor;
	std::size_t Capacity, Threshold, CurrentSize = 0;
	std::list<Node>* Table = nullptr;
};