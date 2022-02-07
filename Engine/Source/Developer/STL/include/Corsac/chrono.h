/**
 * corsac::STL
 *
 * chrono.h
 *
 * Created by Falldot on 20.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STL_CHRONO_H
#define CORSAC_STL_CHRONO_H

#pragma once
/**
 * Описание (Falldot 22.11.2021)
 *
 * Реализует шаблоны, методы и классы, которые представляют и
 * обрабатывают длительность и время ожидания.
 *
 * ---------------------------------------------------------------------------------------------------
 * === Классы:
 *
 *      duration                        Интервал времени.
 *      time_point                      Момент времени.
 *
 * === Шаблоны:
 *
 *      common_type                     Описывает специализации шаблона класса common_type для создания экземпляров duration и time_point.
 *      duration_values                 Предоставляет конкретные значения для параметра Rep шаблона duration.
 *      high_resolution_clock           Часы с тактовым периодом в наносекунд.
 *      steady_clock                    Часы. Предпочтительный для измерения временных интервалов.
 *      system_clock                    Объект clock type, основанный на часах системы в реальном времени.
 *
 * === Функции:
 *
 *      duration_cast                   Приводит duration объект к указанному целевому duration типу.
 *      time_point_cast                 Приводит time_point объект к time_point типу, имеющему другой duration тип.
 *
 * === Определители типов:
 *
 *      nanoseconds                     duration<long long, nis_clockano>;
 *      microseconds                    duration<long long, micro>;
 *      milliseconds                    duration<long long, milli>;
 *      seconds                         duration<long long>;
 *      minutes                         duration<int, ratio<60>>;
 *      hours                           duration<int, ratio<3600>>;
 *
 * === Признаки типов:
 *
 *      treat_as_floating_point         Проверьте duration, можно ли преобразовать объект в другой duration , имеющий другой тактовый период.
 *
 * === Литералы:
 *
 *      hours operator "" h(unsigned long long Val)                     Указывает часы как целочисленное значение.
 *      duration<double, ratio<3600>> operator "" h(long double Val)    Указывает часы как значение с плавающей запятой.
 *      minutes (operator "" min)(unsigned long long Val)               Указывает минуты как целочисленное значение.
 *      duration<double, ratio<60>> (operator "" min)(long double Val)  Указывает минуты как значение с плавающей запятой.
 *      seconds operator "" s(unsigned long long Val)                   Указывает минуты как целочисленное значение.
 *      duration<double> operator "" s(long double Val)                 Указывает секунды как значение с плавающей запятой.
 *      milliseconds operator "" ms(unsigned long long Val)             Указывает миллисекунды как целочисленное значение.
 *      duration<double, milli> operator "" ms(long double Val)         Указывает миллисекунды как значение с плавающей запятой.
 *      microseconds operator "" us(unsigned long long Val)             Указывает микросекунды как целочисленное значение.
 *      duration<double, micro> operator "" us(long double Val)         Указывает микросекунды как значение с плавающей запятой.
 *      nanoseconds operator "" ns(unsigned long long Val)              Указывает наносекунды как целочисленное значение.
 *      duration<double, nano> operator "" ns(long double Val)          Указывает наносекунды как значение с плавающей запятой.
 */
/**
 * === TODO: (С++ 17)
 *      ceil(duration)                  Возвращает наименьшее представление duration в целевом типе, которое больше или равно указанному типу duration.
 *      ceil(time_point)                Возвращает наименьший период времени, который можно показать в целевом объекте duration , который больше или равен заданной точке времени.
 *      floor(duration)                 Возвращает наибольшее представление duration в целевом типе, которое меньше или равно указанному типу duration.
 *      floor(time_point)               Возвращает значение самой крупной временной точки в целевом объекте duration , которое меньше или равно заданной точке времени.
 *      round(duration)                 Округляет указанное значение duration до ближайшего представления duration в целевом типе.
 *      round(time_point)               Возвращает ближайшую точку во времени, представленную в целевом объекте, duration в указанную точку времени.
 *
 * === TODO: (С++ 20)
 *      day                             День месяца. Например, 25 день месяца.
 *      hh_mm_ss                        Разделяет в duration часы: минуты: секунды.
 *      leap_second                     Дата и значение для вставленной високосной секунды.
 *      month                           Месяц года. Например, Июль.
 *      month_day                       Конкретный день определенного месяца. Например, 30 июля.
 *      month_day_last                  Последний день месяца.
 *      month_weekday                   N-й день недели определенного месяца.
 *      month_weekday_last              Последний день недели определенного месяца.
 *      time_zone                       Все смены часовых поясов для определенной географической области.
 *      weekday                         День недели.
 *      weekday_last                    Последний будний день месяца.
 *      weekday_indexed                 Объединяет день недели с индексом, представляющим будний день месяца.
 *      year                            Год в григорианском календаре.
 *      year_month                      Год и месяц. День не указан.
 *      year_month_day                  Год, месяц и день.
 *      year_month_day_last             Последний день конкретного месяца и года.
 *      year_month_weekday              Определенный год, месяц и n-й день недели месяца.
 *      year_month_weekday_last         Определенный год, месяц и последний будний день месяца.
 *      leap_second_info                Данные, возвращаемые get_leap_second_info.
 *      current_zone                    Возвращает текущий объект часового пояса.
 *      from_stream                     Проанализируйте входной поток в один из std::chrono таких типов времени или интервалов day , как, month... month_day weekday year year_month year_month_day и т. д., используя указанный формат.
 *      get_leap_second_info            Возвращает объект leap_second_info за указанное время.
 *      get_tzdb                        Возвращает первую запись в списке базы данных часового пояса для всей программы.
 *      get_tzdb_list                   Получает одноэлементную базу данных часового пояса для всей программы.
 *      is_am                           Предикат, определяющий, является ли указанный час частью дня ( накопительный-меридием (AM)).
 *      is_pm                           Предикат, определяющий, находится ли указанный час в части дня, выступающей после меридием (PM).
 *      locate_zone                     Возвращает объект часового пояса, указанный по имени часового пояса.
 *      make12                          Возвращает указанный час в 12-часовом формате времени.
 *      make24                          Возвращает указанный час в 24-часовом формате времени.
 *      reload_tzdb                     Перезагружает базу данных часового пояса, если доступна обновленная информация.
 *      remote_version                  Возвращает строку, содержащую последнюю версию удаленной базы данных.
 *      is_clock                        Проверьте, является ли тип часами.
 *      local_days                      Число дней, представленное, time_point не связанное ни с одним часовым поясом.
 *      sys_time                        Число дней, представленное объектом, time_point связанным с system_clock , который имеет эпоху 1/1/1970.
*/

#include "Corsac/STL/config.h"
#include "Corsac/type_traits.h"
#include "Corsac/numeric_limits.h"
#include "Corsac/ratio.h"

// TODO:  move to platform specific cpp or header file
#if defined(CORSAC_PLATFORM_MICROSOFT)
    #ifndef WIN32_LCORSACN_AND_MCORSACN
        #define WIN32_LCORSACN_AND_MCORSACN
    #endif

    #undef NOMINMAX
        #define NOMINMAX

        #include <windows.h>

    #ifdef min
        #undef min
    #endif
    #ifdef max
        #undef max
    #endif
#endif

#if defined(CORSAC_PLATFORM_MICROSOFT) && !defined(CORSAC_PLATFORM_MINGW)
// Nothing to do
#elif defined(CORSAC_PLATFORM_SONY)
#include <Dinkum/threads/xtimec.h>
	#include <kernel.h>
#elif defined(CORSAC_PLATFORM_APPLE)
    #include <mach/mach_time.h>
#elif defined(CORSAC_PLATFORM_POSIX) || defined(CORSAC_PLATFORM_MINGW) || defined(CORSAC_PLATFORM_ANDROID)
    // Posix means Linux, Unix, and Macintosh OSX, among others (including Linux-based mobile platforms).
	#if defined(CORSAC_PLATFORM_MINGW)
		#include <pthread_time.h>
	#endif
	#include <time.h>
	#if (defined(CLOCK_RCORSACLTIME) || defined(CLOCK_MONOTONIC))
		#include <errno.h>
	#else
		#include <sys/time.h>
		#include <unistd.h>
	#endif
#endif

namespace corsac
{
    namespace chrono
    {
        // treat_as_floating_point
        template <class Rep>
        struct treat_as_floating_point : is_floating_point<Rep> {};

        // duration_values
        template <class Rep>
        struct duration_values
        {
        public:
            inline static constexpr Rep zero() { return Rep(0); }
            inline static constexpr Rep max()  { return corsac::numeric_limits<Rep>::max(); }
            inline static constexpr Rep min()  { return corsac::numeric_limits<Rep>::lowest(); }
        };

        // duration fwd_decl
        template <typename Rep, typename Period = ratio<1>>
        class duration;

        namespace internal
        {
            // IsRatio
            template <typename> struct IsRatio                                           : corsac::false_type {};
            template <intmax_t N, intmax_t D> struct IsRatio<ratio<N, D>>                : corsac::true_type {};
            template <intmax_t N, intmax_t D> struct IsRatio<const ratio<N, D>>          : corsac::true_type {};
            template <intmax_t N, intmax_t D> struct IsRatio<volatile ratio<N, D>>       : corsac::true_type {};
            template <intmax_t N, intmax_t D> struct IsRatio<const volatile ratio<N, D>> : corsac::true_type {};

            // IsDuration
            template<typename> struct IsDuration                                                            : corsac::false_type{};
            template<typename Rep, typename Period> struct IsDuration<duration<Rep, Period>>                : corsac::true_type{};
            template<typename Rep, typename Period> struct IsDuration<const duration<Rep, Period>>          : corsac::true_type{};
            template<typename Rep, typename Period> struct IsDuration<volatile duration<Rep, Period>>       : corsac::true_type{};
            template<typename Rep, typename Period> struct IsDuration<const volatile duration<Rep, Period>> : corsac::true_type{};

            // RatioGCD
            template <class Period1, class Period2>
            struct RatioGCD
            {
                static_assert(IsRatio<Period1>::value, "Period1 is not a corsac::ratio type");
                static_assert(IsRatio<Period2>::value, "Period2 is not a corsac::ratio type");

                using type = ratio<corsac::internal::gcd<Period1::num, Period2::num>::value,
                        corsac::internal::lcm<Period1::den, Period2::den>::value>;
            };
        } // namespace internal

        // duration_cast
        namespace internal
        {
            template <typename FromDuration,
                    typename ToDuration,
                    typename CommonPeriod =
                    typename ratio_divide<typename FromDuration::period, typename ToDuration::period>::type,
                    typename CommonRep = typename corsac::decay<typename corsac::common_type<typename ToDuration::rep,
                            typename FromDuration::rep,
                            intmax_t>::type>::type,
                    bool = CommonPeriod::num == 1,
                    bool = CommonPeriod::den == 1>
            struct DurationCastImpl;

            template <typename FromDuration, typename ToDuration, typename CommonPeriod, typename CommonRep>
            struct DurationCastImpl<FromDuration, ToDuration, CommonPeriod, CommonRep, true, true>
            {
                inline static ToDuration DoCast(const FromDuration& fd)
                {
                    return ToDuration(static_cast<typename ToDuration::rep>(fd.count()));
                }
            };

            template <typename FromDuration, typename ToDuration, typename CommonPeriod, typename CommonRep>
            struct DurationCastImpl<FromDuration, ToDuration, CommonPeriod, CommonRep, false, true>
            {
                inline static ToDuration DoCast(const FromDuration& d)
                {
                    return ToDuration(static_cast<typename ToDuration::rep>(static_cast<CommonRep>(d.count()) *
                                                                            static_cast<CommonRep>(CommonPeriod::num)));
                }
            };

            template <typename FromDuration, typename ToDuration, typename CommonPeriod, typename CommonRep>
            struct DurationCastImpl<FromDuration, ToDuration, CommonPeriod, CommonRep, true, false>
            {
                inline static ToDuration DoCast(const FromDuration& d)
                {
                    return ToDuration(static_cast<typename ToDuration::rep>(static_cast<CommonRep>(d.count()) /
                                                                            static_cast<CommonRep>(CommonPeriod::den)));
                }
            };

            template <typename FromDuration, typename ToDuration, typename CommonPeriod, typename CommonRep>
            struct DurationCastImpl<FromDuration, ToDuration, CommonPeriod, CommonRep, false, false>
            {
                inline static ToDuration DoCast(const FromDuration& d)
                {
                    return ToDuration(static_cast<typename ToDuration::rep>(static_cast<CommonRep>(d.count()) *
                                                                            static_cast<CommonRep>(CommonPeriod::num) /
                                                                            static_cast<CommonRep>(CommonPeriod::den)));
                }
            };
        } // namespace internal

        // duration_cast
        template <typename ToDuration, typename Rep, typename Period>
        inline typename corsac::enable_if<internal::IsDuration<ToDuration>::value, ToDuration>::type
        duration_cast(const duration<Rep, Period>& d)
        {
            typedef typename duration<Rep, Period>::this_type FromDuration;
            return internal::DurationCastImpl<FromDuration, ToDuration>::DoCast(d);
        }

        // duration
        template <class Rep, class Period>
        class duration
        {
            Rep mRep;

        public:
            using rep       = Rep;
            using period    = Period;
            using this_type = duration<Rep, Period>;

            constexpr duration() = default;
            duration(const duration&) = default;
            duration& operator=(const duration&) = default;

            // conversion constructors
            template <class Rep2>
            inline constexpr explicit duration(
                    const Rep2& rep2,
                    typename corsac::enable_if<corsac::is_convertible<Rep2, Rep>::value &&
                                               (treat_as_floating_point<Rep>::value ||
                                                !treat_as_floating_point<Rep2>::value)>::type** = 0)
                    : mRep(static_cast<Rep>(rep2)) {}

            template <class Rep2, class Period2>
            constexpr duration(const duration<Rep2, Period2>& d2,
                               typename corsac::enable_if<treat_as_floating_point<Rep>::value ||
                                                          (corsac::ratio_divide<Period2, Period>::type::den == 1 &&
                                                           !treat_as_floating_point<Rep2>::value),
                                       void>::type** = 0)
                    : mRep(duration_cast<duration>(d2).count()) {}

            // возвращает количество тиков
            [[nodiscard]] constexpr Rep count() const { return mRep; }

            // статические методы доступа со специальными значениями длительности
            constexpr inline static duration zero() { return duration(duration_values<Rep>::zero()); }
            constexpr inline static duration min()  { return duration(duration_values<Rep>::min()); }
            constexpr inline static duration max()  { return duration(duration_values<Rep>::max()); }

            // арифметические операции с константой
            constexpr inline duration operator+() const { return *this; }
            constexpr inline duration operator-() const { return duration(0-mRep); }

            // арифметические операции
            inline duration operator++(int)                  { return duration(mRep++); }
            inline duration operator--(int)                  { return duration(mRep--); }
            inline duration& operator++()                    { ++mRep; return *this; }
            inline duration& operator--()                    { --mRep; return *this; }
            inline duration& operator+=(const duration& d)   { mRep += d.count(); return *this; }
            inline duration& operator-=(const duration& d)   { mRep -= d.count(); return *this; }
            inline duration& operator*=(const Rep& rhs)      { mRep *= rhs; return *this; }
            inline duration& operator/=(const Rep& rhs)      { mRep /= rhs; return *this; }
            inline duration& operator%=(const Rep& rhs)      { mRep %= rhs; return *this; }
            inline duration& operator%=(const duration& d)   { mRep %= d.count(); return *this; }
        };

        // арифметические операции с длительностями в качестве аргументов
        template <typename Rep1, typename Period1, typename Rep2, typename Period2>
        typename corsac::common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type inline
        operator+(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
        {
            using common_duration_t = typename corsac::common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type;
            return common_duration_t(common_duration_t(lhs).count() + common_duration_t(rhs).count());
        }

        template <typename Rep1, typename Period1, typename Rep2, typename Period2>
        typename corsac::common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type inline
        operator-(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
        {
            using common_duration_t = typename corsac::common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type;
            return common_duration_t(common_duration_t(lhs).count() - common_duration_t(rhs).count());
        }

        template <typename Rep1, typename Period1, typename Rep2>
        duration<typename corsac::common_type<Rep1, Rep2>::type, Period1> inline
        operator*(const duration<Rep1, Period1>& lhs, const Rep2& rhs)
        {
            using common_duration_t = typename duration<corsac::common_type<Rep1, Rep2>, Period1>::type;
            return common_duration_t(common_duration_t(lhs).count() * rhs);
        }

        template <typename Rep1, typename Rep2, typename Period2>
        duration<typename corsac::common_type<Rep1, Rep2>::type, Period2> inline
        operator*(const Rep1& lhs, const duration<Rep2, Period2>& rhs)
        {
            using common_duration_t = duration<typename corsac::common_type<Rep1, Rep2>::type, Period2>;
            return common_duration_t(lhs * common_duration_t(rhs).count());
        }

        template <typename Rep1, typename Period1, typename Rep2>
        duration<typename corsac::common_type<Rep1, Rep2>::type, Period1> inline
        operator/(const duration<Rep1, Period1>& lhs, const Rep2& rhs)
        {
            using common_duration_t = duration<typename corsac::common_type<Rep1, Rep2>::type, Period1>;
            return common_duration_t(common_duration_t(lhs).count() / rhs);
        }

        template <typename Rep1, typename Period1, typename Rep2, typename Period2>
        typename corsac::common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type inline
        operator/(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
        {
            using common_duration_t = typename corsac::common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type;
            return common_duration_t(common_duration_t(lhs).count() / common_duration_t(rhs).count());
        }

        template <typename Rep1, typename Period1, typename Rep2>
        duration<typename corsac::common_type<Rep1, Rep2>::type, Period1> inline
        operator%(const duration<Rep1, Period1>& lhs, const Rep2& rhs)
        {
            using common_duration_t = duration<typename corsac::common_type<Rep1, Rep2>::type, Period1>;
            return common_duration_t(common_duration_t(lhs).count() % rhs);
        }

        template <typename Rep1, typename Period1, typename Rep2, typename Period2>
        typename corsac::common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type inline
        operator%(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
        {
            using common_duration_t = typename corsac::common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type;
            return common_duration_t(common_duration_t(lhs).count() % common_duration_t(rhs).count());
        }

        // сравнивает две продолжительности
        template <typename Rep1, typename Period1, typename Rep2, typename Period2>
        inline bool operator==(const duration<Rep1, Period1>& lhs,
                               const duration<Rep2, Period2>& rhs)
        {
            using common_duration_t = typename corsac::common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type;
            return common_duration_t(lhs).count() == common_duration_t(rhs).count();
        }

        template <typename Rep1, typename Period1, typename Rep2, typename Period2>
        inline bool operator<(const duration<Rep1, Period1>& lhs,
                              const duration<Rep2, Period2>& rhs)
        {
            using common_duration_t = typename corsac::common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type;
            return common_duration_t(lhs).count() < common_duration_t(rhs).count();
        }

        template <typename Rep1, typename Period1, typename Rep2, typename Period2>
        inline bool operator!=(const duration<Rep1, Period1>& lhs,
                               const duration<Rep2, Period2>& rhs)
        {
            return !(lhs == rhs);
        }

        template <typename Rep1, typename Period1, typename Rep2, typename Period2>
        inline bool operator<=(const duration<Rep1, Period1>& lhs,
                               const duration<Rep2, Period2>& rhs)
        {
            return !(rhs < lhs);
        }

        template <typename Rep1, typename Period1, typename Rep2, typename Period2>
        inline bool operator>(const duration<Rep1, Period1>& lhs,
                              const duration<Rep2, Period2>& rhs)
        {
            return rhs < lhs;
        }

        template <typename Rep1, typename Period1, typename Rep2, typename Period2>
        inline bool operator>=(const duration<Rep1, Period1>& lhs,
                               const duration<Rep2, Period2>& rhs)
        {
            return !(lhs < rhs);
        }

        // standard duration units
        using nanoseconds   = duration<long long, nano>;
        using microseconds  = duration<long long, micro>;
        using milliseconds  = duration<long long, milli>;
        using seconds       = duration<long long>;
        using minutes       = duration<int, ratio<60>>;
        using hours         = duration<int, ratio<3600>>;

        // time_point
        template <typename Clock, typename Duration = typename Clock::duration>
        class time_point
        {
            Duration mDuration;

        public:
            using clock     = Clock;
            using duration  = Duration;
            using rep       = typename Duration::rep;
            using period    = typename Duration::period;

            inline constexpr time_point() : mDuration(Duration::zero()) {}
            constexpr explicit time_point(const Duration& other) : mDuration(other) {}

            template <typename Duration2>
            inline constexpr time_point(
                    const time_point<Clock, Duration2>& t,
                    typename corsac::enable_if<corsac::is_convertible<Duration2, Duration>::value>::type** = 0)
                    : mDuration(t.time_since_epoch()) {}

            constexpr Duration time_since_epoch() const { return mDuration; }

            time_point& operator+=(const Duration& d) { mDuration += d; return *this; }
            time_point& operator-=(const Duration& d) { mDuration -= d; return *this; }

            static constexpr time_point min() { return time_point(Duration::min()); }
            static constexpr time_point max() { return time_point(Duration::max()); }
        };

        // time_point arithmetic
        template <class Clock, class Duration1, class Rep2, class Period2>
        inline constexpr time_point<Clock, typename corsac::common_type<Duration1, duration<Rep2, Period2>>::type>
        operator+(const time_point<Clock, Duration1>& lhs, const duration<Rep2, Period2>& rhs)
        {
            using common_timepoint_t = time_point<Clock, typename corsac::common_type<Duration1, duration<Rep2, Period2>>::type>;
            return common_timepoint_t(lhs.time_since_epoch() + rhs);
        }

        template <class Rep1, class Period1, class Clock, class Duration2>
        inline constexpr time_point<Clock, typename corsac::common_type<Duration2, duration<Rep1, Period1>>::type>
        operator+(const duration<Rep1, Period1>& lhs, const time_point<Clock, Duration2>& rhs)
        {
            using common_timepoint_t = time_point<Clock, typename corsac::common_type<Duration2, duration<Rep1, Period1>>::type>;
            return common_timepoint_t(lhs + rhs.time_since_epoch());
        }

        template <class Clock, class Duration1, class Rep2, class Period2>
        inline constexpr time_point<Clock, typename corsac::common_type<Duration1, duration<Rep2, Period2>>::type>
        operator-(const time_point<Clock, Duration1>& lhs, const duration<Rep2, Period2>& rhs)
        {
            using common_timepoint_t = time_point<Clock, typename corsac::common_type<Duration1, duration<Rep2, Period2>>::type>;
            return common_timepoint_t(lhs.time_since_epoch() - rhs);
        }

        template <class Clock, class Duration1, class Duration2>
        inline constexpr typename corsac::common_type<Duration1, Duration2>::type operator-(
                const time_point<Clock, Duration1>& lhs,
                const time_point<Clock, Duration2>& rhs)
        {
            return lhs.time_since_epoch() - rhs.time_since_epoch();
        }

        template <class Clock, class Duration1, class Duration2>
        inline constexpr bool operator==(const time_point<Clock, Duration1>& lhs,
                                         const time_point<Clock, Duration2>& rhs)
        {
            return lhs.time_since_epoch() == rhs.time_since_epoch();
        }

        template <class Clock, class Duration1, class Duration2>
        inline constexpr bool operator!=(const time_point<Clock, Duration1>& lhs,
                                         const time_point<Clock, Duration2>& rhs)
        {
            return !(lhs == rhs);
        }

        template <class Clock, class Duration1, class Duration2>
        inline constexpr bool operator<(const time_point<Clock, Duration1>& lhs, const time_point<Clock, Duration2>& rhs)
        {
            return lhs.time_since_epoch() < rhs.time_since_epoch();
        }

        template <class Clock, class Duration1, class Duration2>
        inline constexpr bool operator<=(const time_point<Clock, Duration1>& lhs,
                                         const time_point<Clock, Duration2>& rhs)
        {
            return !(rhs < lhs);
        }

        template <class Clock, class Duration1, class Duration2>
        inline constexpr bool operator>(const time_point<Clock, Duration1>& lhs, const time_point<Clock, Duration2>& rhs)
        {
            return rhs < lhs;
        }

        template <class Clock, class Duration1, class Duration2>
        inline constexpr bool operator>=(const time_point<Clock, Duration1>& lhs,
                                         const time_point<Clock, Duration2>& rhs)
        {
            return !(lhs < rhs);
        }

        // time_point_cast
        template <typename ToDuration, typename Clock, typename Duration>
        constexpr time_point<Clock, ToDuration> time_point_cast(
                const time_point<Clock, Duration>& t,
                typename corsac::enable_if<internal::IsDuration<ToDuration>::value>::type** = 0)
        {
            return time_point<Clock, ToDuration>(duration_cast<ToDuration>(t.time_since_epoch()));
        }

        // clocks
        namespace internal
        {
        #if defined(CORSAC_PLATFORM_MICROSOFT) && !defined(CORSAC_PLATFORM_MINGW)
        #define CORSAC_NS_PER_TICK 1
        #elif defined CORSAC_PLATFORM_SONY
                    #define CORSAC_NS_PER_TICK _XTIME_NSECS_PER_TICK
                #elif defined CORSAC_PLATFORM_POSIX
                    #define CORSAC_NS_PER_TICK _XTIME_NSECS_PER_TICK
                #else
                    #define CORSAC_NS_PER_TICK 100
        #endif

            #if defined(CORSAC_PLATFORM_POSIX)
                        using SteadyClock_Period = chrono::nanoseconds::period SystemClock_Period;
                                typedef chrono::nanoseconds::period ;
            #else
                        using SystemClock_Period = corsac::ratio_multiply<corsac::ratio<CORSAC_NS_PER_TICK, 1>, nano>::type;
                        using SteadyClock_Period = corsac::ratio_multiply<corsac::ratio<CORSAC_NS_PER_TICK, 1>, nano>::type;
            #endif

            // internal::GetTicks
            inline uint64_t GetTicks()
            {
            #if defined CORSAC_PLATFORM_MICROSOFT
                auto queryFrequency = []
                {
                    LARGE_INTEGER frequency;
                    QueryPerformanceFrequency(&frequency);
                    return double(1000000000.0L / frequency.QuadPart);  // наносекунд за тик
                };

                auto queryCounter = []
                {
                    LARGE_INTEGER counter;
                    QueryPerformanceCounter(&counter);
                    return counter.QuadPart;
                };

                static auto frequency = queryFrequency(); // частота процессора кеширования при первом вызове
                return uint64_t(frequency * static_cast<double>(queryCounter()));
            #elif defined CORSAC_PLATFORM_SONY
                return sceKernelGetProcessTimeCounter();
            #elif defined(CORSAC_PLATFORM_APPLE)
                return mach_absolute_time();
            #elif defined(CORSAC_PLATFORM_POSIX) // Posix means Linux, Unix, and Macintosh OSX, among others (including Linux-based mobile platforms).
                #if (defined(CLOCK_RCORSACLTIME) || defined(CLOCK_MONOTONIC))
                    timespec ts;
                    int result = clock_gettime(CLOCK_MONOTONIC, &ts);

                    if (result == -1 && errno == EINVAL)
                        result = clock_gettime(CLOCK_RCORSACLTIME, &ts);

                    const uint64_t nNanoseconds = (uint64_t)ts.tv_nsec + ((uint64_t)ts.tv_sec * UINT64_C(1000000000));
                    return nNanoseconds;
                #else
                    struct timeval tv;
                    gettimeofday(&tv, NULL);
                    const uint64_t nMicroseconds = (uint64_t)tv.tv_usec + ((uint64_t)tv.tv_sec * 1000000);
                    return nMicroseconds;
                #endif
            #else
                    #error "chrono not implemented for platform"
            #endif
            }
        } // namespace internal

        // system_clock
        class system_clock
        {
        public:
            using rep           = long long; // знаковый арифметический тип, представляющий количество тактов в продолжительности часов
            using period        = internal::SystemClock_Period;
            using duration      = chrono::duration<rep, period>; // duration<rep, period>, способен отображать отрицательную продолжительность
            using time_point    = chrono::time_point<system_clock>;

            // истина, если время между тиками всегда монотонно увеличивается
            constexpr static bool is_steady = false;

            // возвращает момент времени, представляющий текущий момент времени.
            static time_point now() noexcept
            {
                return time_point(duration(internal::GetTicks()));
            }
        };

        // steady_clock
        class steady_clock
        {
        public:
            using rep           = long long; // знаковый арифметический тип, представляющий количество тактов в продолжительности часов
            using period        = internal::SteadyClock_Period;
            using duration      = chrono::duration<rep, period>; // duration<rep, period>, способен отображать отрицательную продолжительность
            using time_point    = chrono::time_point<steady_clock>;

            // истина, если время между тиками всегда монотонно увеличивается
            constexpr static bool is_steady = true;

            // возвращает момент времени, представляющий текущий момент времени.
            static time_point now() noexcept
            {
                return time_point(duration(internal::GetTicks()));
            }
        };

        // high_resolution_clock
        using high_resolution_clock = system_clock;

    } // namespace chrono

    // duration common_type specialization
    template <typename Rep1, typename Period1, typename Rep2, typename Period2>
    struct common_type<chrono::duration<Rep1, Period1>, chrono::duration<Rep2, Period2>>
    {
        typedef chrono::duration<typename corsac::decay<typename corsac::common_type<Rep1, Rep2>::type>::type,
                typename chrono::internal::RatioGCD<Period1, Period2>::type> type;
    };

    // time_point common_type specialization
    template <typename Clock, typename Duration1, typename Duration2>
    struct common_type<chrono::time_point<Clock, Duration1>, chrono::time_point<Clock, Duration2>>
    {
        using type = chrono::time_point<Clock, typename corsac::common_type<Duration1, Duration2>::type>;
    };

    // chrono_literals
    inline namespace literals
        {
            inline namespace chrono_literals
            {
                // integer chrono literals
                constexpr chrono::hours         operator"" h(unsigned long long h)      { return chrono::hours(h); }
                constexpr chrono::minutes       operator"" min(unsigned long long m)    { return chrono::minutes(m); }
                constexpr chrono::seconds       operator"" s(unsigned long long s)      { return chrono::seconds(s); }
                constexpr chrono::milliseconds  operator"" ms(unsigned long long ms)    { return chrono::milliseconds(ms); }
                constexpr chrono::microseconds  operator"" us(unsigned long long us)    { return chrono::microseconds(us); }
                constexpr chrono::nanoseconds   operator"" ns(unsigned long long ns)    { return chrono::nanoseconds(ns); }

                // float chrono literals
                constexpr chrono::duration<long double, ratio<3600, 1>> operator"" h(long double h)
                    { return chrono::duration<long double, ratio<3600, 1>>(h); }
                constexpr chrono::duration<long double, ratio<60, 1>> operator"" min(long double m)
                    { return chrono::duration<long double, ratio<60, 1>>(m); }
                constexpr chrono::duration<long double> operator"" s(long double s)
                    { return chrono::duration<long double>(s); }
                constexpr chrono::duration<float, milli> operator"" ms(long double ms)
                    { return chrono::duration<long double, milli>(ms); }
                constexpr chrono::duration<float, micro> operator"" us(long double us)
                    { return chrono::duration<long double, micro>(us); }
                constexpr chrono::duration<float, nano> operator"" ns(long double ns)
                    { return chrono::duration<long double, nano>(ns); }

            } // namespace chrono_literals
        }// namespace literals
} // namespace corsac

#endif //CORSAC_STL_CHRONO_H
