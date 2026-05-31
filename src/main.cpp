#include <iostream>
#include "DynamicArray.h"
#include "LinkedList.h"
#include "ListStack.h"

int main()
{
    ListStack<int> Stack;

    Stack.Push(10);
    Stack.Push(20);
    Stack.Push(30);

    std::cout << Stack << '\n';

    std::cout << Stack.Pop() << '\n';
    std::cout << Stack.Pop() << '\n';

    std::cout << Stack.Peek() << '\n';

    std::cout << Stack << '\n';

    return 0;

}