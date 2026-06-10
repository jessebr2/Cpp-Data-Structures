#include <iostream>
#include "DynamicArray.h"
#include "LinkedList.h"
#include "ListStack.h"
#include "ArrayStack.h"
#include "ArrayQueue.h"
#include "ListQueue.h"
#include "PriorityQueue.h"
#include "BinarySearchTree.h"

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

void TestArrayQueue()
{
    std::cout << "\n===== ARRAY QUEUE =====\n";

    ArrayQueue<int> Queue(5);

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
    Queue.Enqueue(60);
    Queue.Enqueue(70);

    std::cout << Queue << '\n';

    std::cout << "IsFull: " << Queue.IsFull() << '\n';

    try
    {
        Queue.Enqueue(80);
    }
    catch (const std::exception& Exception)
    {
        std::cout << "Exception: " << Exception.what() << '\n';
    }

    while (!Queue.IsEmpty())
    {
        std::cout << "Removed: " << Queue.Dequeue() << '\n';
    }

    try
    {
        Queue.Dequeue();
    }
    catch (const std::exception& Exception)
    {
        std::cout << "Exception: " << Exception.what() << '\n';
    }
}

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

int main()
{
    //TestPriorityQueue();
    //TestPriorityQueueDuplicates();
    //TestPriorityQueueStress();
    TestBinarySearchTree();

    return 0;
}
