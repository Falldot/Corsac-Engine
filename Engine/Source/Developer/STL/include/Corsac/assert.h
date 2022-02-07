/**
 * corsac::STL
 *
 * assert.h
 *
 * Created by Falldot on 26.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STL_ASSERT_H
#define CORSAC_STL_ASSERT_H

#pragma once
/**
 * Описание (Falldot 26.11.2021)
 *
 * Реализация некоторых вспомогательных assert макросов, помогающие
 * отлаживать и тестировать программу.
 *
 * Установите директиву #define CORSAC_DEBUG 1, для их выполнения
 *
 *  CORSAC_ASSERT(expr)
 *  CORSAC_ASSERT_MSG(expr, msg)
 *  CORSAC_ASSERT_FORMATTED(expr, fmt)
 *  CORSAC_FAIL()
 *  CORSAC_FAIL_MSG(msg)
 *  CORSAC_FAIL_FORMATTED(fmt)
 *  CORSAC_CRASH()
 *
 * В случаях проверок на стадии компиляции следует использовать static_assert().
 */

#include "Corsac/Base/base.h"
#include "Corsac/STL/config.h"

#include <stdarg.h>
#include <stdio.h>

/**
 * CORSAC_ASSERT_ENABLED
 *
 * Определяем использование assert, только в случаи Debug режима
 */
#ifdef CORSAC_DEBUG
    #ifndef CORSAC_ASSERT_ENABLED
        #define CORSAC_ASSERT_ENABLED 1
    #endif
#endif

/**
 * Windows дает дополнительный удобный механизм для отлова ошибок.
 * Используем его в Debug режиме.
 */
#if defined(CORSAC_ASSERT_ENABLED)
    #if defined(CORSAC_PLATFORM_MICROSOFT)
        #if !defined(_WIN32_WINNT) || (_WIN32_WINNT < 0x0400)
            #undef  _WIN32_WINNT
            #define _WIN32_WINNT 0x0400
        #endif
        #ifndef WIN32_LEAN_AND_MEAN
            #define WIN32_LEAN_AND_MEAN
        #endif
            CORSAC_DISABLE_WIN_VC_WARNING(0)
            #include <Windows.h> // ::IsDebuggerPresent
            CORSAC_RESTORE_VC_WARNING()
    #endif
#endif

/**
 * CORSAC_ASSERT_VSNPRINTF, CORSAC_ASSERT_SNPRINTF
 *
 * Безопасные методы компилятора подсчитывания число
 * записанных символов, не считая завершающего нуль символа.
 */
#if !defined(CORSAC_ASSERT_VSNPRINTF)
    #if defined(CORSAC_PLATFORM_MICROSOFT)
        #define CORSAC_ASSERT_VSNPRINTF _vsnprintf_s
        #define CORSAC_ASSERT_SNPRINTF  _snprintf_s
    #else
        #define CORSAC_ASSERT_VSNPRINTF vsnprintf
        #define CORSAC_ASSERT_SNPRINTF   snprintf
    #endif
#endif

namespace corsac::internal
{
    using FailureCallback = bool (*)(const char*, const char*, int, const char*, const char*, va_list);

    #if defined(CORSAC_ASSERT_ENABLED)
        static void PlatformPrint(const char *str)
        {
            #if defined(CORSAC_PLATFORM_MICROSOFT)
            if (IsDebuggerPresent())
            {
                ::OutputDebugStringA(str);
            }
            #endif

            puts(str);

            #if defined(CORSAC_PLATFORM_MOBILE)
                // Мобильные платформы нуждаются в этом, потому что в противном случае вы
                // можете легко потерять выходные данные, если устройство выйдет из строя.
                fflush(stdout);
            #endif
        }
    #endif

    bool DefaultFailureCallback(const char* expr, const char* filename, int line, const char* function, const char* msg, va_list args)
    {
        #if defined(CORSAC_ASSERT_ENABLED)
            const int largeEnough = 2048;
            char output[largeEnough + 1] = {};
            char fmtMsg[largeEnough + 1] = {};

            int len = CORSAC_ASSERT_VSNPRINTF(fmtMsg, largeEnough, msg, args);

            if(len==0)
            {
                len = CORSAC_ASSERT_SNPRINTF(fmtMsg, largeEnough, "none");
            }

            if (len < 0 || len > largeEnough)
                len = largeEnough;

            fmtMsg[len] = '\0';

            len = CORSAC_ASSERT_SNPRINTF(output, largeEnough,
                                         "%s(%d) : CORSAC_ASSERT failed: '%s' in function: %s\n, message: %s",
                                         filename, line, expr, function, fmtMsg);
            if (len < 0 || len > largeEnough)
                len = largeEnough;

            output[len] = '\0';

            PlatformPrint(output);
        #else
            CORSAC_UNUSED(expr);
            CORSAC_UNUSED(filename);
            CORSAC_UNUSED(line);
            CORSAC_UNUSED(function);
            CORSAC_UNUSED(msg);
            CORSAC_UNUSED(args);
        #endif

        return true;
    }

    FailureCallback GetFailureCallback()
    {
        return &DefaultFailureCallback;
    }

    bool VCall(const char *expr, const char *filename, int line, const char *function, const char *msg, ...)
    {
        va_list args;
        va_start(args, msg);
        bool ret = (*GetFailureCallback())(expr, filename, line, function, msg, args);
        va_end(args);
        return ret;
    }

    bool Call(const char *expr, const char *filename, int line, const char *function, const char *msg)
    {
        return VCall(expr, filename, line, function, "%s", msg);
    }

    bool Call(const char *expr, const char *filename, int line, const char *function)
    {
        return VCall(expr, filename, line, function, "");
    }

    struct Forwarder
    {
        const char* mExpr;
        const char* mFunction;
        const char* mFilename;
        int mLine;
        Forwarder(const char* expr, const char* filename, int line,  const char* function)
                : mExpr(expr), mFunction(function), mFilename(filename), mLine(line) {}

        bool Call(const char* msg, ...) const
        {
            va_list args;
            va_start(args, msg);
            bool ret = (*GetFailureCallback())(mExpr, mFilename, mLine, mFunction, msg, args);
            va_end(args);
            return ret;
        }
    };
}

#ifdef CORSAC_COMPILER_NO_VARIADIC_MACROS
    #define CORSAC_ASSERT_FORMATTED_HANDLER_CALL(EXPR, FILE, LINE, FUNCTION, FMT) corsac::internal::Forwarder(EXPR, FILE, LINE, FUNCTION).Call FMT
#else
    #define CORSAC_ASSERT_FORMATTED_HANDLER_CALL(EXPR, FILE, LINE, FUNCTION, FMT) corsac::internal::VCall(EXPR, FILE, LINE, FUNCTION, CORSAC_ASSERT_EXPAND_VA_ARGS FMT)
    #define CORSAC_ASSERT_EXPAND_VA_ARGS(...) __VA_ARGS__
#endif


/*
* CORSAC_DEBUG_BREAK
*
* Эта функция заставляет приложение немедленно останавливаться под отладчиком.
* Он реализован как макрос для того, чтобы программа останавливались на месте вызова.
*/

// CORSAC_COMPILER_HAS_BUILTIN
#ifndef CORSAC_COMPILER_HAS_BUILTIN
    #if defined(__clang__)
        #define CORSAC_COMPILER_HAS_BUILTIN(x) __has_builtin(x)
    #else
        #define CORSAC_COMPILER_HAS_BUILTIN(x) 0
    #endif
#endif

#if ! defined CORSAC_DEBUG_BREAK && ! defined CORSAC_DEBUG_BREAK_DEFINED
    #if defined(CORSAC_PLATFORM_SONY) && defined(CORSAC_PROCESSOR_X86_64)
        #define CORSAC_DEBUG_BREAK() do { { __asm volatile ("int $0x41"); } } while(0)
    #elif defined _MSC_VER
        #define CORSAC_DEBUG_BREAK() __debugbreak()
    #elif CORSAC_COMPILER_HAS_BUILTIN(__builtin_debugtrap)
        #define CORSAC_DEBUG_BREAK() __builtin_debugtrap()
    #elif (defined(CORSAC_PROCESSOR_X86) || defined(CORSAC_PROCESSOR_X86_64)) && defined(__GNUC__)
        #define CORSAC_DEBUG_BREAK() __asm__("int3")
    #elif defined(CORSAC_PROCESSOR_ARM64) && defined(__GNUC__)
        #define CORSAC_DEBUG_BREAK() asm("brk 10")
    #elif defined(CORSAC_PROCESSOR_ARM) && defined(__APPLE__)
        #include <signal.h>
        #include <unistd.h>
        #define CORSAC_DEBUG_BREAK() kill( getpid(), SIGINT )
    #elif defined(CORSAC_PROCESSOR_ARM) && defined(__GNUC__)
        #define CORSAC_DEBUG_BREAK() asm("BKPT 10")
    #elif defined(CORSAC_PROCESSOR_ARM) && defined(__ARMCC_VERSION)
        #define CORSAC_DEBUG_BREAK() __breakpoint(10)
    #else
        #define CORSAC_DEBUG_BREAK() *(int*)(0) = 0
    #endif

    #define CORSAC_DEBUG_BREAK_DEFINED
#endif

/**
* CORSAC_CRASH
*
* Выполняет недопустимую запись в память,
* что должно приводить к исключению на большинстве платформ.
*/
#if !defined(CORSAC_CRASH) && !defined(CORSAC_CRASH_DEFINED)
    #define CORSAC_CRASH() *(volatile int*)(0) = 0
    #define CORSAC_CRASH_DEFINED
#endif

/**
* CORSAC_CURRENT_FUNCTION
*
* Получить текущее имя функции в виде макроса.
*/
#ifndef CORSAC_CURRENT_FUNCTION
    #if defined __GNUC__ || (defined __ICC && __ICC >= 600)
        #define CORSAC_CURRENT_FUNCTION __PRETTY_FUNCTION__
    #elif defined(__FUNCSIG__)
        #define CORSAC_CURRENT_FUNCTION __FUNCSIG__
        #elif (defined __INTEL_COMPILER && __INTEL_COMPILER >= 600) || (defined __IBMCPP__ && __IBMCPP__ >= 500) || (defined CS_UNDEFINED_STRING && CS_UNDEFINED_STRING >= 0x4200)
            #define CORSAC_CURRENT_FUNCTION __FUNCTION__
        #elif defined __BORLANDC__ && __BORLANDC__ >= 0x550
            #define CORSAC_CURRENT_FUNCTION __FUNC__
        #elif defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901
            #define CORSAC_CURRENT_FUNCTION __func__
        #else
            #define CORSAC_CURRENT_FUNCTION "(unknown function)"
    #endif
#endif

/**
 * Набор assert макросов
 */
#ifdef CORSAC_ASSERT_ENABLED
    #ifdef __cplusplus
		#ifndef CORSAC_ASSERT
			#define CORSAC_ASSERT(expr) \
				CORSAC_DISABLE_VC_WARNING(4127) \
				do { \
					CORSAC_ANALYSIS_ASSUME(expr); \
					if (!(expr) && corsac::internal::Call(#expr, __FILE__, __LINE__, CORSAC_CURRENT_FUNCTION)) \
						CORSAC_DEBUG_BREAK(); \
					else \
						((void)0);\
				} while(0) \
				CORSAC_RESTORE_VC_WARNING()
		#endif
		#ifndef CORSAC_ASSERT_MSG
			#define CORSAC_ASSERT_MSG(expr, msg) \
				CORSAC_DISABLE_VC_WARNING(4127) \
				do { \
					CORSAC_ANALYSIS_ASSUME(expr); \
					if (!(expr) && corsac::internal::Call(#expr, __FILE__, __LINE__, CORSAC_CURRENT_FUNCTION, msg)) \
						CORSAC_DEBUG_BREAK(); \
					else \
						((void)0);\
				} while(0) \
				CORSAC_RESTORE_VC_WARNING()
		#endif
		#ifndef CORSAC_ASSERT_FORMATTED
			#define CORSAC_ASSERT_FORMATTED(expr, fmt) if (!(expr) && CORSAC_ASSERT_FORMATTED_HANDLER_CALL(#expr, __FILE__, __LINE__, CORSAC_CURRENT_FUNCTION, fmt)) {CORSAC_DEBUG_BREAK();} else {((void)0);}
		#endif
		#ifndef CORSAC_FAIL
			#define CORSAC_FAIL() if ( corsac::internal::Call("CORSAC_FAIL", __FILE__, __LINE__, CORSAC_CURRENT_FUNCTION)) {CORSAC_DEBUG_BREAK();} else {((void)0);}
		#endif
		#ifndef CORSAC_FAIL_MSG
			#define CORSAC_FAIL_MSG(msg) if ( corsac::internal::Call("CORSAC_FAIL", __FILE__, __LINE__, CORSAC_CURRENT_FUNCTION, msg)) {CORSAC_DEBUG_BREAK();} else {((void)0);}
		#endif
		#ifndef CORSAC_FAIL_FORMATTED
			#define CORSAC_FAIL_FORMATTED(fmt) if (CORSAC_ASSERT_FORMATTED_HANDLER_CALL("CORSAC_FAIL", __FILE__, __LINE__, CORSAC_CURRENT_FUNCTION, fmt)) {CORSAC_DEBUG_BREAK();} else {((void)0);}
		#endif
	#else
		#ifndef CORSAC_ASSERT
			#define CORSAC_ASSERT(expr) \
				CORSAC_DISABLE_VC_WARNING(4127)\
				do {\
					CORSAC_ANALYSIS_ASSUME(expr);\
					if(!(expr))\
					{\
						printf("\nCORSAC_ASSERT(%s) failed in %s(%d)\n", #expr, __FILE__, __LINE__); CORSAC_DEBUG_BREAK();\
					}\
					else\
					{\
						((void)0);\
					}\
				} while(0)\
				CORSAC_RESTORE_VC_WARNING()
		#endif
		#ifndef CORSAC_ASSERT_MSG
			#define CORSAC_ASSERT_MSG(expr, msg) \
				CORSAC_DISABLE_VC_WARNING(4127)\
				do\
				{ \
					CORSAC_ANALYSIS_ASSUME(expr); \
					if (!(expr)) \
					{\
						printf("\nCORSAC_ASSERT(%s) failed in %s(%d): %s\n", #expr, __FILE__, __LINE__, msg);CORSAC_DEBUG_BREAK();\
					}\
					else \
					{ \
						((void)0); \
					} \
				} while(0) \
				CORSAC_RESTORE_VC_WARNING()
		#endif
		#ifndef CORSAC_ASSERT_FORMATTED
			#define CORSAC_ASSERT_FORMATTED(expr, fmt) \
				CORSAC_DISABLE_VC_WARNING(4127) \
				do {\
					CORSAC_ANALYSIS_ASSUME(expr); \
					if (!(expr)) \
					{ \
						printf fmt; CORSAC_DEBUG_BREAK(); CORSAC_DEBUG_BREAK(); \
					} \
					else \
					{ \
						((void)0); \
					} \
				} while(0) \
				CORSAC_RESTORE_VC_WARNING()
		#endif
		#ifndef CORSAC_FAIL
			#define CORSAC_FAIL() \
				CORSAC_DISABLE_VC_WARNING(4127) \
				do {\
					printf("\nCORSAC_FAIL: %s(%d)\n", __FILE__, __LINE__); CORSAC_DEBUG_BREAK(); \
				} while(0) \
				CORSAC_RESTORE_VC_WARNING()
		#endif
		#ifndef CORSAC_FAIL_MSG
			#define CORSAC_FAIL_MSG(msg) \
				CORSAC_DISABLE_VC_WARNING(4127) \
				do {\
					printf("\nCORSAC_FAIL: %s(%d): %s\n", __FILE__, __LINE__, msg); CORSAC_DEBUG_BREAK(); \
				} while(0) \
				CORSAC_RESTORE_VC_WARNING()
		#endif
		#ifndef CORSAC_FAIL_FORMATTED
			#define CORSAC_FAIL_FORMATTED(fmt) \
				CORSAC_DISABLE_VC_WARNING(4127) \
				do {\
					printf fmt; CORSAC_DEBUG_BREAK(); \
				} while(0) \
				CORSAC_RESTORE_VC_WARNING()
		#endif
	#endif
#else
    #ifndef CORSAC_ASSERT
        #define CORSAC_ASSERT(expr)                        ((void)0)
    #endif
    #ifndef CORSAC_ASSERT_MSG
        #define CORSAC_ASSERT_MSG(expr, msg)               ((void)0)
    #endif
    #ifndef CORSAC_ASSERT_FORMATTED
        #define CORSAC_ASSERT_FORMATTED(expr, fmt)         ((void)0)
    #endif
    #ifndef CORSAC_FAIL
        #define CORSAC_FAIL()                              ((void)0)
    #endif
    #ifndef CORSAC_FAIL_MSG
        #define CORSAC_FAIL_MSG(msg)                       ((void)0)
    #endif
    #ifndef CORSAC_FAIL_FORMATTED
        #define CORSAC_FAIL_FORMATTED(fmt)                 ((void)0)
    #endif
#endif

/**
* CORSAC_EMPTY_REFERENCE_ASSERT_ENABLED
*
* Определяется как 0 или ненулевое значение. По умолчанию такое же, как CORSAC_ASSERT_ENABLED.
* Это похоже на CORSAC_ASSERT_ENABLED, за исключением ссылок на пустые контейнеры.
* Иногда людям нравится иметь возможность ссылаться на переднюю часть контейнера,
* чем писать дополнительный код, проверяющий, пуст ли контейнер.
*/
#ifndef CORSAC_EMPTY_REFERENCE_ASSERT_ENABLED
    #define CORSAC_EMPTY_REFERENCE_ASSERT_ENABLED CORSAC_ASSERT_ENABLED
#endif

#endif //CORSAC_STL_ASSERT_H
