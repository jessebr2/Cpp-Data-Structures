#pragma once

#include <cstddef>
#include <ostream>
#include <atomic>
#include <utility>
#include <new>
#include "LowLatency/LLConstants.h"

/**
* @brief Ultra-low latency, lock-free Sighle-Producer Single-Consumer (SPSC) Ring Buffer.
* 
* @author Jesse Andrade, jesse.berto.andrade@gmail.com
*/

template <typename T, std::size_t Capacity>
class LockFreeSPSCQueue
{
	// Compile-time static validation enforcing power-of-two constraints to optimize indexing math.
	static_assert(Capacity > 0 && (Capacity & (Capacity - 1)) == 0, "Capacity must be power a power of 2!");

public:
	explicit LockFreeSPSCQueue() = default;
	~LockFreeSPSCQueue() = default;

	// Delete copy and move semantics to block accdental data structural duplication.
	LockFreeSPSCQueue(const LockFreeSPSCQueue&) = delete;
	LockFreeSPSCQueue& operator=(const LockFreeSPSCQueue&) = delete;
	LockFreeSPSCQueue(LockFreeSPSCQueue&&) = delete;
	LockFreeSPSCQueue& operator=(LockFreeSPSCQueue&&) = delete;

	static constexpr std::size_t mask_ = Capacity - 1;

	/**
	* @brief Evaluats an instantaneous snapshot of the queue sizing
	* @note In multi-theaded executions, this represents an approximate estimation.
	*/
	std::size_t size() const
	{
		std::size_t current_back = back_index_.load(std::memory_order_relaxed);
		std::size_t current_front = front_index_.load(std::memory_order_relaxed);

		return current_back >= current_front ? current_back - current_front : (Capacity - current_front) + current_back;
	}

	bool is_empty() const
	{
		// Direct, lock-free equality validation.
		return front_index_.load(std::memory_order_relaxed) == back_index_.load(std::memory_order_relaxed);
	}

	constexpr std::size_t capacity() const
	{
		return Capacity;
	}

	bool is_full() const
	{
		std::size_t current_back = back_index_.load(std::memory_order_relaxed);
		std::size_t current_front = front_index_.load(std::memory_order_relaxed);
		return ((current_back + 1) & mask_) == current_front;
	}

	/**
	* @brief Safe hot-path check of the next element without mutating the queue or throwing exceptions.
	* @return True if data was fetched successfully, false if the queue was empty.
	*/
	bool peek(T& out_element) const
	{
		std::size_t current_front = front_index_.load(std::memory_order_relaxed);

		if (current_front == back_index_.load(std::memory_order_relaxed)) [[unlikely]]
		{
			return false;
		}

		out_element = data_[current_front];
		return true;
	}

	/**
	* @brief Thread-safe ingestion method. MUST BE invoked exclusively by the Producer Thread.
	*/
	bool enqueue(const T& element)
	{
		std::size_t current_back = back_index_.load(std::memory_order_relaxed);
		std::size_t next_back = (current_back + 1) & mask_; // Fast bitwise masking increments.

		// Hot Path (branchless in the atomic point of view. We have a local cache do compare
		// In the vast majority of cases we'll have free space, so comparing next_back with 
		// cached_front_index_ will avoid us to have to acquire the real front_index_
		if (next_back == cached_front_index_) [[unlikely]]
		{
			// Cold Path (rarelly)
			// We only peform the heavy atomic load if the local cache hits the limit
			cached_front_index_ = front_index_.load(std::memory_order_acquire); // Update the local cache

			if (next_back == cached_front_index_) [[unlikely]] // If the Queue is really full
			{
				return false; // Queue is full, return immediately preventing hot path stalls
			}
		}

		data_[current_back] = element;

		// Release ordering acts as a memory fence, ensuring data payload updates
		// are physically committed before the index step triggers.
		back_index_.store(next_back, std::memory_order_release);
		return true;
	}

	/**
	* @brief Thread-safe ingestion method. MUST BE invoked exclusively by the Producer Thread.
	* Movement overload
	*/
	bool enqueue(T&& element)
	{
		std::size_t current_back = back_index_.load(std::memory_order_relaxed);
		std::size_t next_back = (current_back + 1) & mask_; // Fast bitwise masking increments.

		// Hot Path (branchless in the atomic point of view. We have a local cache do compare
		// In the vast majority of cases we'll have free space, so comparing next_back with 
		// cached_front_index_ will avoid us to have to acquire the real front_index_
		if (next_back == cached_front_index_) [[unlikely]]
		{
			// Cold Path (rarelly)
			// We only peform the heavy atomic load if the local cache hits the limit
			cached_front_index_ = front_index_.load(std::memory_order_acquire); // Update the local cache

			if (next_back == cached_front_index_) [[unlikely]] // If the Queue is really full
			{
				return false; // Queue is full, return immediately preventing hot path stalls
			}
		}

		data_[current_back] = std::move(element);

		// Release ordering acts as a memory fence, ensuring data payload updates
		// are physically committed before the index step triggers.
		back_index_.store(next_back, std::memory_order_release);
		return true;
	}

	/**
	* @brief Thread-safe extraction method. MUST BE invoked exclusively by the Consumer Thread.
	*/
	bool dequeue(T& out_element)
	{
		std::size_t current_front = front_index_.load(std::memory_order_relaxed); 

		// Hot Path (branchless and heavy atomic barries free) => same logic of enqueue
		if (current_front == cached_back_index_) [[unlikely]]
		{
			// Cold Path, now we have to update the cached_current_back_ and real check if the Queue is empty
			cached_back_index_ = back_index_.load(std::memory_order_acquire); 

			if (current_front == cached_back_index_) [[unlikely]]
			{
				return false; // Queue is empty
			}
		}

		out_element = std::move(data_[current_front]);

		// Release ordering guarantees reading/moving operations conclude entirely before
		// front_index_ flags the slot as recycled and ready for production.
		front_index_.store((current_front + 1) & mask_, std::memory_order_release);
		return true;
	}

	// Thead-safe stream extraction wrapper for debug reporting
	friend std::ostream& operator<<(std::ostream& output_stream, const LockFreeSPSCQueue& queue)
	{
		std::size_t current_front = queue.front_index_.load(std::memory_order_relaxed);
		std::size_t current_size = queue.size();

		output_stream << "[";
		for (std::size_t i = 0; i < current_size; ++i)
		{
			output_stream << queue.data_[(current_front + i) & queue.mask_];
			if (i < current_size - 1)
			{
				output_stream << ", ";
			}
		}
		output_stream << "]";
		return output_stream;
	}

private:
	// ===========================================================================
	// CACHE-LINE ISOLATED DATA ARCHITECTURE
	// ===========================================================================

	// Cache Line 1: Private boundary written/modified exclusively by the Consumer.
	alignas(ll::config::kCacheLineSize) std::atomic<std::size_t> front_index_{ 0 };
	std::size_t cached_back_index_{ 0 };

	// Cache Line 2: Private bondary written/modified exclusively by the Producer.
	alignas(ll::config::kCacheLineSize) std::atomic<std::size_t> back_index_{ 0 };
	std::size_t cached_front_index_{ 0 };

	// Cache Line 3: Read-Only architectural pointers shared inside core pipelines.
	alignas(ll::config::kCacheLineSize) T data_[Capacity];
};