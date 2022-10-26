#pragma once

#include <tuple>

#include "Corsac/tuple_vector.h"
#include "utility.hpp"

using std::tie;
using std::move;
using std::tuple;
using std::forward;
using std::true_type;
using std::false_type;
using std::make_tuple;
using std::tuple_size_v;
using std::conditional_t;
using std::index_sequence;
using std::tuple_element_t;
using std::is_fundamental_v;

using corsac::tuple_vector;

namespace dotl
{
	/**
	 * scheme_meta
	 */
	template<typename... Types>
	struct scheme_meta {
		using is_component 	 = false_type;
		using is_true_scheme = false_type;

		constexpr static bool is_component_v = is_component::value;
		constexpr static bool is_true_scheme_v = is_true_scheme::value;
	};

	template<typename... ComponentTypes, typename... FundamentalTypes>
	struct scheme_meta<static_list<ComponentTypes...>, static_list<FundamentalTypes...>>
	{
		using is_true_scheme 	= true_type;
		using is_component 	    = conditional_t<sizeof...(ComponentTypes) <= 1, true_type, false_type>;

		using component_types 	= static_list<ComponentTypes...>;
		using simple_types 		= static_list<FundamentalTypes...>;

		constexpr static bool is_component_v = is_component::value;
		constexpr static bool is_true_scheme_v = is_true_scheme::value;
	};

	/**
	 * is_scheme, is_scheme_v
	 */
	template<typename T, typename = void>
	struct is_scheme : false_type {};

	template<typename T>
	struct is_scheme<T, decltype(std::declval<T>().is_true_scheme_v, void())> : true_type {};

	template<typename T>
	constexpr static bool is_scheme_v = is_scheme<T>::value;

    /**
     * scheme_builder
     */
    template<typename scheme_type>
	struct scheme_builder
	{
		using self = scheme_builder<scheme_type>;

		scheme_builder(scheme_type* sheme, size_t index) : mSheme(sheme), mIndex(index) {};

		self create() 	{ mIndex = mSheme->create_and_get_index(); 	return *this; }
		self remove() 	{ mSheme->remove(mIndex); 					return *this; }

		template<typename T, typename... Ts>
		self set(Ts&&... args) { mSheme->template set<T>(mIndex, forward<Ts>(args)...); return *this; }

		template<size_t N, typename... Ts>
		self set(Ts&&... args) { mSheme->template set<N>(mIndex, forward<Ts>(args)...); return *this; }

		template<typename T>
		[[nodiscard]] auto get() -> decltype(auto) { return mSheme->template get<T>(mIndex); }

		template<size_t N>
		[[nodiscard]] auto get() -> decltype(auto) { return mSheme->template get<N>(mIndex); }

		self reserve()	{ mSheme->reserve(); 			return *this; }
		self reset() 	{ mSheme->reset_lose_memory(); 	return *this; }
		self clear() 	{ mSheme->clear(); 				return *this; }

	private:
		scheme_type* mSheme;
		size_t mIndex;
	};

	/**
	 * scheme_len, scheme_len_v
	 * */

	template<class Type, bool is_scheme>
	struct scheme_len_impl
	{};

	template<class Type>
	struct scheme_len_impl<Type, true>
	{ constexpr static const size_t value = Type::simple_types::len; };

	template<class Type>
	struct scheme_len_impl<Type, false>
	{ constexpr static const size_t value = 1; };

	template<class Type>
	struct scheme_len
	{ constexpr static const size_t value = scheme_len_impl<Type, is_scheme_v<Type>>::value; };

	template<class Type>
	constexpr static const size_t scheme_len_v = scheme_len_impl<Type, is_scheme_v<Type>>::value;

    /**
     * find_component_index, find_component_index_v
     */
    template <class Type, class List>
	struct find_component_index;

	template <class Item, class... Tail>
	struct find_component_index<Item, static_list<Item, Tail...>> {
		constexpr static const size_t value = 0;
	};

	template <class Item, class Head, class... Tail>
	struct find_component_index<Item, static_list<Head, Tail...>> {
		constexpr static const size_t value = scheme_len_v<Head> + find_component_index<Item, static_list<Tail...>>::value;
	};

	template<class Item, class... List>
	constexpr static const size_t find_component_index_v = find_component_index<Item, List...>::value;

    /**
     * scheme_store
     */
    template<typename meta_data>
	struct scheme_store : private meta_data::simple_types::template to<tuple_vector>
	{
	private:
		using base_tuple_vector = typename meta_data::simple_types::template to<tuple_vector>;
		using base_tuple 		= typename meta_data::simple_types::to_tuple;

		using base_tuple_vector::get;
		using base_tuple_vector::clear;
		using base_tuple_vector::reserve;
		using base_tuple_vector::erase_unsorted;
		using base_tuple_vector::push_back_index;
		using base_tuple_vector::reset_lose_memory;

		template<size_t... Ns>
		struct sheme_element_impl
		{ using type = tuple<tuple_element_t<Ns, base_tuple>& ...>; };

		template<size_t N>
		struct sheme_element_impl<N>
		{
			static_assert(N < tuple_size_v<base_tuple>, "Trying to get an index value larger than the tuple size");
			using type = tuple_element_t<N, base_tuple>&;
		};

		template<size_t... Ns>
		using sheme_element = typename sheme_element_impl<Ns...>::type;

	public:
		using base_tuple_vector::begin;
		using base_tuple_vector::end;
		using base_tuple_vector::size;
		using base_tuple_vector::capacity;

		using this_t	= scheme_store<meta_data>;
		using self 		= scheme_builder<this_t>;

		template<typename... Types, typename Func>
		inline void each(Func func)
		{
			for (int i = 0; i < size(); i++)
			{
				std::apply(func, make_tuple(i, get<Types>(i)...));
			}
		}

		// TODO: Разложить на отдельные методы и вычислисть возвращаемый тип
		template <typename Scheme,
			std::enable_if_t<is_scheme_v<Scheme>, bool> = true
		>
		[[nodiscard]] constexpr auto get(size_t index) -> decltype(auto)
		{
			// if constexpr(is_scheme_v<Type>)
			// {
				constexpr size_t scheme_len = Scheme::simple_types::len;

				if constexpr(Scheme::is_component_v)
				{
					constexpr size_t component_index = find_component_index<Scheme, typename meta_data::component_types>::value;
					if constexpr (scheme_len == 1)
						return get<component_index>(index);
					else
						return get(index, make_index_range<component_index, component_index + scheme_len>{});
				}
				else
				{
					constexpr size_t scheme_index = find_component_index<typename Scheme::component_types::front, typename meta_data::component_types>::value;
					if constexpr (scheme_len == 1)
						return get<scheme_index>(index);
					else
						return get(index, make_index_range<scheme_index, scheme_index + scheme_len>{});
				}
			// }
			// else
			// {
			// 	constexpr size_t component_index = find<Type, typename meta_data::simple_types>::value;
			// 	return get<component_index>(index);
			// }
		}

		template <typename Type,
			std::enable_if_t<is_scheme_v<Type>, bool> = false
		>
		[[nodiscard]] constexpr Type& get(size_t index)
		{
			constexpr size_t component_index = find<Type, typename meta_data::simple_types>::value;
			return get<component_index>(index);
		}

		template<size_t N>
		[[nodiscard]] constexpr inline sheme_element<N> get(size_t index)
		{ return get<N>()[index]; }

		template<size_t... Ns>
		[[nodiscard]] constexpr inline sheme_element<Ns...> get(size_t index, index_sequence<Ns...>)
		{ return tie(get<Ns>(index)...); }

		inline self create() 					{ return self(this, push_back_index()); }

		inline size_t create_and_get_index() 	{ return push_back_index(); }

		inline self remove(size_t index) 		{ erase_unsorted(index); self(this, index); }

		template<typename T, typename... Ts>
		constexpr inline self set(size_t index, Ts... args) 	{ get<T>(index) = make_tuple(args...); 	return self(this, index); }

		template<size_t N, typename... Ts>
		constexpr inline self set(size_t index, Ts... args) 	{ get<N>(index) = make_tuple(args...); 	return self(this, index); }

		template<typename T, typename U>
		constexpr inline self set(size_t index, U arg) 			{ get<T>(index) = arg; 					return self(this, index); }

		template<size_t N, typename U>
		constexpr inline self set(size_t index, U arg) 			{ get<N>(index) = arg; 					return self(this, index); }

		// Установить размер контейнера
		inline self reserve(size_t n) 	{ reserve(n); 			return *this; }

		// Сбросить контейнер до нуля, не освобождая память
		inline self reset()    			{ reset_lose_memory(); 	return *this; }

		// Сбросить контейнер до нуля, освободив память
		inline self clear() 			{ clear(); 				return *this; }
	};

	/**
	 * get_component_types, get_component_types_t
	 */

	template <typename Type, bool is_scheme_type>
	struct get_component_types_scheme {};

	template <typename Type>
	struct get_component_types_scheme<Type, true>
	{ using type = conditional_t<Type::is_component_v, static_list<Type>, typename Type::component_types>; };

	template <typename Type>
	struct get_component_types_scheme<Type, false>
	{ using type = static_list<Type>; };

	template <typename Type, bool is_fundamental_type>
	struct get_component_types {};

	template <typename Type>
	struct get_component_types<Type, true>
	{ using type = static_list<>; };

	template <typename Type>
	struct get_component_types<Type, false>
	{ using type = typename get_component_types_scheme<Type, is_scheme_v<Type>>::type; };

	template <typename Type>
	using get_component_types_t = typename get_component_types<Type, is_fundamental_v<Type>>::type;

	/**
	 * get_simple_types, get_simple_types_t
	 */

	template <typename Type, bool is_scheme_type>
	struct get_simple_types_scheme {};

	template <typename Type>
	struct get_simple_types_scheme<Type, true>
	{ using type = conditional_t<Type::is_true_scheme_v, typename Type::simple_types, static_list<Type>>; };

	template <typename Type>
	struct get_simple_types_scheme<Type, false>
	{ using type = static_list<Type>; };


	template <typename Type, bool is_fundamental_type>
	struct get_simple_types {};

	template <typename Type>
	struct get_simple_types<Type, false>
	{ using type = typename get_simple_types_scheme<Type, is_scheme_v<Type>>::type; };

	template <typename Type>
	struct get_simple_types<Type, true>
	{ using type = static_list<Type>; };

	template <typename Type>
	using get_simple_types_t = typename get_simple_types<Type, is_fundamental_v<Type>>::type;

    /**
	 * scheme_constructor, scheme_constructor_t
	 */
	template<typename... Types>
	struct scheme_constructor
	{
		using type = scheme_meta<
			cat_t<get_component_types_t<Types>...>,
			cat_t<get_simple_types_t<Types>...>
		>;
	};

	template <typename... Types>
	using scheme_constructor_t = typename scheme_constructor<Types...>::type;

	/**
	 * scheme
	 */

	template<typename... Ts>
	using scheme = scheme_constructor_t<Ts...>;

	/**
	 * store
	 */
	template<typename T>
	using store = scheme_store<T>;

} // namespace dotl
