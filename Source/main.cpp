#include <iostream>
#include "temp.hpp"

int main(int argc, char *argv[])
{
    std::cout << "argc: " << argc << "\n";
    for (auto i = 0; i < argc; ++i)
    {
        std::cout << argv[i] << "\n";
    }

    std::cout << VALUES_1 << "hello, world\n";
    return 0;
}