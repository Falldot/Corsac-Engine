/**
 * corsac::STL
 *
 * internal/base/have.h
 *
 * Created by Falldot on 08.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */

#ifndef CORSAC_HAVE_H
#define CORSAC_HAVE_H

#pragma once

#include "internal/base/base.h"

// Dinkumware
#if !defined(CORSAC_LIBRARY_DINKUMWARE)
    #include <cstddef> // Необходимо запустить компиляцию yval.h без прямого использования <yval.h>, потому что он может не существовать.

    #if defined(_CPPLIB_VER) /* При использовании стандартной библиотеки Dinkumware ... */
        #define CORSAC_LIBRARY_DINKUMWARE 1
    #endif
#endif

// GCC, Clang libstdc++
#if !defined(CORSAC_LIBRARY_LIBSTDCPP)
    #if defined(__GLIBCXX__) /* Если вы используете libstdc++ ... */
        #define CORSAC_LIBRARY_LIBSTDCPP 1
    #endif
#endif

// Clang libc++
#if !defined(CORSAC_LIBRARY_LIBCPP)
    #if CORSAC_HAS_INCLUDE_AVAILABLE
        #if CORSAC_HAS_INCLUDE(<__config>)
            #define CORSAC_LIBRARY_LIBCPP 1 // Мы также могли бы включить <ciso646> и проверить, определено ли оно (_LIBCPP_VERSION).
        #endif
    #endif
#endif

// #include <initializer_list>
#if !defined(CORSAC_HAVE_INITIALIZER_LIST) && !defined(CORSAC_NO_HAVE_INITIALIZER_LIST)
    #if defined(CORSAC_LIBRARY_DINKUMWARE)
        #define CORSAC_HAVE_INITIALIZER_LIST 1
    #elif defined(CORSAC_LIBRARY_LIBSTDCPP) && defined(CORSAC_COMPILER_CLANG) && !defined(CORSAC_PLATFORM_APPLE)
        #define CORSAC_HAVE_INITIALIZER_LIST 1
    #elif defined(CORSAC_LIBRARY_LIBCPP) && defined(CORSAC_COMPILER_CLANG) && !defined(CORSAC_PLATFORM_APPLE)
        #define CORSAC_HAVE_INITIALIZER_LIST 1
    #elif defined(CORSAC_LIBRARY_LIBSTDCPP) && defined(CORSAC_COMPILER_GCC) && !defined(CORSAC_PLATFORM_APPLE)
        #define CORSAC_HAVE_INITIALIZER_LIST 1
    #elif defined(CORSAC_LIBRARY_LIBCPP)
        #define CORSAC_HAVE_INITIALIZER_LIST 1
    #else
        #define CORSAC_NO_HAVE_INITIALIZER_LIST 1
    #endif
#endif

#endif //CORSAC_HAVE_H
