/**
 * corsac::STL
 *
 * type_traits.h
 *
 * Created by Falldot on 01.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STL_TYPE_TRAITS_H
#define CORSAC_STL_TYPE_TRAITS_H

#pragma once
/**
 * Описание (Falldot 07.11.2021)
 *
 * Реализация здесь почти полностью основана на метапрограммировании шаблонов.
 * То есть, обозначает, использование функциональности шаблонов для определения
 * и преобразования типов на стадии компиляции вашей программы. API и алгоритмы
 * обновляеться с учетом последних версий библиотек corsac и Boost, а также в зависимости от
 * требований Corsac Engine. Мы здесь делаем сильный упор в возможности компилятора,
 * поэтому функциональность может меняться от одного компилятора к другому.
 *
 *  Trait                                            Description
 * ---------------------------------------------------------------------------------------------------
 * === Определители типов:
 *
 *    is_void<T>                                T преднадлежит типу void.
 *    is_null_pointer<T>                        Т принадлежит типу nullptr_t.
 *    is_integral<T>                            T принадлежит целому чесловому типу.
 *    is_floating_point<T>                      T принадлежит типу числа с плавающей точкой.
 *    is_array<T>                               T принадлежит типу массива.
 *    is_enum<T>                                T принадлежит типу перечисления.
 *    is_union<T>                               T принадлежит типу объединения.
 *    is_object<T>                              T принадлежит типу объекта.
 *    is_class<T>                               T принадлежит типу класса или структуры.
 *    is_function<T>                            T принадлежит типу функции.
 *
 *    is_signed<T>                              T являеться целочисленным типом со знаком.
 *    is_unsigned<T>                            T являеться целочисленным типом без знака.
 *
 *    is_pointer<T>                             T являеться указателем на тип. Включая указатель на функцию.
 *    is_member_object_pointer<T>               T являеться указателем на объект.
 *    is_member_function_pointer<T>             T являеться указателем на функцию.
 *    is_reference<T>                           T являеться ссылочным типом. Включая ссылку на функцию.
 *    is_rvalue_reference<T>                    T являеться ссылочным типом rvalue.
 *    is_lvalue_reference<T>                    T являеться ссылочным типом lvalue.
 *
 *    is_fundamental<T>                         T является фундаментальным типом (void, integral или floating).
 *    is_arithmetic<T>                          T является арифметическим типом (integral или floating).
 *    is_scalar<T>                              T является скалярным типом (arithmetic, enum, pointer, member_pointer)
 *    is_compound<T>                            T являеться вычесляемым типом (fundamental).
 *    is_member_pointer<T>                      T является указателем на экземпляр или экземпляр функции.
 *
 *    is_const<T>                               T определен как const.
 *    is_volatile<T>                            T определен как volatile.
 *
 *    is_trivial<T>                             T является тривиальным типом (scalar, trivially copyable, trivially constructible).
 *    is_trivially_copyable<T>                  Т являеться тривиально копируемым типом (полным типом или массивом с неизвестной границей).
 *    is_trivially_constructible<T>             Т владеет тривиальным конструктором (все аргументы полного типа, или/и void, или/и массивы с неизвестной границей).
 *    is_standard_layout<T>                     Т являеться типом стандартной компоновки (Standard Layout Type), которая не содержит специальных возможностей C++.
 *    is_pod<T>                                 T являеться pod (Plain Old Data) типом (scalar, trivially, is_standard_layout). Совместим с C.
 *
 *    is_empty<T>                               T являеться пустым классом или структурой.
 *    is_polymorphic<T>                         T являеться полиморфным классом или структурой (объявляет или наследует хотя бы одну виртуальную функцию).
 *    is_abstract<T>                            T являеться абстрактным классом или структурой (объявляет или наследует хотя бы одну чистую виртуальную функцию).
 *
 *    is_same<T, U>                             T эквивалентен U.
 *    is_base_of<T, U>                          T являеться производным U.
 *    is_convertible<T, U>                      Т возможно неявно преобразовать в U.
 *
 *    is_default_constructible<T>               T владеет конструктором по умолчанию
 *    is_constructible<T, Args...>              T владеет конструтором определяюший набор Args типов.
 *    is_nothrow_constructible<T, Args...>      T владеет конструктором определяюший набор Args типов не генерирующий исключения.
 *    is_trivially_default_constructible<T>     T владеет тривиальным конструктором по умолчанию.
 *    is_nothrow_default_constructible<T>       T владеет тривиальным конструктором по умолчанию не генерирующий исключения.
 *
 *    is_copy_constructible<T>                  T владеет конструктором для копирования.
 *    is_trivially_copy_constructible<T>        T владеет тревиальным конструктором для копирования.
 *    is_nothrow_copy_constructible<T>          T владеет не генерирующим исключения конструктором для копирования.
 *
 *    is_move_constructible<T>                  T владеет конструктором для переноса.
 *    is_trivially_move_constructible<T>        T владеет тревиальным конструктором для переноса.
 *    is_nothrow_move_constructible<T>          T владеет не генерирующим исключения конструктором для переноса.
 *
 *    is_assignable<T, U>                       T может быть присвоен из U.
 *    is_trivially_assignable<T, U>             T может быть, тривиально, присвоен из U.
 *    is_nothrow_assignable<T, U>               T может не генерируя исключения, быть присвоен из U.
 *    is_copy_assignable<T, U>                  T может быть присваиваемым при копировании из U.
 *    is_trivially_copy_assignable<T, U>        T может быть, тривиально, присваиваемым при копировании из U.
 *    is_nothrow_copy_assignable<T, U>          T может не генерируя исключения, быть присваиваемым при копировании из U.
 *    is_move_assignable<T, U>                  T может быть присваиваемым при перемещении из U.
 *    is_trivially_move_assignable<T, U>        T может быть, тривиально, присваиваемым при перемещении из U.
 *    is_nothrow_move_assignable<T, U>          T может не генерируя исключения, быть присваиваемым при перемещении из U.
 *
 *    is_destructible<T>                        T владеет диструктором.
 *    is_trivially_destructible<T>              T владеет тривиальным диструктором.
 *    is_nothrow_destructible<T>                T владеет диструктором не генерирующий исключения.
 *    has_virtual_destructor<T>                 T владеет вируальным диструктором.
 *
 *
 *    alignment_of<T>
 *      Целочисленное значение, представляющее количество байтов выравнивания объектов типа T;
 *      объект типа T может быть размещен по адресу, кратному его выравниванию.
 *
 *    rank<T>
 *      Целочисленное значение, представляющее ранг объектов типа T.
 *      Термин "ранг" здесь используется для описания количества измерений типа массива.
 *
 *    extent<T, i>
 *      Целочисленное значение, представляющее протяженность (размерность) i-й границы объектов типа T.
 *      Если тип T не является типом массива, имеет ранг меньше I или если I == 0 и T имеет тип массив
 *      неизвестной границы U, тогда значение должно быть равно нулю;
 *      в противном случае value будет оцениваться как количество элементов в i-й границе массива T.
 *      Термин «степень» здесь используется для описания количества элементов в типе массива.
 *
 * === Преобразователи типов:
 *
 *    remove_cv<T>                              T переопределяеться без const и volatile.
 *    remove_const<T>                           T переопределяеться без const.
 *    remove_volatile<T>                        T переопределяеться без volatile.
 *    add_cv<T>                                 T переопределяеться как const и volatile.
 *    add_const<T>                              T переопределяеться как const.
 *    add_volatile<T>                           T переопределяеться как volatile.
 *
 *    remove_reference<T>                       Возвращает тип на который ссылался Т.
 *    add_lvalue_reference<T>                   Возвращает lvalue Т.
 *    add_rvalue_reference<T>                   Возвращает rvalue Т.
 *
 *    remove_pointer<T>                         Возвращает тип на который указывал Т.
 *    add_pointer<T>                            Возвращает указатель на Т.
 *
 *    make_signed<T>                            Возвращет соответстующий Т, знаковый целочисленный тип.
 *    make_unsigned<T>                          Возвращет соответстующий Т, беззнаковый целочисленный тип.
 *
 *    remove_extent<T>                          Удаляет измерение из массива.
 *    remove_all_extents<T>                     Удаляет все измерение из массива.
 *
 *    aligned_storage<T, U>                     Возвращает статический массив Т размера, выровненный по U значению.
 *    aligned_union<T, U>                       Возвращает объединение Т размера, выровненный по U типу.
 *    decay<T>                                  Преобразование lvalue в rvalue, массива в указатель, функции в указатель.
 *    conditional<T, U, I>                      Возращает U, если выражение T true или I, если T false.
 *    common_type<T...>                         Определяет общий тип одного или нескольких типов.
 *    underlying_type<T>                        Создает базовый целочисленный тип для типа перечисления.
 *    result_of<T, Args...>                     Определяет возвращаемый тип вызываемого типа, который принимает заданные типы аргументов.
 *    identity<T>                               Просто устанавливает T как тип.
 *
 * === Служебный типы:
 *
 *    disable_if
 *    enable_if
 *    integral_constant
 *    bool_constant
 *    true_type
 *    false_type
 *
 * === Взаимствование с eastl:
 *
 *    call_traits<T>                            Гарантирует, что такие проблемы, rак «ссылки на ссылки», никогда не возникают, и что параметры передаются наиболее эффективным способом.
 *    is_aligned<T>                             Определяется как истина, если у типа требования к выравниванию выше, чем выравнивание по умолчанию, которое принимается равным 8.
 *    union_cast<sT, dT>(dV)                    Обеспечивает удобное для чтения приведение между типами, которые не связаны между собой, но имеют двоичную эквивалентность. Классический вариант использования - преобразование между представлениями битов float и int32_t.
 *    is_array_of_known_bounds<T>               Т являеться массивом с известными границами.
 *    is_array_of_unknown_bounds<T>             Т являеться массивом с неизвестными границами.
 *    add_reference<T>                          Возвращает ссылку на Т.
 *
 *    yes_type                                  Служебный тип.
 *    no_type                                   Служебный тип.
 *
 *    is_swappable                              <STL/utility.h>
 *    is_nothrow_swappable                      "
 */
#include "Corsac/STL/config.h"

#include "Corsac/STL/type_converters.h"
#include "Corsac/STL/type_fundamental.h"
#include "Corsac/STL/type_transformation.h"
#include "Corsac/STL/type_properties.h"
#include "Corsac/STL/type_compound.h"
#include "Corsac/STL/type_pod.h"

namespace corsac
{
    /**
    * call_traits
    *
    * Дизайн call_traits здесь очень похож на тот, который можно найти в библиотеках
    * метапрограммирования шаблонов, таких как Boost, GCC и Metrowerks, учитывая, что эти
    * библиотеки установили этот интерфейс как стандарт для решения этой проблемы.
    * Кроме того, они описаны в различных книгах по теме метапрограммирования шаблонов,
    * таких как «Современный дизайн C++».
    *
    * Цель call_traits заключается в том, чтобы гарантировать, что такие проблемы,
    * как «ссылки на ссылки», никогда не возникают, и что параметры передаются
    * наиболее эффективным способом. В каждом случае, если ваша существующая практика
    * заключается в использовании типа, определенного слева, замените его call_traits определенным типом справа.
    * Примеры:
    *      call_traits<T>::value_type      -> T        (возврат по значению)
    *      call_traits<T>::reference       -> T&       (возвращаемое значение)
    *      call_traits<T>::const_reference -> const T& (возвращаемое значение)
    *      call_traits<T>::param_type      -> const T& (параметр функции)
    */
    template <typename T, bool small_>
    struct ct_imp2 { using param_type = const T&; };

    template <typename T>
    struct ct_imp2<T, true> { using param_type = const T; };

    template <typename T, bool isp, bool b1>
    struct ct_imp { using param_type = const T&; };

    template <typename T, bool isp>
    struct ct_imp<T, isp, true> { using param_type = typename ct_imp2<T, sizeof(T) <= sizeof(void*)>::param_type; };

    template <typename T, bool b1>
    struct ct_imp<T, true, b1> { using param_type = T const; };

    template <typename T>
    struct call_traits
    {
    public:
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using param_type = typename ct_imp<T, is_pointer<T>::value, is_arithmetic<T>::value>::param_type ;
    };

    template <typename T>
    struct call_traits<T&>
    {
        using value_type = T&;
        using reference = T&;
        using const_reference = const T&;
        using param_type = T&;
    };

    template <typename T, size_t N>
    struct call_traits<T [N]>
    {
    private:
        typedef T array_type[N];

    public:
        using value_type = const T*;
        using reference = array_type&;
        using const_reference = const array_type&;
        using param_type = const T* const;
    };

    template <typename T, size_t N>
    struct call_traits<const T [N]>
    {
    private:
        typedef const T array_type[N];

    public:
        using value_type = const T*;
        using reference = array_type&;
        using const_reference = const array_type&;
        using param_type = const T* const;
    };

    /**
     * integer_sequence
     *
     * Представляет последовательность целых чисел. Может использоваться для вывода и расширения пакетов
     * параметров в Variadic типах, таких как std::Tuple<T...>, которые передаются в качестве аргументов функции.
     */
    template <typename T, T... Ints>
    class integer_sequence
    {
    public:
        using value_type = T;
        static_assert(corsac::is_integral<T>::value, "corsac::integer_sequence can only be instantiated with an integral type");
        static constexpr size_t size() noexcept { return sizeof...(Ints); }
    };

    template <size_t N, typename IndexSeq>
    struct make_index_sequence_impl;

    template <size_t N, size_t... Is>
    struct make_index_sequence_impl<N, integer_sequence<size_t, Is...>>
    {
        using type = typename make_index_sequence_impl<N - 1, integer_sequence<size_t, N - 1, Is...>>::type;
    };

    template <size_t... Is>
    struct make_index_sequence_impl<0, integer_sequence<size_t, Is...>>
    {
        using type = integer_sequence<size_t, Is...>;
    };

    template <size_t... Is>
    using index_sequence = integer_sequence<size_t, Is...>;

    template <size_t N>
    using make_index_sequence = typename make_index_sequence_impl<N, integer_sequence<size_t>>::type;

    template <typename Target, typename Seq>
    struct integer_sequence_convert_impl;

    template <typename Target, size_t... Is>
    struct integer_sequence_convert_impl<Target, integer_sequence<size_t, Is...>>
    {
        using type = integer_sequence<Target, Is...>;
    };

    template <typename T, size_t N>
    struct make_integer_sequence_impl
    {
        using type = typename integer_sequence_convert_impl<T, make_index_sequence<N>>::type;
    };

    template <typename T, size_t N>
    using make_integer_sequence = typename make_integer_sequence_impl<T, N>::type;

    // Шаблон псевдонима помощника, который преобразует любой набор параметров типа в последовательность индексов той же длины
    template<typename... T>
    using index_sequence_for = make_index_sequence<sizeof...(T)>;

    namespace internal
    {

        template <typename T>
        struct integer_sequence_size_helper;

        template <typename T, T... Ints>
        struct integer_sequence_size_helper<corsac::integer_sequence<T, Ints...>> : public integral_constant<size_t, sizeof...(Ints)>
        {};

        template <typename T>
        struct integer_sequence_size : public integer_sequence_size_helper<corsac::remove_cv_t<T>>
        {};

        template <typename T>
        struct index_sequence_size : public integer_sequence_size_helper<corsac::remove_cv_t<T>>
        {};

        template <typename T>
        inline constexpr size_t integer_sequence_size_v = integer_sequence_size<T>::value;

        template <typename T>
        inline constexpr size_t index_sequence_size_v = index_sequence_size<T>::value;
    }
}

#endif //CORSAC_STL_TYPE_TRAITS_H
