#include <iostream>
#include "DynamicArray.h"
#include "LinkedList.h"
#include "ListStack.h"
#include "ArrayStack.h"
#include "ArrayQueue.h"
#include "ListQueue.h"

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

int main()
{
    TestListQueue();
    TestArrayQueue();

    return 0;

}
