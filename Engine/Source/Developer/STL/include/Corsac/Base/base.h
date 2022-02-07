/**
 * corsac::STL
 *
 * base.h
 *
 * Created by Falldot on 26.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STL_BASE_H
#define CORSAC_STL_BASE_H

#include "Corsac/Base/platform.h"
#include "Corsac/Base/compiler.h"
#include "Corsac/Base/compiler_traits.h"

#pragma once
/**
 * Описание (Falldot 26.11.2021)
 *
 *
 */

/**
* Стандарт C ++ определяет size_t как встроенный тип. Некоторые компиляторы не соответствуют
* стандартам в этом отношении, поэтому нам нужно дополнительное включение.
* Случай аналогичен wchar_t в C ++.
*/
#if defined(CORSAC_COMPILER_GNUC) || defined(CORSAC_COMPILER_MSVC) || defined(CORSAC_WCHAR_T_NON_NATIVE) || defined(CORSAC_PLATFORM_SONY)
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
* По умолчанию GCC определяет NULL как ((void)0), что является определением C.
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
* wchar types
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

/**
* CORSAC_CHAR8_UNIQUE
*
* Проверьте поддержку char8_t в системе типов cpp.
*/
#ifdef __cpp_char8_t
    #define CHAR8_T_DEFINED
	#define CORSAC_CHAR8_UNIQUE 1
#else
    #define CORSAC_CHAR8_UNIQUE 0
#endif

#endif //STLC_BASE_H
