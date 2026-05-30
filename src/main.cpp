#include <iostream>
#include "DynamicArray.h"

int main()
{
    DynamicArray<int> Array;

    std::cout << "===== INITIAL STATE =====\n";
    std::cout << "CurrentSize: " << Array.Size() << '\n';
    std::cout << "TotalCapacity: " << Array.Capacity() << '\n';
    std::cout << "IsEmpty: " << Array.IsEmpty() << '\n';

    std::cout << "\n===== ADD ELEMENTS =====\n";

    for (int Value = 1; Value <= 20; ++Value)
    {
        Array.Add(Value);

        std::cout
            << "Added: "
            << Value
            << " | CurrentSize: "
            << Array.Size()
            << " | TotalCapacity: "
            << Array.Capacity()
            << '\n';
    }

    std::cout << "\n===== ARRAY CONTENT =====\n";
    std::cout << Array << '\n';

    std::cout << "\n===== GET =====\n";
    std::cout << "Index 0: " << Array.Get(0) << '\n';
    std::cout << "Index 5: " << Array.Get(5) << '\n';
    std::cout << "Index 10: " << Array.Get(10) << '\n';

    std::cout << "\n===== SET =====\n";

    Array.Set(0, 100);
    Array.Set(1, 200);

    std::cout << Array << '\n';

    std::cout << "\n===== INDEX OF =====\n";

    std::cout << "100 -> " << Array.IndexOf(100) << '\n';
    std::cout << "200 -> " << Array.IndexOf(200) << '\n';
    std::cout << "999 -> " << Array.IndexOf(999) << '\n';

    std::cout << "\n===== CONTAINS =====\n";

    std::cout << "Contains 100: " << Array.Contains(100) << '\n';
    std::cout << "Contains 999: " << Array.Contains(999) << '\n';

    std::cout << "\n===== REMOVE AT =====\n";

    Array.RemoveAt(0);

    std::cout << Array << '\n';
    std::cout << "CurrentSize: " << Array.Size() << '\n';

    std::cout << "\n===== REMOVE VALUE =====\n";

    Array.Remove(10);
    Array.Remove(15);

    std::cout << Array << '\n';

    std::cout << "\n===== RANGE FOR =====\n";

    for (const int Value : Array)
    {
        std::cout << Value << ' ';
    }

    std::cout << '\n';

    std::cout << "\n===== CLEAR =====\n";

    Array.Clear();

    std::cout << "CurrentSize: " << Array.Size() << '\n';
    std::cout << "IsEmpty: " << Array.IsEmpty() << '\n';

    std::cout << "\n===== EXCEPTION TEST =====\n";

    try
    {
        Array.Get(0);
    }
    catch (const std::exception& Exception)
    {
        std::cout << "Caught exception: " << Exception.what() << '\n';
    }

    return 0;

}