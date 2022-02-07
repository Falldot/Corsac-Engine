/**
 * corsac::STL
 *
 * config.h
 *
 * Created by Falldot on 26.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STL_CONFIG_H
#define CORSAC_STL_CONFIG_H

#pragma once

#include "Corsac/Base/base.h"
#include "Corsac/assert.h"

/**
* CORSAC_DEBUG
*
* Определяется как целое число >= 0. По умолчанию 1 для отладочных сборок и 0 для
* релизных сборок.  Это определение также является главным переключателем для
* значения по умолчанию для некоторых других настроек.
*
* Пример использования:
*    #if CORSAC_DEBUG
*       ...
*    #endif
*
*/
#if defined(CORSAC_DEBUG) || defined(_DEBUG) || defined(DEBUG)
    #define CORSAC_DEBUG 1
#else
    #define CORSAC_DEBUG 0
#endif

/**
 * CORSAC_DLL, CORSAC_API, CORSAC_LOCAL
 *
 * Определяем, каким образом распространяеться библиотека
 * в случаи как динамическая библиотека, потребуется указывать
 * экспортируемым функциям специальную директиву.
 */
#ifndef CORSAC_DLL
    #if defined(CORSAC_DLL)
        #define CORSAC_DLL 1
    #else
        #define CORSAC_DLL 0
    #endif
#endif

#ifndef CORSAC_API
    #if CORSAC_DLL
        #if defined(_MSC_VER)
			#define CORSAC_API      __declspec(dllimport)
			#define CORSAC_LOCAL
		#elif defined(__CYGWIN__)
			#define CORSAC_API      __attribute__((dllimport))
			#define CORSAC_LOCAL
		#elif (defined(__GNUC__))
			#define CORSAC_API      __attribute__ ((visibility("default")))
			#define CORSAC_LOCAL    __attribute__ ((visibility("hidden")))
		#else
			#define CORSAC_API
			#define CORSAC_LOCAL
		#endif
    #else
        #define CORSAC_API
        #define CORSAC_LOCAL
    #endif
#endif

/**
 * CORSAC_EXCEPTIONS_ENABLED
 *
 * Определяется как 0 или 1. По умолчанию соответствует настройкам компилятора.
 * Пользователь может предопределить CORSAC_EXCEPTIONS_ENABLED равным 0 или 1;
 * однако, если компилятор настроен на отключение исключений, то
 * CORSAC_EXCEPTIONS_ENABLED принудительно принимает значение 0
 * независимо от предопределенного пользователем.
 *
 * Обратите внимание, что мы не включаем исключения CORSAC по умолчанию, если
 * в компиляторе включены исключения. Чтобы включить
 * CORSAC_EXCEPTIONS_ENABLED, вам необходимо вручную установить его на 1.
 */
#if !defined(CORSAC_EXCEPTIONS_ENABLED) || ((CORSAC_EXCEPTIONS_ENABLED == 1) && defined(CORSAC_COMPILER_NO_EXCEPTIONS))
    #define CORSAC_EXCEPTIONS_ENABLED 0
#endif

/**
 * CORSAC_ALLOCATOR_TYPE
 *
 * тип аллоктора по умолчанию
 */
#ifndef CORSAC_ALLOCATOR_TYPE
    #define CORSAC_ALLOCATOR_TYPE corsac::allocator
#endif

/**
* CORSAC_MINMAX_ENABLED
*
* Defined as 0 or 1; default is 1.
* Указывает, доступны ли алгоритмы минимума и максимума.
* Может быть полезно отключить алгоритмы минимума и максимума, потому что иногда
* существуют определения минимума и максимума, которые могут конфликтовать с
* минимальным и максимальным CORSAC. Обратите внимание, что в CORSAC уже есть альтернативные версии
* min и max с функциями min_alt и max_alt.  Вы можете использовать их, не сталкиваясь с макросами minmax,
* которые могут существовать.
*/
#ifndef CORSAC_MINMAX_ENABLED
    #define CORSAC_MINMAX_ENABLED 1
#endif

/**
* CORSAC_DEBUG_PARAMS_LEVEL
*
* CORSAC_DEBUG_PARAMS_LEVEL по умолчанию контролирует, какая отладочная информация
* передается распределителю. Это значение может быть определено пользователем ...
* в противном случае оно будет по умолчанию равно 1 для сборок CORSAC_DEBUG,
* в противном случае - 0.
*
*  0 - никакая отладочная информация не передается в вызовы распределителя.
*  1 - 'name' передается вызовам распределителя.
*  2 - name ', __FILE__ и __LINE__ передаются вызовам распределителя.
*
* Этот параметр отражает эквивалентный параметр в пакете CoreAllocator.
*/
#ifndef CORSAC_DEBUG_PARAMS_LEVEL
    #if CORSAC_DEBUG
        #define CORSAC_DEBUG_PARAMS_LEVEL 2
    #else
        #define CORSAC_DEBUG_PARAMS_LEVEL 0
    #endif
#endif


/**
* CORSAC_NAME_ENABLED / CORSAC_NAME / CORSAC_NAME_VAL
*
* Используется для обертывания имен строк отладки. В сборке выпуска определение исчезает.
* Они присутствуют, чтобы избежать предупреждений компилятора сборки выпуска и упростить код.
*
* Пример использования CORSAC_NAME_ENABLED:
*    // pName будет определено в сборке выпуска и, таким образом, предотвратит предупреждения компилятора.
*    void allocator::set_name(const char* CORSAC_NAME(pName))
*    {
*        #if CORSAC_NAME_ENABLED
*            mpName = pName;
*        #endif
*    }
*
* Пример использования CORSAC_NAME_VAL:
*    // xxx имеет значение NULL в релизной сборке.
*    vector<T, Allocator>::vector(const allocator_type& allocator = allocator_type(CORSAC_NAME_VAL("xxx")));
*/
#ifndef CORSAC_NAME_ENABLED
    #define CORSAC_NAME_ENABLED CORSAC_DEBUG
#endif

#ifndef CORSAC_NAME
    #if CORSAC_NAME_ENABLED
        #define CORSAC_NAME(x)      x
        #define CORSAC_NAME_VAL(x)  x
    #else
        #define CORSAC_NAME(x)
        #define CORSAC_NAME_VAL(x) ((const char*)NULL)
    #endif
#endif

#ifndef CORSAC_DEFAULT_ALLOCATOR_TYPE
    #define CORSAC_DEFAULT_ALLOCATOR_TYPE corsac::allocator
#endif

#ifndef CORSAC_DEFAULT_ALLOCATOR
    // CORSAC_DEFAULT_ALLOCATOR возвращает экземпляр распределителя по умолчанию.
    #define CORSAC_DEFAULT_ALLOCATOR corsac::GetDefaultAllocator
#endif

/**
* CORSAC_ALLOCATOR_MIN_ALIGNMENT
*
* Определяется как интегральная степень двойки, которая >= 1.
* Определяет минимальное выравнивание, которое CORSAC должен использовать в своих аллокаторов.
* В CORSAC есть код, который решает, выполнять ли вызов Malloc или MallocAligned,
* и обычно лучше, если он может использовать вызов Malloc. Но для этого
* необходимо знать, каково минимально возможное выравнивание.
*/
#if !defined(CORSAC_ALLOCATOR_MIN_ALIGNMENT)
    #define CORSAC_ALLOCATOR_MIN_ALIGNMENT CORSAC_PLATFORM_MIN_MALLOC_ALIGNMENT
#endif

#ifndef CORSAC_ALLOC // На заметку: вместо вызова через pAllocator просто перейдите непосредственно к оператору new, поскольку это то, что делает распределитель.
    #define CORSAC_ALLOC(allocator, n) (allocator).allocate(n)
#endif

#ifndef CORSAC_Free
    #define CORSAC_Free(allocator, p, size) (allocator).deallocate(static_cast<void*>(p), (size)) // важно преобразовать в void, поскольку p может быть неконстантным.
#endif

#ifndef CORSAC_ALLOC_ALIGNED
    #define CORSAC_ALLOC_ALIGNED(allocator, n, alignment, offset) (allocator).allocate((n), (alignment), (offset))
#endif

/**
 * CORSAC_DEFAULT_NAME_PREFIX
 *
 * Определен как строковый литерал. По умолчанию "CORSAC".
 * Это определение используется в качестве имени по умолчанию для CORSAC
 * если на это есть ссылка в CORSAC. Например, если пользователь не указывает имя аллокатора
 * для своей двухсторонней очереди, она называется «CORSAC deque». Однако вы можете переопределить,
 * чтобы сказать «SuperBaseball deque», изменив CORSAC_DEFAULT_NAME_PREFIX.
 *
 * Пример использования (который просто взят из того, как deque.h использует это определение):
 *     #ifndef CORSAC_DEQUE_DEFAULT_NAME
 *         #define CORSAC_DEQUE_DEFAULT_NAME CORSAC_DEFAULT_NAME_PREFIX " deque"
 *     #endif
 */
#ifndef CORSAC_DEFAULT_NAME_PREFIX
    #define CORSAC_DEFAULT_NAME_PREFIX "CORSAC"
#endif

/**
 * CORSAC_DEFAULT_ALLOCATOR_NAME
 *
 * Определяет имя распределителя по умолчанию при отсутствии имени, предоставленного пользователем.
 */
#ifndef CORSAC_DEFAULT_ALLOCATOR_NAME
    #define CORSAC_DEFAULT_ALLOCATOR_NAME CORSAC_DEFAULT_NAME_PREFIX
#endif

/**
* CORSAC_INT128_SUPPORTED
*
* Определяет поддержку int128
*/
#ifndef CORSAC_INT128_SUPPORTED
    #if defined(__SIZEOF_INT128__) || (defined(CORSAC_COMPILER_INTMAX_SIZE) && (CORSAC_COMPILER_INTMAX_SIZE >= 16))
        #define CORSAC_INT128_SUPPORTED 1
    #else
        #define CORSAC_INT128_SUPPORTED 0
    #endif
#endif

#ifndef CORSAC_MAX_STACK_USAGE
    #define CORSAC_MAX_STACK_USAGE 4000
#endif

/**
* CORSAC_MAY_ALIAS
*
* Определен как макрос, который является оболочкой для атрибута GCC may_alias.
* Этот атрибут не имеет значения для VC ++, поскольку VC ++ не поддерживает концепцию строгого псевдонима.
* Пользователи должны избегать написания кода, нарушающего строгие правила псевдонимов;
* CORSAC_MAY_ALIAS - для случаев, у которых нет альтернативы.
*/
#if defined(CORSAC_COMPILER_GCC) && !defined(CORSAC_COMPILER_RVCT)
    #define CORSAC_MAY_ALIAS __attribute__((__may_alias__))
#else
    #define CORSAC_MAY_ALIAS
#endif

/**
 *  char8_t
 *
 *  До стандарта С++ 20 char8_t не имел оффициальну поддержку
 *  объявляем его если компилятор не имеет свою реализацию.
 */
// TODO: В С++ 20 char8_t добавлен в оффициальную поддержу
#ifdef __cpp_char8_t
    // Не чего не делаем ...
#else
    #if defined(CORSAC_PLATFORM_APPLE)
        #define char8_t char // TODO: Проверить! Отладчик Apple не знает, что char8_t определяется типом как char, поэтому мы определяем его.
    #else
        #ifdef CORSAC_COMPILER_MSVC
            #pragma warning(disable: 5052)
        #endif
        using char8_t = char;
    #endif

    /**
    * char16_t, char32_t
    *
    * В C++ char16_t и char32_t уже определены компилятором.
    * В MS C char16_t и char32_t уже определены стандартной библиотекой компилятора.
    * в GCC C, мы должны определить char16_t и char32_t из специальных макросов.
    */
    #if defined(CORSAC_COMPILER_GCC) && !defined(__GXX_EXPERIMENTAL_CXX0X__) && defined(__CHAR16_TYPE__)
        using char16_t = __CHAR16_TYPE__;
        using char32_t = __CHAR32_TYPE__;
    #elif (CORSAC_WCHAR_SIZE == 2)
        #if defined(CORSAC_COMPILER_MSVC)
            #if !defined(_CHAR16T)
                #define _CHAR16T
            #endif
            #if !defined(_HAS_CHAR16_T_LANGUAGE_SUPPORT) || !_HAS_CHAR16_T_LANGUAGE_SUPPORT
                using char16_t = wchar_t;
                using char32_t = uint32_t;
             #endif
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
    #define CORSAC_CHAR8_(s) u8 ## s
    #define CORSAC_CHAR8(s) s
#endif

#ifndef CORSAC_WCHAR
    #define CORSAC_WCHAR_(s) L ## s
    #define CORSAC_WCHAR(s)  CORSAC_WCHAR_(s)
#endif

#ifndef CORSAC_CHAR16
    #if !defined(CORSAC_COMPILER_MSVC) // Microsoft не поддерживает строковые литералы CHAR16_T.
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
    #if !defined(CORSAC_COMPILER_MSVC) // Microsoft не поддерживает строковые литералы CHAR32_T.
        #define CORSAC_CHAR32_(s) U ## s
        #define CORSAC_CHAR32(s)  CORSAC_CHAR32_(s)
    #elif (CORSAC_WCHAR_SIZE == 2)
        #if defined(CORSAC_COMPILER_MSVC)
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

// CORSAC_NOMINMAX
#ifndef CORSAC_NOMINMAX
    #define CORSAC_NOMINMAX 1
#endif

/**
* CORSAC_FIXED_SIZE_TRACKING_ENABLED
*
* Определяется как целое число >= 0. По умолчанию такое же, как CORSAC_DEBUG
* Если CORSAC_FIXED_SIZE_TRACKING_ENABLED включен, то фиксированные контейнеры в отладочных
* сборках отслеживают максимальное количество объектов, которые были в контейнере.
* Это позволяет настраивать фиксированные размеры контейнеров до их минимально необходимого размера.
*/
#ifndef CORSAC_FIXED_SIZE_TRACKING_ENABLED
    #define CORSAC_FIXED_SIZE_TRACKING_ENABLED CORSAC_DEBUG
#endif

#define CORSAC_STATIC_ASSERT(expression) static_assert(expression, #expression)

/**
* CORSAC_RTTI_ENABLED
*
* Определяется как 0 или 1. По умолчанию - 1, если RTTI поддерживается компилятором
* Это определение существует для того, чтобы мы могли использовать в коде некоторые операции
* dynamic_cast без предупреждения. dynamic_cast используется только в том случае, если он
* конкретно относится к нему;
*
* Пример использования:
*     #if CORSAC_RTTI_ENABLED
*         pChildClass = dynamic_cast<ChildClass*>(pParentClass);
*     #endif
*/
#ifndef CORSAC_RTTI_ENABLED
    #if defined(CORSAC_COMPILER_NO_RTTI) || (defined(CORSAC_COMPILER_MSVC) && defined(CORSAC_LIBRARY_DINKUMWARE) && !(defined(_HAS_EXCEPTIONS) && _HAS_EXCEPTIONS))
        #define CORSAC_RTTI_ENABLED 0
    #else
        #define CORSAC_RTTI_ENABLED 1
    #endif
#endif

#endif //CORSAC_STL_CONFIG_H
