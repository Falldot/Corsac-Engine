// Copyright (c) 2021 Corsac Team
// License: MIT License
// MIT License web page: https://opensource.org/licenses/MIT
#ifndef CORSAC_CONFIG_H
#define CORSAC_CONFIG_H

#pragma once

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
#if !defined(CORSAC_EXCEPTIONS_ENABLED) || ((CORSAC_EXCEPTIONS_ENABLED == 1) && defined(CORSAC_EXCEPTIONS_ENABLED))
    #define CORSAC_EXCEPTIONS_ENABLED 0
#endif

/**
 * CORSAC_COMPILER_INTRINSIC_TYPE_TRAITS_AVAILABLE
 *
 * Определяется как 0 или 1; по умолчанию основан на автоматическом обнаружении. Указывает, предоставляет
 * ли компилятор встроенную поддержку признаков типа компилятора (например, __is_abstract ()). Не уточняет
 * какие-либо подробности о том, какие черты доступны или насколько они соответствуют стандартам. Тем не менее
 * это полезный макрос-идентификатор для нашей реализации свойств типа.
 */
#ifndef CORSAC_COMPILER_INTRINSIC_TYPE_TRAITS_AVAILABLE
    #if defined(_MSC_VER) && (_MSC_VER >= 1500) && !defined(EA_COMPILER_CLANG_CL) // VS2008 or later
    #pragma warning(push, 0)
                #include <yvals.h>
            #pragma warning(pop)
            #if ((defined(_HAS_TR1) && _HAS_TR1) || _MSC_VER >= 1700)  // VS2012 (1700) and later has built-in type traits support.
                #define CORSAC_COMPILER_INTRINSIC_TYPE_TRAITS_AVAILABLE 1
            #else
                #define CORSAC_COMPILER_INTRINSIC_TYPE_TRAITS_AVAILABLE 0
            #endif
    #elif defined(__clang__) && defined(__APPLE__) && defined(_CXXCONFIG) // Apple clang but with GCC's libstdc++.
    #define CORSAC_COMPILER_INTRINSIC_TYPE_TRAITS_AVAILABLE 0
    #elif defined(__clang__)
    #define CORSAC_COMPILER_INTRINSIC_TYPE_TRAITS_AVAILABLE 1
    #elif defined(EA_COMPILER_GNUC) && (EA_COMPILER_VERSION >= 4003) && !defined(__GCCXML__)
    #define CORSAC_COMPILER_INTRINSIC_TYPE_TRAITS_AVAILABLE 1
        #elif defined(__MSL_CPP__) && (__MSL_CPP__ >= 0x8000) // CodeWarrior compiler.
            #define CORSAC_COMPILER_INTRINSIC_TYPE_TRAITS_AVAILABLE 1
        #else
            #define CORSAC_COMPILER_INTRINSIC_TYPE_TRAITS_AVAILABLE 0
    #endif
#endif

namespace corsac
{
    
} // namespace corsac


#endif // Header include guard