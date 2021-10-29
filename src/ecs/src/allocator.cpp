#include "allocator.h"

void* corsac::allocator::allocate(size_t size)
{
    return allocate(size, CORSAC_SYSTEM_ALLOCATOR_MIN_ALIGNMENT);
}

void* corsac::allocator::allocate(size_t size, size_t alignment)
{
    size_t needAlignment = (alignment > CORSAC_PLATFORM_PTR_SIZE) ? alignment : CORSAC_PLATFORM_PTR_SIZE;

    void* data = new char[size + needAlignment + CORSAC_PLATFORM_PTR_SIZE];
    std::cout << (size + needAlignment + CORSAC_PLATFORM_PTR_SIZE) << std::endl;
    void* point = (void*)((uintptr_t)data + CORSAC_PLATFORM_PTR_SIZE);
    void* aligned = (void*)(((uintptr_t)point + needAlignment - 1) & ~(needAlignment - 1));

    void** resultPtr = (void**)aligned - 1;
    *(resultPtr) = data;
    return aligned;
}

void corsac::allocator::deallocate(void* point, size_t)
{
    if (point != nullptr)
    {
        void* pointAllocation = *((void**)point - 1);
        delete[](char*)pointAllocation;
    }
}