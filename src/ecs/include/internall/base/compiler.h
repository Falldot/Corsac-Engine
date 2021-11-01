//
// internal/base/compiler.h
//
// Created by Falldot on 01.11.2021.
// Copyright (c) 2021 Corsac. All rights reserved.
//
#ifndef CORSAC_ENGINE_COMPILER_H
#define CORSAC_ENGINE_COMPILER_H

#ifndef INTERNAL_STRINGIZE
    #define INTERNAL_STRINGIZE(x) INTERNAL_PRIMITIVE_STRINGIZE(x)
#endif

#ifndef INTERNAL_PRIMITIVE_STRINGIZE
    #define INTERNAL_PRIMITIVE_STRINGIZE(x) #x
#endif

#if defined(__clang__)
    #define CORSAC_СOMPILER_CLANG 1
    #define CORSAC_COMPILER_NAME "Clang"
    #define CORSAC_COMPILER_VERSION (__clang_major__ * 100 + __clang_minor__)
    #define CORSAC_COMPILER CORSAC_COMPILER_NAME " compiler, version " __clang_version__
    #if CORSAC_COMPILER_VERSION < 1200
        #error Unsupported compiler version
    #endif
#elif defined(_MSC_VER)
    #define CORSAC_COMPILER_MSVC 1
    #define CORSAC_COMPILER_MICROSOFT 1
    #define CORSAC_COMPILER_NAME "Microsoft Visual C++"
    #define CORSAC_COMPILER_VERSION _MSC_VER
    #if defined(__clang__)
        #define CORSAC_COMPILER_CLANG 1
	#endif
    #define CORSAC_COMPILER CORSAC_COMPILER_NAME " compiler, version " _MSC_VER
    #if CORSAC_COMPILER_VERSION < 1929
        #error Unsupported compiler version
    #endif
#elif defined(__GNUC__)
    #define CORSAC_COMPILER_GCC 1
    #define CORSAC_COMPILER_NAME "GCC"
    #define CORSAC_COMPILER_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
    #define CORSAC_COMPILER CORSAC_COMPILER_NAME " compiler, version " INTERNAL_STRINGIZE(__GNUC__) "." INTERNAL_STRINGIZE(__GNUC_MINOR__)
    #if CORSAC_COMPILER_VERSION < 10003
        #error Unsupported compiler version
    #endif
#elif
    #error Unsupported compiler
#endif

// CORSAC_COMPILER_WINRTCX_ENABLED
//
// Определяется как 1, если в компиляторе включена доступная поддержка C ++ CX, в противном случае - не определено.
// Это, в частности, означает, что соответствующая единица компиляции была создана с включенными компонентами среды выполнения Windows,
// обычно с использованием флагов компилятора «-ZW». Этот параметр позволяет использовать объекты типа шляпы '^'
// с подсчетом ссылок и другие ключевые слова C ++ CX, такие как «ref new».
#if !defined(CORSAC_COMPILER_WINRTCX_ENABLED) && defined(__cplusplus_winrt)
    #define CORSAC_COMPILER_WINRTCX_ENABLED 1
#endif

#endif //CORSAC_ENGINE_COMPILER_H
