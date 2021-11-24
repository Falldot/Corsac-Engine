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

#include "Corsac/sparse_set.h"

#include "Corsac/chrono.h"
#include <chrono>

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

    return 0;
}
