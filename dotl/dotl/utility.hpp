#pragma once

#include <utility>

using std::size_t;

using std::integer_sequence;
using std::make_integer_sequence;

using std::tuple;
using std::tuple_element_t;

namespace dotl
{
    template<typename... Ts>
    struct static_list;

    /**
     * find, find_v
     *
     * Линейный поиск по шаблонному списку типов
     */
	template <class Item, class List>
	struct find;

	template <class Item, class... Tail>
	struct find<Item, static_list<Item, Tail...>>
    { constexpr static const size_t value = 0; };

	template <class Item, class Head, class... Tail>
	struct find<Item, static_list<Head, Tail...>>
    { constexpr static const size_t value = find<Item, static_list<Tail...>>::value + 1; };

    template<typename Item, class List>
    constexpr static const size_t find_v = find<Item, List>::value;

    /**
     * front, front_t
     *
     * Получить первый элемент шаблонного списка типов
     */
    template <class List>
	struct front;

	template <typename Head, typename... Tail>
	struct front<static_list<Head, Tail...>>
    { using type = Head; };

    template<typename... List>
    using front_t = typename front<List...>::type;

	template<typename List>
	constexpr static const size_t len_v = List::len;

    /**
     * cat, cat_t
     *
     * Объединить несколько шаблонных списков в один
     */
    // template <typename... Us>
	// struct cat
	// { using type = static_list<Us...>; };

	// template <typename... Us>
	// struct cat<static_list<Us...>>
	// { using type = static_list<Us...>; };

	// template<typename ... Us1, typename... Us2>
	// struct cat<static_list<Us1...>, Us2...>
	// { using type = cat<static_list<Us1..., Us2...>>; };

	// template<typename ... Us1, typename ... Us2, typename... Us3>
	// struct cat<static_list<Us1...>, static_list<Us2...>, Us3...>
	// { using type = cat<static_list<Us1..., Us2...>, Us3...>; };

	template <typename Lists, typename ...>
	struct cat
	{ using type = Lists; };

	template <typename ... FirstList, typename ... SecondList, typename ... Lists>
	struct cat<static_list<FirstList...>, static_list<SecondList...>, Lists...>
	: public cat<static_list<FirstList..., SecondList...>, Lists...>
	{ };

	template<typename ... Lists>
	using cat_t = typename cat<Lists...>::type;

    /**
	 * make_index_range
	 *
	 * Получить набор целых чисел в заданном диапазоне
	 */
	template<typename T, typename, T Begin, bool Increasing>
	struct make_integer_range_impl;

	template<typename T, T... Is, T Begin>
	struct make_integer_range_impl<T, integer_sequence<T, Is...>, Begin, true>
	{
		using type = integer_sequence<T, Is+Begin...>;
	};

	template<typename T, T... Is, T Begin>
	struct make_integer_range_impl<T, integer_sequence<T, Is...>, Begin, false>
	{
		using type = integer_sequence<T, Begin-Is...>;
	};

	template<typename T, T Begin, T End>
	using make_integer_range = typename make_integer_range_impl<T, make_integer_sequence<T, (Begin < End) ? End-Begin : Begin-End>, Begin, (Begin < End)>::type;

	template<size_t Begin, size_t End>
	using make_index_range = make_integer_range<size_t, Begin, End>;

	/**
	 * static_list
	 */
	template<typename... Ts>
    struct static_list
    {
        // Получить первый элемент
        using front = front_t<static_list<Ts...>>;

        template <class... Us>
        using push = static_list<Ts..., Us...>;

        template <class... Us>
        using unshift = static_list<Us..., Ts...>;

        // Конвертация в схожий шаблон
        template<template<typename...> class U>
		using to = U<Ts...>;

        using to_tuple = to<tuple>;

        // Поиск элемента
		template<class T>
		constexpr static const size_t find = find_v<T, static_list<Ts...>>;

		// Длина
		constexpr static const size_t len = sizeof...(Ts);
    };

} // namespace dotl
