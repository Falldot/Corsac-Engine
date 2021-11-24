//// Copyright (c) 2021 Corsac Team
//// License: MIT License
//// MIT License web page: https://opensource.org/licenses/MIT
//#ifndef CORSAC_ALLOCATOR_MALLOC_H
//#define CORSAC_ALLOCATOR_MALLOC_H
//
//#pragma once
//
//#include <malloc.h>
//
//#ifndef corsac_allocator_type
//	#define corsac_allocator_type corsac::malloc_allocator
//#endif
//
//#ifndef corsac_alloc
//	#define corsac_alloc(allocator, size, alignment, offset) (allocator).allocate((size), (alignment), (offset))
//#endif
//
//#ifndef corsac_free
//	#define corsac_free(allocator, point, size) (allocator).deallocate(static_cast<void*>(point), (size))
//#endif
//
//namespace corsac
//{
*class allocator_malloc
*{
*public:
//
//        void* allocate(size_t size);
//        void* allocate(size_t size, size_t alignment, size_t offset);
//
//        void deallocate(void* point, size_t);
*};
//
*void* allocator_malloc::allocate(size_t size)
*{
//        return malloc(size);
*}
//
*void* allocator_malloc::allocate(size_t size, size_t alignment, size_t offset)
*{
//        return malloc(size + alignment + offset);
*}
//
*void allocator_malloc::deallocate(void* point, size_t)
*{
//        if (point != nullptr)
//        {
//            free(point);
//        }
*}
//
*template <typename Allocator>
*inline void* allocate_memory(Allocator& allocator, size_t value, size_t alignment, size_t offset)
*{
//        return corsac_alloc(allocator, value, alignment, offset);
*}
//
*template<class T>
*void destruct(T* p)
*{
//        p->~T();
*}
//
*template<class T, typename Iterator>
*void destruct(Iterator first, Iterator last)
*{
//        for(; first != last; ++first)
//            (*first).~T();
*}
//
//} // namespace corsac
//
//
//#endif // Header include guard