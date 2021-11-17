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

    set.Add(3);
    set.Add(66);
    set.Add(134);
    set.Add(423);
    set.Add(987);

    std::cout << set.Has(3) << std::endl;
    std::cout << set.Has(66) << std::endl;
    std::cout << set.Has(134) << std::endl;
    std::cout << set.Has(423) << std::endl;
    std::cout << set.Has(987) << std::endl;

    set.Remove(66);
    set.Remove(423);

    std::cout << set.Has(3) << std::endl;
    std::cout << set.Has(66) << std::endl;
    std::cout << set.Has(134) << std::endl;
    std::cout << set.Has(423) << std::endl;
    std::cout << set.Has(987) << std::endl;


    return 0;
}
