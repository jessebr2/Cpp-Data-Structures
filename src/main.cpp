#include <iostream>
#include "DynamicArray.h"
#include "LinkedList.h"

int main()
{
    DoublyLinkedList<int> List;

    std::cout << "===== INITIAL STATE =====\n";
    std::cout << "Size: " << List.Size() << '\n';
    std::cout << "IsEmpty: " << List.IsEmpty() << '\n';

    std::cout << "\n===== ADD LAST =====\n";

    List.Add(10);
    List.Add(20);
    List.Add(30);

    std::cout << List << '\n';

    std::cout << "\n===== ADD FIRST =====\n";

    List.AddFirst(5);
    List.AddFirst(1);

    std::cout << List << '\n';

    std::cout << "\n===== PEEK =====\n";

    std::cout << "First: " << List.PeekFirst() << '\n';
    std::cout << "Last : " << List.PeekLast() << '\n';

    std::cout << "\n===== SIZE =====\n";

    std::cout << "Size: " << List.Size() << '\n';

    std::cout << "\n===== CONTAINS =====\n";

    std::cout << "Contains 20: " << List.Contains(20) << '\n';
    std::cout << "Contains 999: " << List.Contains(999) << '\n';

    std::cout << "\n===== INDEX OF =====\n";

    std::cout << "IndexOf(20): " << List.IndexOf(20) << '\n';
    std::cout << "IndexOf(999): " << List.IndexOf(999) << '\n';

    std::cout << "\n===== REMOVE FIRST =====\n";

    int RemovedFirst = List.RemoveFirst();

    std::cout << "Removed: " << RemovedFirst << '\n';
    std::cout << List << '\n';

    std::cout << "\n===== REMOVE LAST =====\n";

    int RemovedLast = List.RemoveLast();

    std::cout << "Removed: " << RemovedLast << '\n';
    std::cout << List << '\n';

    std::cout << "\n===== REMOVE VALUE =====\n";

    bool Removed = List.Remove(20);

    std::cout << "Removed 20: " << Removed << '\n';
    std::cout << List << '\n';

    std::cout << "\n===== REMOVE AT =====\n";

    List.Add(100);
    List.Add(200);
    List.Add(300);

    std::cout << "Before: " << List << '\n';

    int RemovedAt = List.RemoveAt(1);

    std::cout << "Removed: " << RemovedAt << '\n';
    std::cout << "After : " << List << '\n';

    std::cout << "\n===== CLEAR =====\n";

    List.Clear();

    std::cout << "Size: " << List.Size() << '\n';
    std::cout << "IsEmpty: " << List.IsEmpty() << '\n';
    std::cout << List << '\n';

    std::cout << "\n===== EXCEPTION TEST =====\n";

    try
    {
        List.PeekFirst();
    }
    catch (const std::exception& Exception)
    {
        std::cout << "Caught exception: "
            << Exception.what()
            << '\n';
    }

    return 0;

}