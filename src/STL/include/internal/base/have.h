/**
 * corsac::STL
 *
 * internall/base/have.h
 *
 * Created by Falldot on 08.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */

#ifndef CORSAC_HAVE_H
#define CORSAC_HAVE_H

#pragma once

#include "internall/base/base.h"

// Dinkumware
#if !defined(CORSAC_HAVE_DINKUMWARE_CPP_LIBRARY) && !defined(CORSAC_NO_HAVE_DINKUMWARE_CPP_LIBRARY)
    #if defined(__cplusplus)
        #include <cstddef> // Необходимо запустить компиляцию yval.h без прямого использования <yval.h>, потому что он может не существовать.
    #endif

    #if defined(__cplusplus) && defined(_CPPLIB_VER) /* При использовании стандартной библиотеки Dinkumware ... */
        #define CORSAC_HAVE_DINKUMWARE_CPP_LIBRARY 1
    #else
        #define CORSAC_NO_HAVE_DINKUMWARE_CPP_LIBRARY 1
    #endif
#endif

// GCC, Clang libstdc++
#if !defined(CORSAC_HAVE_LIBSTDCPP_LIBRARY) && !defined(CORSAC_NO_HAVE_LIBSTDCPP_LIBRARY)
    #if defined(__GLIBCXX__) /* Если вы используете libstdc++ ... */
        #define CORSAC_HAVE_LIBSTDCPP_LIBRARY 1
    #else
        #define CORSAC_NO_HAVE_LIBSTDCPP_LIBRARY 1
    #endif
#endif

// Clang libc++
#if !defined(CORSAC_HAVE_LIBCPP_LIBRARY) && !defined(CORSAC_NO_HAVE_LIBCPP_LIBRARY)
    #if CORSAC_HAS_INCLUDE_AVAILABLE
        #if CORSAC_HAS_INCLUDE(<__config>)
            #define CORSAC_HAVE_LIBCPP_LIBRARY 1 // Мы также могли бы включить <ciso646> и проверить, определено ли оно (_LIBCPP_VERSION).
        #endif
    #endif

    #if !defined(CORSAC_HAVE_LIBCPP_LIBRARY)
        #define CORSAC_NO_HAVE_LIBCPP_LIBRARY 1
    #endif
#endif

// #include <initializer_list>
#if !defined(CORSAC_HAVE_INITIALIZER_LIST) && !defined(CORSAC_NO_HAVE_INITIALIZER_LIST)
    #if defined(CORSAC_HAVE_DINKUMWARE_CPP_LIBRARY)
        #define CORSAC_HAVE_INITIALIZER_LIST 1
    #elif defined(CORSAC_HAVE_LIBSTDCPP_LIBRARY) && defined(CORSAC_COMPILER_CLANG) && !defined(CORSAC_PLATFORM_APPLE)
        #define CORSAC_HAVE_INITIALIZER_LIST 1
    #elif defined(CORSAC_HAVE_LIBCPP_LIBRARY) && defined(CORSAC_COMPILER_CLANG) && !defined(CORSAC_PLATFORM_APPLE)
        #define CORSAC_HAVE_INITIALIZER_LIST 1
    #elif defined(CORSAC_HAVE_LIBSTDCPP_LIBRARY) && defined(CORSAC_COMPILER_GCC) && !defined(CORSAC_PLATFORM_APPLE)
        #define CORSAC_HAVE_INITIALIZER_LIST 1
    #elif defined(CORSAC_HAVE_LIBCPP_LIBRARY)
        #define CORSAC_HAVE_INITIALIZER_LIST 1
    #else
        #define CORSAC_NO_HAVE_INITIALIZER_LIST 1
    #endif
#endif

#endif //CORSAC_HAVE_H
