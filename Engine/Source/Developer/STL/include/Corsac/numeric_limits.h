/**
 * corsac::STL
 *
 * numeric_limits.h
 *
 * Created by Falldot on 09.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STL_NUMERIC_LIMITS_H
#define CORSAC_STL_NUMERIC_LIMITS_H

#pragma once

/**
 * Описание (Falldot 20.11.2021)
 *
 * Здесь реализованн шаблон описания арифметических свойств
 * встроенных числовых типов.
 *
 * Для типов:
 * ---------------------------------------------------------------------------------------------
 *      bool, char, signed char, unsigned char,
 *      short, unsigned short, signed short,
 *      int, unsigned int, signed int,
 *      long, unsigned long, signed long,
 *      long long, unsigned long long, signed long long,
 *      float, double, long double, char16_t, char32_t, wchar_t
 *
 * Статические функции и константы:
 * ---------------------------------------------------------------------------------------------
 *      denorm_min	            Возвращает наименьшее ненулевое денормализованное значение.
 *      digits	                Возвращает количество цифр основания системы счисления, которое тип может представлять без потери точности.
 *      digits10	            Возвращает количество дробных десятичных цифр, которых тип может представлять без потери точности.
 *      epsilon	                Возвращает разницу между 1 и наименьшим значением больше 1, которое этот тип данных может представлять.
 *      has_denorm	            Проверяет, допускает ли тип денормализованные значения.
 *      has_denorm_loss	        Проверяет, обнаружена ли потеря точности как потеря денормализации, а не неточный результат.
 *      has_infinity	        Проверяет, может ли тип представлять положительную бесконечность.
 *      has_quiet_NaN	        Проверяет, имеет ли тип представление для скрытого, не являющегося числом (NAN), которое не сообщается.
 *      has_signaling_NaN	    Проверяет, может ли тип представлять сообщения об обнаружении нечисла (NAN).
 *      infinity	            Представление положительной бесконечности для типа, если доступно.
 *      is_bounded	            Проверяет, конечен ли набор значений, представляемый типом.
 *      is_exact	            Проверяет, не содержат ли вычисления с типом ошибок округления.
 *      is_iec559	            Проверяет, соответствует ли тип стандартам IEC 559.
 *      is_integer	            Проверяет, может ли тип представлять целые числа.
 *      is_modulo	            Проверяет, может ли тип представлять модуль.
 *      is_signed	            Проверяет, может ли тип представлять числа со знаком.
 *      is_specialized	        Проверяет, имеет ли тип явную специализацию, определенную в шаблоне класса numeric_limits.
 *      lowest	                Возвращает наибольшее отрицательное конечное значение.
 *      max	                    Возвращает максимальное конечное значение типа.
 *      max_digits10	        Возвращает количество цифр дробной части, необходимых, чтобы у двух различных значений типа были уникальные десятичные представления.
 *      max_exponent	        Возвращает максимальную положительную целую степень, которую тип с плавающей запятой может представить как конечное значение при возведении основания системы счисления в эту степень.
 *      max_exponent10	        Возвращает максимальную положительную целую степень, которую тип с плавающей запятой может представить как конечное значение при возведении десяти в эту степень.
 *      min	                    Возвращает минимальное нормализованное значение для типа.
 *      min_exponent	        Возвращает максимальную отрицательную целую степень, которую тип с плавающей запятой может представить как конечное значение при возведении основания системы счисления в эту степень.
 *      min_exponent10	        Возвращает максимальную отрицательную целую степень, которую тип с плавающей запятой может представить как конечное значение при возведении десяти в эту степень.
 *      quiet_NaN	            Возвращает представление "тихого" нечисла (NaN) для типа.
 *      radix                   Возвращает целочисленное основание системы счисления, используемое для представления типа.
 *      round_error	            Возвращает максимальную ошибку округления для типа.
 *      round_style	            Возвращает значение, описывающее различные методы, которые могут быть выбраны реализацией для округления значения с плавающей запятой до целочисленного.
 *      signaling_NaN	        Возвращает представление обозначения нечисла (NaN) для типа.
 *      tinyness_before	        Проверяет, может ли тип определить, что значение слишком мало для представления в качестве нормализованного значения, до его округления.
 *      traps	                Проверяет, реализованы ли для типа исключения при выполнении арифметических операций.
 */

#include "Corsac/STL/config.h"
#include "Corsac/type_traits.h"

#if defined(CORSAC_LIBRARY_DINKUMWARE) // Dinkumware.
    #include <ymath.h>
#endif

#if CORSAC_NOMINMAX
    #ifdef min
		#undef min
	#endif
	#ifdef max
		#undef max
	#endif
#endif

/**
* CORSAC_CUSTOM_FLOAT_CONSTANTS_REQUIRED
*
* Определяется как 0 или 1.
* Указывает, нужно ли нам определять собственные реализации констант с плавающей запятой inf, nan, snan, denorm.
*/
#if !defined(CORSAC_CUSTOM_FLOAT_CONSTANTS_REQUIRED)
    #if (defined(CORSAC_COMPILER_GCC) || defined(CORSAC_COMPILER_CLANG) && defined(__FLT_MIN__)) || defined(CORSAC_LIBRARY_DINKUMWARE)
        #define CORSAC_CUSTOM_FLOAT_CONSTANTS_REQUIRED 0
    #else
        #define CORSAC_CUSTOM_FLOAT_CONSTANTS_REQUIRED 1
    #endif
#endif


/**
* CORSAC_LIMITS macros
* Они применимы только к целым типам.
*/

// true or false.
#define CORSAC_LIMITS_IS_SIGNED(T)    ((T)(-1) < 0)

// Минимальное возможное значение T.
#define CORSAC_LIMITS_MIN_S(T) ((T)((T)1 << CORSAC_LIMITS_DIGITS_S(T)))
#define CORSAC_LIMITS_MIN_U(T) ((T)0)
#define CORSAC_LIMITS_MIN(T)   ((CORSAC_LIMITS_IS_SIGNED(T) ? CORSAC_LIMITS_MIN_S(T) : CORSAC_LIMITS_MIN_U(T)))

// Максимально возможное значение T.
#define CORSAC_LIMITS_MAX_S(T) ((T)(((((T)1 << (CORSAC_LIMITS_DIGITS(T) - 1)) - 1) << 1) + 1))
#define CORSAC_LIMITS_MAX_U(T) ((T)~(T)0)
#define CORSAC_LIMITS_MAX(T)   ((CORSAC_LIMITS_IS_SIGNED(T) ? CORSAC_LIMITS_MAX_S(T) : CORSAC_LIMITS_MAX_U(T)))

// Количество бит в представлении T.
#define CORSAC_LIMITS_DIGITS(T)   ((CORSAC_LIMITS_IS_SIGNED(T) ? CORSAC_LIMITS_DIGITS_S(T) : CORSAC_LIMITS_DIGITS_U(T)))

// Количество десятичных цифр, которое может быть представлено T.
#define CORSAC_LIMITS_DIGITS10_S(T) ((CORSAC_LIMITS_DIGITS_S(T) * 643L) / 2136) // (643 / 2136) ~= log10(2).
#define CORSAC_LIMITS_DIGITS10_U(T) ((CORSAC_LIMITS_DIGITS_U(T) * 643L) / 2136)
#define CORSAC_LIMITS_DIGITS10(T)   ((CORSAC_LIMITS_IS_SIGNED(T) ? CORSAC_LIMITS_DIGITS10_S(T) : CORSAC_LIMITS_DIGITS10_U(T)))

namespace corsac
{
    enum float_round_style
    {
        round_indeterminate       = -1,    /// Intermediate.
        round_toward_zero         =  0,    /// To zero.
        round_to_nearest          =  1,    /// To the nearest representable value.
        round_toward_infinity     =  2,    /// To infinity.
        round_toward_neg_infinity =  3     /// To negative infinity.
    };

    enum float_denorm_style
    {
        denorm_indeterminate = -1,          /// Нельзя определить, допускает ли тип денормализованные значения.
        denorm_absent        =  0,          /// Тип не допускает денормализованных значений.
        denorm_present       =  1           /// Тип допускает денормализованные значения.
    };


    namespace internal
    {
        // Определяет значения по умолчанию для numeric_limits, которые могут быть переопределены специализациями класса.
        struct numeric_limits_base
        {
            // Значение true, если тип имеет явную специализацию, определенную в классе шаблона; ложь, если нет.
            static constexpr bool is_specialized = false;

            // Целочисленные типы: количество бит в представлении T.
            // Плавающие типы: количество цифр в мантиссе T (то же, что и FLT_MANT_DIG, DBL_MANT_DIG или LDBL_MANT_DIG).
            static constexpr int digits = 0;

            // Количество десятичных цифр, которое может быть представлено T.
            // Эквивалентен FLT_DIG, DBL_DIG или LDBL_DIG для плавающих типов.
            static constexpr int digits10 = 0;

            // Количество десятичных цифр, необходимое для того, чтобы два различных значения типа имели разные десятичные представления.
            static constexpr int max_digits10 = 0;

            // Истинно, если тип подписан.
            static constexpr bool is_signed = false;

            // Истинно, если тип является целым.
            static constexpr bool is_integer = false;

            // стинно, если тип использует точное представление. Все целочисленные типы точны, но могут быть и другие типы.
            static constexpr bool is_exact = false;

            // Целочисленные типы: основа представления. Всегда 2 для целых чисел.
            // лавающие типы: основа экспоненциального представления. Всегда FLT_RADIX (обычно 2) для числа с плавающей запятой.
            static constexpr int radix = 0;

            // Минимальная целая экспонента на основе системы счисления, представляемая типом.
            static constexpr int min_exponent = 0;

            // Минимальная целая экспонента с основанием 10, представляемая типом.
            static constexpr int min_exponent10 = 0;

            // Максимальный целочисленный показатель степени на основе системы счисления, представляемый типом
            static constexpr int max_exponent = 0;

            // Максимальная целая экспонента с основанием 10, представляемая типом.
            static constexpr int max_exponent10 = 0;

            // Истинно, если тип имеет представление для положительной бесконечности.
            static constexpr bool has_infinity = false;

            // Истинно, если тип имеет представление для тихого (не сигнализирующего) NaN.
            static constexpr bool has_quiet_NaN = false;

            // Истинно, если тип имеет представление для сигнализации NaN.
            static constexpr bool has_signaling_NaN = false;

            // Перечисление, которое определяет поведение денормализации.
            // На практике приложение может изменить это во время выполнения с помощью аппаратно-зависимых команд.
            static constexpr float_denorm_style has_denorm = denorm_absent;

            // Истинно, если потеря точности определяется как потеря денормализации.
            // Обычно false для целочисленных типов и true для типов с плавающей запятой.
            static constexpr bool has_denorm_loss = false;

            // Истинно, если тип имеет ограниченный набор представимых значений. Обычно верно для всех
            // встроенных числовых типов (целые числа и числа с плавающей запятой).
            static constexpr bool is_bounded = false;

            // Истинно, если тип имеет представление по модулю (если возможно сложить два
            // положительных числа и получить результат, который преобразуется в третье
            // меньшее число. Обычно верно для целых чисел и ложно для типов с плавающей запятой.
            static constexpr bool is_modulo = false;

            // Истинно, если для этого типа реализован перехват (арифметическая генерация исключения).
            // Обычно верно для целочисленных типов (деление на ноль) и ложно для типов с плавающей запятой,
            // хотя на практике приложение может иметь возможность заменить плавающую точку на ловушку во время выполнения.
            static constexpr bool traps = false;

            // Истинно, если крошечность обнаружена до округления.
            static constexpr bool tinyness_before = false;

            // Перечисление, которое определяет поведение округления по умолчанию.
            // На практике приложение может изменить это во время выполнения с помощью аппаратно-зависимых команд.
            static constexpr float_round_style round_style = round_toward_zero;

            // Истинно, если тип - с плавающей запятой и соответствует стандарту IEC 559 (IEEE 754).
            // На практике приложение или ОС могут изменить это во время выполнения с помощью аппаратно-зависимых команд или путем оптимизации компилятора.
            static constexpr bool is_iec559 = false;
        };

    } // namespace internal

    template<typename T>
    class numeric_limits : public internal::numeric_limits_base
    {
    public:
        using value_type = T;

        static value_type min()
        { return value_type(0); }

        static value_type max()
        { return value_type(0); }

        static value_type lowest()
        { return min(); }

        static value_type epsilon()
        { return value_type(0); }

        static value_type round_error()
        { return value_type(0); }

        static value_type denorm_min()
        { return value_type(0); }

        static value_type infinity()
        { return value_type(0); }

        static value_type quiet_NaN()
        { return value_type(0); }

        static value_type signaling_NaN()
        { return value_type(0); }
    };


    // Const/volatile variations of numeric_limits.
    template<typename T>
    class numeric_limits<const T> : public numeric_limits<T>
    {};

    template<typename T>
    class numeric_limits<volatile T> : public numeric_limits<T>
    {};

    template<typename T>
    class numeric_limits<const volatile T> : public numeric_limits<T>
    {};

    // numeric_limits<bool>
    template<>
    struct numeric_limits<bool>
    {
        typedef bool value_type;

        static constexpr bool               is_specialized    = true;
        static constexpr int                digits            = 1;      // На практике bool хранится как байт, а иногда и как int.
        static constexpr int                digits10          = 0;
        static constexpr int                max_digits10      = 0;
        static constexpr bool               is_signed         = false;  // На практике bool может быть реализован как знаковый char.
        static constexpr bool               is_integer        = true;
        static constexpr bool               is_exact          = true;
        static constexpr int                radix             = 2;
        static constexpr int                min_exponent      = 0;
        static constexpr int                min_exponent10    = 0;
        static constexpr int                max_exponent      = 0;
        static constexpr int                max_exponent10    = 0;
        static constexpr bool               is_bounded        = true;
        static constexpr bool               is_modulo         = false;
        static constexpr bool               traps             = true;   // Это должно быть правдой или ложью? Учитывая, что он реализован аппаратно как целочисленный тип, мы используем true.
        static constexpr bool               tinyness_before   = false;
        static constexpr float_round_style  round_style       = round_toward_zero;
        static constexpr bool               has_infinity      = false;
        static constexpr bool               has_quiet_NaN     = false;
        static constexpr bool               has_signaling_NaN = false;
        static constexpr float_denorm_style has_denorm        = denorm_absent;
        static constexpr bool               has_denorm_loss   = false;
        static constexpr bool               is_iec559         = false;

        static constexpr value_type min()
        { return false; }

        static constexpr value_type max()
        { return true; }

        static constexpr value_type lowest()
        { return false; }

        static constexpr value_type epsilon()
        { return false; }

        static constexpr value_type round_error()
        { return false; }

        static constexpr value_type infinity()
        { return value_type(); }

        static constexpr value_type quiet_NaN()
        { return value_type(); }

        static constexpr value_type signaling_NaN()
        { return value_type(); }

        static constexpr value_type denorm_min()
        { return value_type(); }
    };


    // numeric_limits<char>
    template<>
    struct numeric_limits<char>
    {
        typedef char value_type;

        static constexpr bool               is_specialized    = true;
        static constexpr int                digits            = CORSAC_LIMITS_DIGITS(value_type);
        static constexpr int                digits10          = CORSAC_LIMITS_DIGITS10(value_type);
        static constexpr int                max_digits10      = 0;
        static constexpr bool               is_signed         = CORSAC_LIMITS_IS_SIGNED(value_type);
        static constexpr bool               is_integer        = true;
        static constexpr bool               is_exact          = true;
        static constexpr int                radix             = 2;
        static constexpr int                min_exponent      = 0;
        static constexpr int                min_exponent10    = 0;
        static constexpr int                max_exponent      = 0;
        static constexpr int                max_exponent10    = 0;
        static constexpr bool               is_bounded        = true;
        static constexpr bool               is_modulo         = true;
        static constexpr bool               traps             = true;
        static constexpr bool               tinyness_before   = false;
        static constexpr float_round_style  round_style       = round_toward_zero;
        static constexpr bool               has_infinity      = false;
        static constexpr bool               has_quiet_NaN     = false;
        static constexpr bool               has_signaling_NaN = false;
        static constexpr float_denorm_style has_denorm        = denorm_absent;
        static constexpr bool               has_denorm_loss   = false;
        static constexpr bool               is_iec559         = false;

        static constexpr value_type min()
        { return CORSAC_LIMITS_MIN(value_type); }

        static constexpr value_type max()
        { return CORSAC_LIMITS_MAX(value_type); }

        static constexpr value_type lowest()
        { return CORSAC_LIMITS_MIN(value_type); }

        static constexpr value_type epsilon()
        { return 0; }

        static constexpr value_type round_error()
        { return 0; }

        static constexpr value_type infinity()
        { return value_type(); }    // Вопрос: здесь следует возвращать 0 или value_type ()?

        static constexpr value_type quiet_NaN()
        { return value_type(); }

        static constexpr value_type signaling_NaN()
        { return value_type(); }

        static constexpr value_type denorm_min()
        { return static_cast<value_type>(0); }
    };


    // numeric_limits<unsigned char>
    template<>
    struct numeric_limits<unsigned char>
    {
        typedef unsigned char value_type;

        static constexpr bool               is_specialized    = true;
        static constexpr int                digits            = CORSAC_LIMITS_DIGITS_U(value_type);
        static constexpr int                digits10          = CORSAC_LIMITS_DIGITS10_U(value_type);
        static constexpr int                max_digits10      = 0;
        static constexpr bool               is_signed         = false;
        static constexpr bool               is_integer        = true;
        static constexpr bool               is_exact          = true;
        static constexpr int                radix             = 2;
        static constexpr int                min_exponent      = 0;
        static constexpr int                min_exponent10    = 0;
        static constexpr int                max_exponent      = 0;
        static constexpr int                max_exponent10    = 0;
        static constexpr bool               is_bounded        = true;
        static constexpr bool               is_modulo         = true;
        static constexpr bool               traps             = true;
        static constexpr bool               tinyness_before   = false;
        static constexpr float_round_style  round_style       = round_toward_zero;
        static constexpr bool               has_infinity      = false;
        static constexpr bool               has_quiet_NaN     = false;
        static constexpr bool               has_signaling_NaN = false;
        static constexpr float_denorm_style has_denorm        = denorm_absent;
        static constexpr bool               has_denorm_loss   = false;
        static constexpr bool               is_iec559         = false;

        static constexpr value_type min()
        { return 0; }

        static constexpr value_type max()
        { return CORSAC_LIMITS_MAX_U(value_type); }

        static constexpr value_type lowest()
        { return 0; }

        static constexpr value_type epsilon()
        { return 0; }

        static constexpr value_type round_error()
        { return 0; }

        static constexpr value_type infinity()
        { return value_type(); }

        static constexpr value_type quiet_NaN()
        { return value_type(); }

        static constexpr value_type signaling_NaN()
        { return value_type(); }

        static constexpr value_type denorm_min()
        { return static_cast<value_type>(0); }
    };


    // numeric_limits<signed char>
    template<>
    struct numeric_limits<signed char>
    {
        typedef signed char value_type;

        static constexpr bool               is_specialized    = true;
        static constexpr int                digits            = CORSAC_LIMITS_DIGITS_S(value_type);
        static constexpr int                digits10          = CORSAC_LIMITS_DIGITS10_S(value_type);
        static constexpr int                max_digits10      = 0;
        static constexpr bool               is_signed         = true;
        static constexpr bool               is_integer        = true;
        static constexpr bool               is_exact          = true;
        static constexpr int                radix             = 2;
        static constexpr int                min_exponent      = 0;
        static constexpr int                min_exponent10    = 0;
        static constexpr int                max_exponent      = 0;
        static constexpr int                max_exponent10    = 0;
        static constexpr bool               is_bounded        = true;
        static constexpr bool               is_modulo         = true;
        static constexpr bool               traps             = true;
        static constexpr bool               tinyness_before   = false;
        static constexpr float_round_style  round_style       = round_toward_zero;
        static constexpr bool               has_infinity      = false;
        static constexpr bool               has_quiet_NaN     = false;
        static constexpr bool               has_signaling_NaN = false;
        static constexpr float_denorm_style has_denorm        = denorm_absent;
        static constexpr bool               has_denorm_loss   = false;
        static constexpr bool               is_iec559         = false;

        static constexpr value_type min()
        { return CORSAC_LIMITS_MIN_S(value_type); }

        static constexpr value_type max()
        { return CORSAC_LIMITS_MAX_S(value_type); }

        static constexpr value_type lowest()
        { return CORSAC_LIMITS_MIN_S(value_type); }

        static constexpr value_type epsilon()
        { return 0; }

        static constexpr value_type round_error()
        { return 0; }

        static constexpr value_type infinity()
        { return value_type(); }

        static constexpr value_type quiet_NaN()
        { return value_type(); }

        static constexpr value_type signaling_NaN()
        { return value_type(); }

        static constexpr value_type denorm_min()
        { return static_cast<value_type>(0); }
    };

    // numeric_limits<wchar_t>
    // VC++ имеет возможность сделать wchar_t просто беззнаковым коротким. Если это включено,
    // приведенный ниже код может вызвать сбои компиляции из-за избыточности.  Лучшим решением может
    // быть использование __wchar_t здесь для VC ++ вместо wchar_t, поскольку __wchar_t всегда является
    // истинно уникальным типом в VC++.
    template<>
    struct numeric_limits<wchar_t>
    {
        typedef wchar_t value_type;

        static constexpr bool               is_specialized    = true;
        static constexpr int                digits            = CORSAC_LIMITS_DIGITS(value_type);
        static constexpr int                digits10          = CORSAC_LIMITS_DIGITS10(value_type);
        static constexpr int                max_digits10      = 0;
        static constexpr bool               is_signed         = CORSAC_LIMITS_IS_SIGNED(value_type);
        static constexpr bool               is_integer        = true;
        static constexpr bool               is_exact          = true;
        static constexpr int                radix             = 2;
        static constexpr int                min_exponent      = 0;
        static constexpr int                min_exponent10    = 0;
        static constexpr int                max_exponent      = 0;
        static constexpr int                max_exponent10    = 0;
        static constexpr bool               is_bounded        = true;
        static constexpr bool               is_modulo         = true;
        static constexpr bool               traps             = true;
        static constexpr bool               tinyness_before   = false;
        static constexpr float_round_style  round_style       = round_toward_zero;
        static constexpr bool               has_infinity      = false;
        static constexpr bool               has_quiet_NaN     = false;
        static constexpr bool               has_signaling_NaN = false;
        static constexpr float_denorm_style has_denorm        = denorm_absent;
        static constexpr bool               has_denorm_loss   = false;
        static constexpr bool               is_iec559         = false;

        static constexpr value_type min()
        { return CORSAC_LIMITS_MIN(value_type); }

        static constexpr value_type max()
        { return CORSAC_LIMITS_MAX(value_type); }

        static constexpr value_type lowest()
        { return CORSAC_LIMITS_MIN(value_type); }

        static constexpr value_type epsilon()
        { return 0; }

        static constexpr value_type round_error()
        { return 0; }

        static constexpr value_type infinity()
        { return value_type(); }

        static constexpr value_type quiet_NaN()
        { return value_type(); }

        static constexpr value_type signaling_NaN()
        { return value_type(); }

        static constexpr value_type denorm_min()
        { return static_cast<value_type>(0); }
    };

    // numeric_limits<char16_t>
		template<>
		struct numeric_limits<char16_t>
		{
			typedef char16_t value_type;

			static constexpr bool               is_specialized    = true;
			static constexpr int                digits            = CORSAC_LIMITS_DIGITS(value_type);
			static constexpr int                digits10          = CORSAC_LIMITS_DIGITS10(value_type);
			static constexpr int                max_digits10      = 0;
			static constexpr bool               is_signed         = CORSAC_LIMITS_IS_SIGNED(value_type);
			static constexpr bool               is_integer        = true;
			static constexpr bool               is_exact          = true;
			static constexpr int                radix             = 2;
			static constexpr int                min_exponent      = 0;
			static constexpr int                min_exponent10    = 0;
			static constexpr int                max_exponent      = 0;
			static constexpr int                max_exponent10    = 0;
			static constexpr bool               is_bounded        = true;
			static constexpr bool               is_modulo         = true;
			static constexpr bool               traps             = true;
			static constexpr bool               tinyness_before   = false;
			static constexpr float_round_style  round_style       = round_toward_zero;
			static constexpr bool               has_infinity      = false;
			static constexpr bool               has_quiet_NaN     = false;
			static constexpr bool               has_signaling_NaN = false;
			static constexpr float_denorm_style has_denorm        = denorm_absent;
			static constexpr bool               has_denorm_loss   = false;
			static constexpr bool               is_iec559         = false;

			static constexpr value_type min()
				{ return CORSAC_LIMITS_MIN(value_type); }

			static constexpr value_type max()
				{ return CORSAC_LIMITS_MAX(value_type); }

			static constexpr value_type lowest()
				{ return CORSAC_LIMITS_MIN(value_type); }

			static constexpr value_type epsilon()
				{ return 0; }

			static constexpr value_type round_error()
				{ return 0; }

			static constexpr value_type infinity()
				{ return value_type(); }

			static constexpr value_type quiet_NaN()
				{ return value_type(); }

			static constexpr value_type signaling_NaN()
				{ return value_type(); }

			static constexpr value_type denorm_min()
				{ return static_cast<value_type>(0); }
		};

    // numeric_limits<char32_t>
		template<>
		struct numeric_limits<char32_t>
		{
			typedef char32_t value_type;

			static constexpr bool               is_specialized    = true;
			static constexpr int                digits            = CORSAC_LIMITS_DIGITS(value_type);
			static constexpr int                digits10          = CORSAC_LIMITS_DIGITS10(value_type);
			static constexpr int                max_digits10      = 0;
			static constexpr bool               is_signed         = CORSAC_LIMITS_IS_SIGNED(value_type);
			static constexpr bool               is_integer        = true;
			static constexpr bool               is_exact          = true;
			static constexpr int                radix             = 2;
			static constexpr int                min_exponent      = 0;
			static constexpr int                min_exponent10    = 0;
			static constexpr int                max_exponent      = 0;
			static constexpr int                max_exponent10    = 0;
			static constexpr bool               is_bounded        = true;
			static constexpr bool               is_modulo         = true;
			static constexpr bool               traps             = true;
			static constexpr bool               tinyness_before   = false;
			static constexpr float_round_style  round_style       = round_toward_zero;
			static constexpr bool               has_infinity      = false;
			static constexpr bool               has_quiet_NaN     = false;
			static constexpr bool               has_signaling_NaN = false;
			static constexpr float_denorm_style has_denorm        = denorm_absent;
			static constexpr bool               has_denorm_loss   = false;
			static constexpr bool               is_iec559         = false;

			static constexpr value_type min()
				{ return CORSAC_LIMITS_MIN(value_type); }

			static constexpr value_type max()
				{ return CORSAC_LIMITS_MAX(value_type); }

			static constexpr value_type lowest()
				{ return CORSAC_LIMITS_MIN(value_type); }

			static constexpr value_type epsilon()
				{ return 0; }

			static constexpr value_type round_error()
				{ return 0; }

			static constexpr value_type infinity()
				{ return value_type(); }

			static constexpr value_type quiet_NaN()
				{ return value_type(); }

			static constexpr value_type signaling_NaN()
				{ return value_type(); }

			static constexpr value_type denorm_min()
				{ return static_cast<value_type>(0); }
		};

    // numeric_limits<unsigned short>
    template<>
    struct numeric_limits<unsigned short>
    {
        typedef unsigned short value_type;

        static constexpr bool               is_specialized    = true;
        static constexpr int                digits            = CORSAC_LIMITS_DIGITS_U(value_type);
        static constexpr int                digits10          = CORSAC_LIMITS_DIGITS10_U(value_type);
        static constexpr int                max_digits10      = 0;
        static constexpr bool               is_signed         = false;
        static constexpr bool               is_integer        = true;
        static constexpr bool               is_exact          = true;
        static constexpr int                radix             = 2;
        static constexpr int                min_exponent      = 0;
        static constexpr int                min_exponent10    = 0;
        static constexpr int                max_exponent      = 0;
        static constexpr int                max_exponent10    = 0;
        static constexpr bool               is_bounded        = true;
        static constexpr bool               is_modulo         = true;
        static constexpr bool               traps             = true;
        static constexpr bool               tinyness_before   = false;
        static constexpr float_round_style  round_style       = round_toward_zero;
        static constexpr bool               has_infinity      = false;
        static constexpr bool               has_quiet_NaN     = false;
        static constexpr bool               has_signaling_NaN = false;
        static constexpr float_denorm_style has_denorm        = denorm_absent;
        static constexpr bool               has_denorm_loss   = false;
        static constexpr bool               is_iec559         = false;

        static constexpr value_type min()
        { return 0; }

        static constexpr value_type max()
        { return CORSAC_LIMITS_MAX_U(value_type); }

        static constexpr value_type lowest()
        { return 0; }

        static constexpr value_type epsilon()
        { return 0; }

        static constexpr value_type round_error()
        { return 0; }

        static constexpr value_type infinity()
        { return value_type(); }

        static constexpr value_type quiet_NaN()
        { return value_type(); }

        static constexpr value_type signaling_NaN()
        { return value_type(); }

        static constexpr value_type denorm_min()
        { return static_cast<value_type>(0); }
    };


    // numeric_limits<signed short>
    template<>
    struct numeric_limits<signed short>
    {
        typedef signed short value_type;

        static constexpr bool               is_specialized    = true;
        static constexpr int                digits            = CORSAC_LIMITS_DIGITS_S(value_type);
        static constexpr int                digits10          = CORSAC_LIMITS_DIGITS10_S(value_type);
        static constexpr int                max_digits10      = 0;
        static constexpr bool               is_signed         = true;
        static constexpr bool               is_integer        = true;
        static constexpr bool               is_exact          = true;
        static constexpr int                radix             = 2;
        static constexpr int                min_exponent      = 0;
        static constexpr int                min_exponent10    = 0;
        static constexpr int                max_exponent      = 0;
        static constexpr int                max_exponent10    = 0;
        static constexpr bool               is_bounded        = true;
        static constexpr bool               is_modulo         = true;
        static constexpr bool               traps             = true;
        static constexpr bool               tinyness_before   = false;
        static constexpr float_round_style  round_style       = round_toward_zero;
        static constexpr bool               has_infinity      = false;
        static constexpr bool               has_quiet_NaN     = false;
        static constexpr bool               has_signaling_NaN = false;
        static constexpr float_denorm_style has_denorm        = denorm_absent;
        static constexpr bool               has_denorm_loss   = false;
        static constexpr bool               is_iec559         = false;

        static constexpr value_type min()
        { return CORSAC_LIMITS_MIN_S(value_type); }

        static constexpr value_type max()
        { return CORSAC_LIMITS_MAX_S(value_type); }

        static constexpr value_type lowest()
        { return CORSAC_LIMITS_MIN_S(value_type); }

        static constexpr value_type epsilon()
        { return 0; }

        static constexpr value_type round_error()
        { return 0; }

        static constexpr value_type infinity()
        { return value_type(); }

        static constexpr value_type quiet_NaN()
        { return value_type(); }

        static constexpr value_type signaling_NaN()
        { return value_type(); }

        static constexpr value_type denorm_min()
        { return static_cast<value_type>(0); }
    };

    // numeric_limits<unsigned int>
    template<>
    struct numeric_limits<unsigned int>
    {
        typedef unsigned int value_type;

        static constexpr bool               is_specialized    = true;
        static constexpr int                digits            = CORSAC_LIMITS_DIGITS_U(value_type);
        static constexpr int                digits10          = CORSAC_LIMITS_DIGITS10_U(value_type);
        static constexpr int                max_digits10      = 0;
        static constexpr bool               is_signed         = false;
        static constexpr bool               is_integer        = true;
        static constexpr bool               is_exact          = true;
        static constexpr int                radix             = 2;
        static constexpr int                min_exponent      = 0;
        static constexpr int                min_exponent10    = 0;
        static constexpr int                max_exponent      = 0;
        static constexpr int                max_exponent10    = 0;
        static constexpr bool               is_bounded        = true;
        static constexpr bool               is_modulo         = true;
        static constexpr bool               traps             = true;
        static constexpr bool               tinyness_before   = false;
        static constexpr float_round_style  round_style       = round_toward_zero;
        static constexpr bool               has_infinity      = false;
        static constexpr bool               has_quiet_NaN     = false;
        static constexpr bool               has_signaling_NaN = false;
        static constexpr float_denorm_style has_denorm        = denorm_absent;
        static constexpr bool               has_denorm_loss   = false;
        static constexpr bool               is_iec559         = false;

        static constexpr value_type min()
        { return 0; }

        static constexpr value_type max()
        { return CORSAC_LIMITS_MAX_U(value_type); }

        static constexpr value_type lowest()
        { return 0; }

        static constexpr value_type epsilon()
        { return 0; }

        static constexpr value_type round_error()
        { return 0; }

        static constexpr value_type infinity()
        { return value_type(); }

        static constexpr value_type quiet_NaN()
        { return value_type(); }

        static constexpr value_type signaling_NaN()
        { return value_type(); }

        static constexpr value_type denorm_min()
        { return static_cast<value_type>(0); }
    };

    // numeric_limits<signed int>
    template<>
    struct numeric_limits<signed int>
    {
        typedef signed int value_type;

        static constexpr bool               is_specialized    = true;
        static constexpr int                digits            = CORSAC_LIMITS_DIGITS_S(value_type);
        static constexpr int                digits10          = CORSAC_LIMITS_DIGITS10_S(value_type);
        static constexpr int                max_digits10      = 0;
        static constexpr bool               is_signed         = true;
        static constexpr bool               is_integer        = true;
        static constexpr bool               is_exact          = true;
        static constexpr int                radix             = 2;
        static constexpr int                min_exponent      = 0;
        static constexpr int                min_exponent10    = 0;
        static constexpr int                max_exponent      = 0;
        static constexpr int                max_exponent10    = 0;
        static constexpr bool               is_bounded        = true;
        static constexpr bool               is_modulo         = true;
        static constexpr bool               traps             = true;
        static constexpr bool               tinyness_before   = false;
        static constexpr float_round_style  round_style       = round_toward_zero;
        static constexpr bool               has_infinity      = false;
        static constexpr bool               has_quiet_NaN     = false;
        static constexpr bool               has_signaling_NaN = false;
        static constexpr float_denorm_style has_denorm        = denorm_absent;
        static constexpr bool               has_denorm_loss   = false;
        static constexpr bool               is_iec559         = false;

        static constexpr value_type min()
        { return INT_MIN; } // It's hard to get CORSAC_LIMITS_MIN_S to work with all compilers here.

        static constexpr value_type max()
        { return CORSAC_LIMITS_MAX_S(value_type); }

        static constexpr value_type lowest()
        { return INT_MIN; }

        static constexpr value_type epsilon()
        { return 0; }

        static constexpr value_type round_error()
        { return 0; }

        static constexpr value_type infinity()
        { return value_type(); }

        static constexpr value_type quiet_NaN()
        { return value_type(); }

        static constexpr value_type signaling_NaN()
        { return value_type(); }

        static constexpr value_type denorm_min()
        { return static_cast<value_type>(0); }
    };

    // numeric_limits<unsigned long>
    template<>
    struct numeric_limits<unsigned long>
    {
        typedef unsigned long value_type;

        static constexpr bool               is_specialized    = true;
        static constexpr int                digits            = CORSAC_LIMITS_DIGITS_U(value_type);
        static constexpr int                digits10          = CORSAC_LIMITS_DIGITS10_U(value_type);
        static constexpr int                max_digits10      = 0;
        static constexpr bool               is_signed         = false;
        static constexpr bool               is_integer        = true;
        static constexpr bool               is_exact          = true;
        static constexpr int                radix             = 2;
        static constexpr int                min_exponent      = 0;
        static constexpr int                min_exponent10    = 0;
        static constexpr int                max_exponent      = 0;
        static constexpr int                max_exponent10    = 0;
        static constexpr bool               is_bounded        = true;
        static constexpr bool               is_modulo         = true;
        static constexpr bool               traps             = true;
        static constexpr bool               tinyness_before   = false;
        static constexpr float_round_style  round_style       = round_toward_zero;
        static constexpr bool               has_infinity      = false;
        static constexpr bool               has_quiet_NaN     = false;
        static constexpr bool               has_signaling_NaN = false;
        static constexpr float_denorm_style has_denorm        = denorm_absent;
        static constexpr bool               has_denorm_loss   = false;
        static constexpr bool               is_iec559         = false;

        static constexpr value_type min()
        { return 0; }

        static constexpr value_type max()
        { return CORSAC_LIMITS_MAX_U(value_type); }

        static constexpr value_type lowest()
        { return 0; }

        static constexpr value_type epsilon()
        { return 0; }

        static constexpr value_type round_error()
        { return 0; }

        static constexpr value_type infinity()
        { return value_type(); }

        static constexpr value_type quiet_NaN()
        { return value_type(); }

        static constexpr value_type signaling_NaN()
        { return value_type(); }

        static constexpr value_type denorm_min()
        { return static_cast<value_type>(0); }
    };

    // numeric_limits<signed long>
    template<>
    struct numeric_limits<signed long>
    {
        typedef signed long value_type;

        static constexpr bool               is_specialized    = true;
        static constexpr int                digits            = CORSAC_LIMITS_DIGITS_S(value_type);
        static constexpr int                digits10          = CORSAC_LIMITS_DIGITS10_S(value_type);
        static constexpr int                max_digits10      = 0;
        static constexpr bool               is_signed         = true;
        static constexpr bool               is_integer        = true;
        static constexpr bool               is_exact          = true;
        static constexpr int                radix             = 2;
        static constexpr int                min_exponent      = 0;
        static constexpr int                min_exponent10    = 0;
        static constexpr int                max_exponent      = 0;
        static constexpr int                max_exponent10    = 0;
        static constexpr bool               is_bounded        = true;
        static constexpr bool               is_modulo         = true;
        static constexpr bool               traps             = true;
        static constexpr bool               tinyness_before   = false;
        static constexpr float_round_style  round_style       = round_toward_zero;
        static constexpr bool               has_infinity      = false;
        static constexpr bool               has_quiet_NaN     = false;
        static constexpr bool               has_signaling_NaN = false;
        static constexpr float_denorm_style has_denorm        = denorm_absent;
        static constexpr bool               has_denorm_loss   = false;
        static constexpr bool               is_iec559         = false;

        static constexpr value_type min()
        { return LONG_MIN; }  // It's hard to get CORSAC_LIMITS_MIN_S to work with all compilers here.

        static constexpr value_type max()
        { return CORSAC_LIMITS_MAX_S(value_type); }

        static constexpr value_type lowest()
        { return LONG_MIN; }

        static constexpr value_type epsilon()
        { return 0; }

        static constexpr value_type round_error()
        { return 0; }

        static constexpr value_type infinity()
        { return value_type(); }

        static constexpr value_type quiet_NaN()
        { return value_type(); }

        static constexpr value_type signaling_NaN()
        { return value_type(); }

        static constexpr value_type denorm_min()
        { return static_cast<value_type>(0); }
    };

    // numeric_limits<unsigned long long>
    template<>
    struct numeric_limits<unsigned long long>
    {
        typedef unsigned long long value_type;

        static constexpr bool               is_specialized    = true;
        static constexpr int                digits            = CORSAC_LIMITS_DIGITS_U(value_type);
        static constexpr int                digits10          = CORSAC_LIMITS_DIGITS10_U(value_type);
        static constexpr int                max_digits10      = 0;
        static constexpr bool               is_signed         = false;
        static constexpr bool               is_integer        = true;
        static constexpr bool               is_exact          = true;
        static constexpr int                radix             = 2;
        static constexpr int                min_exponent      = 0;
        static constexpr int                min_exponent10    = 0;
        static constexpr int                max_exponent      = 0;
        static constexpr int                max_exponent10    = 0;
        static constexpr bool               is_bounded        = true;
        static constexpr bool               is_modulo         = true;
        static constexpr bool               traps             = true;
        static constexpr bool               tinyness_before   = false;
        static constexpr float_round_style  round_style       = round_toward_zero;
        static constexpr bool               has_infinity      = false;
        static constexpr bool               has_quiet_NaN     = false;
        static constexpr bool               has_signaling_NaN = false;
        static constexpr float_denorm_style has_denorm        = denorm_absent;
        static constexpr bool               has_denorm_loss   = false;
        static constexpr bool               is_iec559         = false;

        static constexpr value_type min()
        { return 0; }

        static constexpr value_type max()
        { return CORSAC_LIMITS_MAX_U(value_type); }

        static constexpr value_type lowest()
        { return 0; }

        static constexpr value_type epsilon()
        { return 0; }

        static constexpr value_type round_error()
        { return 0; }

        static constexpr value_type infinity()
        { return value_type(); }

        static constexpr value_type quiet_NaN()
        { return value_type(); }

        static constexpr value_type signaling_NaN()
        { return value_type(); }

        static constexpr value_type denorm_min()
        { return static_cast<value_type>(0); }
    };

    // numeric_limits<signed long long>
    template<>
    struct numeric_limits<signed long long>
    {
        typedef signed long long value_type;

        static constexpr bool               is_specialized    = true;
        static constexpr int                digits            = CORSAC_LIMITS_DIGITS_S(value_type);
        static constexpr int                digits10          = CORSAC_LIMITS_DIGITS10_S(value_type);
        static constexpr int                max_digits10      = 0;
        static constexpr bool               is_signed         = true;
        static constexpr bool               is_integer        = true;
        static constexpr bool               is_exact          = true;
        static constexpr int                radix             = 2;
        static constexpr int                min_exponent      = 0;
        static constexpr int                min_exponent10    = 0;
        static constexpr int                max_exponent      = 0;
        static constexpr int                max_exponent10    = 0;
        static constexpr bool               is_bounded        = true;
        static constexpr bool               is_modulo         = true;
        static constexpr bool               traps             = true;
        static constexpr bool               tinyness_before   = false;
        static constexpr float_round_style  round_style       = round_toward_zero;
        static constexpr bool               has_infinity      = false;
        static constexpr bool               has_quiet_NaN     = false;
        static constexpr bool               has_signaling_NaN = false;
        static constexpr float_denorm_style has_denorm        = denorm_absent;
        static constexpr bool               has_denorm_loss   = false;
        static constexpr bool               is_iec559         = false;

        static constexpr value_type min()
        { return CORSAC_LIMITS_MIN_S(value_type); }

        static constexpr value_type max()
        { return CORSAC_LIMITS_MAX_S(value_type); }

        static constexpr value_type lowest()
        { return CORSAC_LIMITS_MIN_S(value_type); }

        static constexpr value_type epsilon()
        { return 0; }

        static constexpr value_type round_error()
        { return 0; }

        static constexpr value_type infinity()
        { return value_type(); }

        static constexpr value_type quiet_NaN()
        { return value_type(); }

        static constexpr value_type signaling_NaN()
        { return value_type(); }

        static constexpr value_type denorm_min()
        { return static_cast<value_type>(0); }
    };


#if (CORSAC_COMPILER_INTMAX_SIZE >= 16) && (defined(CORSAC_COMPILER_GNUC) || defined(CORSAC_COMPILER_CLANG))
    // numeric_limits<__uint128_t>
		template<>
		struct numeric_limits<__uint128_t>
		{
			typedef __uint128_t value_type;

			static constexpr bool               is_specialized    = true;
			static constexpr int                digits            = CORSAC_LIMITS_DIGITS_U(value_type);
			static constexpr int                digits10          = CORSAC_LIMITS_DIGITS10_U(value_type);
			static constexpr int                max_digits10      = 0;
			static constexpr bool               is_signed         = false;
			static constexpr bool               is_integer        = true;
			static constexpr bool               is_exact          = true;
			static constexpr int                radix             = 2;
			static constexpr int                min_exponent      = 0;
			static constexpr int                min_exponent10    = 0;
			static constexpr int                max_exponent      = 0;
			static constexpr int                max_exponent10    = 0;
			static constexpr bool               is_bounded        = true;
			static constexpr bool               is_modulo         = true;
			static constexpr bool               traps             = true;
			static constexpr bool               tinyness_before   = false;
			static constexpr float_round_style  round_style       = round_toward_zero;
			static constexpr bool               has_infinity      = false;
			static constexpr bool               has_quiet_NaN     = false;
			static constexpr bool               has_signaling_NaN = false;
			static constexpr float_denorm_style has_denorm        = denorm_absent;
			static constexpr bool               has_denorm_loss   = false;
			static constexpr bool               is_iec559         = false;

			static constexpr value_type min()
				{ return 0; }

			static constexpr value_type max()
				{ return CORSAC_LIMITS_MAX_U(value_type); }

			static constexpr value_type lowest()
				{ return 0; }

			static constexpr value_type epsilon()
				{ return 0; }

			static constexpr value_type round_error()
				{ return 0; }

			static constexpr value_type infinity()
				{ return value_type(); }

			static constexpr value_type quiet_NaN()
				{ return value_type(); }

			static constexpr value_type signaling_NaN()
				{ return value_type(); }

			static constexpr value_type denorm_min()
				{ return static_cast<value_type>(0); }
		};


		// numeric_limits<__int128_t>
		template<>
		struct numeric_limits<__int128_t>
		{
			typedef __int128_t value_type;

			static constexpr bool               is_specialized    = true;
			static constexpr int                digits            = CORSAC_LIMITS_DIGITS_S(value_type);
			static constexpr int                digits10          = CORSAC_LIMITS_DIGITS10_S(value_type);
			static constexpr int                max_digits10      = 0;
			static constexpr bool               is_signed         = true;
			static constexpr bool               is_integer        = true;
			static constexpr bool               is_exact          = true;
			static constexpr int                radix             = 2;
			static constexpr int                min_exponent      = 0;
			static constexpr int                min_exponent10    = 0;
			static constexpr int                max_exponent      = 0;
			static constexpr int                max_exponent10    = 0;
			static constexpr bool               is_bounded        = true;
			static constexpr bool               is_modulo         = true;
			static constexpr bool               traps             = true;
			static constexpr bool               tinyness_before   = false;
			static constexpr float_round_style  round_style       = round_toward_zero;
			static constexpr bool               has_infinity      = false;
			static constexpr bool               has_quiet_NaN     = false;
			static constexpr bool               has_signaling_NaN = false;
			static constexpr float_denorm_style has_denorm        = denorm_absent;
			static constexpr bool               has_denorm_loss   = false;
			static constexpr bool               is_iec559         = false;

			static constexpr value_type min()
				{ return CORSAC_LIMITS_MIN_S(value_type); }

			static constexpr value_type max()
				{ return CORSAC_LIMITS_MAX_S(value_type); }

			static constexpr value_type lowest()
				{ return CORSAC_LIMITS_MIN_S(value_type); }

			static constexpr value_type epsilon()
				{ return 0; }

			static constexpr value_type round_error()
				{ return 0; }

			static constexpr value_type infinity()
				{ return value_type(); }

			static constexpr value_type quiet_NaN()
				{ return value_type(); }

			static constexpr value_type signaling_NaN()
				{ return value_type(); }

			static constexpr value_type denorm_min()
				{ return static_cast<value_type>(0); }
		};
#endif

    // numeric_limits<float>
    template<>
    struct numeric_limits<float>
    {
        typedef float value_type;

        static constexpr bool               is_specialized    = true;
        static constexpr int                digits            = FLT_MANT_DIG;
        static constexpr int                digits10          = FLT_DIG;
        static constexpr int                max_digits10      = FLT_MANT_DIG;
        static constexpr bool               is_signed         = true;
        static constexpr bool               is_integer        = false;
        static constexpr bool               is_exact          = false;
        static constexpr int                radix             = FLT_RADIX;
        static constexpr int                min_exponent      = FLT_MIN_EXP;
        static constexpr int                min_exponent10    = FLT_MIN_10_EXP;
        static constexpr int                max_exponent      = FLT_MAX_EXP;
        static constexpr int                max_exponent10    = FLT_MAX_10_EXP;
        static constexpr bool               is_bounded        = true;
        static constexpr bool               is_modulo         = false;
        static constexpr bool               traps             = true;
        static constexpr bool               tinyness_before   = false;
        static constexpr float_round_style  round_style       = round_to_nearest;
        static constexpr bool               has_infinity      = true;
        static constexpr bool               has_quiet_NaN     = true;                   // Это может быть неправильным для некоторых платформ.
        static constexpr bool               has_signaling_NaN = true;                   // Это может быть неправильным для некоторых платформ.
        static constexpr float_denorm_style has_denorm        = denorm_present;         // Это может быть неправильным для некоторых платформ.
        static constexpr bool               has_denorm_loss   = false;                  // Это может быть неправильным для некоторых платформ.
        static constexpr bool               is_iec559         = has_infinity && has_quiet_NaN && (has_denorm == denorm_present);

#if CORSAC_CUSTOM_FLOAT_CONSTANTS_REQUIRED
        static value_type min()
        { return FLT_MIN; }

        static value_type max()
        { return FLT_MAX; }

        static value_type lowest()
        { return -FLT_MAX; }

        static value_type epsilon()
        { return FLT_EPSILON; }

        static value_type round_error()
        { return 0.5f; }

        static value_type infinity()
        { return internal::gFloatInfinity; }

        static value_type quiet_NaN()
        { return internal::gFloatNaN; }

        static value_type signaling_NaN()
        { return internal::gFloatSNaN; }

        static value_type denorm_min()
        { return internal::gFloatDenorm; }

#elif (defined(CORSAC_COMPILER_GNUC) || defined(CORSAC_COMPILER_CLANG)) && defined(__FLT_MIN__)
        static constexpr value_type min()
				{ return __FLT_MIN__; }

			static constexpr value_type max()
				{ return __FLT_MAX__; }

			static constexpr value_type lowest()
				{ return -__FLT_MAX__; }

			static constexpr value_type epsilon()
				{ return __FLT_EPSILON__; }

			static constexpr value_type round_error()
				{ return 0.5f; }

			static constexpr value_type infinity()
				{ return __builtin_huge_valf(); }

			static constexpr value_type quiet_NaN()
				{ return __builtin_nanf(""); }

			static constexpr value_type signaling_NaN()
				{ return __builtin_nansf(""); }

			static constexpr value_type denorm_min()
				{ return __FLT_DENORM_MIN__; }

#elif defined(CORSAC_LIBRARY_DINKUMWARE) // При использовании стандартной библиотеки Dinkumware ...
        static value_type min()
				{ return FLT_MIN; }

			static value_type max()
				{ return FLT_MAX; }

			static value_type lowest()
				{ return -FLT_MAX; }

			static value_type epsilon()
				{ return FLT_EPSILON; }

			static value_type round_error()
				{ return 0.5f; }

			static value_type infinity()
				{ return _CSTD _FInf._Float; }

			static value_type quiet_NaN()
				{ return _CSTD _FNan._Float; }

			static value_type signaling_NaN()
				{ return _CSTD _FSnan._Float; }

			static value_type denorm_min()
				{ return _CSTD _FDenorm._Float; }
#endif
    };

    // numeric_limits<double>
    template<>
    struct numeric_limits<double>
    {
        typedef double value_type;

        static constexpr bool               is_specialized    = true;
        static constexpr int                digits            = DBL_MANT_DIG;
        static constexpr int                digits10          = DBL_DIG;
        static constexpr int                max_digits10      = DBL_MANT_DIG;
        static constexpr bool               is_signed         = true;
        static constexpr bool               is_integer        = false;
        static constexpr bool               is_exact          = false;
        static constexpr int                radix             = FLT_RADIX;              // FLT_RADIX применяется ко всем типам с плавающей запятой.
        static constexpr int                min_exponent      = DBL_MIN_EXP;
        static constexpr int                min_exponent10    = DBL_MIN_10_EXP;
        static constexpr int                max_exponent      = DBL_MAX_EXP;
        static constexpr int                max_exponent10    = DBL_MAX_10_EXP;
        static constexpr bool               is_bounded        = true;
        static constexpr bool               is_modulo         = false;
        static constexpr bool               traps             = true;
        static constexpr bool               tinyness_before   = false;
        static constexpr float_round_style  round_style       = round_to_nearest;
        static constexpr bool               has_infinity      = true;
        static constexpr bool               has_quiet_NaN     = true;                   // Это может быть неправильным для некоторых платформ.
        static constexpr bool               has_signaling_NaN = true;                   // Это может быть неправильным для некоторых платформ.
        static constexpr float_denorm_style has_denorm        = denorm_present;         // Это может быть неправильным для некоторых платформ.
        static constexpr bool               has_denorm_loss   = false;                  // Это может быть неправильным для некоторых платформ.
        static constexpr bool               is_iec559         = has_infinity && has_quiet_NaN && (has_denorm == denorm_present);

#if CORSAC_CUSTOM_FLOAT_CONSTANTS_REQUIRED
        static value_type min()
        { return DBL_MIN; }

        static value_type max()
        { return DBL_MAX; }

        static value_type lowest()
        { return -DBL_MAX; }

        static value_type epsilon()
        { return DBL_EPSILON; }

        static value_type round_error()
        { return 0.5f; }

        static value_type infinity()
        { return internal::gDoubleInfinity; }

        static value_type quiet_NaN()
        { return internal::gDoubleNaN; }

        static value_type signaling_NaN()
        { return internal::gDoubleSNaN; }

        static value_type denorm_min()
        { return internal::gDoubleDenorm; }

#elif (defined(CORSAC_COMPILER_GNUC) || defined(CORSAC_COMPILER_CLANG)) && defined(__DBL_MIN__)
        static constexpr value_type min()
				{ return __DBL_MIN__; }

			static constexpr value_type max()
				{ return __DBL_MAX__; }

			static constexpr value_type lowest()
				{ return -__DBL_MAX__; }

			static constexpr value_type epsilon()
				{ return __DBL_EPSILON__; }

			static constexpr value_type round_error()
				{ return 0.5f; }

			static constexpr value_type infinity()
				{ return __builtin_huge_val(); }

			static constexpr value_type quiet_NaN()
				{ return __builtin_nan(""); }

			static constexpr value_type signaling_NaN()
				{ return __builtin_nans(""); }

			static constexpr value_type denorm_min()
				{ return __DBL_DENORM_MIN__; }

#elif defined(CORSAC_LIBRARY_DINKUMWARE) // При использовании стандартной библиотеки Dinkumware ...
        static value_type min()
				{ return DBL_MIN; }

			static value_type max()
				{ return DBL_MAX; }

			static value_type lowest()
				{ return -DBL_MAX; }

			static value_type epsilon()
				{ return DBL_EPSILON; }

			static value_type round_error()
				{ return 0.5f; }

			static value_type infinity()
				{ return _CSTD _Inf._Double; }

			static value_type quiet_NaN()
				{ return _CSTD _Nan._Double; }

			static value_type signaling_NaN()
				{ return _CSTD _Snan._Double; }

			static value_type denorm_min()
				{ return _CSTD _Denorm._Double; }

#endif
    };

    // numeric_limits<long double>
    template<>
    struct numeric_limits<long double>
    {
        typedef long double value_type;

        static constexpr bool               is_specialized    = true;
        static constexpr int                digits            = LDBL_MANT_DIG;
        static constexpr int                digits10          = LDBL_DIG;
        static constexpr int                max_digits10      = LDBL_MANT_DIG;
        static constexpr bool               is_signed         = true;
        static constexpr bool               is_integer        = false;
        static constexpr bool               is_exact          = false;
        static constexpr int                radix             = FLT_RADIX;              // FLT_RADIX применяется ко всем типам с плавающей запятой.
        static constexpr int                min_exponent      = LDBL_MIN_EXP;
        static constexpr int                min_exponent10    = LDBL_MIN_10_EXP;
        static constexpr int                max_exponent      = LDBL_MAX_EXP;
        static constexpr int                max_exponent10    = LDBL_MAX_10_EXP;
        static constexpr bool               is_bounded        = true;
        static constexpr bool               is_modulo         = false;
        static constexpr bool               traps             = true;
        static constexpr bool               tinyness_before   = false;
        static constexpr float_round_style  round_style       = round_to_nearest;
        static constexpr bool               has_infinity      = true;
        static constexpr bool               has_quiet_NaN     = true;                   // Это может быть неправильным для некоторых платформ.
        static constexpr bool               has_signaling_NaN = true;                   // Это может быть неправильным для некоторых платформ.
        static constexpr float_denorm_style has_denorm        = denorm_present;         // Это может быть неправильным для некоторых платформ.
        static constexpr bool               has_denorm_loss   = false;                  // Это может быть неправильным для некоторых платформ.
        static constexpr bool               is_iec559         = has_denorm == denorm_present;

#if CORSAC_CUSTOM_FLOAT_CONSTANTS_REQUIRED
        static value_type min()
        { return LDBL_MIN; }

        static value_type max()
        { return LDBL_MAX; }

        static value_type lowest()
        { return -LDBL_MAX; }

        static value_type epsilon()
        { return LDBL_EPSILON; }

        static value_type round_error()
        { return 0.5f; }

        static value_type infinity()
        { return internal::gLongDoubleInfinity; }

        static value_type quiet_NaN()
        { return internal::gLongDoubleNaN; }

        static value_type signaling_NaN()
        { return internal::gLongDoubleSNaN; }

        static value_type denorm_min()
        { return internal::gLongDoubleDenorm; }

#elif (defined(CORSAC_COMPILER_GNUC) || defined(CORSAC_COMPILER_CLANG)) && defined(__LDBL_MIN__)
        static constexpr value_type min()
				{ return __LDBL_MIN__; }

			static constexpr value_type max()
				{ return __LDBL_MAX__; }

			static constexpr value_type lowest()
				{ return -__LDBL_MAX__; }

			static constexpr value_type epsilon()
				{ return __LDBL_EPSILON__; }

			static constexpr value_type round_error()
				{ return 0.5f; }

			static constexpr value_type infinity()
				{ return __builtin_huge_val(); }

			static constexpr value_type quiet_NaN()
				{ return __builtin_nan(""); }

			static constexpr value_type signaling_NaN()
				{ return __builtin_nans(""); }

			static constexpr value_type denorm_min()
				{ return __LDBL_DENORM_MIN__; }

    #elif defined(CORSAC_LIBRARY_DINKUMWARE) // При использовании стандартной библиотеки Dinkumware ...
        static value_type min()
				{ return LDBL_MIN; }

			static value_type max()
				{ return LDBL_MAX; }

			static value_type lowest()
				{ return -LDBL_MAX; }

			static value_type epsilon()
				{ return LDBL_EPSILON; }

			static value_type round_error()
				{ return 0.5f; }

			static value_type infinity()
				{ return _CSTD _LInf._Long_double; }

			static value_type quiet_NaN()
				{ return _CSTD _LNan._Long_double; }

			static value_type signaling_NaN()
				{ return _CSTD _LSnan._Long_double; }

			static value_type denorm_min()
				{ return _CSTD _LDenorm._Long_double; }

#endif
    };
}

#endif //CORSAC_STL_NUMERIC_LIMITS_H
