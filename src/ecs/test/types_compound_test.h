//
// test/types_compound_test.h
//
// Created by Falldot on 01.11.2021.
// Copyright (c) 2021 Corsac. All rights reserved.
//
#ifndef CORSAC_ENGINE_TYPES_COMPOUND_TEST_H
#define CORSAC_ENGINE_TYPES_COMPOUND_TEST_H

template <typename T, typename U>
struct decay_equiv :
        std::is_same<typename std::decay<T>::type, U>::type
{};

template <class T>
struct Number { T n; };

template <class T, class U>
Number<typename std::common_type<T, U>::type> operator+(const Number<T>& lhs,
                                                        const Number<U>& rhs)
{
    return {lhs.n + rhs.n};
}

bool types_compound_test(CorsacTest* assert)
{
    assert->add_block("extent", [](CorsacTest* assert)
    {
        assert->equal("param is int[10]", corsac::extent<int[10]>::value, 10);
        assert->equal("param is int[3][6]", corsac::extent<int[3][6], 1>::value, 6);
        assert->equal("param is int[3][6]", corsac::extent<int[3][8][1], 2>::value, 1);
        assert->equal("param is int[]", corsac::extent<int[]>::value, 0);
    });
    assert->add_block("is_array", [](CorsacTest* assert)
    {
        assert->is_true("param is int[10]", corsac::is_array<int[10]>::value);
        assert->is_true("param is int[]", corsac::is_array<int[]>::value);
        assert->is_false("param is int", corsac::is_array<int>::value);
    });
    assert->add_block("is_array_of_known_bounds", [](CorsacTest* assert)
    {
        assert->is_true("param is int[10]", corsac::is_array_of_known_bounds<int[10]>::value);
        assert->is_false("param is int[]", corsac::is_array_of_known_bounds<int[]>::value);
        assert->is_false("param is int", corsac::is_array_of_known_bounds<int>::value);
    });
    assert->add_block("is_array_of_known_bounds", [](CorsacTest* assert)
    {
        assert->is_false("param is int[10]", corsac::is_array_of_unknown_bounds<int[10]>::value);
        assert->is_true("param is int[]", corsac::is_array_of_unknown_bounds<int[]>::value);
        assert->is_false("param is int", corsac::is_array_of_unknown_bounds<int>::value);
    });
    assert->add_block("is_member_function_pointer", [](CorsacTest* assert)
    {
        struct A {void member() {}};
        assert->is_true("param is decltype(&class::member)", corsac::is_member_function_pointer<decltype(&A::member)>::value);
        assert->is_false("param is void", corsac::is_member_function_pointer<void>::value);
    });
    assert->add_block("is_member_pointer", [](CorsacTest* assert)
    {
        class A {};
        assert->is_true("param is int(class::*)", corsac::is_member_pointer<int(A::*)>::value);
        assert->is_false("param is void", corsac::is_member_pointer<void>::value);
    });
    assert->add_block("is_member_object_pointer", [](CorsacTest* assert)
    {
        class A {};
        assert->is_true("param is int(class::*)", corsac::is_member_object_pointer<int(A::*)>::value);
        assert->is_false("param is int(class::*)()", corsac::is_member_object_pointer<int(A::*)()>::value);
    });
    assert->add_block("is_pointer", [](CorsacTest* assert)
    {
        class A {};
        assert->is_false("param is class", corsac::is_pointer<A>::value);
        assert->is_true("param is class*", corsac::is_pointer<A*>::value);
        assert->is_false("param is class&", corsac::is_pointer<A&>::value);
        assert->is_true("param is class**", corsac::is_pointer<A**>::value);
        assert->is_false("param is decltype(nullptr)", corsac::is_pointer<decltype(nullptr)>::value);
    });
    assert->add_block("is_convertible", [](CorsacTest* assert)
    {
        class A {};
        class B : public A {};
        class C {};
        class D { public: operator C() { return c; }  C c; };
        assert->is_true("param is <B*, A*>", corsac::is_convertible<B*, A*>::value);
        assert->is_false("param is <A*, B*>*", corsac::is_convertible<A*, B*>::value);
        assert->is_true("param is <D, C>", corsac::is_convertible<D, C>::value);
        assert->is_false("param is <B*, C*>", corsac::is_convertible<B*, C*>::value);
    });
    assert->add_block("is_union", [](CorsacTest* assert)
    {
        struct A {};
        typedef union {
            int a;
            float b;
        } B;
        struct C {
            B d;
        };
        #if defined(CORSAC_COMPILER_MSVC) || defined(CORSAC_COMPILER_GCC)
            assert->is_false("param is <A>", corsac::is_union<A>::value);
            assert->is_true("param is <B>", corsac::is_union<B>::value);
            assert->is_false("param is <C>", corsac::is_union<C>::value);
            assert->is_false("param is int", corsac::is_union<int>::value);
        #else
            assert->is_false("param is <A>", corsac::is_union<A>::value);
            assert->is_true("param is <B>", corsac::is_union<B>::value);
            assert->is_false("param is <C>", corsac::is_union<C>::value);
            assert->is_false("param is int", corsac::is_union<int>::value);
        #endif
    });
    assert->add_block("is_convertible", [](CorsacTest* assert)
    {
        struct A {};
        class B {};
        enum class C {};
        assert->is_true("param is <A>", corsac::is_class<A>::value);
        assert->is_true("param is <B>", corsac::is_class<B>::value);
        assert->is_false("param is <C>", corsac::is_class<C>::value);
        assert->is_false("param is int", corsac::is_class<int>::value);
    });
    assert->add_block("is_enum", [](CorsacTest* assert)
    {
        struct A { enum E { }; };
        enum E {};
        enum class Ec : int {};
        assert->is_false("param is A", corsac::is_enum<A>::value);
        assert->is_true("param is E", corsac::is_enum<E>::value);
        assert->is_true("param is A::E", corsac::is_enum<A::E>::value);
        assert->is_false("param is int", corsac::is_enum<int>::value);
        assert->is_true("param is Ec", corsac::is_enum<Ec>::value);
    });
    assert->add_block("is_polymorphic", [](CorsacTest* assert)
    {
        struct A {int m;};
        struct B {virtual void foo();};
        struct C : B {};
        struct D {virtual ~D() = default;};
        assert->is_false("param is A", corsac::is_polymorphic<A>::value);
        assert->is_true("param is B", corsac::is_polymorphic<B>::value);
        assert->is_true("param is C", corsac::is_polymorphic<C>::value);
        assert->is_true("param is D", corsac::is_polymorphic<D>::value);
    });
    assert->add_block("is_object", [](CorsacTest* assert)
    {
        class A {};
        assert->is_true("param is int", corsac::is_object<int>::value);
        assert->is_false("param is int&", corsac::is_object<int&>::value);
        assert->is_true("param is A", corsac::is_object<A>::value);
        assert->is_false("param is A&", corsac::is_object<A&>::value);
    });
    assert->add_block("is_scalar", [](CorsacTest* assert)
    {
        class A {};
        assert->is_true("param is int", corsac::is_scalar<int>::value);
        assert->is_false("param is A", corsac::is_scalar<A>::value);
    });
    assert->add_block("is_compound", [](CorsacTest* assert)
    {
        class A {};
        assert->is_true("param is A", corsac::is_compound<A>::value);
        assert->is_false("param is int", corsac::is_compound<int>::value);
    });
    assert->add_block("decay", [](CorsacTest* assert)
    {
        assert->is_true("param is int&&, int", decay_equiv<int&&, int>::value);
        assert->is_true("param is int&, int", decay_equiv<int&, int>::value);
        assert->is_true("param is int&&, int", decay_equiv<int&&, int>::value);
        assert->is_true("param is const int&, int", decay_equiv<const int&, int>::value);
        assert->is_true("param is int[2], int*", decay_equiv<int[2], int*>::value);
        assert->is_true("param is int(int), int(*)(int)", decay_equiv<int(int), int(*)(int)>::value);
    });
    assert->add_block("decay", [](CorsacTest* assert)
    {
        Number<int> i1 = {1}, i2 = {2};
        Number<double> d1 = {2.3}, d2 = {3.5};
        assert->equal("param is Number<int>", (i1 + i2).n, 3);
        assert->equal("param is Number<double>", (i1 + d2).n, 4.5);
        assert->equal("param is Number<double>", (d1 + i2).n, 4.3);
        assert->equal("param is Number<double>", (d1 + d2).n, 5.8);
    });
    return true;
}

#endif //CORSAC_ENGINE_TYPES_COMPOUND_TEST_H
