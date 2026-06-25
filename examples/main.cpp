#include <cassert>
#include <iostream>
#include <string>
#include "DynamicArray.h"
#include "LinkedList.h"
#include "ListStack.h"
#include "ArrayStack.h"
#include "ArrayQueue.h"
#include "ListQueue.h"
#include "PriorityQueue.h"
#include "BinarySearchTree.h"
#include "HashTable/SeparateChainingHashTable.h"
#include "HashTable/HashTableQuadraticProbing.h"
#include "UnionFind.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>
#include <cassert>
#include <atomic>

// Use PAUSE only on Linux x86/x86_64; otherwise fallback to scheduler yield.
#if defined(__linux__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#endif

#include "LowLatency/LockFreeSPSCQueue.h"   

inline void cpu_spin_pause()
{
#if defined(__linux__) && (defined(__x86_64__) || defined(__i386__))
    _mm_pause();
#else
    std::this_thread::yield();
#endif
}


void TestListQueue()
{
    std::cout << "\n===== LIST QUEUE =====\n";

    ListQueue<int> Queue;

    std::cout << "Empty: " << Queue.IsEmpty() << '\n';

    Queue.Enqueue(10);
    Queue.Enqueue(20);
    Queue.Enqueue(30);

    std::cout << Queue << '\n';

    std::cout << "Peek: " << Queue.Peek() << '\n';
    std::cout << "Size: " << Queue.Size() << '\n';

    std::cout << "Dequeue: " << Queue.Dequeue() << '\n';
    std::cout << "Dequeue: " << Queue.Dequeue() << '\n';

    std::cout << Queue << '\n';

    Queue.Enqueue(40);
    Queue.Enqueue(50);

    std::cout << Queue << '\n';

    while (!Queue.IsEmpty())
    {
        std::cout << "Removed: " << Queue.Dequeue() << '\n';
    }

    std::cout << "Empty: " << Queue.IsEmpty() << '\n';

    try
    {
        Queue.Peek();
    }
    catch (const std::exception& Exception)
    {
        std::cout << "Exception: " << Exception.what() << '\n';
    }
}

//void TestArrayQueue()
//{
//    std::cout << "\n===== ARRAY QUEUE =====\n";
//
//    ArrayQueue<int> Queue(5);
//
//    Queue.Enqueue(10);
//    Queue.Enqueue(20);
//    Queue.Enqueue(30);
//
//    std::cout << Queue << '\n';
//
//    std::cout << "Peek: " << Queue.Peek() << '\n';
//    std::cout << "Size: " << Queue.Size() << '\n';
//
//    std::cout << "Dequeue: " << Queue.Dequeue() << '\n';
//    std::cout << "Dequeue: " << Queue.Dequeue() << '\n';
//
//    std::cout << Queue << '\n';
//
//    Queue.Enqueue(40);
//    Queue.Enqueue(50);
//    Queue.Enqueue(60);
//    Queue.Enqueue(70);
//
//    std::cout << Queue << '\n';
//
//    std::cout << "IsFull: " << Queue.IsFull() << '\n';
//
//    try
//    {
//        Queue.Enqueue(80);
//    }
//    catch (const std::exception& Exception)
//    {
//        std::cout << "Exception: " << Exception.what() << '\n';
//    }
//
//    while (!Queue.IsEmpty())
//    {
//        std::cout << "Removed: " << Queue.Dequeue() << '\n';
//    }
//
//    try
//    {
//        Queue.Dequeue();
//    }
//    catch (const std::exception& Exception)
//    {
//        std::cout << "Exception: " << Exception.what() << '\n';
//    }
//}

void TestPriorityQueue()
{
    std::cout << "\n===== PRIORITY QUEUE =====\n";

    PriorityQueue<int> Queue
    {
        8, 3, 5, 1, 7, 2, 9, 4, 6
    };

    std::cout << "Size: " << Queue.Size() << '\n';
    std::cout << "Peek: " << Queue.Peek() << '\n';

    std::cout << "IsMinHeap: "
        << std::boolalpha
        << Queue.IsMinHeap()
        << '\n';

    std::cout << "\nPolling all elements:\n";

    while (!Queue.IsEmpty())
    {
        std::cout << Queue.Poll() << ' ';
    }

    std::cout << "\n\n";
}

void TestPriorityQueueDuplicates()
{
    std::cout << "\n===== DUPLICATES =====\n";

    PriorityQueue<int> Queue
    {
        5, 1, 3, 1, 7, 5, 2, 1
    };

    std::cout << "Contains(7): "
        << Queue.Contains(7)
        << '\n';

    std::cout << "Contains(42): "
        << Queue.Contains(42)
        << '\n';

    std::cout << "Remove(5): "
        << Queue.Remove(5)
        << '\n';

    std::cout << "Remove(42): "
        << Queue.Remove(42)
        << '\n';

    std::cout << "\nPolling:\n";

    while (!Queue.IsEmpty())
    {
        std::cout << Queue.Poll() << ' ';
    }

    std::cout << "\n";
}

void TestPriorityQueueStress()
{
    PriorityQueue<int> Queue;

    for (int i = 1000; i >= 1; --i)
    {
        Queue.Add(i);
    }

    int Expected = 1;

    while (!Queue.IsEmpty())
    {
        int Value = Queue.Poll();

        if (Value != Expected)
        {
            throw std::runtime_error("Heap ordering failed!");
        }

        ++Expected;
    }

    std::cout << "Stress test passed.\n";
}

void PrintTraversal(const BinarySearchTree<int>& Tree, TraversalOrder Order, const char* Name)
{
    std::cout << Name << ": " << Tree.Traverse(Order) << '\n';
}

void TestBinarySearchTree()
{
    BinarySearchTree<int> Tree;

    Tree.Add(8);
    Tree.Add(3);
    Tree.Add(10);
    Tree.Add(1);
    Tree.Add(6);
    Tree.Add(14);
    Tree.Add(4);
    Tree.Add(7);
    Tree.Add(13);

    std::cout << "Tree InOrder: " << Tree << '\n';
    std::cout << "Size: " << Tree.Size() << '\n';
    std::cout << "Height: " << Tree.Height() << '\n';

    std::cout << "Contains 6: " << Tree.Contains(6) << '\n';
    std::cout << "Contains 99: " << Tree.Contains(99) << '\n';

    PrintTraversal(Tree, TraversalOrder::InOrder, "InOrder");
    PrintTraversal(Tree, TraversalOrder::PreOrder, "PreOrder");
    PrintTraversal(Tree, TraversalOrder::PostOrder, "PostOrder");
    PrintTraversal(Tree, TraversalOrder::LevelOrder, "LevelOrder");

    std::cout << "\nRemove leaf 4\n";
    Tree.Remove(4);
    std::cout << Tree << '\n';

    std::cout << "\nRemove node with one child 14\n";
    Tree.Remove(14);
    std::cout << Tree << '\n';

    std::cout << "\nRemove node with two children 3\n";
    Tree.Remove(3);
    std::cout << Tree << '\n';

    std::cout << "\nRemove root 8\n";
    Tree.Remove(8);
    std::cout << Tree << '\n';

    std::cout << "Final Size: " << Tree.Size() << '\n';
    std::cout << "Final Height: " << Tree.Height() << '\n';

    Tree.Clear();

    std::cout << "\nAfter Clear\n";
    std::cout << "IsEmpty: " << Tree.IsEmpty() << '\n';
    std::cout << "Size: " << Tree.Size() << '\n';
    std::cout << "Height: " << Tree.Height() << '\n';
}

void TestSeparateChainingHashTable()
{
    std::cout << "\n===== SEPARATE CHAINING HASH TABLE =====\n";

    SeparateChainingHashTable<std::string, int> Table;

    std::cout << "Empty: " << std::boolalpha << Table.IsEmpty() << '\n';

    // Insert
    Table.Insert("apple", 1);
    Table.Insert("banana", 2);
    Table.Insert("cherry", 3);
    Table.Insert("date", 4);
    Table.Insert("elderberry", 5);

    std::cout << "\nAfter Insert:\n";
    std::cout << Table << '\n';
    std::cout << "Size: " << Table.Size() << '\n';

    // Get
    std::cout << "\nGet 'banana': " << Table.Get("banana").value_or(-1) << '\n';
    std::cout << "Get 'missing': " << Table.Get("missing").value_or(-1) << '\n';

    // HasKey / ContainsKey
    std::cout << "\nHasKey 'cherry': " << Table.HasKey("cherry") << '\n';
    std::cout << "HasKey 'missing': " << Table.HasKey("missing") << '\n';

    // Update existing key
    std::cout << "\nUpdating 'apple' to 99:\n";
    Table.Insert("apple", 99);
    std::cout << "Get 'apple': " << Table.Get("apple").value_or(-1) << '\n';

    // Keys / Values
    std::cout << "\nKeys:\n";
    for (const auto& Key : Table.Keys())
    {
        std::cout << "  " << Key << '\n';
    }

    std::cout << "\nValues:\n";
    for (const auto& Value : Table.Values())
    {
        std::cout << "  " << Value << '\n';
    }

    // Remove
    std::cout << "\nRemoving 'banana': " << Table.Remove("banana") << '\n';
    std::cout << "Size after remove: " << Table.Size() << '\n';
    std::cout << "HasKey 'banana': " << Table.HasKey("banana") << '\n';

    // Trigger resize by inserting many keys
    std::cout << "\nInserting many keys to trigger resize...\n";
    for (int i = 0; i < 20; ++i)
    {
        Table.Insert("key" + std::to_string(i), i);
    }
    std::cout << "Size after bulk insert: " << Table.Size() << '\n';

    // Clear
    Table.Clear();
    std::cout << "\nAfter Clear:\n";
    std::cout << "Empty: " << Table.IsEmpty() << '\n';
    std::cout << "Size: " << Table.Size() << '\n';

    // Exception: remove from empty
    try
    {
        Table.Remove("apple");
    }
    catch (const std::exception& Exception)
    {
        std::cout << "Exception: " << Exception.what() << '\n';
    }

    // Exception: invalid key
    try
    {
        Table.Insert("", 1);
    }
    catch (const std::exception& Exception)
    {
        std::cout << "Exception: " << Exception.what() << '\n';
    }
}

void testHashTableQuadraticProbing()
{
    std::cout << "\n===== HASH TABLE (QUADRATIC PROBING) =====\n";

    HashTableQuadraticProbing<int, std::string> table(8, 0.45);

    std::cout << "Initial state\n";
    std::cout << "  empty: " << std::boolalpha << table.empty() << '\n';
    std::cout << "  size : " << table.size() << '\n';
    assert(table.empty());
    assert(table.size() == 0);

    std::cout << "\n[Insert]\n";
    std::string old = table.insert(1, "one");
    std::cout << "  insert(1, \"one\") -> old: \"" << old << "\"\n";
    assert(old.empty());

    old = table.insert(2, "two");
    std::cout << "  insert(2, \"two\") -> old: \"" << old << "\"\n";
    assert(old.empty());

    old = table.insert(3, "three");
    std::cout << "  insert(3, \"three\") -> old: \"" << old << "\"\n";
    assert(old.empty());

    std::cout << "  size after insert: " << table.size() << '\n';
    assert(!table.empty());
    assert(table.size() == 3);

    std::cout << "\n[Get / Contains]\n";
    std::cout << "  contains(1): " << table.contains(1) << '\n';
    std::cout << "  contains(99): " << table.contains(99) << '\n';
    assert(table.contains(1));
    assert(!table.contains(99));

    std::cout << "  get(1): " << table.get(1) << '\n';
    std::cout << "  get(2): " << table.get(2) << '\n';
    assert(table.get(1) == "one");
    assert(table.get(2) == "two");

    std::cout << "\n[Update existing key]\n";
    old = table.insert(2, "TWO");
    std::cout << "  insert(2, \"TWO\") -> old: \"" << old << "\"\n";
    std::cout << "  get(2): " << table.get(2) << '\n';
    assert(old == "two");
    assert(table.get(2) == "TWO");
    assert(table.size() == 3);

    std::cout << "\n[Remove]\n";
    std::cout << "  remove(2): " << table.remove(2) << '\n';
    assert(!table.contains(2));
    assert(table.size() == 2);

    std::cout << "  remove(999): " << table.remove(999) << '\n';
    assert(!table.remove(999));

    std::cout << "\n[keys / values]\n";
    const std::vector<int> keysBeforeBulk = table.keys();
    const std::vector<std::string> valuesBeforeBulk = table.values();

    std::cout << "  keys (" << keysBeforeBulk.size() << "): ";
    for (const int k : keysBeforeBulk) { std::cout << k << ' '; }
    std::cout << '\n';

    std::cout << "  values (" << valuesBeforeBulk.size() << "): ";
    for (const auto& v : valuesBeforeBulk) { std::cout << v << ' '; }
    std::cout << '\n';

    assert(keysBeforeBulk.size() == table.size());
    assert(valuesBeforeBulk.size() == table.size());

    std::cout << "\n[Bulk insert]\n";
    for (int k = 4; k <= 30; ++k)
    {
        table.insert(k, "v" + std::to_string(k));
    }
    std::cout << "  size after bulk insert: " << table.size() << '\n';
    std::cout << "  contains(30): " << table.contains(30) << '\n';
    std::cout << "  get(30): " << table.get(30) << '\n';
    assert(table.contains(30));
    assert(table.get(30) == "v30");

    std::cout << "\n[Move constructor]\n";
    HashTableQuadraticProbing<int, std::string> moved(std::move(table));
    std::cout << "  moved.contains(1): " << moved.contains(1) << '\n';
    std::cout << "  moved.contains(30): " << moved.contains(30) << '\n';
    std::cout << "  moved.size(): " << moved.size() << '\n';
    assert(moved.contains(1));
    assert(moved.contains(30));

    std::cout << "\n[Clear]\n";
    moved.clear();
    std::cout << "  moved.empty(): " << moved.empty() << '\n';
    std::cout << "  moved.size() : " << moved.size() << '\n';
    assert(moved.empty());
    assert(moved.size() == 0);

    std::cout << "\nAll HashTableQuadraticProbing tests passed.\n";
}

void TestUnionFind()
{
    std::cout << "\n===== UNION FIND =====\n";

    UnionFind Uf(10);

    std::cout << "Initial state:\n";
    std::cout << "  num elements : " << Uf.size() << '\n';
    std::cout << "  components   : " << Uf.components() << '\n';

    std::cout << "\n[Unify]\n";
    Uf.unify(1, 2);
    std::cout << "  unify(1,2) -> components: " << Uf.components() << '\n';

    Uf.unify(2, 3);
    std::cout << "  unify(2,3) -> components: " << Uf.components() << '\n';

    Uf.unify(4, 5);
    std::cout << "  unify(4,5) -> components: " << Uf.components() << '\n';

    Uf.unify(6, 7);
    std::cout << "  unify(6,7) -> components: " << Uf.components() << '\n';

    Uf.unify(7, 8);
    std::cout << "  unify(7,8) -> components: " << Uf.components() << '\n';

    Uf.unify(3, 8);
    std::cout << "  unify(3,8) -> components: " << Uf.components() << '\n';

    std::cout << "\n[Connected]\n";
    std::cout << "  connected(1,3): " << std::boolalpha << Uf.connected(1, 3) << '\n';
    std::cout << "  connected(1,8): " << std::boolalpha << Uf.connected(1, 8) << '\n';
    std::cout << "  connected(1,9): " << std::boolalpha << Uf.connected(1, 9) << '\n';
    std::cout << "  connected(4,5): " << std::boolalpha << Uf.connected(4, 5) << '\n';
    std::cout << "  connected(4,6): " << std::boolalpha << Uf.connected(4, 6) << '\n';

    std::cout << "\n[Find roots]\n";
    std::cout << "  find(1): " << Uf.find(1) << '\n';
    std::cout << "  find(2): " << Uf.find(2) << '\n';
    std::cout << "  find(8): " << Uf.find(8) << '\n';
    std::cout << "  find(9): " << Uf.find(9) << '\n';

    std::cout << "\n[Component sizes]\n";
    std::cout << "  componentSize(1): " << Uf.componentSize(1) << '\n';
    std::cout << "  componentSize(4): " << Uf.componentSize(4) << '\n';
    std::cout << "  componentSize(9): " << Uf.componentSize(9) << '\n';

    std::cout << "\nFinal components: " << Uf.components() << '\n';
    std::cout << "===== END UNION FIND TEST =====\n";
}

// 1. Wrap the original ArrayQueue with a Mutex to make it thread-safe fairly
template <typename T>
class SynchronizedArrayQueue {
private:
    ArrayQueue<T> queue_;
    std::mutex mutex_;
public:
    explicit SynchronizedArrayQueue(std::size_t capacity) : queue_(capacity) {}

    bool enqueue(const T& element) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.full()) return false;
        queue_.enqueue(element);
        return true;
    }

    bool dequeue(T& out_element) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) return false;
        out_element = queue_.dequeue();
        return true;
    }
};

// Structure simulating HFT market data
struct MarketTick {
    uint64_t sequence;
    double price;
    uint32_t quantity;
};

// Global stress test settings (C++14 digit separator notation)
constexpr std::size_t kNumOperations = 5'000'000; // 5 million iterations
constexpr std::size_t kQueueCapacity = 4'096;     // Must be power of 2 for lock-free

// ===========================================================================
// BENCHMARK / STRESS FUNCTIONS
// ===========================================================================

void run_lock_free_stress_test() {
    LockFreeSPSCQueue<MarketTick, kQueueCapacity> spsc_queue;
    std::atomic<bool> start_signal{ false };
    std::atomic<uint64_t> checksum_produced{ 0 };
    std::atomic<uint64_t> checksum_consumed{ 0 };

    std::thread producer([&]() {
        while (!start_signal.load(std::memory_order_relaxed));
        uint64_t local_checksum = 0;

        for (uint64_t i = 1; i <= kNumOperations; ++i) {
            MarketTick tick{ i, 100.50 + (i % 10), static_cast<uint32_t>(10 * i) };
            local_checksum += tick.sequence;

            while (!spsc_queue.enqueue(std::move(tick))) {
                cpu_spin_pause();
            }
        }
        checksum_produced.store(local_checksum, std::memory_order_relaxed);
        });

    std::thread consumer([&]() {
        while (!start_signal.load(std::memory_order_relaxed));
        uint64_t local_checksum = 0;
        std::size_t consumed_count = 0;
        MarketTick tick;

        while (consumed_count < kNumOperations) {
            if (spsc_queue.dequeue(tick)) {
                local_checksum += tick.sequence;
                consumed_count++;
            }
            else {
                cpu_spin_pause();
            }
        }
        checksum_consumed.store(local_checksum, std::memory_order_relaxed);
        });

    auto start_time = std::chrono::high_resolution_clock::now();
    start_signal.store(true);

    producer.join();
    consumer.join();
    auto end_time = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    std::cout << "\n========================================\n";
    std::cout << "= RESULTS: LOCK-FREE SPSC QUEUE (LOW LATENCY VERSION)\n";
    std::cout << "========================================\n";
    std::cout << "Total Time: " << duration << " ms\n";
    std::cout << "Throughput:  " << (kNumOperations / (duration / 1000.0)) / 1'000'000.0 << " million ops/s\n";

    if (checksum_produced.load() == checksum_consumed.load()) {
        std::cout << "STATUS: Success! Checksum is intact. Zero losses.\n";
    }
    else {
        std::cout << "STATUS: Concurrency error detected!\n";
    }
}

void run_synchronized_array_test() {
    SynchronizedArrayQueue<MarketTick> sync_queue(kQueueCapacity);
    std::atomic<bool> start_signal{ false };
    std::atomic<uint64_t> checksum_produced{ 0 };
    std::atomic<uint64_t> checksum_consumed{ 0 };

    std::thread producer([&]() {
        while (!start_signal.load(std::memory_order_relaxed));
        uint64_t local_checksum = 0;

        for (uint64_t i = 1; i <= kNumOperations; ++i) {
            MarketTick tick{ i, 100.50 + (i % 10), static_cast<uint32_t>(10 * i) };
            local_checksum += tick.sequence;

            while (!sync_queue.enqueue(tick)) {
                cpu_spin_pause();
            }
        }
        checksum_produced.store(local_checksum, std::memory_order_relaxed);
        });

    std::thread consumer([&]() {
        while (!start_signal.load(std::memory_order_relaxed));
        uint64_t local_checksum = 0;
        std::size_t consumed_count = 0;
        MarketTick tick;

        while (consumed_count < kNumOperations) {
            if (sync_queue.dequeue(tick)) {
                local_checksum += tick.sequence;
                consumed_count++;
            }
            else {
                cpu_spin_pause();
            }
        }
        checksum_consumed.store(local_checksum, std::memory_order_relaxed);
        });

    auto start_time = std::chrono::high_resolution_clock::now();
    start_signal.store(true);

    producer.join();
    consumer.join();
    auto end_time = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    std::cout << "\n========================================\n";
    std::cout << "= RESULTS: SYNCHRONIZED ARRAY QUEUE (CLASSICAL VERSION)\n";
    std::cout << "========================================\n";
    std::cout << "Total Time: " << duration << " ms\n";
    std::cout << "Throughput:  " << (kNumOperations / (duration / 1000.0)) / 1'000'000.0 << " million ops/s\n";

    if (checksum_produced.load() == checksum_consumed.load()) {
        std::cout << "STATUS: Success! Checksum is intact. Zero losses.\n";
    }
    else {
        std::cout << "STATUS: Concurrency error detected!\n";
    }
}

int main()
{
    //TestPriorityQueue();
    //TestPriorityQueueDuplicates();
    //TestPriorityQueueStress();
    //TestBinarySearchTree();
    //TestSeparateChainingHashTable();
    //testHashTableQuadraticProbing();

    //TestUnionFind();

    std::cout << "Starting Stress Test in Ubuntu x86_64...\n";
    std::cout << "Processing volume per queue: " << kNumOperations << " Market Ticks.\n";

    // Run comparative tests
    run_lock_free_stress_test();
    run_synchronized_array_test();

    return 0;
}
