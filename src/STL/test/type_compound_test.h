//
// test/types_compound_test.h
//
// Created by Falldot on 01.11.2021.
// Copyright (c) 2021 Corsac. All rights reserved.
//
#ifndef CORSAC_ENGINE_TYPE_COMPOUND_TEST_H
#define CORSAC_ENGINE_TYPE_COMPOUND_TEST_H

template <typename T, typename U>
struct decay_equiv :
        corsac::is_same<typename std::decay<T>::type, U>::type
{};

template <class T>
struct Number { T n; };

template <class T, class U>
Number<typename corsac::common_type<T, U>::type> operator+(const Number<T>& lhs,
                                                        const Number<U>& rhs)
{
    return {lhs.n + rhs.n};
}

bool type_compound_test(Corsac::Block* assert)
{
    assert->add_block("extent", [](Corsac::Block* assert)
    {
        assert->equal("param is int[10]", corsac::extent<int[10]>::value, 10);
        assert->equal("param is int[3][6]", corsac::extent<int[3][6], 1>::value, 6);
        assert->equal("param is int[3][6]", corsac::extent<int[3][8][1], 2>::value, 1);
        assert->equal("param is int[]", corsac::extent<int[]>::value, 0);
    });
    assert->add_block("is_array", [](Corsac::Block* assert)
    {
        assert->is_true("param is int[10]", corsac::is_array<int[10]>::value);
        assert->is_true("param is int[]", corsac::is_array<int[]>::value);
        assert->is_false("param is int", corsac::is_array<int>::value);
    });
    assert->add_block("is_array_of_known_bounds", [](Corsac::Block* assert)
    {
        assert->is_true("param is int[10]", corsac::is_array_of_known_bounds<int[10]>::value);
        assert->is_false("param is int[]", corsac::is_array_of_known_bounds<int[]>::value);
        assert->is_false("param is int", corsac::is_array_of_known_bounds<int>::value);
    });
    assert->add_block("is_array_of_known_bounds", [](Corsac::Block* assert)
    {
        assert->is_false("param is int[10]", corsac::is_array_of_unknown_bounds<int[10]>::value);
        assert->is_true("param is int[]", corsac::is_array_of_unknown_bounds<int[]>::value);
        assert->is_false("param is int", corsac::is_array_of_unknown_bounds<int>::value);
    });
    assert->add_block("is_member_function_pointer", [](Corsac::Block* assert)
    {
        struct A {void member() {}};
        assert->is_true("param is decltype(&class::member)", corsac::is_member_function_pointer<decltype(&A::member)>::value);
        assert->is_false("param is void", corsac::is_member_function_pointer<void>::value);
    });
    assert->add_block("is_member_pointer", [](Corsac::Block* assert)
    {
        class A {};
        assert->is_true("param is int(class::*)", corsac::is_member_pointer<int(A::*)>::value);
        assert->is_false("param is void", corsac::is_member_pointer<void>::value);
    });
    assert->add_block("is_member_object_pointer", [](Corsac::Block* assert)
    {
        class A {};
        assert->is_true("param is int(class::*)", corsac::is_member_object_pointer<int(A::*)>::value);
        assert->is_false("param is int(class::*)()", corsac::is_member_object_pointer<int(A::*)()>::value);
    });
    assert->add_block("is_pointer", [](Corsac::Block* assert)
    {
        class A {};
        assert->is_false("param is class", corsac::is_pointer<A>::value);
        assert->is_true("param is class*", corsac::is_pointer<A*>::value);
        assert->is_false("param is class&", corsac::is_pointer<A&>::value);
        assert->is_true("param is class**", corsac::is_pointer<A**>::value);
        assert->is_false("param is decltype(nullptr)", corsac::is_pointer<decltype(nullptr)>::value);
    });
    assert->add_block("is_convertible", [](Corsac::Block* assert)
    {
        class A {};
        class B : public A {};
        class C {};
        class D { public: operator C() { return c; }  C c; };
        assert->is_true("param is <B*, A*>", corsac::is_convertible<B*, A*>::value);
        assert->is_false("param is <A*, B*>*", corsac::is_convertible<A*, B*>::value);
        assert->is_true("param is <D, C>", corsac::is_convertible<D, C>::value);
        assert->is_false("param is <B*, C*>", corsac::is_convertible<B*, C*>::value)
                ->add_comment("compiler dependent");
    });
    assert->add_block("is_union", [](Corsac::Block* assert)
    {
        struct A {};
        typedef union {
            int a;
            float b;
        } B;
        struct C {
            B d;
        };
        assert->is_false("param is <A>", corsac::is_union<A>::value);
        assert->is_true("param is <B>", corsac::is_union<B>::value);
        assert->is_false("param is <C>", corsac::is_union<C>::value);
        assert->is_false("param is int", corsac::is_union<int>::value)
                ->add_comment("compiler dependent");
    });
    assert->add_block("is_enum", [](Corsac::Block* assert)
    {
        struct A { enum E { }; };
        enum E {};
        enum class Ec : int {};
        assert->is_false("param is A", corsac::is_enum<A>::value);
        assert->is_true("param is E", corsac::is_enum<E>::value);
        assert->is_true("param is A::E", corsac::is_enum<A::E>::value);
        assert->is_false("param is int", corsac::is_enum<int>::value);
        assert->is_true("param is Ec", corsac::is_enum<Ec>::value)
                ->add_comment("compiler dependent");
    });
    assert->add_block("is_polymorphic", [](Corsac::Block* assert)
    {
        struct A {int m;};
        struct B {virtual void foo() {};};
        struct C : B {};
        struct D {virtual ~D() = default;};
        assert->is_false("param is A", corsac::is_polymorphic<A>::value);
        assert->is_true("param is B", corsac::is_polymorphic<B>::value);
        assert->is_true("param is C", corsac::is_polymorphic<C>::value);
        assert->is_true("param is D", corsac::is_polymorphic<D>::value)
            ->add_comment("compiler dependent");
    });
    assert->add_block("is_object", [](Corsac::Block* assert)
    {
        class A {};
        assert->is_true("param is int", corsac::is_object<int>::value);
        assert->is_false("param is int&", corsac::is_object<int&>::value);
        assert->is_true("param is A", corsac::is_object<A>::value);
        assert->is_false("param is A&", corsac::is_object<A&>::value);
    });
    assert->add_block("is_scalar", [](Corsac::Block* assert)
    {
        class A {};
        assert->is_true("param is int", corsac::is_scalar<int>::value);
        assert->is_false("param is A", corsac::is_scalar<A>::value);
    });
    assert->add_block("is_compound", [](Corsac::Block* assert)
    {
        class A {};
        assert->is_true("param is A", corsac::is_compound<A>::value);
        assert->is_false("param is int", corsac::is_compound<int>::value);
    });
    assert->add_block("decay", [](Corsac::Block* assert)
    {
        assert->is_true("param is int&&, int", decay_equiv<int&&, int>::value);
        assert->is_true("param is int&, int", decay_equiv<int&, int>::value);
        assert->is_true("param is int&&, int", decay_equiv<int&&, int>::value);
        assert->is_true("param is const int&, int", decay_equiv<const int&, int>::value);
        assert->is_true("param is int[2], int*", decay_equiv<int[2], int*>::value);
        assert->is_true("param is int(int), int(*)(int)", decay_equiv<int(int), int(*)(int)>::value);
    });
    assert->add_block("common_type", [](Corsac::Block* assert)
    {
        Number<int> i1 = {1}, i2 = {2};
        Number<double> d1 = {2.3}, d2 = {3.5};
        assert->equal("param is Number<int>", (i1 + i2).n, 3);
        assert->equal("param is Number<double>", (i1 + d2).n, 4.5);
        assert->equal("param is Number<double>", (d1 + i2).n, 4.3);
        assert->equal("param is Number<double>", (d1 + d2).n, 5.8);
    });
    assert->add_block("is_final", [](Corsac::Block* assert)
    {
        class A {};
        class B final {};
        assert->is_false("param is A", corsac::is_final<A>::value);
        assert->is_true("param is B final", corsac::is_final<B>::value)
                ->add_comment("compiler dependent, CLANG is not support!!!");
    });
    assert->add_block("is_aggregate", [](Corsac::Block* assert)
    {
        struct A { int x, y; };
        struct B { B (int, const char*) {} };
        assert->is_true("param is A", corsac::is_aggregate<A>::value);
        assert->is_false("param is B", corsac::is_aggregate<B>::value)
                ->add_comment("compiler dependent, CLANG is not support!!!");
    });
    return true;
}

#endif //CORSAC_ENGINE_TYPES_COMPOUND_TEST_H
