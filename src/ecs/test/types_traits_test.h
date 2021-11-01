//
// test/types_traits_test.h
//
// Created by Falldot on 01.11.2021.
// Copyright (c) 2021 Corsac. All rights reserved.
//
#ifndef CORSAC_ENGINE_TYPES_TRAITS_TEST_H
#define CORSAC_ENGINE_TYPES_TRAITS_TEST_H

bool types_traits_test(CorsacTest* assert)
{
    assert->add_block("conditional", [](CorsacTest* assert)
    {
        using Type1 = corsac::conditional<true, int, double>::type;
        using Type2 = corsac::conditional<false, int, double>::type;
        using Type3 = corsac::conditional<sizeof(int) >= sizeof(double), int, double>::type;
        assert->equal("param is true, int, double", typeid(Type1).name(), typeid(int).name());
        assert->equal("param is false, int, double", typeid(Type2).name(), typeid(double).name());
        assert->equal("param is sizeof(int) >= sizeof(double), int, double", typeid(Type3).name(), typeid(double).name());
    });
    assert->add_block("is_same", [](CorsacTest* assert)
    {
        assert->is_true("param is int, int", corsac::is_same<int, int>::value);
        assert->is_false("param is int, double", corsac::is_same<int, double>::value);
    });
    assert->add_block("is_const", [](CorsacTest* assert)
    {
        assert->is_false("param is int", corsac::is_const<int>::value);
        assert->is_true("param is int const", corsac::is_const<int const>::value);
        assert->is_true("param is const int", corsac::is_const<const int>::value);
    });
    assert->add_block("is_volatile", [](CorsacTest* assert)
    {
        assert->is_false("param is int", corsac::is_volatile<int>::value);
        assert->is_true("param is int volatile", corsac::is_volatile<int volatile>::value);
        assert->is_true("param is volatile int", corsac::is_volatile<volatile int>::value);
    });
    assert->add_block("is_reference", [](CorsacTest* assert)
    {
        assert->is_false("param is int", corsac::is_reference<int>::value);
        assert->is_true("param is int&", corsac::is_reference<int&>::value);
        assert->is_true("param is int&&", corsac::is_reference<int&&>::value);
    });
    assert->add_block("is_function", [](CorsacTest* assert)
    {
        int f();
        struct A {int fun() const&;};
        assert->is_false("param is int", corsac::is_function<int>::value);
        assert->is_true("param is int(int)", corsac::is_function<int(int)>::value);
        assert->is_true("param is decltype(f)", corsac::is_function<decltype(f)>::value);
        assert->is_false("param is decltype(&A::fun)", corsac::is_function<decltype(&A::fun)>::value);
    });
    assert->add_block("remove_const", [](CorsacTest* assert)
    {
        using type1 = std::remove_const<const int>::type;
        using type2 = std::remove_const<volatile int>::type;
        using type3 = std::remove_const<const volatile int>::type;
        using type4 = std::remove_const<const volatile int*>::type;
        using type5 = std::remove_const<int * const volatile>::type;
        assert->is_true("param is const int", corsac::is_same<int, type1>::value);
        assert->is_true("param is volatile int", corsac::is_same<volatile int, type2>::value);
        assert->is_true("param is const volatile int", corsac::is_same<volatile int, type3>::value);
        assert->is_true("param is const volatile int*", corsac::is_same<const volatile int*, type4>::value);
        assert->is_true("param is int * const volatile", corsac::is_same<int* volatile, type5>::value);
    });
    assert->add_block("remove_volatile", [](CorsacTest* assert)
    {
        using type1 = std::remove_volatile<const int>::type;
        using type2 = std::remove_volatile<volatile int>::type;
        using type3 = std::remove_volatile<const volatile int>::type;
        using type4 = std::remove_volatile<const volatile int*>::type;
        using type5 = std::remove_volatile<int * const volatile>::type;
        assert->is_true("param is const int", corsac::is_same<const int, type1>::value);
        assert->is_true("param is volatile int", corsac::is_same<int, type2>::value);
        assert->is_true("param is const volatile int", corsac::is_same<const int, type3>::value);
        assert->is_true("param is const volatile int*", corsac::is_same<const volatile int*, type4>::value);
        assert->is_true("param is int * const volatile", corsac::is_same<int* const, type5>::value);
    });
    assert->add_block("remove_volatile", [](CorsacTest* assert)
    {
        using type1 = std::remove_cv<const int>::type;
        using type2 = std::remove_cv<volatile int>::type;
        using type3 = std::remove_cv<const volatile int>::type;
        using type4 = std::remove_cv<const volatile int*>::type;
        using type5 = std::remove_cv<int * const volatile>::type;
        assert->is_true("param is const int", corsac::is_same<int, type1>::value);
        assert->is_true("param is volatile int", corsac::is_same<int, type2>::value);
        assert->is_true("param is const volatile int", corsac::is_same<int, type3>::value);
        assert->is_true("param is const volatile int*", corsac::is_same<const volatile int*, type4>::value);
        assert->is_true("param is int * const volatile", corsac::is_same<int*, type5>::value);
    });
    return true;
}

#endif //CORSAC_ENGINE_TYPES_TRAITS_TEST_H
