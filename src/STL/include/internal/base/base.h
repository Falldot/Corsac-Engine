//
// internal/base/base.h
//
// Created by Falldot on 01.11.2021.
// Copyright (c) 2021 Corsac. All rights reserved.
//

#ifndef CORSAC_BASE_H
#define CORSAC_BASE_H

#include "compiler.h"
#include "compiler_traits.h"
#include "platform.h"

/**
* Стандарт C ++ определяет size_t как встроенный тип.
* Некоторые компиляторы не соответствуют стандартам в этом отношении, поэтому нам нужно дополнительное включение.
* Случай аналогичен wchar_t в C +.
*/
#if defined(CORSAC_COMPILER_GCC) || defined(CORSAC_COMPILER_MSVC) || defined(CORSAC_WCHAR_T_NON_NATIVE) || defined(CORSAC_PLATFORM_SONY)
    #if defined(CORSAC_COMPILER_MSVC)
            #pragma warning(push, 0)
            #pragma warning(disable: 4265 4365 4836 4574)
        #endif
        #include <stddef.h>
        #if defined(CORSAC_COMPILER_MSVC)
            #pragma warning(pop)
        #endif
    #endif

/**
* Включите stddef.h в компилятор Apple clang, чтобы обеспечить определение типа ptrdiff_t.
*/
#if defined(CORSAC_COMPILER_CLANG) && defined(CORSAC_PLATFORM_APPLE)
    #include <stddef.h>
#endif

/**
* По умолчанию GCC определяет NULL как ((void) 0), что является определением C.
* Это вызывает всевозможные проблемы для кода C++,
* поэтому можно обойтись путем отмены определения NULL.
*/
#if defined(NULL)
#undef NULL
#endif

/**
* Определите указатель NULL.
* Обычно это определяется в <stddef.h>, но мы не хотим вызывать глобальную зависимость
* от этого заголовка, поэтому определение здесь дублируется.
*/
#if defined(__cplusplus)
#define NULL 0
#else
#define NULL ((void*)0)
#endif

/**
* Включите заголовок stdint, чтобы определить и получить необходимые типы.
* Кроме того, включите inttypes.h, так как многие компиляторы, включая варианты GCC
* определяют вещи в inttypes.h, которые, согласно стандарту C99, входят в stdint.h.
*/
#if defined(CORSAC_COMPILER_MSVC)
    #include <inttypes.h> // PRId8, SCNd8, etc.
#endif

#include <stdint.h>   // int32_t, INT64_C, UINT8_MAX, etc.
#include <math.h>     // float_t, double_t, etc.
#include <float.h>    // FLT_EVAL_METHOD.`

/**
* Character types
*/
#if defined(CORSAC_COMPILER_MSVC)
    #if defined(CORSAC_WCHAR_T_NON_NATIVE)
	   // В этом случае wchar_t не определен, если мы не включаем
	   // wchar.h или если компилятор делает его встроенным.
	   #ifdef CORSAC_COMPILER_MSVC
		  #pragma warning(push, 3)
	   #endif
	   #include <wchar.h>
	   #ifdef CORSAC_COMPILER_MSVC
		  #pragma warning(pop)
	   #endif
	#endif
#endif


// ------------------------------------------------------------------------
// char8_t  -- Гарантированно совпадает с типом данных компилятора char.
//             Некоторые компиляторы реализуют char8_t как unsigned,
//             хотя char обычно устанавливается как подписанный.
//
// char16_t -- Это 16-битное значение без знака. Если компилятор
//             имеет wchar_t как 16-битное значение без знака, тогда char16_t
//             устанавливается как то же самое, что и wchar_t, чтобы пользователь мог
//             использовать char16_t со стандартными функциями wchar_t.
//
// char32_t -- Это устанавливается на 32-битное значение без знака.Если компилятор
// 			   имеет wchar_t в качестве 32-битного значения без знака, то char32_t
// 			   установить то же самое, что и wchar_t для того, чтобы разрешить
// 			   пользователь для использования char32_t со стандартными функциями wchar_t.
//
// CORSAC_CHAR8_UNIQUE
// CORSAC_CHAR16_NATIVE
// CORSAC_CHAR32_NATIVE
// CORSAC_WCHAR_UNIQUE
//
// VS2010 в одностороннем порядке определяет char16_t и char32_t в заголовке yvals.h
// Если _has_char16_t_language_support или _char16t определены.
// Однако VS2010 не поддерживает C++ 0x U "" и u "" строковые литералы,
// Что делает его определение CHAR16_T и CHAR32_T несколько бесполезно.
// До VC++ поддерживает строковые литералы, система сборки должна определять
// _char16t и пусть corsacbase определяет char16_t и corsac_char16.
//
// GCC определяет CHAR16_T и CHAR32_T в компиляторе C в режиме GNU99,
// как __char16_type__ и __char32_type__, а для компилятора C++
// в -std = C ++ 0x и -Std = GNU ++ 0x режимы, как CHAR16_T и CHAR32_T.
//
// Символ CORC_WCHAR_UNIQUE определяется на 1, если wchar_t отличается от
// char8_t, char16_t и char32_t и определено на 0, если нет. В некоторых случаях,
// Если компилятор не поддерживает CHAR16_T / CHAR32_T, один из этих двух типов
// Обычно это типичный файл или определяет whar_t. Для компиляторов, которые поддерживают
// Типы символов C ++ 11 Unicode, часто перегрузки должны быть предоставлены
// Поддержите существующий код, который передает широкую строку CHAR к функции, которая
// принимает строку Unicode.
//
// Символ CORCAC_CHAR8_UNIQUE определяется на 1, если CHAR8_T является отличным типом
// Из CHAR в системе типа и определены до 0, если иначе.

// CORSAC_CHAR16_NATIVE
#if !defined(CORSAC_CHAR16_NATIVE)
    #if defined(CORSAC_COMPILER_MSVC) && defined(_HAS_CHAR16_T_LANGUAGE_SUPPORT) && _HAS_CHAR16_T_LANGUAGE_SUPPORT
        #define CORSAC_CHAR16_NATIVE 1
    #elif defined(CORSAC_COMPILER_CLANG)
        #if __has_feature(cxx_unicode_literals)
            #define CORSAC_CHAR16_NATIVE 1
        #elif !(defined(CORSAC_PLATFORM_IPHONE) || defined(CORSAC_PLATFORM_OSX))
            #define CORSAC_CHAR16_NATIVE 1
        #elif defined(CORSAC_PLATFORM_APPLE)
            #define CORSAC_CHAR16_NATIVE 1
        #else
            #define CORSAC_CHAR16_NATIVE 0
        #endif
    #elif defined(CORSAC_COMPILER_GCC)
        #define CORSAC_CHAR16_NATIVE 1
    #else
        #define CORSAC_CHAR16_NATIVE 0
    #endif
#endif

// CORSAC_CHAR32_NATIVE
#if !defined(CORSAC_CHAR32_NATIVE)
    #if defined(CORSAC_COMPILER_MSVC)&& defined(_HAS_CHAR16_T_LANGUAGE_SUPPORT) && _HAS_CHAR16_T_LANGUAGE_SUPPORT
        #define CORSAC_CHAR32_NATIVE 1
    #elif defined(CORSAC_COMPILER_CLANG)
        #if __has_feature(cxx_unicode_literals)
            #define CORSAC_CHAR32_NATIVE 1
        #elif !(defined(CORSAC_PLATFORM_IPHONE) || defined(CORSAC_PLATFORM_OSX))
            #define CORSAC_CHAR32_NATIVE 1
        #elif defined(CORSAC_PLATFORM_APPLE)
            #define CORSAC_CHAR32_NATIVE 1
        #else
            #define CORSAC_CHAR32_NATIVE 0
        #endif
    #elif defined(CORSAC_COMPILER_GCC)
        #define CORSAC_CHAR32_NATIVE 1
    #else
        #define CORSAC_CHAR32_NATIVE 0
    #endif
#endif

// CORSAC_WCHAR_UNIQUE
#if CORSAC_CHAR16_NATIVE || CORSAC_CHAR32_NATIVE
    #define CORSAC_WCHAR_UNIQUE 1
#else
    #define CORSAC_WCHAR_UNIQUE 0
#endif

// CORSAC_CHAR8_UNIQUE
#ifdef __cpp_char8_t
    #define CHAR8_T_DEFINED
    #define CORSAC_CHAR8_UNIQUE 1
#else
    #define CORSAC_CHAR8_UNIQUE 0
#endif


#ifndef CHAR8_T_DEFINED
    #define CHAR8_T_DEFINED
    #if defined(CORSAC_PLATFORM_APPLE)
        #define char8_t char
    #else
        using char8_t = char;
    #endif

    #if CORSAC_CHAR16_NATIVE
        // В C ++ CHAR16_T и CHAR32_T уже определены компилятором.
        // в MS C, CHAR16_T и CHAR32_T уже определены компилятором / стандартной библиотекой.
        // в GCC C, __char16_Type__ и __char32_type__, и мы должны определить CHAR16_T и CHAR32_T из них.
        #if defined(CORSAC_COMPILER_GCC) && !defined(__GXX_EXPERIMENTAL_CXX0X__) && defined(__CHAR16_TYPE__)
			using char16_t = __CHAR16_TYPE__;
			using char32_t = __CHAR32_TYPE__;
        #endif
    #elif (CORSAC_WCHAR_SIZE == 2)
        // Если VS2010 + или используя платформы, которые используют Dinkumware под компилятором, который не поддерживает C++ 11 CHAR16_T.
        #if defined(CORSAC_COMPILER_MSVC)
			#if !defined(_CHAR16T)
				#define _CHAR16T
			#endif
			#if !defined(_HAS_CHAR16_T_LANGUAGE_SUPPORT) || !_HAS_CHAR16_T_LANGUAGE_SUPPORT
				using char16_t = wchar_t;
				using char32_t = uint32_t;
			#endif
		#else
			using char16_t = wchar_t;
			using char32_t = uint32_t;
		#endif
    #else
        using char16_t = uint16_t;
        #if defined(__cplusplus)
            using char32_t = wchar_t;
        #else
            using char32_t = uint32_t;
        #endif
    #endif
#endif

// CHAR8_MIN, CHAR8_MAX, etc.
#define CORSAC_LIMITS_DIGITS_S(T)  ((sizeof(T) * 8) - 1)
#define CORSAC_LIMITS_DIGITS_U(T)  ((sizeof(T) * 8))
#define CORSAC_LIMITS_IS_SIGNED(T) ((T)(-1) < 0)
#define CORSAC_LIMITS_DIGITS(T)    ((CORSAC_LIMITS_IS_SIGNED(T) ? CORSAC_LIMITS_DIGITS_S(T) : CORSAC_LIMITS_DIGITS_U(T)))
#define CORSAC_LIMITS_MIN_S(T)     ((T)((T)1 << CORSAC_LIMITS_DIGITS_S(T)))
#define CORSAC_LIMITS_MIN_U(T)     ((T)0)
#define CORSAC_LIMITS_MIN(T)       ((CORSAC_LIMITS_IS_SIGNED(T) ? CORSAC_LIMITS_MIN_S(T) : CORSAC_LIMITS_MIN_U(T)))
#define CORSAC_LIMITS_MAX_S(T)     ((T)(((((T)1 << (CORSAC_LIMITS_DIGITS(T) - 1)) - 1) << 1) + 1))
#define CORSAC_LIMITS_MAX_U(T)     ((T)~(T)0)
#define CORSAC_LIMITS_MAX(T)       ((CORSAC_LIMITS_IS_SIGNED(T) ? CORSAC_LIMITS_MAX_S(T) : CORSAC_LIMITS_MAX_U(T)))

#if !defined(CHAR8_MIN)
    #define CHAR8_MIN CORSAC_LIMITS_MIN(char8_t)
#endif

#if !defined(CHAR8_MAX)
    #define CHAR8_MAX CORSAC_LIMITS_MAX(char8_t)
#endif

#if !defined(CHAR16_MIN)
    #define CHAR16_MIN CORSAC_LIMITS_MIN(char16_t)
#endif

#if !defined(CHAR16_MAX)
    #define CHAR16_MAX CORSAC_LIMITS_MAX(char16_t)
#endif

#if !defined(CHAR32_MIN)
    #define CHAR32_MIN CORSAC_LIMITS_MIN(char32_t)
#endif

#if !defined(CHAR32_MAX)
    #define CHAR32_MAX CORSAC_LIMITS_MAX(char32_t)
#endif

/**
* CORSAC_CHAR8 / CORSAC_CHAR16 / CORSAC_CHAR32 / CORSAC_WCHAR
*
* Поддерживает использование портативных строковых констант.
*
* Пример использования:
*     const char16_t* str = CORSAC_CHAR16("Hello world");
*     const char32_t* str = CORSAC_CHAR32("Hello world");
*     const char16_t  c   = CORSAC_CHAR16('\x3001');
*     const char32_t  c   = CORSAC_CHAR32('\x3001');
*/
#ifndef CORSAC_CHAR8
    #if CORSAC_CHAR8_UNIQUE
        #define CORSAC_CHAR8(s) u8 ## s
    #else
        #define CORSAC_CHAR8(s) s
    #endif
#endif

#ifndef CORSAC_WCHAR
    #define CORSAC_WCHAR_(s) L ## s
    #define CORSAC_WCHAR(s)  CORSAC_WCHAR_(s)
#endif

#ifndef CORSAC_CHAR16
    #if CORSAC_CHAR16_NATIVE && !defined(CORSAC_COMPILER_MSVC) // Microsoft не поддерживает строковые литералы CHAR16_T.
        #define CORSAC_CHAR16_(s) u ## s
		#define CORSAC_CHAR16(s)  CORSAC_CHAR16_(s)
    #elif (CORSAC_WCHAR_SIZE == 2)
        #if defined(CORSAC_COMPILER_MSVC)
			#define CORSAC_CHAR16_(s) u ## s
			#define CORSAC_CHAR16(s)  CORSAC_CHAR16_(s)
		#else
			#define CORSAC_CHAR16_(s) L ## s
			#define CORSAC_CHAR16(s)  CORSAC_CHAR16_(s)
		#endif
    #else
        //#define CORSAC_CHAR16(s) // Невозможно эффективно реализовать.
    #endif
#endif

#ifndef CORSAC_CHAR32
    #if CORSAC_CHAR32_NATIVE && !defined(_MSC_VER) // Microsoft не поддерживает строковые литералы CHAR32_T.
        #define CORSAC_CHAR32_(s) U ## s
        #define CORSAC_CHAR32(s)  CORSAC_CHAR32_(s)
    #elif (CORSAC_WCHAR_SIZE == 2)
        #if defined(_MSC_VER)
            #define CORSAC_CHAR32_(s) U ## s
            #define CORSAC_CHAR32(s)  CORSAC_CHAR32_(s)
        #else
            //#define CORSAC_CHAR32(s) // Невозможно реализовать.
        #endif
    #elif (CORSAC_WCHAR_SIZE == 4)
        #define CORSAC_CHAR32_(s) L ## s
        #define CORSAC_CHAR32(s)  CORSAC_CHAR32_(s)
    #else
        #error Unexpected size of wchar_t
    #endif
#endif

#endif //CORSAC_BASE_H
