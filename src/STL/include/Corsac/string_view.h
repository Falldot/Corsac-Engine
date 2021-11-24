/**
 * corsac::STL
 *
 * string_view.h
 *
 * Created by Falldot on 23.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STRING_VIEW_H
#define CORSAC_STRING_VIEW_H

#pragma once
/**
 * Описание (Falldot 23.11.2021)
 *
 * Семейство специализаций шаблонов предоставляет эффективный способ передачи неответственного,
 * доступного только для чтения, безотносительного и невладеющего обработчика к символьным данным
 * любых строковых объектов, имеющих первый элемент последовательности в позиции 0.
 * Параметр функции типа string_view (typedef для basic_string_view<char> ) может принимать аргументы,
 * такие как std::string , char* или любые другие строковые классы с узкими символами,
 * для которых определено неявное преобразование string_view . Аналогичным образом, параметр wstring_view
 * u16string_view или u32string_view может принимать любой строковый тип,
 * для которого определено неявное преобразование.
 *
 * ---------------------------------------------------------------------------------------------------
 * === Определители типов:
 *
 *      string_view                 Специализация шаблона класса basic_string_view с элементами типа char.
 *      wstring_view                Специализация шаблона класса basic_string_view с элементами типа wchar_t.
 *      u16string_view              Специализация шаблона класса basic_string_view с элементами типа char16_t.
 *      u32string_view              Специализация шаблона класса basic_string_view с элементами типа char32_t.
 *
 * === Литералы:
 *      sv                          Конструирует string_view , wstring_view , или, в u16string_view u32string_view
 *                                  зависимости от типа строкового литерала, к которому он добавляется.
 *
 * === Классы:
 *
 *      basic_string_view           Шаблон класса, предоставляющий представление, доступное только для чтения,
 *                                  к последовательности произвольных символьных объектов.
 *
 *      hash                        Объект функции, создающий хэш-значение для string_view.
 */

#include <Corsac/internal/config.h>
#include <Corsac/internal/char_traits.h>
#include <Corsac/algorithm.h>
#include <Corsac/iterator.h>
#include <Corsac/numeric_limits.h>

#if CORSAC_EXCEPTIONS_ENABLED
	#include <stdexcept> // std::out_of_range.
#endif

namespace corsac
{
    template <typename T>
    class basic_string_view
    {
    public:
        using this_type                 = basic_string_view<T>;
        using value_type                = T;
        using pointer                   = T*;
        using const_pointer             = const T*;
        using reference                 = T&;
        using const_reference           = const T&;
        using iterator                  = T*;
        using const_iterator            = const T*;
        using reverse_iterator          = corsac::reverse_iterator<iterator>;
        using const_reverse_iterator    = corsac::reverse_iterator<const_iterator>;
        using size_type                 = size_t;
        using difference_type           = ptrdiff_t;

        static const constexpr size_type npos = size_type(-1);

    protected:
        const_pointer mpBegin = nullptr;
        size_type mnCount = 0;

    public:
        constexpr basic_string_view() noexcept : mpBegin(nullptr), mnCount(0) {}
        constexpr basic_string_view(const basic_string_view& other) noexcept = default;
        constexpr basic_string_view(const T* s, size_type count) : mpBegin(s), mnCount(count) {}
        constexpr basic_string_view(const T* s) : mpBegin(s), mnCount(s != nullptr ? CharStrlen(s) : 0) {}
        basic_string_view& operator=(const basic_string_view& view) = default;

        [[nodiscard]] constexpr const_iterator begin() const noexcept { return mpBegin; }
        [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return mpBegin; }
        [[nodiscard]] constexpr const_iterator end() const noexcept { return mpBegin + mnCount; }
        [[nodiscard]] constexpr const_iterator cend() const noexcept { return mpBegin + mnCount; }
        [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(mpBegin + mnCount); }
        [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(mpBegin + mnCount); }
        [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(mpBegin); }
        [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(mpBegin); }

        [[nodiscard]] constexpr const_pointer data() const { return mpBegin; }
        [[nodiscard]] constexpr const_reference front() const
        {
            return [&] { CORSAC_ASSERT_MSG(!empty(), "behavior is undefined if string_view is empty"); }(), mpBegin[0];
        }

        [[nodiscard]] constexpr const_reference back() const
        {
            return [&] { CORSAC_ASSERT_MSG(!empty(), "behavior is undefined if string_view is empty"); }(), mpBegin[mnCount - 1];
        }

        constexpr const_reference operator[](size_type pos) const
        {
            // Согласно стандартной спецификации: проверка границ не выполняется: поведение не определено, если pos >= size ().
            return mpBegin[pos];
        }

        [[nodiscard]] constexpr const_reference at(size_type pos) const
        {
            #if CORSAC_EXCEPTIONS_ENABLED
                if(CORSAC_UNLIKELY(pos >= mnCount))
                        throw std::out_of_range("string_view::at -- out of range");
            #elif CORSAC_ASSERT_ENABLED
                if(CORSAC_UNLIKELY(pos >= mnCount))
                        CORSAC_FAIL_MSG("string_view::at -- out of range");
            #endif
            return mpBegin[pos];
        }

        [[nodiscard]] constexpr size_type size() const noexcept { return mnCount; }
        [[nodiscard]] constexpr size_type length() const noexcept { return mnCount; }

        // Избегайте расширения макроса max (...) из заголовков окон (потенциально включенных перед этим файлом),
        // заключив имя функции в квадратные скобки
        [[nodiscard]] constexpr size_type max_size() const noexcept { return (numeric_limits<size_type>::max)(); }
        [[nodiscard]] constexpr bool empty() const noexcept { return mnCount == 0; }


        constexpr void swap(basic_string_view& v)
        {
            corsac::swap(mpBegin, v.mpBegin);
            corsac::swap(mnCount, v.mnCount);
        }

        constexpr void remove_prefix(size_type n)
        {
            CORSAC_ASSERT_MSG(n <= mnCount, "behavior is undefined if moving past the end of the string");
            mpBegin += n;
            mnCount -= n;
        }

        constexpr void remove_suffix(size_type n)
        {
            CORSAC_ASSERT_MSG(n <= mnCount, "behavior is undefined if moving past the beginning of the string");
            mnCount -= n;
        }

        size_type copy(T* pDestination, size_type count, size_type pos = 0) const
        {
            #if CORSAC_EXCEPTIONS_ENABLED
                if(CORSAC_UNLIKELY(pos > mnCount))
                        throw std::out_of_range("string_view::copy -- out of range");
            #elif CORSAC_ASSERT_ENABLED
                if(CORSAC_UNLIKELY(pos > mnCount))
                        CORSAC_FAIL_MSG("string_view::copy -- out of range");
            #endif
            count = corsac::min<size_type>(count, mnCount - pos);
            auto* pResult = CharStringUninitializedCopy(mpBegin + pos, mpBegin + pos + count, pDestination);
            // *pResult = 0; // не пишите null-терминатор
            return pResult - pDestination;
        }

        [[nodiscard]] constexpr basic_string_view substr(size_type pos = 0, size_type count = npos) const
        {
            #if CORSAC_EXCEPTIONS_ENABLED
                if(CORSAC_UNLIKELY(pos > mnCount))
                        throw std::out_of_range("string_view::substr -- out of range");
            #elif CORSAC_ASSERT_ENABLED
                if(CORSAC_UNLIKELY(pos > mnCount))
                        CORSAC_FAIL_MSG("string_view::substr -- out of range");
            #endif
            count = corsac::min<size_type>(count, mnCount - pos);
            return this_type(mpBegin + pos, count);
        }

        static constexpr int compare(const T* pBegin1, const T* pEnd1, const T* pBegin2, const T* pEnd2)
        {
            const ptrdiff_t n1   = pEnd1 - pBegin1;
            const ptrdiff_t n2   = pEnd2 - pBegin2;
            const ptrdiff_t nMin = corsac::min_alt(n1, n2);
            const int       cmp  = Compare(pBegin1, pBegin2, (size_type)nMin);

            return (cmp != 0 ? cmp : (n1 < n2 ? -1 : (n1 > n2 ? 1 : 0)));
        }

        [[nodiscard]] constexpr int compare(basic_string_view sw) const noexcept
        {
            return compare(mpBegin, mpBegin + mnCount, sw.mpBegin, sw.mpBegin + sw.mnCount);
        }

        [[nodiscard]] constexpr int compare(size_type pos1, size_type count1, basic_string_view sw) const
        {
            return substr(pos1, count1).compare(sw);
        }

        [[nodiscard]] constexpr int compare(size_type pos1,
                                 size_type count1,
                                 basic_string_view sw,
                                 size_type pos2,
                                 size_type count2) const
        {
            return substr(pos1, count1).compare(sw.substr(pos2, count2));
        }

        constexpr int compare(const T* s) const { return compare(basic_string_view(s)); }

        constexpr int compare(size_type pos1, size_type count1, const T* s) const
        {
            return substr(pos1, count1).compare(basic_string_view(s));
        }

        constexpr int compare(size_type pos1, size_type count1, const T* s, size_type count2) const
        {
            return substr(pos1, count1).compare(basic_string_view(s, count2));
        }

        [[nodiscard]] constexpr size_type find(basic_string_view sw, size_type pos = 0) const noexcept
        {
            auto* pEnd = mpBegin + mnCount;
            if (CORSAC_LIKELY(((npos - sw.size()) >= pos) && (pos + sw.size()) <= mnCount))
            {
                const value_type* const pTemp = corsac::search(mpBegin + pos, pEnd, sw.data(), sw.data() + sw.size());

                if ((pTemp != pEnd) || (sw.size() == 0))
                    return (size_type)(pTemp - mpBegin);
            }
            return npos;
        }

        [[nodiscard]] constexpr size_type find(T c, size_type pos = 0) const noexcept
        {
            return find(basic_string_view(&c, 1), pos);
        }

        constexpr size_type find(const T* s, size_type pos, size_type count) const
        {
            return find(basic_string_view(s, count), pos);
        }

        constexpr size_type find(const T* s, size_type pos = 0) const { return find(basic_string_view(s), pos); }

        [[nodiscard]] constexpr size_type rfind(basic_string_view sw, size_type pos = npos) const noexcept
        {
            return rfind(sw.mpBegin, pos, sw.mnCount);
        }

        [[nodiscard]] constexpr size_type rfind(T c, size_type pos = npos) const noexcept
        {
            if (CORSAC_LIKELY(mnCount))
            {
                const value_type* const pEnd = mpBegin + corsac::min_alt(mnCount - 1, pos) + 1;
                const value_type* const pResult = CharTypeStringRFind(pEnd, mpBegin, c);

                if (pResult != mpBegin)
                    return (size_type)((pResult - 1) - mpBegin);
            }
            return npos;
        }

        constexpr size_type rfind(const T* s, size_type pos, size_type n) const
        {
            // Обратите внимание, что поиск строки нулевой длины, начинающейся с position = end(), возвращает end(), а не npos.
            if (CORSAC_LIKELY(n <= mnCount))
            {
                if (CORSAC_LIKELY(n))
                {
                    const const_iterator pEnd = mpBegin + corsac::min_alt(mnCount - n, pos) + n;
                    const const_iterator pResult = CharTypeStringRSearch(mpBegin, pEnd, s, s + n);

                    if (pResult != pEnd)
                        return (size_type)(pResult - mpBegin);
                }
                else
                    return corsac::min_alt(mnCount, pos);
            }
            return npos;
        }

        constexpr size_type rfind(const T* s, size_type pos = npos) const
        {
            return rfind(s, pos, (size_type)CharStrlen(s));
        }

        [[nodiscard]] constexpr size_type find_first_of(basic_string_view sw, size_type pos = 0) const noexcept
        {
            return find_first_of(sw.mpBegin, pos, sw.mnCount);
        }

        [[nodiscard]] constexpr size_type find_first_of(T c, size_type pos = 0) const noexcept { return find(c, pos); }

        constexpr size_type find_first_of(const T* s, size_type pos, size_type n) const
        {
            // Если позиция >= size, мы возвращаем npos.
            if (CORSAC_LIKELY((pos < mnCount)))
            {
                const value_type* const pBegin = mpBegin + pos;
                const value_type* const pEnd = mpBegin + mnCount;
                const const_iterator pResult = CharTypeStringFindFirstOf(pBegin, pEnd, s, s + n);

                if (pResult != pEnd)
                    return (size_type)(pResult - mpBegin);
            }
            return npos;
        }

        constexpr size_type find_first_of(const T* s, size_type pos = 0) const
        {
            return find_first_of(s, pos, (size_type)CharStrlen(s));
        }

        [[nodiscard]] constexpr size_type find_last_of(basic_string_view sw, size_type pos = npos) const noexcept
        {
            return find_last_of(sw.mpBegin, pos, sw.mnCount);
        }

        [[nodiscard]] constexpr size_type find_last_of(T c, size_type pos = npos) const noexcept { return rfind(c, pos); }

        constexpr size_type find_last_of(const T* s, size_type pos, size_type n) const
        {
            // Если n равно нулю или позиция >= size, мы возвращаем npos.
            if (CORSAC_LIKELY(mnCount))
            {
                const value_type* const pEnd = mpBegin + corsac::min_alt(mnCount - 1, pos) + 1;
                const value_type* const pResult = CharTypeStringRFindFirstOf(pEnd, mpBegin, s, s + n);

                if (pResult != mpBegin)
                    return (size_type)((pResult - 1) - mpBegin);
            }
            return npos;
        }

        constexpr size_type find_last_of(const T* s, size_type pos = npos) const
        {
            return find_last_of(s, pos, (size_type)CharStrlen(s));
        }

        [[nodiscard]] constexpr size_type find_first_not_of(basic_string_view sw, size_type pos = 0) const noexcept
        {
            return find_first_not_of(sw.mpBegin, pos, sw.mnCount);
        }

        [[nodiscard]] constexpr size_type find_first_not_of(T c, size_type pos = 0) const noexcept
        {
            if (CORSAC_LIKELY(pos <= mnCount))
            {
                const auto pEnd = mpBegin + mnCount;
                // Todo: Возможно сделать специализированную версию CharTypeStringFindFirstNotOf(pBegin, pEnd, c).
                const const_iterator pResult = CharTypeStringFindFirstNotOf(mpBegin + pos, pEnd, &c, &c + 1);

                if (pResult != pEnd)
                    return (size_type)(pResult - mpBegin);
            }
            return npos;
        }

        constexpr size_type find_first_not_of(const T* s, size_type pos, size_type n) const
        {
            if (CORSAC_LIKELY(pos <= mnCount))
            {
                const auto pEnd = mpBegin + mnCount;
                const const_iterator pResult = CharTypeStringFindFirstNotOf(mpBegin + pos, pEnd, s, s + n);

                if (pResult != pEnd)
                    return (size_type)(pResult - mpBegin);
            }
            return npos;
        }

        constexpr size_type find_first_not_of(const T* s, size_type pos = 0) const
        {
            return find_first_not_of(s, pos, (size_type)CharStrlen(s));
        }

        [[nodiscard]] constexpr size_type find_last_not_of(basic_string_view sw, size_type pos = npos) const noexcept
        {
            return find_last_not_of(sw.mpBegin, pos, sw.mnCount);
        }

        [[nodiscard]] constexpr size_type find_last_not_of(T c, size_type pos = npos) const noexcept
        {
            if (CORSAC_LIKELY(mnCount))
            {
                // Todo: Возможно сделать специализированную версию CharTypeStringRFindFirstNotOf(pBegin, pEnd, c).
                const value_type* const pEnd = mpBegin + corsac::min_alt(mnCount - 1, pos) + 1;
                const value_type* const pResult = CharTypeStringRFindFirstNotOf(pEnd, mpBegin, &c, &c + 1);

                if (pResult != mpBegin)
                    return (size_type)((pResult - 1) - mpBegin);
            }
            return npos;
        }

        constexpr size_type find_last_not_of(const T* s, size_type pos, size_type n) const
        {
            if (CORSAC_LIKELY(mnCount))
            {
                const value_type* const pEnd = mpBegin + corsac::min_alt(mnCount - 1, pos) + 1;
                const value_type* const pResult = CharTypeStringRFindFirstNotOf(pEnd, mpBegin, s, s + n);

                if (pResult != mpBegin)
                    return (size_type)((pResult - 1) - mpBegin);
            }
            return npos;
        }

        constexpr size_type find_last_not_of(const T* s, size_type pos = npos) const
        {
            return find_last_not_of(s, pos, (size_type)CharStrlen(s));
        }

        // starts_with
        [[nodiscard]] constexpr bool starts_with(basic_string_view x) const noexcept
        {
            return (size() >= x.size()) && (compare(0, x.size(), x) == 0);
        }

        [[nodiscard]] constexpr bool starts_with(T x) const noexcept
        {
            return starts_with(basic_string_view(&x, 1));
        }

        constexpr bool starts_with(const T* s) const
        {
            return starts_with(basic_string_view(s));
        }

        // ends_with
        [[nodiscard]] constexpr bool ends_with(basic_string_view x) const noexcept
        {
            return (size() >= x.size()) && (compare(size() - x.size(), npos, x) == 0);
        }

        [[nodiscard]] constexpr bool ends_with(T x) const noexcept
        {
            return ends_with(basic_string_view(&x, 1));
        }

        constexpr bool ends_with(const T* s) const
        {
            return ends_with(basic_string_view(s));
        }
    };

    template <class CharT>
    inline constexpr bool operator==(basic_string_view<CharT> lhs, basic_string_view<CharT> rhs) noexcept
    {
        return (lhs.size() == rhs.size()) && (lhs.compare(rhs) == 0);
    }

    template <class CharT>
    inline constexpr bool operator==(decay_t<basic_string_view<CharT>> lhs, basic_string_view<CharT> rhs) noexcept
    {
        return (lhs.size() == rhs.size()) && (lhs.compare(rhs) == 0);
    }

    template <class CharT>
    inline constexpr bool operator==(basic_string_view<CharT> lhs, decay_t<basic_string_view<CharT>> rhs) noexcept
    {
        return (lhs.size() == rhs.size()) && (lhs.compare(rhs) == 0);
    }

    template <class CharT>
    inline constexpr bool operator==(decay_t<basic_string_view<CharT>> lhs, decay_t<basic_string_view<CharT>> rhs) noexcept
    {
        return (lhs.size() == rhs.size()) && (lhs.compare(rhs) == 0);
    }

    template <class CharT>
    inline constexpr bool operator!=(basic_string_view<CharT> lhs, basic_string_view<CharT> rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <class CharT>
    inline constexpr bool operator<(basic_string_view<CharT> lhs, basic_string_view<CharT> rhs) noexcept
    {
        return lhs.compare(rhs) < 0;
    }

    template <class CharT>
    inline constexpr bool operator<=(basic_string_view<CharT> lhs, basic_string_view<CharT> rhs) noexcept
    {
        return !(rhs < lhs);
    }

    template <class CharT>
    inline constexpr bool operator>(basic_string_view<CharT> lhs, basic_string_view<CharT> rhs) noexcept
    {
        return rhs < lhs;
    }

    template <class CharT>
    inline constexpr bool operator>=(basic_string_view<CharT> lhs, basic_string_view<CharT> rhs) noexcept
    {
        return !(lhs < rhs);
    }

    // string_view / wstring_view
    using string_view   = basic_string_view<char>    ;
    using wstring_view  = basic_string_view<wchar_t> ;

    using u8string_view  = basic_string_view<char8_t>  ;  // C++20 функция, но всегда присутствует для единообразия.
    using u16string_view = basic_string_view<char16_t> ;
    using u32string_view = basic_string_view<char32_t> ;

    /**
    *  hash<string_view>
    *
    *  Мы предоставляем объекты хэш-функции Corsac для использования в контейнерах хэш-таблиц.
    *
    * Example usage:
    *     #include <Corsac/hash_set.h>
    *     hash_set<string_view> stringHashSet;
    */
    template <typename T> struct hash;

    template<> struct hash<string_view>
    {
        size_t operator()(const string_view& x) const
        {
            string_view::const_iterator p = x.cbegin();
            string_view::const_iterator end = x.cend();
            uint32_t result = 2166136261U; // Мы реализуем строковый хеш в стиле FNV.
            while (p != end)
                result = (result * 16777619) ^ (uint8_t)*p++;
            return (size_t)result;
        }
    };

    #if defined(CORSAC_CHAR8_UNIQUE) && CORSAC_CHAR8_UNIQUE
    template<> struct hash<u8string_view>
            {
                size_t operator()(const u8string_view& x) const
                {
                    u8string_view::const_iterator p = x.cbegin();
                    u8string_view::const_iterator end = x.cend();
                    uint32_t result = 2166136261U;
                    while (p != end)
                        result = (result * 16777619) ^ (uint8_t)*p++;
                    return (size_t)result;
                }
            };
    #endif

    template<> struct hash<u16string_view>
    {
        size_t operator()(const u16string_view& x) const
        {
            u16string_view::const_iterator p = x.cbegin();
            u16string_view::const_iterator end = x.cend();
            uint32_t result = 2166136261U;
            while (p != end)
                result = (result * 16777619) ^ (uint16_t)*p++;
            return (size_t)result;
        }
    };

    template<> struct hash<u32string_view>
    {
        size_t operator()(const u32string_view& x) const
        {
            u32string_view::const_iterator p = x.cbegin();
            u32string_view::const_iterator end = x.cend();
            uint32_t result = 2166136261U;
            while (p != end)
                result = (result * 16777619) ^ (uint32_t)*p++;
            return (size_t)result;
        }
    };

    #if defined(CORSAC_WCHAR_UNIQUE) && CORSAC_WCHAR_UNIQUE
    template<> struct hash<wstring_view>
            {
                size_t operator()(const wstring_view& x) const
                {
                    wstring_view::const_iterator p = x.cbegin();
                    wstring_view::const_iterator end = x.cend();
                    uint32_t result = 2166136261U;
                    while (p != end)
                        result = (result * 16777619) ^ (uint32_t)*p++;
                    return (size_t)result;
                }
            };
    #endif

    inline namespace literals
    {
        inline namespace string_view_literals
        {
            constexpr inline string_view operator "" sv(const char* str, size_t len) noexcept { return {str, len}; }
            constexpr inline u16string_view operator "" sv(const char16_t* str, size_t len) noexcept { return {str, len}; }
            constexpr inline u32string_view operator "" sv(const char32_t* str, size_t len) noexcept { return {str, len}; }
            constexpr inline wstring_view operator "" sv(const wchar_t* str, size_t len) noexcept { return {str, len}; }

            #if CORSAC_CHAR8_UNIQUE
                constexpr inline u8string_view operator "" sv(const char8_t* str, size_t len) noexcept { return {str, len}; }
            #endif
        }
    }
}

#endif //CORSAC_STRING_VIEW_H
