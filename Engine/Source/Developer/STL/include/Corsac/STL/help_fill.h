/**
 * corsac::STL
 *
 * internal/help_fill.h
 *
 * Created by Falldot on 15.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STL_HELP_FILL_H
#define CORSAC_STL_HELP_FILL_H

#pragma once

#include "Corsac/STL/config.h"

#if defined(CORSAC_COMPILER_MICROSOFT) && (defined(CORSAC_PROCESSOR_X86) || defined(CORSAC_PROCESSOR_X86_64))
    #include <intrin.h>
#endif

namespace corsac
{
    /**
    * fill
    *
    * Мы реализуем некоторые вспомогательные функции заполнения, чтобы мы могли оптимизировать
    * где возможно.
    */
    template <bool bIsScalar>
    struct fill_imp
    {
        template <typename ForwardIterator, typename T>
        static void do_fill(ForwardIterator first, ForwardIterator last, const T& value)
        {
            // Стандарт C ++ не указывает, нужно ли нам создавать временное или нет,
            // но все реализации std STL написаны так, как здесь.
            for(; first != last; ++first)
                *first = value;
        }
    };

    template <>
    struct fill_imp<true>
    {
        template <typename ForwardIterator, typename T>
        static void do_fill(ForwardIterator first, ForwardIterator last, const T& value)
        {
            using value_type = typename corsac::iterator_traits<ForwardIterator>::value_type;
            // Мы создаем временный интервал и заполняем его, потому что значение может быть псевдонимом
            // целевого диапазона, и поэтому компилятор будет вынужден генерировать менее эффективный код.
            for(const T temp = value; first != last; ++first)
            {
                CORSAC_UNUSED(temp);
                *first = static_cast<value_type>(temp);
            }
        }
    };

    /**
    * fill
    *
    * fill похож на memset в том, что он многократно присваивает одно значение целевому диапазону.
    * Он позволяет использовать любой тип итератора (не только массив), а исходное значение может
    * быть любого типа, а не только байтом. Обратите внимание, что исходное значение (которое
    * является ссылкой) может происходить из диапазона назначения.
    *
    * Эффекты: присваивает значение всем итераторам в диапазоне [первый, последний).
    *
    * Сложность: const «последнее - первое».
    *
    * Примечание. В стандарте C ++ ничего не говорится о параметре значения, находящемся в
    * диапазоне от первого до последнего. Все реализации std STL действуют так, как будто стандарт
    * определяет, что значение не должно выходить из этого диапазона.
    */
    template <typename ForwardIterator, typename T>
    inline void fill(ForwardIterator first, ForwardIterator last, const T& value)
    {
        corsac::fill_imp<corsac::is_scalar_v<T>>::do_fill(first, last, value);
    }

    #if (defined(CORSAC_COMPILER_GCC) || defined(CORSAC_COMPILER_CLANG)) && (defined(CORSAC_PROCESSOR_X86) || defined(CORSAC_PROCESSOR_X86_64))
        #if defined(CORSAC_PROCESSOR_X86_64)
			template <typename Value>
			inline void fill(uint64_t* first, uint64_t* last, Value c)
			{
                uintptr_t count = static_cast<uintptr_t>(last - first);
				uint64_t  value = static_cast<uint64_t>(c);

				__asm__ __volatile__ ("cld\n\t"
									  "rep stosq\n\t"
									   : "+c" (count), "+D" (first), "=m" (first)
									   : "a" (value)
									   : "cc" );
			}


			template <typename Value>
			inline void fill(int64_t* first, int64_t* last, Value c)
			{
                uintptr_t count = static_cast<uintptr_t>(last - first);
				int64_t   value = static_cast<int64_t>(c);

				__asm__ __volatile__ ("cld\n\t"
									  "rep stosq\n\t"
									   : "+c" (count), "+D" (first), "=m" (first)
									   : "a" (value)
									   : "cc" );
			}
		#endif

        template <typename Value>
        inline void fill(uint32_t* first, uint32_t* last, Value c)
        {
            uintptr_t count = static_cast<uintptr_t>(last - first);
            uint32_t  value = static_cast<uint32_t>(c);

            __asm__ __volatile__ ("cld\n\t"
                                  "rep stosl\n\t"
                                   : "+c" (count), "+D" (first), "=m" (first)
                                   : "a" (value)
                                   : "cc" );
        }


		template <typename Value>
		inline void fill(int32_t* first, int32_t* last, Value c)
		{
			uintptr_t count = static_cast<uintptr_t>(last - first);
			int32_t   value = static_cast<int32_t>(c);

			__asm__ __volatile__ ("cld\n\t"
								  "rep stosl\n\t"
								   : "+c" (count), "+D" (first), "=m" (first)
								   : "a" (value)
								   : "cc" );
		}


		template <typename Value>
		inline void fill(uint16_t* first, uint16_t* last, Value c)
		{
			uintptr_t count = static_cast<uintptr_t>(last - first);
			uint16_t  value = static_cast<uint16_t>(c);

			__asm__ __volatile__ ("cld\n\t"
								  "rep stosw\n\t"
								   : "+c" (count), "+D" (first), "=m" (first)
								   : "a" (value)
								   : "cc" );
		}


		template <typename Value>
		inline void fill(int16_t* first, int16_t* last, Value c)
		{
			uintptr_t count = static_cast<uintptr_t>(last - first);
			int16_t   value = static_cast<int16_t>(c);

			__asm__ __volatile__ ("cld\n\t"
								  "rep stosw\n\t"
								   : "+c" (count), "+D" (first), "=m" (first)
								   : "a" (value)
								   : "cc" );
		}

    #elif defined(CORSAC_COMPILER_MICROSOFT) && (defined(CORSAC_PROCESSOR_X86) || defined(CORSAC_PROCESSOR_X86_64))
        #if defined(CORSAC_PROCESSOR_X86_64)
			template <typename Value>
			inline void fill(uint64_t* first, uint64_t* last, Value c)
			{
				__stosq(first, static_cast<uint64_t>(c), static_cast<size_t>(last - first));
			}

			template <typename Value>
			inline void fill(int64_t* first, int64_t* last, Value c)
			{
				__stosq(static_cast<uint64_t*>(first),  static_cast<uint64_t>(c),  static_cast<size_t>(last - first));
			}
		#endif

		template <typename Value>
		inline void fill(uint32_t* first, uint32_t* last, Value c)
		{
			__stosd(static_cast<unsigned long*>(first), static_cast<unsigned long>(c), static_cast<size_t>(last - first));
		}

		template <typename Value>
		inline void fill(int32_t* first, int32_t* last, Value c)
		{
			__stosd(static_cast<unsigned long*>(first), static_cast<unsigned long>(c), static_cast<size_t>(last - first));
		}

		template <typename Value>
		inline void fill(uint16_t* first, uint16_t* last, Value c)
		{
			__stosw(first, static_cast<uint16_t>(c), static_cast<size_t>(last - first));
		}

		template <typename Value>
		inline void fill(int16_t* first, int16_t* last, Value c)
		{
			__stosw(static_cast<uint16_t*>(first), static_cast<uint16_t>(c), static_cast<size_t>(last - first));
		}
    #endif


    inline void fill(char* first, char* last, const char& c) // Спорный вопрос, следует ли нам использовать здесь char& c или char c.
    {
        memset(first, static_cast<unsigned char>(c), static_cast<size_t>(last - first));
    }

    inline void fill(char* first, char* last, const int c) // Это используется для таких случаев, как 'fill (first, last, 0)'.
    {
        memset(first, static_cast<unsigned char>(c), static_cast<size_t>(last - first));
    }

    inline void fill(unsigned char* first, unsigned char* last, const unsigned char& c)
    {
        memset(first, static_cast<unsigned char>(c), static_cast<size_t>(last - first));
    }

    inline void fill(unsigned char* first, unsigned char* last, const int c)
    {
        memset(first, static_cast<unsigned char>(c), static_cast<size_t>(last - first));
    }

    inline void fill(signed char* first, signed char* last, const signed char& c)
    {
        memset(first, static_cast<unsigned char>(c), static_cast<size_t>(last - first));
    }

    inline void fill(signed char* first, signed char* last, const int c)
    {
        memset(first, static_cast<unsigned char>(c), static_cast<size_t>(last - first));
    }

    #if defined(CORSAC_COMPILER_MSVC)
        inline void fill(bool* first, bool* last, const bool& b)
        {
            memset(first, static_cast<char>(b), static_cast<size_t>(last - first));
        }
    #endif

    /**
    * fill_n
    *
    * Мы реализуем некоторые вспомогательные функции заполнения, чтобы мы могли оптимизировать его,
    * где это возможно.
    */
    template <bool bIsScalar>
    struct fill_n_imp
    {
        template <typename OutputIterator, typename Size, typename T>
        static OutputIterator do_fill(OutputIterator first, Size n, const T& value)
        {
            for (; n-- > 0; ++first)
                *first = value;
            return first;
        }
    };

    template <>
    struct fill_n_imp<true>
    {
        template <typename OutputIterator, typename Size, typename T>
        static OutputIterator do_fill(OutputIterator first, Size n, const T& value)
        {
            using value_type = typename corsac::iterator_traits<OutputIterator>::value_type;

            // Мы создаем временный интервал и заполняем его, потому что значение может быть псевдонимом
            // целевого диапазона, и поэтому компилятор будет вынужден генерировать менее эффективный код.
            for  (const T temp = value; n-- > 0; ++first)
                *first = static_cast<value_type>(temp);
            return first;
        }
    };

    /**
    * fill_n
    *
    * Функция fill_n очень похожа на memset в том, что a копирует исходное значение n раз в целевой
    * диапазон. Исходное значение может быть в пределах диапазона назначения.
    *
    * Эффекты: присваивает значение всем итераторам в диапазоне [first, first + n).
    *
    * Сложность: ровно n.
    */
    template <typename OutputIterator, typename Size, typename T>
    OutputIterator fill_n(OutputIterator first, Size n, const T& value)
    {
        return corsac::fill_n_imp<corsac::is_scalar_v<T>>::do_fill(first, n, value);
    }

    template <typename Size>
    inline char* fill_n(char* first, Size n, const char& c)
    {
        return static_cast<char*>(memset(first, c, static_cast<size_t>(n)) + n);
    }

    template <typename Size>
    inline unsigned char* fill_n(unsigned char* first, Size n, const unsigned char& c)
    {
        return static_cast<unsigned char*>(memset(first, c, static_cast<size_t>(n)) + n);
    }

    template <typename Size>
    inline signed char* fill_n(signed char* first, Size n, const signed char& c)
    {
        return static_cast<signed char*>(memset(first, c, n) + static_cast<size_t>(n));
    }

    #if defined(CORSAC_COMPILER_MSVC)
        template <typename Size>
        inline bool* fill_n(bool* first, Size n, const bool& b)
        {
            return static_cast<bool*>(memset(first,  static_cast<char>(b), n) + static_cast<size_t>(n));
        }
    #endif

    #if (defined(CORSAC_COMPILER_GCC) || defined(CORSAC_COMPILER_CLANG)) && (defined(CORSAC_PROCESSOR_X86) || defined(CORSAC_PROCESSOR_X86_64))
        #if defined(CORSAC_PROCESSOR_X86_64)
			template <typename Size, typename Value>
			inline uint64_t* fill_n(uint64_t* first, Size n, Value c)
			{
                uintptr_t count = static_cast<uintptr_t>(n);
				uint64_t  value = static_cast<uint64_t>(c);

				__asm__ __volatile__ ("cld\n\t"
									  "rep stosq\n\t"
									   : "+c" (count), "+D" (first), "=m" (first)
									   : "a" (value)
									   : "cc" );
				return first;
			}


			template <typename Size, typename Value>
			inline int64_t* fill_n(int64_t* first, Size n, Value c)
			{
                uintptr_t count = static_cast<uintptr_t>(n);
				int64_t   value = static_cast<int64_t>(c);

				__asm__ __volatile__ ("cld\n\t"
									  "rep stosq\n\t"
									   : "+c" (count), "+D" (first), "=m" (first)
									   : "a" (value)
									   : "cc" );
				return first;
			}
		#endif

		template <typename Size, typename Value>
		inline uint32_t* fill_n(uint32_t* first, Size n, Value c)
		{
            uintptr_t count = static_cast<uintptr_t>(n);
			uint32_t  value = static_cast<uint32_t>(c);

			__asm__ __volatile__ ("cld\n\t"
								  "rep stosl\n\t"
								   : "+c" (count), "+D" (first), "=m" (first)
								   : "a" (value)
								   : "cc" );
			return first;
		}


		template <typename Size, typename Value>
		inline int32_t* fill_n(int32_t* first, Size n, Value c)
		{
            uintptr_t count = static_cast<uintptr_t>(n);
			int32_t   value = static_cast<int32_t>(c);

			__asm__ __volatile__ ("cld\n\t"
								  "rep stosl\n\t"
								   : "+c" (count), "+D" (first), "=m" (first)
								   : "a" (value)
								   : "cc" );
			return first;
		}


		template <typename Size, typename Value>
		inline uint16_t* fill_n(uint16_t* first, Size n, Value c)
		{
            uintptr_t count = static_cast<uintptr_t>(n);
			uint16_t  value = static_cast<uint16_t>(c);

			__asm__ __volatile__ ("cld\n\t"
								  "rep stosw\n\t"
								   : "+c" (count), "+D" (first), "=m" (first)
								   : "a" (value)
								   : "cc" );
			return first;
		}


		template <typename Size, typename Value>
		inline int16_t* fill_n(int16_t* first, Size n, Value c)
		{
            uintptr_t count = static_cast<uintptr_t>(n);
			int16_t   value = static_cast<int16_t>(c);

			__asm__ __volatile__ ("cld\n\t"
								  "rep stosw\n\t"
								   : "+c" (count), "+D" (first), "=m" (first)
								   : "a" (value)
								   : "cc" );
			return first;
		}

    #elif defined(CORSAC_COMPILER_MICROSOFT) && (defined(CORSAC_PROCESSOR_X86) || defined(CORSAC_PROCESSOR_X86_64))
         #if defined(CORSAC_PROCESSOR_X86_64)
            template <typename Size, typename Value>
            inline uint64_t* fill_n(uint64_t* first, Size n, Value c)
            {
                __stosq(first, (uint64_t)c, static_cast<size_t>(n));
                return first + n;
            }

            template <typename Size, typename Value>
            inline int64_t* fill_n(int64_t* first, Size n, Value c)
            {
                __stosq((uint64_t*)first, (uint64_t)c, static_cast<size_t>(n));
                return first + n;
            }
        #endif

        template <typename Size, typename Value>
        inline uint32_t* fill_n(uint32_t* first, Size n, Value c)
        {
            __stosd(static_cast<unsigned long*>(first), static_cast<unsigned long>(c), static_cast<size_t>(n));
            return first + n;
        }

        template <typename Size, typename Value>
        inline int32_t* fill_n(int32_t* first, Size n, Value c)
        {
            __stosd(static_cast<unsigned long*>(first), static_cast<unsigned long>(c), static_cast<size_t>(n));
            return first + n;
        }

        template <typename Size, typename Value>
        inline uint16_t* fill_n(uint16_t* first, Size n, Value c)
        {
            __stosw(first, static_cast<uint16_t>(c), static_cast<size_t>(n));
            return first + n;
        }

        template <typename Size, typename Value>
        inline int16_t* fill_n(int16_t* first, Size n, Value c)
        {
            __stosw(static_cast<uint16_t*>(c), static_cast<uint16_t>(c), static_cast<size_t>(n));
            return first + n;
        }
    #endif
}

#endif //CORSAC_STL_HELP_FILL_H
