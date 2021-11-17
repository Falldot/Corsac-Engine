//
// Created by surve on 03.11.2021.
//

#ifndef CORSAC_ENGINE_CONFIG_H
#define CORSAC_ENGINE_CONFIG_H

#include "base/base.h"

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
#if defined(CORSAC_DEBUG) || defined(_DEBUG)
    #define CORSAC_DEBUG 1
#else
    #define CORSAC_DEBUG 0
#endif

// Отладка разработчика. Помогает разработчикам corsac утверждать, что corsac написан правильно.
// Обычно отключен для пользователей, поскольку он проверяет внутренние вещи, а не пользовательские.
#ifndef CORSAC_DEV_DEBUG
    #define CORSAC_DEV_DEBUG 0
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
* CORSAC_ALLOCATOR_DEFAULT_NAME
*
* Определяет имя распределителя по умолчанию при отсутствии имени, предоставленного пользователем.
*/
#ifndef CORSAC_ALLOCATOR_DEFAULT_NAME
    #define CORSAC_ALLOCATOR_DEFAULT_NAME CORSAC_DEFAULT_NAME_PREFIX
#endif

///////////////////////////////////////////////////////////////////////////////
// CORSAC_ASSERT_MSG
//
// Example usage:
//    CORSAC_ASSERT_MSG(false, "detected error condition!");
//
///////////////////////////////////////////////////////////////////////////////
#ifndef CORSAC_ASSERT_MSG
    #if CORSAC_ASSERT_ENABLED
        #define CORSAC_ASSERT_MSG(expression, message) \
                CORSAC_DISABLE_VC_WARNING(4127) \
                do { \
                    CORSAC_ANALYSIS_ASSUME(expression); \
                    (void)((expression) || (corsac::AssertionFailure(message), 0)); \
                } while (0) \
                CORSAC_RESTORE_VC_WARNING()
    #else
        #define CORSAC_ASSERT_MSG(expression, message)
    #endif
#endif

///////////////////////////////////////////////////////////////////////////////
// CORSAC_ASSERT
//
// Assertion macro. Can be overridden by user with a different value.
//
// Example usage:
// CORSAC_ASSERT(intVector.size() < 100);
//
///////////////////////////////////////////////////////////////////////////////

#ifndef CORSAC_ASSERT
    #if CORSAC_ASSERT_ENABLED
        #define CORSAC_ASSERT(expression) CORSAC_ANALYSIS_ASSUME(expression)
    #else
        #define CORSAC_ASSERT(expression)
    #endif
#endif

/**
* CORSAC_CT_ASSERT
*
* CORSAC_CT_ASSERT это макрос для проверки утверждений во время компиляции,
* полезный для проверки постоянных выражений. Преимущество перед использованием CORSAC_ASSERT
* заключается в том, что ошибки обнаруживаются во время компиляции, а не во время выполнения.
*
* Example usage:
*     CORSAC_CT_ASSERT(sizeof(uint32_t) == 4);
*/

#define CORSAC_CT_ASSERT(expression) static_assert(expression, #expression)

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

#ifndef CORSACAllocatorType
    #define CORSACAllocatorType corsac::allocator
#endif

///////////////////////////////////////////////////////////////////////////////
// CORSAC_RTTI_ENABLED
//
// Defined as 0 or 1. Default is 1 if RTTI is supported by the compiler.
// This define exists so that we can use some dynamic_cast operations in the
// code without warning. dynamic_cast is only used if the specifically refers
// to it; CORSAC won't do dynamic_cast behind your back.
//
// Example usage:
//     #if CORSAC_RTTI_ENABLED
//         pChildClass = dynamic_cast<ChildClass*>(pParentClass);
//     #endif
//
///////////////////////////////////////////////////////////////////////////////

#ifndef CORSAC_RTTI_ENABLED
    #if defined(CORSAC_COMPILER_NO_RTTI) || (defined(CORSAC_COMPILER_MSVC) && defined(CORSAC_LIBRARY_DINKUMWARE) && !(defined(_HAS_EXCEPTIONS) && _HAS_EXCEPTIONS))
        #define CORSAC_RTTI_ENABLED 0
    #else
        #define CORSAC_RTTI_ENABLED 1
    #endif
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

#ifndef CORSAC_Alloc // На заметку: вместо вызова через pAllocator просто перейдите непосредственно к оператору new, поскольку это то, что делает распределитель.
    #define CORSAC_Alloc(allocator, n) (allocator).allocate(n)
#endif

#ifndef CORSAC_AllocAligned
    #define CORSAC_AllocAligned(allocator, n, alignment, offset) (allocator).allocate((n), (alignment), (offset))
#endif


#endif //CORSAC_ENGINE_CONFIG_H
