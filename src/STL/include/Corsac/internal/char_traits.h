/**
 * corsac::STL
 *
 * char_traits.h
 *
 * Created by Falldot on 23.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_CHAR_TRAITS_H
#define CORSAC_CHAR_TRAITS_H

#pragma once
/**
 * Описание (Falldot 23.11.2021)
 *
 * Этот файл реализует аналогичные функции char_traits, который является частью стандартной
 * спецификации библиотеки STL C++. Это предназначено только для внутреннего использования CORSAC.
 * Доступ к функциональным возможностям можно получить с помощью типов corsac::string
 * corsac::string_view types.
 */

#include "Corsac/internal/config.h""
#include "Corsac/type_traits.h"

#include <ctype.h>              // toupper, etc.
#include <string.h>             // memset, etc.

namespace corsac
{
    /**
    * DecodePart
    *
    * Реализует кодирование/декодирование UTF8/UCS2/UCS4.
    */
    CORSAC_API bool DecodePart(const char*& pSrc, const char* pSrcEnd, char*&     pDest, char*     pDestEnd);
    CORSAC_API bool DecodePart(const char*& pSrc, const char* pSrcEnd, char16_t*& pDest, char16_t* pDestEnd);
    CORSAC_API bool DecodePart(const char*& pSrc, const char* pSrcEnd, char32_t*& pDest, char32_t* pDestEnd);

    CORSAC_API bool DecodePart(const char16_t*& pSrc, const char16_t* pSrcEnd, char*&     pDest, char*     pDestEnd);
    CORSAC_API bool DecodePart(const char16_t*& pSrc, const char16_t* pSrcEnd, char16_t*& pDest, char16_t* pDestEnd);
    CORSAC_API bool DecodePart(const char16_t*& pSrc, const char16_t* pSrcEnd, char32_t*& pDest, char32_t* pDestEnd);

    CORSAC_API bool DecodePart(const char32_t*& pSrc, const char32_t* pSrcEnd, char*&     pDest, char*     pDestEnd);
    CORSAC_API bool DecodePart(const char32_t*& pSrc, const char32_t* pSrcEnd, char16_t*& pDest, char16_t* pDestEnd);
    CORSAC_API bool DecodePart(const char32_t*& pSrc, const char32_t* pSrcEnd, char32_t*& pDest, char32_t* pDestEnd);

    CORSAC_API bool DecodePart(const int*& pSrc, const int* pSrcEnd, char*&     pDest, char*     pDestEnd);
    CORSAC_API bool DecodePart(const int*& pSrc, const int* pSrcEnd, char16_t*& pDest, char16_t* pDestEnd);
    CORSAC_API bool DecodePart(const int*& pSrc, const int* pSrcEnd, char32_t*& pDest, char32_t* pDestEnd);

    #if CORSAC_CHAR8_UNIQUE
        bool DecodePart(const char8_t*& pSrc, const char8_t* pSrcEnd, char8_t*&  pDest, char8_t*  pDestEnd);

        bool DecodePart(const char8_t*& pSrc, const char8_t* pSrcEnd, char*&     pDest, char*     pDestEnd);
        bool DecodePart(const char8_t*& pSrc, const char8_t* pSrcEnd, char16_t*& pDest, char16_t* pDestEnd);
        bool DecodePart(const char8_t*& pSrc, const char8_t* pSrcEnd, char32_t*& pDest, char32_t* pDestEnd);

        bool DecodePart(const char*&     pSrc, const char*     pSrcEnd, char8_t*& pDest, char8_t* pDestEnd);
        bool DecodePart(const char16_t*& pSrc, const char16_t* pSrcEnd, char8_t*& pDest, char8_t* pDestEnd);
        bool DecodePart(const char32_t*& pSrc, const char32_t* pSrcEnd, char8_t*& pDest, char8_t* pDestEnd);
    #endif

    #if CORSAC_WCHAR_UNIQUE
        bool DecodePart(const wchar_t*& pSrc, const wchar_t* pSrcEnd, wchar_t*&  pDest, wchar_t*  pDestEnd);

        bool DecodePart(const wchar_t*& pSrc, const wchar_t* pSrcEnd, char*&     pDest, char*     pDestEnd);
        bool DecodePart(const wchar_t*& pSrc, const wchar_t* pSrcEnd, char16_t*& pDest, char16_t* pDestEnd);
        bool DecodePart(const wchar_t*& pSrc, const wchar_t* pSrcEnd, char32_t*& pDest, char32_t* pDestEnd);

        bool DecodePart(const char*&     pSrc, const char*     pSrcEnd, wchar_t*& pDest, wchar_t* pDestEnd);
        bool DecodePart(const char16_t*& pSrc, const char16_t* pSrcEnd, wchar_t*& pDest, wchar_t* pDestEnd);
        bool DecodePart(const char32_t*& pSrc, const char32_t* pSrcEnd, wchar_t*& pDest, wchar_t* pDestEnd);
    #endif

    #if CORSAC_CHAR8_UNIQUE && CORSAC_WCHAR_UNIQUE
        bool DecodePart(const char8_t*& pSrc, const char8_t* pSrcEnd, wchar_t*& pDest, wchar_t* pDestEnd);
        bool DecodePart(const wchar_t*& pSrc, const wchar_t* pSrcEnd, char8_t*& pDest, char8_t* pDestEnd);
    #endif


    #if CORSAC_WCHAR_UNIQUE
        inline bool DecodePart(const wchar_t*& pSrc, const wchar_t* pSrcEnd, wchar_t*& pDest, wchar_t* pDestEnd)
		{
			return DecodePart(reinterpret_cast<const char*&>(pSrc), reinterpret_cast<const char*>(pSrcEnd), reinterpret_cast<char*&>(pDest), reinterpret_cast<char*&>(pDestEnd));
		}

		inline bool DecodePart(const wchar_t*& pSrc, const wchar_t* pSrcEnd, char*& pDest, char* pDestEnd)
		{
		#if (CORSAC_WCHAR_SIZE == 2)
			return DecodePart(reinterpret_cast<const char16_t*&>(pSrc), reinterpret_cast<const char16_t*>(pSrcEnd), pDest, pDestEnd);
		#elif (CORSAC_WCHAR_SIZE == 4)
			return DecodePart(reinterpret_cast<const char32_t*&>(pSrc), reinterpret_cast<const char32_t*>(pSrcEnd), pDest, pDestEnd);
		#endif
		}

		inline bool DecodePart(const wchar_t*& pSrc, const wchar_t* pSrcEnd, char16_t*& pDest, char16_t* pDestEnd)
		{
            #if (CORSAC_WCHAR_SIZE == 2)
                return DecodePart(reinterpret_cast<const char16_t*&>(pSrc), reinterpret_cast<const char16_t*>(pSrcEnd), pDest, pDestEnd);
            #elif (CORSAC_WCHAR_SIZE == 4)
                return DecodePart(reinterpret_cast<const char32_t*&>(pSrc), reinterpret_cast<const char32_t*>(pSrcEnd), pDest, pDestEnd);
            #endif
		}

		inline bool DecodePart(const wchar_t*& pSrc, const wchar_t* pSrcEnd, char32_t*& pDest, char32_t* pDestEnd)
		{
            #if (CORSAC_WCHAR_SIZE == 2)
                return DecodePart(reinterpret_cast<const char16_t*&>(pSrc), reinterpret_cast<const char16_t*>(pSrcEnd), pDest, pDestEnd);
            #elif (CORSAC_WCHAR_SIZE == 4)
                return DecodePart(reinterpret_cast<const char32_t*&>(pSrc), reinterpret_cast<const char32_t*>(pSrcEnd), pDest, pDestEnd);
            #endif
		}

		inline bool DecodePart(const char*& pSrc, const char* pSrcEnd, wchar_t*& pDest, wchar_t* pDestEnd)
		{
            #if (CORSAC_WCHAR_SIZE == 2)
                return DecodePart(pSrc, pSrcEnd, reinterpret_cast<char16_t*&>(pDest), reinterpret_cast<char16_t*>(pDestEnd));
            #elif (CORSAC_WCHAR_SIZE == 4)
                return DecodePart(pSrc, pSrcEnd, reinterpret_cast<char32_t*&>(pDest), reinterpret_cast<char32_t*>(pDestEnd));
            #endif
		}

		inline bool DecodePart(const char16_t*& pSrc, const char16_t* pSrcEnd, wchar_t*& pDest, wchar_t* pDestEnd)
		{
            #if (CORSAC_WCHAR_SIZE == 2)
                return DecodePart(pSrc, pSrcEnd, reinterpret_cast<char16_t*&>(pDest), reinterpret_cast<char16_t*>(pDestEnd));
            #elif (CORSAC_WCHAR_SIZE == 4)
                return DecodePart(pSrc, pSrcEnd, reinterpret_cast<char32_t*&>(pDest), reinterpret_cast<char32_t*>(pDestEnd));
            #endif
		}

		inline bool DecodePart(const char32_t*& pSrc, const char32_t* pSrcEnd, wchar_t*& pDest, wchar_t* pDestEnd)
		{
            #if (CORSAC_WCHAR_SIZE == 2)
                return DecodePart(pSrc, pSrcEnd, reinterpret_cast<char16_t*&>(pDest), reinterpret_cast<char16_t*>(pDestEnd));
            #elif (CORSAC_WCHAR_SIZE == 4)
                return DecodePart(pSrc, pSrcEnd, reinterpret_cast<char32_t*&>(pDest), reinterpret_cast<char32_t*>(pDestEnd));
            #endif
		}
    #endif

    #if CORSAC_CHAR8_UNIQUE
        inline bool DecodePart(const char8_t*& pSrc, const char8_t* pSrcEnd, char8_t*& pDest, char8_t* pDestEnd)
        {
            return DecodePart(reinterpret_cast<const char*&>(pSrc), reinterpret_cast<const char*>(pSrcEnd), reinterpret_cast<char*&>(pDest), reinterpret_cast<char*&>(pDestEnd));
        }

        inline bool DecodePart(const char8_t*& pSrc, const char8_t* pSrcEnd, char*& pDest, char* pDestEnd)
        {
            return DecodePart(reinterpret_cast<const char*&>(pSrc), reinterpret_cast<const char*>(pSrcEnd), pDest, pDestEnd);
        }

        inline bool DecodePart(const char8_t*& pSrc, const char8_t* pSrcEnd, char16_t*& pDest, char16_t* pDestEnd)
        {
            return DecodePart(reinterpret_cast<const char*&>(pSrc), reinterpret_cast<const char*>(pSrcEnd), pDest, pDestEnd);
        }

        inline bool DecodePart(const char8_t*& pSrc, const char8_t* pSrcEnd, char32_t*& pDest, char32_t* pDestEnd)
        {
            return DecodePart(reinterpret_cast<const char*&>(pSrc), reinterpret_cast<const char*>(pSrcEnd), pDest, pDestEnd);
        }

        inline bool DecodePart(const char*& pSrc, const char* pSrcEnd, char8_t*& pDest, char8_t* pDestEnd)
        {
            return DecodePart(pSrc, pSrcEnd, reinterpret_cast<char*&>(pDest), reinterpret_cast<char*&>(pDestEnd));
        }

        inline bool DecodePart(const char16_t*& pSrc, const char16_t* pSrcEnd, char8_t*& pDest, char8_t* pDestEnd)
        {
            return DecodePart(pSrc, pSrcEnd, reinterpret_cast<char*&>(pDest), reinterpret_cast<char*&>(pDestEnd));
        }

        inline bool DecodePart(const char32_t*& pSrc, const char32_t* pSrcEnd, char8_t*& pDest, char8_t* pDestEnd)
        {
            return DecodePart(pSrc, pSrcEnd, reinterpret_cast<char*&>(pDest), reinterpret_cast<char*&>(pDestEnd));
        }
    #endif

    #if CORSAC_CHAR8_UNIQUE && CORSAC_WCHAR_UNIQUE
        inline bool DecodePart(const char8_t*&  pSrc, const char8_t*  pSrcEnd, wchar_t*&  pDest, wchar_t*  pDestEnd)
		{
		#if (CORSAC_WCHAR_SIZE == 2)
		    return DecodePart(pSrc, pSrcEnd, reinterpret_cast<char16_t*&>(pDest), reinterpret_cast<char16_t*>(pDestEnd));
		#elif (CORSAC_WCHAR_SIZE == 4)
		    return DecodePart(pSrc, pSrcEnd, reinterpret_cast<char32_t*&>(pDest), reinterpret_cast<char32_t*>(pDestEnd));
		#endif
		}

		inline bool DecodePart(const wchar_t*&  pSrc, const wchar_t*  pSrcEnd, char8_t*&  pDest, char8_t*  pDestEnd)
		{
		#if (CORSAC_WCHAR_SIZE == 2)
			return DecodePart(reinterpret_cast<const char16_t*&>(pSrc), reinterpret_cast<const char16_t*>(pSrcEnd), reinterpret_cast<char*&>(pDest), reinterpret_cast<char*>(pDestEnd));
		#elif (CORSAC_WCHAR_SIZE == 4)
			return DecodePart(reinterpret_cast<const char32_t*&>(pSrc), reinterpret_cast<const char32_t*>(pSrcEnd), reinterpret_cast<char*&>(pDest), reinterpret_cast<char*>(pDestEnd));
		#endif
		}
    #endif

    // 'char traits' functionality
    inline char CharToLower(char c)
    { return static_cast<char>(tolower(static_cast<unsigned>(c))); }

    template<typename T>
    inline T CharToLower(T c)
    { if (static_cast<unsigned>(c) <= 0xff) return static_cast<T>(tolower(static_cast<unsigned>(c))); return c; }


    inline char CharToUpper(char c)
    { return static_cast<char>(toupper(static_cast<unsigned>(c))); }

    template<typename T>
    inline T CharToUpper(T c)
    { if(static_cast<unsigned>(c) <= 0xff) return static_cast<T>(toupper(static_cast<unsigned>(c))); return c; }


    template <typename T>
    int Compare(const T* p1, const T* p2, size_t n)
    {
        for(; n > 0; ++p1, ++p2, --n)
        {
            if(*p1 != *p2)
                return (static_cast<typename make_unsigned<T>::type>(*p1) <
                        static_cast<typename make_unsigned<T>::type>(*p2)) ? -1 : 1;
        }
        return 0;
    }

    inline int Compare(const char* p1, const char* p2, size_t n)
    {
        return memcmp(p1, p2, n);
    }


    template <typename T>
    inline int CompareI(const T* p1, const T* p2, size_t n)
    {
        for(; n > 0; ++p1, ++p2, --n)
        {
            const T c1 = CharToLower(*p1);
            const T c2 = CharToLower(*p2);

            if(c1 != c2)
                return (static_cast<typename make_unsigned<T>::type>(c1) <
                        static_cast<typename make_unsigned<T>::type>(c2)) ? -1 : 1;
        }
        return 0;
    }


    template<typename T>
    inline const T* Find(const T* p, T c, size_t n)
    {
        for(; n > 0; --n, ++p)
        {
            if(*p == c)
                return p;
        }

        return NULL;
    }

    inline const char* Find(const char* p, char c, size_t n)
    {
        return (const char*)memchr(p, c, n);
    }


    template<typename T>
    inline constexpr size_t CharStrlen(const T* p)
    {
        const auto* pCurrent = p;
        while(*pCurrent)
            ++pCurrent;
        return (size_t)(pCurrent - p);
    }


    template <typename T>
    inline T* CharStringUninitializedCopy(const T* pSource, const T* pSourceEnd, T* pDestination)
    {
        memmove(pDestination, pSource, (size_t)(pSourceEnd - pSource) * sizeof(T));
        return pDestination + (pSourceEnd - pSource);
    }

    template <typename T>
    const T* CharTypeStringFindEnd(const T* pBegin, const T* pEnd, T c)
    {
        const T* pTemp = pEnd;
        while(--pTemp >= pBegin)
        {
            if(*pTemp == c)
                return pTemp;
        }

        return pEnd;
    }

    template <typename T>
    const T* CharTypeStringRSearch(const T* p1Begin, const T* p1End,
                                   const T* p2Begin, const T* p2End)
    {
        // Проверьте строки нулевой длины, и в этом случае у нас есть совпадение или сбой,
        // но возвращаемое значение в любом случае одинаково.
        if((p1Begin == p1End) || (p2Begin == p2End))
            return p1Begin;

        // Тест на длину 1.
        if((p2Begin + 1) == p2End)
            return CharTypeStringFindEnd(p1Begin, p1End, *p2Begin);

        // Проверить, не превышает ли длина строки поиска длина строки.
        if((p2End - p2Begin) > (p1End - p1Begin))
            return p1End;

        // Общий случай.
        const T* pSearchEnd = (p1End - (p2End - p2Begin) + 1);
        const T* pCurrent1;
        const T* pCurrent2;

        while(pSearchEnd != p1Begin)
        {
            // Найдите последнее вхождение *p2Begin.
            pCurrent1 = CharTypeStringFindEnd(p1Begin, pSearchEnd, *p2Begin);
            if(pCurrent1 == pSearchEnd) // Если первый символ p2 не найден,
                return p1End;           //  тогда у нас сразу же сбой.

            // В случае, *pTemp == *p2Begin. Так что сравните остальное.
            pCurrent2 = p2Begin;
            while(*pCurrent1++ == *pCurrent2++)
            {
                if(pCurrent2 == p2End)
                    return (pCurrent1 - (p2End - p2Begin));
            }

            // Более умный алгоритм может знать, что нужно вычесть больше, чем один,
            // но в большинстве случаев это не имеет большого значения.
            --pSearchEnd;
        }

        return p1End;
    }

    template <typename T>
    inline const T* CharTypeStringFindFirstOf(const T* p1Begin, const T* p1End, const T* p2Begin, const T* p2End)
    {
        for (; p1Begin != p1End; ++p1Begin)
        {
            for (const T* pTemp = p2Begin; pTemp != p2End; ++pTemp)
            {
                if (*p1Begin == *pTemp)
                    return p1Begin;
            }
        }
        return p1End;
    }

    template <typename T>
    inline const T* CharTypeStringRFindFirstNotOf(const T* p1RBegin, const T* p1REnd, const T* p2Begin, const T* p2End)
    {
        for (; p1RBegin != p1REnd; --p1RBegin)
        {
            const T* pTemp;
            for (pTemp = p2Begin; pTemp != p2End; ++pTemp)
            {
                if (*(p1RBegin - 1) == *pTemp)
                    break;
            }
            if (pTemp == p2End)
                return p1RBegin;
        }
        return p1REnd;
    }

    template <typename T>
    inline const T* CharTypeStringFindFirstNotOf(const T* p1Begin, const T* p1End, const T* p2Begin, const T* p2End)
    {
        for (; p1Begin != p1End; ++p1Begin)
        {
            const T* pTemp;
            for (pTemp = p2Begin; pTemp != p2End; ++pTemp)
            {
                if (*p1Begin == *pTemp)
                    break;
            }
            if (pTemp == p2End)
                return p1Begin;
        }
        return p1End;
    }

    template <typename T>
    inline const T* CharTypeStringRFindFirstOf(const T* p1RBegin, const T* p1REnd, const T* p2Begin, const T* p2End)
    {
        for (; p1RBegin != p1REnd; --p1RBegin)
        {
            for (const T* pTemp = p2Begin; pTemp != p2End; ++pTemp)
            {
                if (*(p1RBegin - 1) == *pTemp)
                    return p1RBegin;
            }
        }
        return p1REnd;
    }

    template <typename T>
    inline const T* CharTypeStringRFind(const T* pRBegin, const T* pREnd, const T c)
    {
        while (pRBegin > pREnd)
        {
            if (*(pRBegin - 1) == c)
                return pRBegin;
            --pRBegin;
        }
        return pREnd;
    }

    inline char* CharStringUninitializedFillN(char* pDestination, size_t n, const char c)
    {
        if (n)
            memset(pDestination, static_cast<unsigned>(c), static_cast<size_t>(n));
        return pDestination + n;
    }

    template<typename T>
    inline T* CharStringUninitializedFillN(T* pDestination, size_t n, const T c)
    {
        T * pDest           = pDestination;
        const T* const pEnd = pDestination + n;
        while(pDest < pEnd)
            *pDest++ = c;
        return pDestination + n;
    }

    inline char* CharTypeAssignN(char* pDestination, size_t n, char c)
    {
        if (n)
            return (char*)memset(pDestination, c, static_cast<size_t>(n));
        return pDestination;
    }

    template<typename T>
    inline T* CharTypeAssignN(T* pDestination, size_t n, T c)
    {
        T* pDest            = pDestination;
        const T* const pEnd = pDestination + n;
        while(pDest < pEnd)
            *pDest++ = c;
        return pDestination;
    }
}

#endif //CORSAC_CHAR_TRAITS_H
