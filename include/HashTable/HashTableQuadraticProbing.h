#pragma once

#include <algorithm>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename K, typename V>
class HashTableQuadraticProbing
{
private:
	enum class BucketState : std::uint8_t
	{
		Empty,
		Occupied,
		Deleted
	};

	double loadFactor;
	std::size_t capacity = 0, threshold = 0, modificationCount = 0;

	/**
	* 'usedBuckets' counts the total number of used buckets inside the hash-table (includes cells marked as deleted).
	* While 'keyCount' tracks the number of unique keys currently inside the hash-table.
	*/
	std::size_t usedBuckets = 0, keyCount = 0;

	// These arrays store the key-value pairs.
	K* keyTable;
	V* valueTable;
	BucketState* stateTable;

	// Flag used to indicate whether an item was found in the hash table
	bool bContains = false;

	static constexpr std::size_t DEFAULT_CAPACITY = 8;
	static constexpr double DEFAULT_LOAD_FACTOR = .45;

public:
	HashTableQuadraticProbing()
		: HashTableQuadraticProbing(DEFAULT_CAPACITY, DEFAULT_LOAD_FACTOR)
	{
	}

	HashTableQuadraticProbing(std::size_t inCapacity)
		: HashTableQuadraticProbing(inCapacity, DEFAULT_LOAD_FACTOR)
	{
	}

	HashTableQuadraticProbing(std::size_t inCapacity, double inLoadFactor)
		: loadFactor(inLoadFactor)
	{
		if (inLoadFactor <= 0.0 || inLoadFactor >= 1.0)
		{
			throw std::invalid_argument("Load factor must be between 0 and 1.");
		}
		capacity = next2Power(inCapacity);
		threshold = std::max<std::size_t>(1, static_cast<std::size_t> (capacity * loadFactor));
		keyTable = new K[capacity];
		valueTable = new V[capacity];
		stateTable = new BucketState[capacity];
		std::fill_n(stateTable, capacity, BucketState::Empty);
	}

	HashTableQuadraticProbing(const HashTableQuadraticProbing&) = delete;
	HashTableQuadraticProbing& operator=(const HashTableQuadraticProbing&) = delete;

	//move contructor
	HashTableQuadraticProbing(HashTableQuadraticProbing&& other) noexcept : 
		loadFactor(other.loadFactor),
		capacity(other.capacity),
		threshold(other.threshold),
		modificationCount(other.modificationCount),
		usedBuckets(other.usedBuckets),
		keyCount(other.keyCount),
		keyTable(other.keyTable),
		valueTable(other.valueTable),
		stateTable(other.stateTable),
		bContains(other.bContains)
	{
		other.capacity = 0;
		other.threshold = 0;
		other.modificationCount = 0;
		other.usedBuckets = 0;
		other.keyCount = 0;
		other.keyTable = nullptr;
		other.valueTable = nullptr;
		other.stateTable = nullptr;
	}

	//move assignment
	HashTableQuadraticProbing& operator=(HashTableQuadraticProbing&& other) noexcept
	{
		if (this != &other)
		{
			delete[] keyTable;
			delete[] valueTable;
			delete[] stateTable;

			loadFactor = other.loadFactor;
			capacity = other.capacity;
			threshold = other.threshold;
			modificationCount = other.modificationCount;
			usedBuckets = other.usedBuckets;
			keyCount = other.keyCount;
			keyTable = other.keyTable;
			valueTable = other.valueTable;
			stateTable = other.stateTable;
			bContains = other.bContains;

			other.capacity = 0;
			other.threshold = 0;
			other.modificationCount = 0;
			other.usedBuckets = 0;
			other.keyCount = 0;
			other.keyTable = nullptr;
			other.valueTable = nullptr;
			other.stateTable = nullptr;
		}
		return *this;
	}


	~HashTableQuadraticProbing()
	{
		delete[] keyTable;
		delete[] valueTable;
		delete[] stateTable;
	}

	void clear()
	{
		usedBuckets = 0;
		keyCount = 0;
		++modificationCount;
		std::fill_n(stateTable, capacity, BucketState::Empty);
	}

	// Returns the number of currently keys inside the hash-table
	std::size_t size() const { return keyCount; }

	bool empty() const { return keyCount == 0; }

	/**
	* if key already exists we'll update the value and return the old value
	* if it's a new key will return empty value V()
	*/
	V insert(K key, V value)
	{
		if (key == K{})	{ throw std::invalid_argument("Key cannot be default value."); }
		if (usedBuckets >= threshold)
		{
			++modificationCount;
			resizeTable();
		}

		const std::size_t hash = normalizeIndex(std::hash<K>{}(key));
		std::size_t i = hash;
		std::optional<std::size_t> deletedIndex; // deleted index if founded
		std::size_t x = 0; //x for prob

		while (true)
		{
			switch (stateTable[i])
			{
			case BucketState::Deleted:
				if (!deletedIndex.has_value()) deletedIndex = i;
				break;
			case BucketState::Occupied:
				if (keyTable[i] == key) // same key, update value
				{
					V oldValue = std::move(valueTable[i]);
					if (deletedIndex.has_value()) // a bucket was found before and will be used instead this current i
					{
						stateTable[i] = BucketState::Deleted;

						const std::size_t d = deletedIndex.value();
						stateTable[d] = BucketState::Occupied;
						keyTable[d] = std::move(key);
						valueTable[d] = std::move(value);
						++modificationCount;
					}
					else // just update the value
					{
						valueTable[i] = std::move(value);
					}
					return oldValue;
				}
				break;
			case BucketState::Empty:
				if (deletedIndex.has_value()) // a bucket was found before and will be used instead this current i
				{
					const std::size_t d = deletedIndex.value();
					stateTable[d] = BucketState::Occupied;
					keyTable[d] = std::move(key);
					valueTable[d] = std::move(value);
				}
				else // occupy the current i
				{
					stateTable[i] = BucketState::Occupied;
					keyTable[i] = std::move(key);
					valueTable[i] = std::move(value);
					++usedBuckets;
				}
				++keyCount;
				++modificationCount;
				return V{};
			default:
				break;
			}
			i = normalizeIndex(hash + P(++x));
		}
	}

	// Returns true/false on whether a given key exists within the hash-table
	bool contains(const K& key) { return hasKey(key); }

	// Returns true/false on whether a given key exists within the hash-table
	bool hasKey(const K& key)
	{
		// sets the 'bContains' flag
		get(key);
		return bContains;
	}

	/**
	* Get the value associated with the input key.
	* NOTE: returns empty V if the value is null AND also returns
	* empty V if the key does not exists
	*/
	V get(const K& key)
	{
		if(key == K{}) { throw std::invalid_argument("Key cannot be default value."); }
		const std::size_t hash = normalizeIndex(std::hash<K>{}(key));

		std::size_t i = hash;
		std::optional<std::size_t> deletedIndex; // deleted index if founded
		std::size_t x = 0; //x for prob

		/**
		* Starting at the original hash index quadratically probe until we find a spot where
		* our key is or we hit a BucketState::Empty in witch case our element does not exist.
		*/
		while (true)
		{
			switch (stateTable[i])
			{
			case BucketState::Deleted:
				/**
				* Ignore deleted cells, but record where the first index of a deleted cell
				* is found to perform lazy relocation later.
				*/
				if (!deletedIndex.has_value()) deletedIndex = i;
				break;
			case BucketState::Occupied:
				if (keyTable[i] == key) // Found the key in the hash-table
				{
					bContains = true;

					/**
					* If deletedIndex has value means we previously encountered a deleted cell.
					* We can perform an optimization by swapping the entries in cells i and 
					* deletedIndex, so that the next time we seach for this key it will be found
					* faster. This is called lazy deletion/relocation.
					*/
					if (deletedIndex.has_value())
					{
						// Move values to where deleted index is
						const std::size_t d = deletedIndex.value();
						stateTable[d] = BucketState::Occupied;
						keyTable[d] = std::move(keyTable[i]);
						valueTable[d] = std::move(valueTable[i]);

						// Set bucket i as deleted
						stateTable[i] = BucketState::Deleted;

						return valueTable[d];
					}
					else
					{
						return valueTable[i];
					}
				}
				break;
			case BucketState::Empty:
				bContains = false;
				return V{};
			default:
				break;
			}
			i = normalizeIndex(hash + P(++x));
		}
	}

	// remove a key and value from the hash-table
	bool remove(const K& key)
	{
		if (key == K{}) { throw std::invalid_argument("Key cannot be default value."); }
		const std::size_t hash = normalizeIndex(std::hash<K>{}(key));

		std::size_t i = hash;
		std::size_t x = 0; //x for prob

		/**
		* Starting at the original hash index quadratically probe until we find a spot where
		* our key is or we hit a BucketState::Empty in witch case our element does not exist.
		*/
		while (true)
		{
			switch (stateTable[i])
			{
			case BucketState::Deleted:
				// just ignore
				break;
			case BucketState::Occupied:
				// founded, delete the bucket
				if (keyTable[i] == key)
				{
					stateTable[i] = BucketState::Deleted;
					++modificationCount;
					--keyCount;
					return true;
				}
				break;
			case BucketState::Empty:
				// Key not found
				return false;
			default:
				break;
			}
			i = normalizeIndex(hash + P(++x));
		}
	}

	// Returns a vector of keys found in the hash table
	std::vector<K> keys() const
	{
		std::vector<K> out;
		out.reserve(size());
		for (std::size_t i = 0; i < capacity; ++i)
		{
			if (stateTable[i] == BucketState::Occupied)
			{
				out.push_back(keyTable[i]);
			}
		}
		return out;
	}

	// Returns a vector of values found in the hash table
	std::vector<V> values() const
	{
		std::vector<V> out;
		out.reserve(size());
		for (std::size_t i = 0; i < capacity; ++i)
		{
			if (stateTable[i] == BucketState::Occupied)
			{
				out.push_back(valueTable[i]);
			}
		}
		return out;
	}

	// operator<< overload
	friend std::ostream& operator<<(std::ostream& OutputStream, const HashTableQuadraticProbing& HashTable)
	{
		OutputStream << "{\n";
		for (std::size_t i = 0; i < HashTable.capacity; ++i)
		{
			if (HashTable.stateTable[i] == BucketState::Occupied)
			{
				OutputStream << '[' << HashTable.keyTable[i] << ": " << HashTable.valueTable[i] << "]\n";
			}
		}
		OutputStream << '}';
		return OutputStream;
	}


private:
	// Given a number n, this method finds the next power of 2 above this value
	constexpr std::size_t next2Power(std::size_t n) const { return std::bit_ceil(n); }

	// Quadratic probing function (x^2 + x) / 2 = x(x + 1) / 2 = x(x + 1) >> 1 
	// >> 1 right bit shift, is the same / 2 for positive integers
	constexpr std::size_t P(std::size_t x) const { return (x * (x + 1)) >> 1; }

	// Converts a hash to a valid bucket index. 
	// (hash & (capacity-1)) ≡ (hash % capacity), requirement: capacity must be power of 2
	inline std::size_t normalizeIndex(std::size_t keyHash) const { return keyHash & (capacity - 1); }

	void resizeTable()
	{
		const std::size_t oldCapacity = capacity;
		capacity *= 2;
		threshold = std::max<std::size_t>(1, static_cast<std::size_t> (capacity * loadFactor));

		BucketState* oldStateTable = stateTable;
		K* oldKeyTable = keyTable;
		V* oldValueTable = valueTable;

		stateTable = new BucketState[capacity];
		std::fill_n(stateTable, capacity, BucketState::Empty);

		keyTable = new K[capacity];
		valueTable = new V[capacity];

		// Reset the key count and buckets used sunce we are about to
		// re-insert all the keys into the hash-table
		keyCount = 0;
		usedBuckets = 0;
		std::size_t oldModif = modificationCount;

		for (std::size_t i = 0; i < oldCapacity; ++i)
		{
			if (oldStateTable[i] == BucketState::Occupied)
			{
				insert(oldKeyTable[i], oldValueTable[i]);
			}
		}

		modificationCount = oldModif;
		delete[] oldStateTable;
		delete[] oldKeyTable;
		delete[] oldValueTable;
	}
};