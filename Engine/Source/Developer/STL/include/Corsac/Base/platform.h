/**
 * corsac::STL
 *
 * compiler_traits.h
 *
 * Created by Falldot on 26.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STL_PLATFORM_H
#define CORSAC_STL_PLATFORM_H

#pragma once

// Cygwin
// набор инструментов разработки программного обеспечения для создания приложений.
// Cygwin обеспечивает тесную интеграцию приложений, данных и ресурсов Windows с приложениями,
// данными и ресурсами UNIX-подобной среды.
#if defined(__CYGWIN__)
#define CORSAC_PLATFORM_CYGWIN 1
	#define CORSAC_PLATFORM_DESKTOP 1
#endif

// MinGW
// набор инструментов разработки программного обеспечения для создания приложений под Windows.
#if defined(__MINGW32__) || defined(__MINGW64__)
    #define CORSAC_PLATFORM_MINGW 1
    #define CORSAC_PLATFORM_DESKTOP 1
#endif

#if defined(CORSAC_PLATFORM_PS4) || defined(__ORBIS__) || defined(CORSAC_PLATFORM_KETTLE)
// PlayStation 4
	// Orbis было кодовым названием Sony для платформы, которая сейчас устарела.
	// Kettle - это кодовое название платформы, которое сейчас устарело.
	#if defined(CORSAC_PLATFORM_PS4)
		#undef  CORSAC_PLATFORM_PS4
	#endif
	    #define CORSAC_PLATFORM_PS4 1

    #if defined(CORSAC_PLATFORM_KETTLE)
        #undef  CORSAC_PLATFORM_KETTLE
    #endif

	#define CORSAC_PLATFORM_KETTLE 1
	#define CORSAC_PLATFORM_NAME "PS4"
	#define CORSAC_SYSTEM_LITTLE_ENDIAN 1
	#define CORSAC_PLATFORM_DESCRIPTION "PS4 on x64"
	#define CORSAC_PLATFORM_CONSOLE 1
	#define CORSAC_PLATFORM_SONY 1
	#define CORSAC_PLATFORM_POSIX 1
	#define CORSAC_PROCESSOR_X86_64 1
	#if defined(__GNUC__) || defined(__clang__)
		#define CORSAC_ASM_STYLE_ATT 1
	#endif

#elif defined(CORSAC_PLATFORM_XBOXONE) || defined(_DURANGO) || defined(_XBOX_ONE) || defined(CORSAC_PLATFORM_CAPILANO) || defined(_GAMING_XBOX)
// XBox One
	// Durango было кодовым названием Microsoft для платформы, которая сейчас устарела.
	// Microsoft использует _DURANGO вместо некоторого варианта _XBOX, хотя изначально компилятор не определяет его.
	// Capilano - это кодовое название платформы, но оно устарело
	#if defined(CORSAC_PLATFORM_XBOXONE)
		#undef  CORSAC_PLATFORM_XBOXONE
    #endif

    #define CORSAC_PLATFORM_XBOXONE 1

    #if defined(CORSAC_PLATFORM_CAPILANO)
        #undef  CORSAC_PLATFORM_CAPILANO
    #endif

    #define CORSAC_PLATFORM_CAPILANO 1
    #if defined(CORSAC_PLATFORM_CAPILANO_XDK) && !defined(CORSAC_PLATFORM_XBOXONE_XDK)
        #define CORSAC_PLATFORM_XBOXONE_XDK 1
    #endif

    #if defined(CORSAC_PLATFORM_CAPILANO_ADK) && !defined(CORSAC_PLATFORM_XBOXONE_ADK)
        #define CORSAC_PLATFORM_XBOXONE_ADK 1
    #endif

	#if !defined(_DURANGO)
		#define _DURANGO
    #endif

	#define CORSAC_PLATFORM_NAME "XBox One"
	#define CORSAC_PROCESSOR_X86_64 1
	#define CORSAC_SYSTEM_LITTLE_ENDIAN 1
	#define CORSAC_PLATFORM_DESCRIPTION "XBox One on x64"
	#define CORSAC_ASM_STYLE_INTEL 1
	#define CORSAC_PLATFORM_CONSOLE 1
	#define CORSAC_PLATFORM_MICROSOFT 1

	#define CORSAC_WINAPI_FAMILY_APP         1000
	#define CORSAC_WINAPI_FAMILY_DESKTOP_APP 1001
	#define CORSAC_WINAPI_FAMILY_PHONE_APP   1002
	#define CORSAC_WINAPI_FAMILY_TV_APP      1003
	#define CORSAC_WINAPI_FAMILY_TV_TITLE    1004
	#define CORSAC_WINAPI_FAMILY_GAMES       1006

	#if defined(WINAPI_FAMILY)
		#include <winapifamily.h>
		#if defined(WINAPI_FAMILY_TV_TITLE) && WINAPI_FAMILY == WINAPI_FAMILY_TV_TITLE
			#define CORSAC_WINAPI_FAMILY CORSAC_WINAPI_FAMILY_TV_TITLE
		#elif defined(WINAPI_FAMILY_DESKTOP_APP) && WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
			#define CORSAC_WINAPI_FAMILY CORSAC_WINAPI_FAMILY_DESKTOP_APP
		#elif defined(WINAPI_FAMILY_GAMES) && WINAPI_FAMILY == WINAPI_FAMILY_GAMES
			#define CORSAC_WINAPI_FAMILY CORSAC_WINAPI_FAMILY_GAMES
		#else
			#error Unsupported WINAPI_FAMILY
		#endif
	#else
		#error WINAPI_FAMILY should always be defined on Capilano.
	#endif

	// Макрос, чтобы определить, включен ли раздел.
	#define CORSAC_WINAPI_FAMILY_PARTITION(Partition)	(Partition)

	#if CORSAC_WINAPI_FAMILY == CORSAC_WINAPI_FAMILY_DESKTOP_APP
		#define CORSAC_WINAPI_PARTITION_CORE     1
		#define CORSAC_WINAPI_PARTITION_DESKTOP  1
		#define CORSAC_WINAPI_PARTITION_APP      1
		#define CORSAC_WINAPI_PARTITION_PC_APP   0
		#define CORSAC_WIANPI_PARTITION_PHONE    0
		#define CORSAC_WINAPI_PARTITION_TV_APP   0
		#define CORSAC_WINAPI_PARTITION_TV_TITLE 0
		#define CORSAC_WINAPI_PARTITION_GAMES    0
	#elif CORSAC_WINAPI_FAMILY == CORSAC_WINAPI_FAMILY_TV_TITLE
		#define CORSAC_WINAPI_PARTITION_CORE     1
		#define CORSAC_WINAPI_PARTITION_DESKTOP  0
		#define CORSAC_WINAPI_PARTITION_APP      0
		#define CORSAC_WINAPI_PARTITION_PC_APP   0
		#define CORSAC_WIANPI_PARTITION_PHONE    0
		#define CORSAC_WINAPI_PARTITION_TV_APP   0
		#define CORSAC_WINAPI_PARTITION_TV_TITLE 1
		#define CORSAC_WINAPI_PARTITION_GAMES    0
	#elif CORSAC_WINAPI_FAMILY == CORSAC_WINAPI_FAMILY_GAMES
		#define CORSAC_WINAPI_PARTITION_CORE     1
		#define CORSAC_WINAPI_PARTITION_DESKTOP  0
		#define CORSAC_WINAPI_PARTITION_APP      0
		#define CORSAC_WINAPI_PARTITION_PC_APP   0
		#define CORSAC_WIANPI_PARTITION_PHONE    0
		#define CORSAC_WINAPI_PARTITION_TV_APP   0
		#define CORSAC_WINAPI_PARTITION_TV_TITLE 0
		#define CORSAC_WINAPI_PARTITION_GAMES    1
	#else
		#error Unsupported WINAPI_FAMILY
	#endif

	#if CORSAC_WINAPI_FAMILY_PARTITION(CORSAC_WINAPI_PARTITION_GAMES)
		#define CS_UNDEFINED_STRING 1
		#define CS_UNDEFINED_STRING 1
	#endif

	#if CORSAC_WINAPI_FAMILY_PARTITION(CORSAC_WINAPI_PARTITION_TV_TITLE)
		#define CORSAC_PLATFORM_XBOXONE_XDK 1
	#endif
#elif defined(CORSAC_PLATFORM_LRB) || defined(__LRB__) || (defined(__EDG__) && defined(__ICC) && defined(__x86_64__))
#undef  CORSAC_PLATFORM_LRB
	#define CORSAC_PLATFORM_LRB         1
	#define CORSAC_PLATFORM_NAME        "Larrabee"
	#define CORSAC_PLATFORM_DESCRIPTION "Larrabee on LRB1"
	#define CORSAC_PROCESSOR_X86_64 1
	#if defined(BYTE_ORDER) && (BYTE_ORDER == 4321)
		#define CORSAC_SYSTEM_BIG_ENDIAN 1
	#else
		#define CORSAC_SYSTEM_LITTLE_ENDIAN 1
	#endif
	#define CORSAC_PROCESSOR_LRB 1
	#define CORSAC_PROCESSOR_LRB1 1
	#define CORSAC_ASM_STYLE_ATT 1
	#define CORSAC_ASM_STYLE_INTEL 1
	#define CORSAC_PLATFORM_DESKTOP 1

// Android (Google phone OS)
#elif defined(CORSAC_PLATFORM_ANDROID) || defined(__ANDROID__)
#undef  CORSAC_PLATFORM_ANDROID
	#define CORSAC_PLATFORM_ANDROID 1
	#define CORSAC_PLATFORM_LINUX 1
	#define CORSAC_PLATFORM_UNIX 1
	#define CORSAC_PLATFORM_POSIX 1
	#define CORSAC_PLATFORM_NAME "Android"
	#define CORSAC_ASM_STYLE_ATT 1
	#if defined(__arm__)
		#define CORSAC_ABI_ARM_LINUX 1  // a.k.a. "ARM eabi"
		#define CORSAC_PROCESSOR_ARM32 1
		#define CORSAC_PLATFORM_DESCRIPTION "Android on ARM"
	#elif defined(__aarch64__)
		#define CORSAC_PROCESSOR_ARM64 1
		#define CORSAC_PLATFORM_DESCRIPTION "Android on ARM64"
	#elif defined(__i386__)
		#define CORSAC_PROCESSOR_X86 1
		#define CORSAC_PLATFORM_DESCRIPTION "Android on x86"
	#elif defined(__x86_64)
		#define CORSAC_PROCESSOR_X86_64 1
		#define CORSAC_PLATFORM_DESCRIPTION "Android on x64"
	#else
		#error Unknown processor
	#endif
	#if !defined(CORSAC_SYSTEM_BIG_ENDIAN) && !defined(CORSAC_SYSTEM_LITTLE_ENDIAN)
		#define CORSAC_SYSTEM_LITTLE_ENDIAN 1
	#endif
	#define CORSAC_PLATFORM_MOBILE 1

// Samsung SMART TV - a Linux-based smart TV
#elif defined(CORSAC_PLATFORM_SAMSUNG_TV)
#undef  CORSAC_PLATFORM_SAMSUNG_TV
	#define CORSAC_PLATFORM_SAMSUNG_TV 1
	#define CORSAC_PLATFORM_LINUX 1
	#define CORSAC_PLATFORM_UNIX 1
	#define CORSAC_PLATFORM_POSIX 1
	#define CORSAC_PLATFORM_NAME "SamsungTV"
	#define CORSAC_PLATFORM_DESCRIPTION "Samsung SMART TV on ARM"
	#define CORSAC_ASM_STYLE_ATT 1
	#define CORSAC_SYSTEM_LITTLE_ENDIAN 1
	#define CORSAC_PROCESSOR_ARM32 1
	#define CORSAC_ABI_ARM_LINUX 1 // a.k.a. "ARM eabi"
	#define CORSAC_PROCESSOR_ARM7 1

#elif defined(__APPLE__) && __APPLE__
#include <TargetConditionals.h>

	#define CORSAC_PLATFORM_APPLE
	#define CORSAC_PLATFORM_POSIX 1

	// iPhone
	// TARGET_OS_IPHONE будет неопределенным в неизвестном компиляторе и будет определен в gcc.
	#if defined(CORSAC_PLATFORM_IPHONE) || defined(__IPHONE__) || (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) || (defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR)
		#undef  CORSAC_PLATFORM_IPHONE
		#define CORSAC_PLATFORM_IPHONE 1
		#define CORSAC_PLATFORM_NAME "iPhone"
		#define CORSAC_ASM_STYLE_ATT 1
		#define CORSAC_POSIX_THRCORSACDS_AVAILABLE 1
		#if defined(__arm__)
			#define CORSAC_ABI_ARM_APPLE 1
			#define CORSAC_PROCESSOR_ARM32 1
			#define CORSAC_SYSTEM_LITTLE_ENDIAN 1
			#define CORSAC_PLATFORM_DESCRIPTION "iPhone on ARM"
		#elif defined(__aarch64__) || defined(__AARCH64)
			#define CORSAC_ABI_ARM64_APPLE 1
			#define CORSAC_PROCESSOR_ARM64 1
			#define CORSAC_SYSTEM_LITTLE_ENDIAN 1
			#define CORSAC_PLATFORM_DESCRIPTION "iPhone on ARM64"
		#elif defined(__i386__)
			#define CORSAC_PLATFORM_IPHONE_SIMULATOR 1
			#define CORSAC_PROCESSOR_X86 1
			#define CORSAC_SYSTEM_LITTLE_ENDIAN 1
			#define CORSAC_PLATFORM_DESCRIPTION "iPhone simulator on x86"
		#elif defined(__x86_64) || defined(__amd64)
			#define CORSAC_PROCESSOR_X86_64 1
			#define CORSAC_SYSTEM_LITTLE_ENDIAN 1
			#define CORSAC_PLATFORM_DESCRIPTION "iPhone simulator on x64"
		#else
			#error Unknown processor
		#endif
		#define CORSAC_PLATFORM_MOBILE 1

// Macintosh OSX
// TARGET_OS_MAC определяется компиляторами Metrowerks и более ранними версиями AppleC.
// Однако TARGET_OS_MAC определяется как 1 во всех случаях.
// __i386__ and __intel__ определяется компилятором GCC.
// __dest_os определяется компилятором Metrowerks.
// __MACH__ определяется компилятороми Metrowerks и GCC .
// powerc and __powerc определяются компиляторами Metrowerks и GCC.
#elif defined(CORSAC_PLATFORM_OSX) || defined(__MACH__) || (defined(__MSL__) && (__dest_os == __mac_os_x))
    #undef  CORSAC_PLATFORM_OSX
    #define CORSAC_PLATFORM_OSX 1
    #define CORSAC_PLATFORM_UNIX 1
    #define CORSAC_PLATFORM_POSIX 1
    #define CORSAC_PLATFORM_NAME "OSX"
    #if defined(__i386__) || defined(__intel__)
        #define CORSAC_PROCESSOR_X86 1
        #define CORSAC_SYSTEM_LITTLE_ENDIAN 1
        #define CORSAC_PLATFORM_DESCRIPTION "OSX on x86"
    #elif defined(__x86_64) || defined(__amd64)
        #define CORSAC_PROCESSOR_X86_64 1
        #define CORSAC_SYSTEM_LITTLE_ENDIAN 1
        #define CORSAC_PLATFORM_DESCRIPTION "OSX on x64"
    #elif defined(__arm__)
        #define CORSAC_ABI_ARM_APPLE 1
        #define CORSAC_PROCESSOR_ARM32 1
        #define CORSAC_SYSTEM_LITTLE_ENDIAN 1
        #define CORSAC_PLATFORM_DESCRIPTION "OSX on ARM"
    #elif defined(__aarch64__) || defined(__AARCH64)
        #define CORSAC_ABI_ARM64_APPLE 1
        #define CORSAC_PROCESSOR_ARM64 1
        #define CORSAC_SYSTEM_LITTLE_ENDIAN 1
        #define CORSAC_PLATFORM_DESCRIPTION "OSX on ARM64"
    #elif defined(__POWERPC64__) || defined(__powerpc64__)
        #define CORSAC_PROCESSOR_POWERPC 1
        #define CORSAC_PROCESSOR_POWERPC_64 1
        #define CORSAC_SYSTEM_BIG_ENDIAN 1
        #define CORSAC_PLATFORM_DESCRIPTION "OSX on PowerPC 64"
    #elif defined(__POWERPC__) || defined(__powerpc__)
        #define CORSAC_PROCESSOR_POWERPC 1
        #define CORSAC_PROCESSOR_POWERPC_32 1
        #define CORSAC_SYSTEM_BIG_ENDIAN 1
        #define CORSAC_PLATFORM_DESCRIPTION "OSX on PowerPC"
    #else
        #error Unknown processor
    #endif
    #if defined(__GNUC__)
        #define CORSAC_ASM_STYLE_ATT 1
    #else
        #define CORSAC_ASM_STYLE_MOTOROLA 1
    #endif
    #define CORSAC_PLATFORM_DESKTOP 1
#else
    #error Unknown Apple Platform
#endif

// Linux
// __linux и __linux__ определяется компиляторами GCC и Borland.
// __i386__ и __intel__ определяется компиляторам GCC.
// __i386__ определяется компиляторам Metrowerks.
// _M_IX86 определяется компиляторами Borland compiler.
// __sparc__ определяется компиляторам GCC.
// __powerpc__ определяется компиляторам GCC .
// __ARM_CORSACBI__ определяется компиляторам GCC толлько на ARM v6l (Raspberry Pi 1)
// __ARM_ARCH_7A__ определяется компиляторам GCC только на ARM v7l (Raspberry Pi 2)
#elif defined(CORSAC_PLATFORM_LINUX) || (defined(__linux) || defined(__linux__))
#undef  CORSAC_PLATFORM_LINUX
	#define CORSAC_PLATFORM_LINUX 1
	#define CORSAC_PLATFORM_UNIX 1
	#define CORSAC_PLATFORM_POSIX 1
	#define CORSAC_PLATFORM_NAME "Linux"
	#if defined(__i386__) || defined(__intel__) || defined(_M_IX86)
		#define CORSAC_PROCESSOR_X86 1
		#define CORSAC_SYSTEM_LITTLE_ENDIAN 1
		#define CORSAC_PLATFORM_DESCRIPTION "Linux on x86"
	#elif defined(__ARM_ARCH_7A__) || defined(__ARM_CORSACBI__)
		#define CORSAC_ABI_ARM_LINUX 1
		#define CORSAC_PROCESSOR_ARM32 1
		#define CORSAC_PLATFORM_DESCRIPTION "Linux on ARM 6/7 32-bits"
	#elif defined(__aarch64__) || defined(__AARCH64)
		#define CORSAC_PROCESSOR_ARM64 1
		#define CORSAC_PLATFORM_DESCRIPTION "Linux on ARM64"
	#elif defined(__x86_64__)
		#define CORSAC_PROCESSOR_X86_64 1
		#define CORSAC_SYSTEM_LITTLE_ENDIAN 1
		#define CORSAC_PLATFORM_DESCRIPTION "Linux on x64"
	#elif defined(__powerpc64__)
		#define CORSAC_PROCESSOR_POWERPC 1
		#define CORSAC_PROCESSOR_POWERPC_64 1
		#define CORSAC_SYSTEM_BIG_ENDIAN 1
		#define CORSAC_PLATFORM_DESCRIPTION "Linux on PowerPC 64"
	#elif defined(__powerpc__)
		#define CORSAC_PROCESSOR_POWERPC 1
		#define CORSAC_PROCESSOR_POWERPC_32 1
		#define CORSAC_SYSTEM_BIG_ENDIAN 1
		#define CORSAC_PLATFORM_DESCRIPTION "Linux on PowerPC"
	#else
		#error Unknown processor
		#error Unknown endianness
	#endif
	#if defined(__GNUC__)
		#define CORSAC_ASM_STYLE_ATT 1
	#endif
	#define CORSAC_PLATFORM_DESKTOP 1


#elif defined(CORSAC_PLATFORM_BSD) || (defined(__BSD__) || defined(__FreeBSD__))
#undef  CORSAC_PLATFORM_BSD
	#define CORSAC_PLATFORM_BSD 1
	#define CORSAC_PLATFORM_UNIX 1
	#define CORSAC_PLATFORM_POSIX 1     // Совместимость с BSD posix не идентична Linux
	#define CORSAC_PLATFORM_NAME "BSD Unix"
	#if defined(__i386__) || defined(__intel__)
		#define CORSAC_PROCESSOR_X86 1
		#define CORSAC_SYSTEM_LITTLE_ENDIAN 1
		#define CORSAC_PLATFORM_DESCRIPTION "BSD on x86"
	#elif defined(__x86_64__)
		#define CORSAC_PROCESSOR_X86_64 1
		#define CORSAC_SYSTEM_LITTLE_ENDIAN 1
		#define CORSAC_PLATFORM_DESCRIPTION "BSD on x64"
	#elif defined(__powerpc64__)
		#define CORSAC_PROCESSOR_POWERPC 1
		#define CORSAC_PROCESSOR_POWERPC_64 1
		#define CORSAC_SYSTEM_BIG_ENDIAN 1
		#define CORSAC_PLATFORM_DESCRIPTION "BSD on PowerPC 64"
	#elif defined(__powerpc__)
		#define CORSAC_PROCESSOR_POWERPC 1
		#define CORSAC_PROCESSOR_POWERPC_32 1
		#define CORSAC_SYSTEM_BIG_ENDIAN 1
		#define CORSAC_PLATFORM_DESCRIPTION "BSD on PowerPC"
	#else
		#error Unknown processor
		#error Unknown endianness
	#endif
	#if !defined(CORSAC_PLATFORM_FREEBSD) && defined(__FreeBSD__)
		#define CORSAC_PLATFORM_FREEBSD 1 // Это вариант BSD.
	#endif
	#if defined(__GNUC__)
		#define CORSAC_ASM_STYLE_ATT 1
	#endif
	#define CORSAC_PLATFORM_DESKTOP 1


#elif defined(CORSAC_PLATFORM_WINDOWS_PHONE)
#undef CORSAC_PLATFORM_WINDOWS_PHONE
	#define CORSAC_PLATFORM_WINDOWS_PHONE 1
	#define CORSAC_PLATFORM_NAME "Windows Phone"
	#if defined(_M_AMD64) || defined(_AMD64_) || defined(__x86_64__)
		#define CORSAC_PROCESSOR_X86_64 1
		#define CORSAC_SYSTEM_LITTLE_ENDIAN 1
		#define CORSAC_PLATFORM_DESCRIPTION "Windows Phone on x64"
	#elif defined(_M_IX86) || defined(_X86_)
		#define CORSAC_PROCESSOR_X86 1
		#define CORSAC_SYSTEM_LITTLE_ENDIAN 1
		#define CORSAC_PLATFORM_DESCRIPTION "Windows Phone on X86"
	#elif defined(_M_ARM)
		#define CORSAC_ABI_ARM_WINCE 1
		#define CORSAC_PROCESSOR_ARM32 1
		#define CORSAC_SYSTEM_LITTLE_ENDIAN 1
		#define CORSAC_PLATFORM_DESCRIPTION "Windows Phone on ARM"
	#else // Возможны другие варианты Windows Phone
		#error Unknown processor
		#error Unknown endianness
	#endif
	#define CORSAC_PLATFORM_MICROSOFT 1

	// WINAPI_FAMILY defines - mirrored from winapifamily.h
	#define CORSAC_WINAPI_FAMILY_APP         1
	#define CORSAC_WINAPI_FAMILY_DESKTOP_APP 2
	#define CORSAC_WINAPI_FAMILY_PHONE_APP   3

	#if defined(WINAPI_FAMILY)
		#include <winapifamily.h>
		#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
			#define CORSAC_WINAPI_FAMILY CORSAC_WINAPI_FAMILY_PHONE_APP
		#else
			#error Unsupported WINAPI_FAMILY for Windows Phone
		#endif
	#else
		#error WINAPI_FAMILY should always be defined on Windows Phone.
	#endif

	// Макрос, чтобы определить, включен ли раздел.
	#define CORSAC_WINAPI_FAMILY_PARTITION(Partition)   (Partition)

	// Включите соответствующие разделы для текущего семейства
	#if CORSAC_WINAPI_FAMILY == CORSAC_WINAPI_FAMILY_PHONE_APP
	#   define CORSAC_WINAPI_PARTITION_CORE    1
	#   define CORSAC_WINAPI_PARTITION_PHONE   1
	#   define CORSAC_WINAPI_PARTITION_APP     1
	#else
	#   error Unsupported WINAPI_FAMILY for Windows Phone
	#endif


// Windows
// _WIN32 определяется компиляторами VC++, Intel and GCC.
// _WIN64 определяется компиляторами VC++, Intel and GCC.
// __WIN32__ определяется компилятором Borland.
// __INTEL__ определяется компилятором Metrowerks.
// _M_IX86, _M_AMD64 and _M_IA64 определяется компиляторами VC++, Intel, and Borland.
// _X86_, _AMD64_, and _IA64_ определяется компиляторам Metrowerks.
// _M_ARM определяется компиляторам VC++.
#elif (defined(CORSAC_PLATFORM_WINDOWS) || (defined(_WIN32) || defined(__WIN32__) || defined(_WIN64))) && !defined(CS_UNDEFINED_STRING)
#undef  CORSAC_PLATFORM_WINDOWS
#define CORSAC_PLATFORM_WINDOWS 1
#define CORSAC_PLATFORM_NAME "Windows"
#ifdef _WIN64
#define CORSAC_PLATFORM_WIN64 1
#else
#define CORSAC_PLATFORM_WIN32 1
#endif
#if defined(_M_AMD64) || defined(_AMD64_) || defined(__x86_64__)
#define CORSAC_PROCESSOR_X86_64 1
#define CORSAC_SYSTEM_LITTLE_ENDIAN 1
#define CORSAC_PLATFORM_DESCRIPTION "Windows on x64"
#elif defined(_M_IX86) || defined(_X86_)
#define CORSAC_PROCESSOR_X86 1
		#define CORSAC_SYSTEM_LITTLE_ENDIAN 1
		#define CORSAC_PLATFORM_DESCRIPTION "Windows on X86"
	#elif defined(_M_IA64) || defined(_IA64_)
		#define CORSAC_PROCESSOR_IA64 1
		#define CORSAC_SYSTEM_LITTLE_ENDIAN 1
		#define CORSAC_PLATFORM_DESCRIPTION "Windows on IA-64"
	#elif defined(_M_ARM)
		#define CORSAC_ABI_ARM_WINCE 1
		#define CORSAC_PROCESSOR_ARM32 1
		#define CORSAC_SYSTEM_LITTLE_ENDIAN 1
		#define CORSAC_PLATFORM_DESCRIPTION "Windows on ARM"
	#elif defined(_M_ARM64)
		#define CORSAC_PROCESSOR_ARM64 1
		#define CORSAC_SYSTEM_LITTLE_ENDIAN 1
		#define CORSAC_PLATFORM_DESCRIPTION "Windows on ARM64"
	#else //Possibly other Windows CE variants
		#error Unknown processor
		#error Unknown endianness
#endif
#if defined(__GNUC__)
#define CORSAC_ASM_STYLE_ATT 1
#elif defined(_MSC_VER) || defined(__BORLANDC__) || defined(__ICL)
#define CORSAC_ASM_STYLE_INTEL 1
#endif
#define CORSAC_PLATFORM_DESKTOP 1
#define CORSAC_PLATFORM_MICROSOFT 1

// WINAPI_FAMILY определяеться поддержкой Windows 8 Metro Apps - зеркало winapifamily.h в Windows 8 SDK
#define CORSAC_WINAPI_FAMILY_APP         1000
#define CORSAC_WINAPI_FAMILY_DESKTOP_APP 1001
#define CORSAC_WINAPI_FAMILY_GAMES       1006

#if defined(WINAPI_FAMILY)
    #if defined(_MSC_VER)
			#pragma warning(push, 0)
	#endif
    #include <winapifamily.h>
    #if defined(_MSC_VER)
        #pragma warning(pop)
    #endif
    #if defined(WINAPI_FAMILY_DESKTOP_APP) && WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
        #define CORSAC_WINAPI_FAMILY CORSAC_WINAPI_FAMILY_DESKTOP_APP
    #elif defined(WINAPI_FAMILY_APP) && WINAPI_FAMILY == WINAPI_FAMILY_APP
        #define CORSAC_WINAPI_FAMILY CORSAC_WINAPI_FAMILY_APP
    #elif defined(WINAPI_FAMILY_GAMES) && WINAPI_FAMILY == WINAPI_FAMILY_GAMES
        #define CORSAC_WINAPI_FAMILY CORSAC_WINAPI_FAMILY_GAMES
    #else
        #error Unsupported WINAPI_FAMILY
    #endif
#else
    #define CORSAC_WINAPI_FAMILY CORSAC_WINAPI_FAMILY_DESKTOP_APP
#endif

#define CORSAC_WINAPI_PARTITION_DESKTOP   1
#define CORSAC_WINAPI_PARTITION_APP       1
#define CORSAC_WINAPI_PARTITION_GAMES    (CORSAC_WINAPI_FAMILY == CORSAC_WINAPI_FAMILY_GAMES)

#define CORSAC_WINAPI_FAMILY_PARTITION(Partition)   (Partition)

// CORSAC_PLATFORM_WINRT
// Это подмножество Windows, которое используется для планшетов и пользовательского интерфейса Metro (ограниченного доступа) Windows.
// WinRT не имеет доступа к "настольному" API Windows, но WinRT, тем не менее, может работать не только на планшетах, но и на настольных компьютерах.
// Windows Phone API является подмножеством WinRT и не включен в него, поскольку является только частью API.
#if defined(__cplusplus_winrt)
    #define CORSAC_PLATFORM_WINRT 1
#endif

#else
#error Unknown platform
	#error Unknown processor
	#error Unknown endianness
#endif

#ifndef CORSAC_PROCESSOR_ARM
    #if defined(CORSAC_PROCESSOR_ARM32) || defined(CORSAC_PROCESSOR_ARM64) || defined(CORSAC_PROCESSOR_ARM7)
        #define CORSAC_PROCESSOR_ARM
    #endif
#endif

// CORSAC_PLATFORM_PTR_SIZE
// Размер указателя платформы; то же, что и sizeof(void*).
// Это не то же самое, что sizeof (int), поскольку int обычно 32 бита даже на 64-битных платформах.
//
// _WIN64 is defined by Win64 compilers, such as VC++.
// _M_IA64 is defined by VC++ and Intel compilers for IA64 processors.
// __LP64__ is defined by HP compilers for the LP64 standard.
// _LP64 is defined by the GCC and Sun compilers for the LP64 standard.
// __ia64__ is defined by the GCC compiler for IA64 processors.
// __arch64__ is defined by the Sparc compiler for 64 bit processors.
// __mips64__ is defined by the GCC compiler for MIPS processors.
// __powerpc64__ is defined by the GCC compiler for PowerPC processors.
// __64BIT__ is defined by the AIX compiler for 64 bit processors.
// __sizeof_ptr is defined by the ARM compiler (armcc, armcpp).
#ifndef CORSAC_PLATFORM_PTR_SIZE
    #if defined(__WORDSIZE) // Определяется GCC.
        #define CORSAC_PLATFORM_PTR_SIZE ((__WORDSIZE) / 8)
    #elif defined(_WIN64) || defined(__LP64__) || defined(_LP64) || defined(_M_IA64) || defined(__ia64__) || defined(__arch64__) || defined(__aarch64__) || defined(__mips64__) || defined(__64BIT__) || defined(__Ptr_Is_64)
        #define CORSAC_PLATFORM_PTR_SIZE 8
    #elif defined(__CC_ARM) && (__sizeof_ptr == 8)
        #define CORSAC_PLATFORM_PTR_SIZE 8
    #else
        #define CORSAC_PLATFORM_PTR_SIZE 4
    #endif
#endif

// CORSAC_PLATFORM_MIN_MALLOC_ALIGNMENT
// This defines the minimal alignment that the platform's malloc
// implementation will return. This should be used when writing custom
// allocators to ensure that the alignment matches that of malloc
#ifndef CORSAC_PLATFORM_MIN_MALLOC_ALIGNMENT
    #if defined(CORSAC_PLATFORM_APPLE)
        #define CORSAC_PLATFORM_MIN_MALLOC_ALIGNMENT 16
    #elif defined(CORSAC_PLATFORM_ANDROID) && defined(CORSAC_PROCESSOR_ARM)
        #define CORSAC_PLATFORM_MIN_MALLOC_ALIGNMENT 8
    #elif defined(CORSAC_PLATFORM_ANDROID) && defined(CORSAC_PROCESSOR_X86_64)
        #define CORSAC_PLATFORM_MIN_MALLOC_ALIGNMENT 8
    #else
        #define CORSAC_PLATFORM_MIN_MALLOC_ALIGNMENT (CORSAC_PLATFORM_PTR_SIZE * 2)
    #endif
#endif

#endif //CORSAC_STL_PLATFORM_H
