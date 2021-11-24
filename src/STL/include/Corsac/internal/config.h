//
// Created by surve on 03.11.2021.
//

#ifndef CORSAC_ENGINE_CONFIG_H
#define CORSAC_ENGINE_CONFIG_H

#include "Corsac/internal/base/base.h"

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

/**
 * CORSAC_DEV_DEBUG
 *
 * Отладка разработчика. Помогает разработчикам corsac утверждать, что библеотека написан правильно.
 * Обычно отключен для пользователей, поскольку он проверяет внутренние вещи, а не пользовательские.
 */
#ifndef CORSAC_DEV_DEBUG
    #define CORSAC_DEV_DEBUG 0
#endif

/**
* CORSAC_DEBUGPARAMS_LEVEL
*
* CORSAC_DEBUGPARAMS_LEVEL по умолчанию контролирует, какая отладочная информация
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
#ifndef CORSAC_DEBUGPARAMS_LEVEL
    #if CORSAC_DEBUG
        #define CORSAC_DEBUGPARAMS_LEVEL 2
    #else
        #define CORSAC_DEBUGPARAMS_LEVEL 0
    #endif
#endif

/**
* CORSAC_DLL
*
* Определяется как 0 или 1. The default is dependent on the definition of CORSAC_DLL.
* CORSAC_DLL определяет, создается ли CORSAC и используется ли он в качестве библиотеки DLL.
* Обычно вы бы этого не делали, но для этого есть варианты использования,
* особенно в случае встраивания C ++ в приложения C.
*/
#if defined(CORSAC_DLL)
    #define CORSAC_DLL 1
#else
    #define CORSAC_DLL 0
#endif

/**
* CORSAC_API
*
* Это используется для обозначения функций как экспорта DLL на платформах Microsoft.
* Функции шаблона CORSAC не помечены как CORSAC_API (и поэтому не экспортируются в сборку DLL).
* Это потому, что невозможно (или, по крайней мере, небезопасно) реализовать встроенные шаблонные функции в DLL.
*
* Пример использования CORSAC_API:
*    CORSAC_API int someVariable = 10;      // Экспортируйте someVariable в сборку DLL.
*
*    struct CORSAC_API SomeClass // Экспортируйте SomeClass и его функции-члены в сборку DLL.
 *   {
*        CORSAC_LOCAL void PrivateMethod(); // Не экспортируется.
*    };
*
*    CORSAC_API void SomeFunction();        // Экспорт SomeFunction в сборку DLL.
*/
#ifndef CORSAC_API // Если файл сборки еще не определил это как dllexport ...
    #if CORSAC_DLL
        #if defined(_MSC_VER)
                #define CORSAC_API      __declspec(dllimport)
                #define CORSAC_LOCAL
        #elif defined(__CYGWIN__)
            #define CORSAC_API      __attribute__((dllimport))
            #define CORSAC_LOCAL
        #elif (defined(__GNUC__) && (__GNUC__ >= 4))
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

///////////////////////////////////////////////////////////////////////////////
// CORSAC_ALLOCATOR_COPY_ENABLED
//
// Defined as 0 or 1. Default is 0 (disabled) until some future date.
// If enabled (1) then container operator= copies the allocator from the
// source container. It ideally should be set to enabled but for backwards
// compatibility with older versions of CORSAC it is currently set to 0.
// Regardless of whether this value is 0 or 1, this container copy constructs
// or copy assigns allocators.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef CORSAC_ALLOCATOR_COPY_ENABLED
    #define CORSAC_ALLOCATOR_COPY_ENABLED 0
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
// CORSAC_ASSERT_ENABLED
//
// Defined as 0 or non-zero. Default is same as CORSAC_DEBUG.
// If CORSAC_ASSERT_ENABLED is non-zero, then asserts will be executed via
// the assertion mechanism.
//
// Example usage:
//     #if CORSAC_ASSERT_ENABLED
//         CORSAC_ASSERT(v.size() > 17);
//     #endif
//
///////////////////////////////////////////////////////////////////////////////

#ifndef CORSAC_ASSERT_ENABLED
    #define CORSAC_ASSERT_ENABLED CORSAC_DEBUG
#endif

// Developer assert. Helps CORSAC developers assert CORSAC is coded correctly.
// Normally disabled for users since it validates internal things and not user things.
#ifndef CORSAC_DEV_ASSERT_ENABLED
    #define CORSAC_DEV_ASSERT_ENABLED CORSAC_DEV_DEBUG
#endif

///////////////////////////////////////////////////////////////////////////////
// CORSAC_ASSERT_ENABLED
//
// Defined as 0 or non-zero. Default is same as CORSAC_DEBUG.
// If CORSAC_ASSERT_ENABLED is non-zero, then asserts will be executed via
// the assertion mechanism.
//
// Example usage:
//     #if CORSAC_ASSERT_ENABLED
//         CORSAC_ASSERT(v.size() > 17);
//     #endif
//
///////////////////////////////////////////////////////////////////////////////

#ifndef CORSAC_ASSERT_ENABLED
    #define CORSAC_ASSERT_ENABLED CORSAC_DEBUG
#endif

// Developer assert. Helps CORSAC developers assert CORSAC is coded correctly.
// Normally disabled for users since it validates internal things and not user things.
#ifndef CORSAC_DEV_ASSERT_ENABLED
    #define CORSAC_DEV_ASSERT_ENABLED CORSAC_DEV_DEBUG
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
                do { \
                    CORSAC_ANALYSIS_ASSUME(expression); \
                    (void)((expression) || (corsac::AssertionFailure(message), 0)); \
                } while (0)
    #else
        #define CORSAC_ASSERT_MSG(expression, message)
    #endif
#endif

///////////////////////////////////////////////////////////////////////////////
// CORSAC_FAIL_MSG
//
// Failure macro. Can be overridden by user with a different value.
//
// Example usage:
//    CORSAC_FAIL("detected error condition!");
//
///////////////////////////////////////////////////////////////////////////////

#ifndef CORSAC_FAIL_MSG
    #if CORSAC_ASSERT_ENABLED
        #define CORSAC_FAIL_MSG(message) (corsac::AssertionFailure(message))
    #else
        #define CORSAC_FAIL_MSG(message)
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

///////////////////////////////////////////////////////////////////////////////
// CORSAC_EMPTY_REFERENCE_ASSERT_ENABLED
//
// Defined as 0 or non-zero. Default is same as CORSAC_ASSERT_ENABLED.
// This is like CORSAC_ASSERT_ENABLED, except it is for empty container
// references. Sometime people like to be able to take a reference to
// the front of the container, but not use it if the container is empty.
// In practice it's often easier and more efficient to do this than to write
// extra code to check if the container is empty.
//
// NOTE: If this is enabled, CORSAC_ASSERT_ENABLED must also be enabled
//
// Example usage:
//     template <typename T, typename Allocator>
//     inline typename vector<T, Allocator>::reference
//     vector<T, Allocator>::front()
//     {
//         #if CORSAC_ASSERT_ENABLED
//             CORSAC_ASSERT(mpEnd > mpBegin);
//         #endif
//
//         return *mpBegin;
//     }
//
///////////////////////////////////////////////////////////////////////////////

#ifndef CORSAC_EMPTY_REFERENCE_ASSERT_ENABLED
    #define CORSAC_EMPTY_REFERENCE_ASSERT_ENABLED CORSAC_ASSERT_ENABLED
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

#ifndef CORSAC_ALLOCATOR_TYPE
    #define CORSAC_ALLOCATOR_TYPE corsac::allocator
#endif

/**
* CORSAC_STRING_OPT_XXXX
*
* Включает некоторые параметры оптимизации, из-за которых поведение класса строки немного
* отличается от стандартного C ++ basic_string. Это варианты, с помощью которых вы можете
* улучшить производительность, избегая операций, которые на практике могут никогда не произойти для вас.
*/
#ifndef CORSAC_STRING_OPT_EXPLICIT_CTORS
    // Определяется как 0 или 1. По умолчанию - 0.
    // Определяет, должны ли мы реализовывать явность в конструкторах, где стандартная строка C++ этого не делает.
    // Преимущество включения явных конструкторов в том, что вы можете это сделать: string s = "hello";
    // в дополнение к строке s("string"); Недостатком включения явных конструкторов является то,
    // что могут выполняться бесшумные преобразования, которые снижают производительность,
    // если пользователь не обращает внимания. Стандартные строковые символы C ++ не являются явными.
    #define CORSAC_STRING_OPT_EXPLICIT_CTORS 0
#endif

#ifndef CORSAC_STRING_OPT_LENGTH_ERRORS
    // Определяется как 0 или 1. По умолчанию равно CORSAC_EXCEPTIONS_ENABLED.
    // Определяет, проверяем ли мы строковые значения, выходящие за пределы kMaxSize
    // (очень большое значение), и запускаем ли выполнение в этом случае.
    // Ожидается, что стандартные строки C++ будут выполнять такие проверки.
    #define CORSAC_STRING_OPT_LENGTH_ERRORS CORSAC_EXCEPTIONS_ENABLED
#endif

#ifndef CORSAC_STRING_OPT_RANGE_ERRORS
    // Определяется как 0 или 1. По умолчанию равно CORSAC_EXCEPTIONS_ENABLED.
    // Определяет, проверяем ли мы ссылки, выходящие за пределы,
    // на позиции строки и генерируем ли исключения. Правильно настроенный код не должен
    // отражать позиции, выходящие за пределы, и поэтому не должен нуждаться в этих проверках.
    // Ожидается, что стандартные строки C++ будут выполнять такие проверки диапазона.
    #define CORSAC_STRING_OPT_RANGE_ERRORS CORSAC_EXCEPTIONS_ENABLED
#endif

#ifndef CORSAC_STRING_OPT_ARGUMENT_ERRORS
    // Определяется как 0 или 1. По умолчанию - 0
    // Определяет, проверяем ли мы аргументы NULL ptr, переданные строковым функциям пользователем,
    // и генерируем исключения, если это так. Правильно настроенный код не должен передавать неверные аргументы
    // и поэтому не нуждается в этих проверках. Кроме того, некоторые пользователи считают,
    // что строки должны проверять указатели NULL во всех своих аргументах и ​​не выполнять никаких операций,
    // если это так. Это очень спорно. Стандартные строки C++ не требуются для проверки таких ошибок аргументов.
    #define CORSAC_STRING_OPT_ARGUMENT_ERRORS 0
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
* CORSAC_OPERATOR_EQUALS_OTHER_ENABLED
*
* Определяется как 0 или 1. По умолчанию 0 до тех пор, пока не будет считаться безопасным.
* Когда включено, включает оператор = для других типов символов, например для такого кода:
*     corsac::string8  s8;
*     corsac::string16 s16;
*     s8 = s16;
* Этот вариант считается экспериментальным
 * и может существовать неопределенное время.
*/
#if !defined(CORSAC_OPERATOR_EQUALS_OTHER_ENABLED)
    #define CORSAC_OPERATOR_EQUALS_OTHER_ENABLED 0
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

#ifndef CORSAC_Free
    #define CORSAC_Free(allocator, p, size) (allocator).deallocate((void*)(p), (size)) // важно преобразовать в void, поскольку p может быть неконстантным.
#endif

#ifndef CORSAC_AllocAligned
    #define CORSAC_AllocAligned(allocator, n, alignment, offset) (allocator).allocate((n), (alignment), (offset))
#endif

#ifndef CORSAC_AllocatorDefault
    // CORSAC_AllocatorDefault возвращает экземпляр распределителя по умолчанию.
    #define CORSAC_AllocatorDefault corsac::GetDefaultAllocator
#endif

/// CORSAC_USER_LITERALS_ENABLED
#ifndef CORSAC_USER_LITERALS_ENABLED
    #define CORSAC_USER_LITERALS_ENABLED 1
#endif

/**
* corsac_size_t
*
* Определяется как беззнаковый целочисленный тип, обычно size_t или uint32_t.
* По умолчанию size_t соответствует std STL, если пользователь не указывает явно использовать
* uint32_t через определение CORSAC_SIZE_T_32BIT
*
* Пример использования:
*     corsac_size_t n = intVector.size();
*/
#ifndef CORSAC_SIZE_T_32BIT         // Определяет, использует ли CORSAC_SIZE_T uint32_t/int32_t, а не size_t/ssize_t.
    #define CORSAC_SIZE_T_32BIT 0   // Это имеет значение на 64-битных платформах, потому что они используют 64-битный size_t.
#endif                              // По умолчанию мы делаем то же, что и std STL, и используем size_t.

#ifndef CORSAC_SIZE_T
    #if (CORSAC_SIZE_T_32BIT == 0) || (CORSAC_PLATFORM_WORD_SIZE == 4)
        #include <stddef.h>
        #define CORSAC_SIZE_T  size_t
        #define CORSAC_SSIZE_T intptr_t
    #else
        #define CORSAC_SIZE_T  uint32_t
        #define CORSAC_SSIZE_T int32_t
    #endif
#endif

using corsac_size_t = CORSAC_SIZE_T;  // То же, что и у std::size_t.
using corsac_ssize_t = CORSAC_SSIZE_T; // Концепция аналогична Posix ssize_t.


#endif //CORSAC_ENGINE_CONFIG_H
