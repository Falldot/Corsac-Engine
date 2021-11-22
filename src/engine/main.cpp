#include <iostream>

#include "window.h"

void* __cdecl operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
    return new uint8_t[size];
}

void* __cdecl operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
    return new uint8_t[size];
}

#include "sparse_set.h"


int main()
{
//    corsac::Window window;
//
//    window.SetTitle("My Game");
//    window.Resize(1200, 800);
//    window.Show();
//
//    SDL_Delay(3000);
//
//    std::cout << window.x() << std::endl;

    auto set = corsac::sparse_set<uint32_t>();


    set.insert(3);
    set.insert(66);
    set.insert(134);
    set.insert(423);
    set.insert(987);

    std::cout << set.has(3);
    std::cout << set.has(66);
    std::cout << set.has(134);
    std::cout << set.has(423);
    std::cout << set.has(987) << std::endl;

    set.erase(3);
    set.erase(987);

    std::cout << set.has(3);
    std::cout << set.has(66);
    std::cout << set.has(134);
    std::cout << set.has(423);
    std::cout << set.has(987) << std::endl;

    return 0;
}
