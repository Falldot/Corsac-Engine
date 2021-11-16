//
// test/type_properties_test.h
//
// Created by Falldot on 04.11.2021.
// Copyright (c) 2021 Corsac. All rights reserved.
//
#ifndef CORSAC_ENGINE_TYPE_PROPERTIES_TEST_H
#define CORSAC_ENGINE_TYPE_PROPERTIES_TEST_H

#include <iomanip>

#pragma once

int fn(int) {return int();}
typedef int(&fn_ref)(int);
typedef int(*fn_ptr)(int);
struct fn_class { int operator()(int i){return i;} };

bool type_properties_test(Corsac::Block* assert)
{
    assert->add_block("underlying_type", [](Corsac::Block* assert) {
        enum e1 {};
        enum class e2 {};
        enum class e3: unsigned {};
        enum class e4: int {};
        assert->is_false("param is enum e1 {}",  corsac::is_same_v<corsac::underlying_type_t<e1>, int>);
        assert->is_true("param is enum class e2 {}",  corsac::is_same_v<corsac::underlying_type_t<e2>, int>);
        assert->is_false("param is enum class e3: unsigned {}",  corsac::is_same_v<corsac::underlying_type_t<e3>, int>);
        assert->is_true("param is enum class e4: int {}",  corsac::is_same_v<corsac::underlying_type_t<e4>, int>);
    });
    assert->add_block("to_underlying", [](Corsac::Block* assert) {
        enum class E1 : char { e };
        enum struct E2 : long { e };
        enum E3 : unsigned { e };
        assert->is_true("param is char { e };", corsac::is_same_v<char, decltype(corsac::to_underlying(E1::e))>);
        assert->is_true("param is long { e };", corsac::is_same_v<long, decltype(corsac::to_underlying(E2::e))>);
        assert->is_true("param is unsigned { e };", corsac::is_same_v<unsigned, decltype(corsac::to_underlying(e))>);
    });
    assert->add_block("has_unique_object_representations", [](Corsac::Block* assert) {
        struct foo {char c; float f; short st; int i; };
        struct bar {int a; int b; };
        assert->is_false("param is {char c; float f; short st; int i;}", corsac::has_unique_object_representations_v<foo>);
        assert->is_true("param is {int a; int b;}", corsac::has_unique_object_representations_v<bar>);
    });
    assert->add_block("is_signed", [](Corsac::Block* assert) {
        class A {};
        enum B : int {};
        enum class C : int {};
        assert->is_false("param is class A {}", corsac::is_signed<A>::value);
        assert->is_true("param is float", corsac::is_signed<float>::value);
        assert->is_true("param is signed int", corsac::is_signed<signed int>::value);
        assert->is_false("param is unsigned int", corsac::is_signed<unsigned int>::value);
        assert->is_false("param is enum B : int {}", corsac::is_signed<B>::value);
        assert->is_false("param is enum class C : int {}", corsac::is_signed<C>::value);
        assert->is_false("param is bool", corsac::is_signed<bool>::value);
        assert->is_true("param is signed int", corsac::is_signed<signed int>::value);
        assert->is_false("param is unsigned int", corsac::is_signed<unsigned int>::value);
    });
    assert->add_block("is_unsigned", [](Corsac::Block* assert) {
        class A {};
        enum B : int {};
        enum class C : int {};
        assert->is_false("param is class A {}", corsac::is_unsigned<A>::value);
        assert->is_false("param is float", corsac::is_unsigned<float>::value);
        assert->is_false("param is signed int", corsac::is_unsigned<signed int>::value);
        assert->is_true("param is unsigned int", corsac::is_unsigned<unsigned int>::value);
        assert->is_false("param is enum B : int {}", corsac::is_unsigned<B>::value);
        assert->is_false("param is enum class C : int {}", corsac::is_unsigned<C>::value);
        assert->is_false("param is bool", corsac::is_unsigned<bool>::value);
        assert->is_false("param is signed int", corsac::is_unsigned<signed int>::value);
        assert->is_true("param is unsigned int", corsac::is_unsigned<unsigned int>::value);
    });
    assert->add_block("alignment_of", [](Corsac::Block* assert) {
        struct A {};
        struct B { int8_t p; int16_t q; };
        assert->equal("param is struct {}", corsac::alignment_of<A>::value, 1);
        assert->equal("param is struct { int8_t p; int16_t q; }", corsac::alignment_of<B>::value, 2);
        assert->equal("param is int", corsac::alignment_of_v<int>, 4);
        assert->equal("param is double", corsac::alignment_of_v<double>, 8);
    });
    assert->add_block("is_aligned", [](Corsac::Block* assert) {
        struct A { int64_t p; int64_t q; };
        assert->is_false("param is struct { int64_t p; int64_t q; }", corsac::is_aligned<A>::value);
        assert->is_false("param is struct int", corsac::is_aligned<int>::value);
    });
    assert->add_block("rank", [](Corsac::Block* assert) {
        assert->is_false("param is struct int", corsac::rank<int>::value);
        assert->is_true("param is struct int[]", corsac::rank<int[]>::value);
        assert->is_true("param is struct int[3]", corsac::rank<int[3]>::value);
    });
    assert->add_block("is_base_of", [](Corsac::Block* assert) {
        class A {};
        class B : A {};
        class C {};
        assert->is_true("param is A, B", corsac::is_base_of<A, B>::value);
        assert->is_false("param is B, A", corsac::is_base_of<B, A>::value);
        assert->is_false("param is C, B", corsac::is_base_of<C, B>::value);
        assert->is_true("param is C, C", corsac::is_base_of<C, C>::value);
    });
    assert->add_block("is_lvalue_reference", [](Corsac::Block* assert) {
        assert->is_false("param is int", corsac::is_lvalue_reference<int>::value);
        assert->is_true("param is int&", corsac::is_lvalue_reference<int&>::value);
        assert->is_false("param is int&&", corsac::is_lvalue_reference<int&&>::value);
    });
    assert->add_block("is_rvalue_reference", [](Corsac::Block* assert) {
        assert->is_false("param is int", corsac::is_rvalue_reference<int>::value);
        assert->is_false("param is int&", corsac::is_rvalue_reference<int&>::value);
        assert->is_true("param si int&&", corsac::is_rvalue_reference<int&&>::value);
    });
    assert->add_block("result_of", [](Corsac::Block* assert) {
        using A = corsac::result_of<decltype(fn)&(int)>::type;
        using B = corsac::result_of<fn_ref(int)>::type;
        using C = corsac::result_of<fn_ptr(int)>::type;
        using D = corsac::result_of<fn_class(int)>::type;
        assert->is_true("param is decltype(fn)&(int)", std::is_same<int,A>::value);
        assert->is_true("param is fn_ref(int)", std::is_same<int,B>::value);
        assert->is_true("param is fn_ptr(int)", std::is_same<int,C>::value);
        assert->is_true("param is fn_class(int)", std::is_same<int,D>::value);
    });
    assert->add_block("has_equality", [](Corsac::Block* assert) {
        assert->is_true("param is int", corsac::has_equality<int>::value);
    });
    return true;
}

#endif //CORSAC_TYPE_PROPERTIES_H
