//
// test/types_compound_test.h
//
// Created by Falldot on 01.11.2021.
// Copyright (c) 2021 Corsac. All rights reserved.
//
#ifndef CORSAC_ENGINE_TYPES_COMPOUND_TEST_H
#define CORSAC_ENGINE_TYPES_COMPOUND_TEST_H

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
    return true;
}

#endif //CORSAC_ENGINE_TYPES_COMPOUND_TEST_H
