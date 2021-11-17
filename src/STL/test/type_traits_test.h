//
// test/types_traits_test.h
//
// Created by Falldot on 01.11.2021.
// Copyright (c) 2021 Corsac. All rights reserved.
//
#ifndef CORSAC_ENGINE_TYPE_TRAITS_TEST_H
#define CORSAC_ENGINE_TYPE_TRAITS_TEST_H

bool type_traits_test(Corsac::Block* assert)
{
    assert->add_block("conditional", [](Corsac::Block* assert)
    {
        using Type1 = corsac::conditional<true, int, double>::type;
        using Type2 = corsac::conditional<false, int, double>::type;
        using Type3 = corsac::conditional<sizeof(int) >= sizeof(double), int, double>::type;
        assert->equal("param is true, int, double", typeid(Type1).name(), typeid(int).name());
        assert->equal("param is false, int, double", typeid(Type2).name(), typeid(double).name());
        assert->equal("param is sizeof(int) >= sizeof(double), int, double", typeid(Type3).name(), typeid(double).name());
    });
    assert->add_block("is_same", [](Corsac::Block* assert)
    {
        assert->is_true("param is int, int", corsac::is_same<int, int>::value);
        assert->is_false("param is int, double", corsac::is_same<int, double>::value);
    });
    assert->add_block("is_const", [](Corsac::Block* assert)
    {
        assert->is_false("param is int", corsac::is_const<int>::value);
        assert->is_true("param is int const", corsac::is_const<int const>::value);
        assert->is_true("param is const int", corsac::is_const<const int>::value);
    });
    assert->add_block("is_volatile", [](Corsac::Block* assert)
    {
        assert->is_false("param is int", corsac::is_volatile<int>::value);
        assert->is_true("param is int volatile", corsac::is_volatile<int volatile>::value);
        assert->is_true("param is volatile int", corsac::is_volatile<volatile int>::value);
    });
    assert->add_block("is_reference", [](Corsac::Block* assert)
    {
        assert->is_false("param is int", corsac::is_reference<int>::value);
        assert->is_true("param is int&", corsac::is_reference<int&>::value);
        assert->is_true("param is int&&", corsac::is_reference<int&&>::value);
    });
    assert->add_block("is_function", [](Corsac::Block* assert)
    {
        int f();
        struct A {int fun() const&;};
        assert->is_false("param is int", corsac::is_function<int>::value);
        assert->is_true("param is int(int)", corsac::is_function<int(int)>::value);
        assert->is_true("param is decltype(f)", corsac::is_function<decltype(f)>::value);
        assert->is_false("param is decltype(&A::fun)", corsac::is_function<decltype(&A::fun)>::value);
    });
    assert->add_block("remove_const", [](Corsac::Block* assert)
    {
        using type1 = corsac::remove_const<const int>::type;
        using type2 = corsac::remove_const<volatile int>::type;
        using type3 = corsac::remove_const<const volatile int>::type;
        using type4 = corsac::remove_const<const volatile int*>::type;
        using type5 = corsac::remove_const<int * const volatile>::type;
        assert->is_true("param is const int", corsac::is_same<int, type1>::value);
        assert->is_true("param is volatile int", corsac::is_same<volatile int, type2>::value);
        assert->is_true("param is const volatile int", corsac::is_same<volatile int, type3>::value);
        assert->is_true("param is const volatile int*", corsac::is_same<const volatile int*, type4>::value);
        assert->is_true("param is int * const volatile", corsac::is_same<int* volatile, type5>::value);
    });
    assert->add_block("remove_volatile", [](Corsac::Block* assert)
    {
        using type1 = corsac::remove_volatile<const int>::type;
        using type2 = corsac::remove_volatile<volatile int>::type;
        using type3 = corsac::remove_volatile<const volatile int>::type;
        using type4 = corsac::remove_volatile<const volatile int*>::type;
        using type5 = corsac::remove_volatile<int * const volatile>::type;
        assert->is_true("param is const int", corsac::is_same<const int, type1>::value);
        assert->is_true("param is volatile int", corsac::is_same<int, type2>::value);
        assert->is_true("param is const volatile int", corsac::is_same<const int, type3>::value);
        assert->is_true("param is const volatile int*", corsac::is_same<const volatile int*, type4>::value);
        assert->is_true("param is int * const volatile", corsac::is_same<int* const, type5>::value);
    });
    assert->add_block("remove_cv", [](Corsac::Block* assert)
    {
        using type1 = corsac::remove_cv<const int>::type;
        using type2 = corsac::remove_cv<volatile int>::type;
        using type3 = corsac::remove_cv<const volatile int>::type;
        using type4 = corsac::remove_cv<const volatile int*>::type;
        using type5 = corsac::remove_cv<int * const volatile>::type;
        assert->is_true("param is const int", corsac::is_same<int, type1>::value);
        assert->is_true("param is volatile int", corsac::is_same<int, type2>::value);
        assert->is_true("param is const volatile int", corsac::is_same<int, type3>::value);
        assert->is_true("param is const volatile int*", corsac::is_same<const volatile int*, type4>::value);
        assert->is_true("param is int * const volatile", corsac::is_same<int*, type5>::value);
    });
    assert->add_block("add_reference", [](Corsac::Block* assert)
    {
        using type1 = corsac::add_reference<int>::type;
        using type2 = corsac::add_reference<const int>::type;
        using type3 = corsac::add_reference<int&>::type;
        assert->is_true("param is int", corsac::is_same<int&, type1>::value);
        assert->is_true("param is const int", corsac::is_same<const int&, type2>::value);
        assert->is_true("param is int&", corsac::is_same<int&, type3>::value);
    });
    assert->add_block("remove_reference", [](Corsac::Block* assert)
    {
        using type1 = corsac::remove_reference<int&>::type;
        using type2 = corsac::remove_reference<const int&>::type;
        using type3 = corsac::remove_reference<int&&>::type;
        assert->is_true("param is int&", corsac::is_same<int, type1>::value);
        assert->is_true("param is const int&", corsac::is_same<const int, type2>::value);
        assert->is_true("param is int&&", corsac::is_same<int, type3>::value);
    });
    assert->add_block("remove_cvref", [](Corsac::Block* assert)
    {
        using type1 = corsac::remove_cvref<const int&>::type;
        using type2 = corsac::remove_cvref<const int(&)[2]>::type;
        using type3 = corsac::remove_cvref<volatile int&&>::type;
        assert->is_true("param is int&", corsac::is_same<int, type1>::value);
        assert->is_true("param is const int&", corsac::is_same<int[2], type2>::value);
        assert->is_true("param is int&&", corsac::is_same<int, type3>::value);
    });
    assert->add_block("add_lvalue_reference", [](Corsac::Block* assert)
    {
        using type1 = corsac::add_lvalue_reference<void>::type;
        using type2 = corsac::add_lvalue_reference<int>::type;
        using type3 = corsac::add_lvalue_reference<int&>::type;
        assert->is_true("param is void", corsac::is_same<void, type1>::value);
        assert->is_true("param is int", corsac::is_same<int&, type2>::value);
        assert->is_true("param is int&", corsac::is_same<int&, type3>::value);
    });
    assert->add_block("add_rvalue_reference", [](Corsac::Block* assert)
    {
        using type1 = corsac::add_rvalue_reference<void>::type;
        using type2 = corsac::add_rvalue_reference<int>::type;
        using type3 = corsac::add_rvalue_reference<int&>::type;
        assert->is_true("param is void", corsac::is_same<void, type1>::value);
        assert->is_true("param is int", corsac::is_same<int&&, type2>::value);
        assert->is_true("param is int&", corsac::is_same<int&, type3>::value);
    });
    assert->add_block("declval", [](Corsac::Block* assert)
    {
        struct Default { int foo() const { return 1; } };
        struct NonDefault { NonDefault() = delete; int foo() const { return 1; } };
        decltype(Default().foo()) n1 = 1;
        decltype(corsac::declval<NonDefault>().foo()) n2 = n1;
        assert->equal("param is Default().foo()", n1, 1);
        assert->equal("param is declval<NonDefault>().foo()", n2, n1);
    });
    assert->add_block("static_max", [](Corsac::Block* assert)
    {
        assert->equal("param is 3, 5, 11, 3", corsac::static_max<3, 5, 11, 3>::value, 11);
    });
    assert->add_block("static_min", [](Corsac::Block* assert)
    {
        assert->equal("param is 3, 5, 11, 3", corsac::static_min<3, 5, 11, 3>::value, 3);
    });
    return true;
}

#endif //CORSAC_ENGINE_TYPES_TRAITS_TEST_H
