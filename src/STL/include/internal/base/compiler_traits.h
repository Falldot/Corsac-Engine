/**
 * corsac::STL
 *
 * internall/base/compiler_traits.h
 *
 * Created by Falldot on 08.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */

#ifndef CORSAC_COMPILER_TRAITS_H
#define CORSAC_COMPILER_TRAITS_H

#pragma once

#include "platform.h"
#include "compiler.h"

/**
* CORSAC_LIKELY / CORSAC_UNLIKELY
*
* Определяется как макрос, который дает компилятору подсказку для предсказания ветвления.
* GCC дает вам возможность вручную указать компилятору результат сравнения,
* хотя часто лучше скомпилировать код доставки с обратной связью по профилированию как в GCC
* (-fprofile-arcs), так и в VC++ (LTCG: PGO и т.д.) . Однако бывают случаи, когда вы очень
* уверены, что логическое выражение обычно оценивается как истинное или ложное и может
* помочь компилятору с помощью директивы явной ясности.
*
* Пример использования:
*    if(EA_LIKELY(a == 0)) // Сообщите компилятору, что a обычно равно 0.
*       { ... }
*
* Example usage:
*    if(EA_UNLIKELY(a == 0)) // Сообщите компилятору, что a обычно не равно 0.
*       { ... }
*/
#ifndef CORSAC_LIKELY
    #if (defined(CORSAC_COMPILER_CLANG) || defined(CORSAC_COMPILER_GCC))
        #if defined(__cplusplus)
            #define CORSAC_LIKELY(x)   __builtin_expect(!!(x), true)
            #define CORSAC_UNLIKELY(x) __builtin_expect(!!(x), false)
        #else
            #define CORSAC_LIKELY(x)   __builtin_expect(!!(x), 1)
            #define CORSAC_UNLIKELY(x) __builtin_expect(!!(x), 0)
        #endif
    #else
        #define CORSAC_LIKELY(x)   (x)
        #define CORSAC_UNLIKELY(x) (x)
    #endif
#endif

/**
* CORSAC_ANALYSIS_ASSUME
*
* Это действует так же, как директива VC++ __analysis_assume, и реализована просто как
* оболочка вокруг нее, чтобы ее можно было переносить и использовать ее преимущества, если
* и когда она появляется в других компиляторах.
*
* Пример использования:
*    char Function(char* p) {
*       CORSAC_ANALYSIS_ASSUME(p != NULL);
*       return *p;
*    }
*/
#ifndef CORSAC_ANALYSIS_ASSUME
    #if defined(_MSC_VER)
        #define CORSAC_ANALYSIS_ASSUME(x) __analysis_assume(!!(x))
    #else
        #define CORSAC_ANALYSIS_ASSUME(x)
    #endif
#endif

/**
* CORSAC_HAS_INCLUDE_AVAILABLE
*
* Используется для защиты от макроса CORSAC_HAS_INCLUDE() на компиляторах,
* которые не поддерживают указанную функцию.
*
* Пример использования:
*
* #if CORSAC_HAS_INCLUDE_AVAILABLE
*     #if CORSAC_HAS_INCLUDE("myinclude.h")
*         #include "myinclude.h"
*     #endif
* #endif
*/
#if !defined(CORSAC_HAS_INCLUDE_AVAILABLE)
    #define CORSAC_HAS_INCLUDE_AVAILABLE 1
#endif

/**
* CORSAC_HAS_INCLUDE
*
* Может использоваться в выражениях #if и #elif для проверки существования заголовка,
* указанного в операнде. Если возможно, он принимает ненулевое значение,
* в противном случае - ноль. Операнд имеет ту же форму, что и файл в директиве include.
*
* Пример использования:
*
* #if CORSAC_HAS_INCLUDE("myinclude.h")
*     #include "myinclude.h"
* #endif
*
* #if CORSAC_HAS_INCLUDE(<myinclude.h>)
*     #include <myinclude.h>
* #endif
*/
#if !defined(CORSAC_HAS_INCLUDE)
    #define CORSAC_HAS_INCLUDE(x) __has_include(x)
#endif

/**
* CORSAC_UNUSED
*
* Удаляет предупреждения компилятора о неиспользуемых переменных.
*
* Пример использования:
*    void Function(int x)
*    {
*        int y;
*        CORSAC_UNUSED(x);
*        CORSAC_UNUSED(y);
*    }
*/
#ifndef CORSAC_UNUSED
    #define CORSAC_UNUSED(x) (void)x
#endif

#endif //CORSAC_COMPILER_TRAITS_H
