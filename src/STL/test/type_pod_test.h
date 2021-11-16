//
// test/type_pod_test.h
//
// Created by Falldot on 05.11.2021.
// Copyright (c) 2021 Corsac. All rights reserved.
//

#ifndef CORSAC_TYPE_POD_TEST_H
#define CORSAC_TYPE_POD_TEST_H

bool type_pod_test(Corsac::Block* assert)
{
    assert->add_block("is_empty", [](Corsac::Block* assert) {
        class A {};
        struct B {};
        class C {int a;};
        assert->is_true("param is A {}",  corsac::is_empty<A>::value);
        assert->is_true("param is B {}",  corsac::is_empty<B>::value);
        assert->is_false("param is C {int a;}",  corsac::is_empty<C>::value);
    });
    assert->add_block("is_pod", [](Corsac::Block* assert) {
        struct A {int m;};
        struct B {int m1; private:int m2;};
        struct C {virtual void foo() {};};
        assert->is_true("param is A {int m;}",  corsac::is_pod<A>::value);
        assert->is_false("param is B {int m1; private:int m2;}",  corsac::is_pod<B>::value);
        assert->is_false("param is C {virtual void foo();}",  corsac::is_pod<C>::value);
    });
    assert->add_block("is_standard_layout", [](Corsac::Block* assert) {
        struct A {int m;};
        struct B {int m1; private:int m2;};
        struct C {virtual void foo() {};};
        assert->is_true("param is A {int m;}",  corsac::is_standard_layout<A>::value);
        assert->is_false("param is B {int m1; private:int m2;}",  corsac::is_standard_layout<B>::value);
        assert->is_false("param is C {virtual void foo();}",  corsac::is_standard_layout<C>::value);
    });
    assert->add_block("is_abstract", [](Corsac::Block* assert) {
        struct A {int m;};
        struct B {virtual void foo() {};};
        struct C {virtual void foo() = 0;};
        struct D : C {};
        assert->is_false("param is A {int m;}",  corsac::is_abstract<A>::value);
        assert->is_false("param is B {virtual void foo() {};}",  corsac::is_abstract<B>::value);
        assert->is_true("param is C {virtual void foo() = 0;}",  corsac::is_abstract<C>::value);
        assert->is_true("param is D : C {}",  corsac::is_abstract<D>::value);
    });
    assert->add_block("is_trivially_copyable", [](Corsac::Block* assert) {
        struct A {int m;};
        struct B {B(B const&) {}};
        struct C {virtual void foo() {};};
        struct D {int m;D(D const&) = default; D(int x): m(x+1) {} };
        assert->is_true("param is A {int m;}",  corsac::is_trivially_copyable<A>::value);
        assert->is_false("param is B {B() noexcept {};}",  corsac::is_trivially_copyable<B>::value);
        assert->is_false("param is C C {virtual void foo() {};}",  corsac::is_trivially_copyable<C>::value);
        assert->is_true("param isD {int m;D(D const&) = default; D(int x): m(x+1) {} }",  corsac::is_trivially_copyable<D>::value);
    });
    assert->add_block("is_constructible", [](Corsac::Block* assert) {
        class Foo {int v1;double v2;public:Foo(int n) : v1(n), v2() {}Foo(int n, double f) noexcept : v1(n), v2(f) {}};
        assert->is_true("param is Foo",  corsac::is_constructible<Foo, const Foo&>::value);
        assert->is_true("param is Foo",  corsac::is_constructible<Foo, int>::value);
        assert->is_true("param is Foo",  corsac::is_constructible<Foo, int, double>::value);
    });
    assert->add_block("is_trivially_constructible", [](Corsac::Block* assert) {
        class Foo {int v1;double v2;public:Foo(int n) : v1(n), v2() {}Foo(int n, double f) noexcept : v1(n), v2(f) {}};
        assert->is_true("param is Foo",  corsac::is_trivially_constructible<Foo, const Foo&>::value);
        assert->is_false("param is Foo",  corsac::is_trivially_constructible<Foo, int>::value);
        assert->is_false("param is Foo",  corsac::is_trivially_constructible<Foo, int, double>::value);
    });
    assert->add_block("is_trivial", [](Corsac::Block* assert) {
        struct A {int m;};
        struct B {B() {}};
        assert->is_true("param is A {int m;}",  corsac::is_trivial<A>::value);
        assert->is_false("param is B {B() {}}",  corsac::is_trivial<B>::value);
    });
    assert->add_block("is_nothrow_constructible", [](Corsac::Block* assert) {
        class Foo {int v1;double v2;public:Foo(int n) : v1(n), v2() {}Foo(int n, double f) noexcept : v1(n), v2(f) {}};
        assert->is_false("param is Foo",  corsac::is_nothrow_constructible<Foo, int>::value);
        assert->is_true("param is Foo",  corsac::is_nothrow_constructible<Foo, int, double>::value);
    });
    assert->add_block("is_default_constructible", [](Corsac::Block* assert) {
        struct A {std::string str;};
        struct B {int n;B() = default;};
        assert->is_true("param is {std::string str;}",  corsac::is_default_constructible<A>::value);
        assert->is_true("param is {int n;B() = default;}",  corsac::is_default_constructible<B>::value);
    });
    assert->add_block("is_nothrow_default_constructible", [](Corsac::Block* assert) {
        struct A {std::string str;};
        struct B {int n;B() = default;};
        assert->is_true("param is {std::string str;}",  corsac::is_nothrow_default_constructible<A>::value);
        assert->is_true("param is {int n;B() = default;}",  corsac::is_nothrow_default_constructible<B>::value);
    });
    assert->add_block("is_nothrow_default_constructible", [](Corsac::Block* assert) {
        struct A {std::string str;};
        struct B {int n;B() = default;};
        assert->is_true("param is {std::string str;}",  corsac::is_nothrow_default_constructible<A>::value);
        assert->is_true("param is {int n;B() = default;}",  corsac::is_nothrow_default_constructible<B>::value);
    });
    assert->add_block("is_constructible", [](Corsac::Block* assert) {
        class Foo {int v1;double v2;public:Foo(int n) : v1(n), v2() {}Foo(int n, double f) noexcept : v1(n), v2(f) {}};
        assert->is_true("param is Foo",  corsac::is_constructible<Foo, int>::value);
    });
    assert->add_block("is_trivially_copy_constructible", [](Corsac::Block* assert) {
        struct A {std::string str;};
        struct B {int n;B() = default;};
        assert->is_false("param is {std::string str;}",  corsac::is_trivially_copy_constructible<A>::value);
        assert->is_true("param is {int n;B() = default;}",  corsac::is_trivially_copy_constructible<B>::value);
    });
    assert->add_block("is_nothrow_copy_constructible", [](Corsac::Block* assert) {
        struct A {std::string str;};
        struct B {int n;B() = default;};
        assert->is_false("param is {std::string str;}",  corsac::is_nothrow_copy_constructible<A>::value);
        assert->is_true("param is {int n;B() = default;}",  corsac::is_nothrow_copy_constructible<B>::value);
    });
    assert->add_block("is_move_constructible", [](Corsac::Block* assert) {
        struct NoMove {NoMove(const NoMove&) {}};
        assert->is_true("param is {NoMove(const NoMove&) {}",  corsac::is_move_constructible<NoMove>::value);
    });
    assert->add_block("is_trivially_move_constructible", [](Corsac::Block* assert) {
        struct A {std::string str;};
        struct B {int n;B() = default;};
        assert->is_false("param is {std::string str;}",  corsac::is_trivially_move_constructible<A>::value);
        assert->is_true("param is {int n;B() = default;}",  corsac::is_trivially_move_constructible<B>::value);
    });
    assert->add_block("is_assignable", [](Corsac::Block* assert) {
        assert->is_false("param is int, int",  corsac::is_assignable<int, int>::value);
        assert->is_true("param is int&, int",  corsac::is_assignable<int&, int>::value);
        assert->is_false("param is int, double",  corsac::is_assignable<int, double>::value);
    });
    assert->add_block("is_lvalue_assignable", [](Corsac::Block* assert) {
        class A {};
        assert->is_true("param is A",  corsac::is_copy_assignable<A>::value);
        assert->is_true("param is A&",  corsac::is_copy_assignable<A&>::value);
    });
    assert->add_block("is_trivially_assignable", [](Corsac::Block* assert) {
        struct A {int n;};
        assert->is_true("param is A",  corsac::is_trivially_assignable<A&, const A&>::value);
    });
    assert->add_block("is_nothrow_assignable", [](Corsac::Block* assert) {
        assert->is_true("param is int&, double",  corsac::is_nothrow_assignable<int&, double>::value);
    });
    assert->add_block("is_copy_assignable", [](Corsac::Block* assert) {
        assert->is_false("param is int[2]",  corsac::is_copy_assignable<int[2]>::value);
    });
    assert->add_block("is_trivially_copy_assignable", [](Corsac::Block* assert) {
        struct A {int n;};
        assert->is_true("param is {int n;}",  corsac::is_trivially_copy_assignable<A>::value);
    });
    assert->add_block("is_nothrow_copy_assignable", [](Corsac::Block* assert) {
        assert->is_true("param is int",  corsac::is_nothrow_copy_assignable<int>::value);
    });
    assert->add_block("is_nothrow_copy_assignable", [](Corsac::Block* assert) {
        assert->is_true("param is int",  corsac::is_nothrow_copy_assignable<int>::value);
    });
    assert->add_block("is_move_assignable", [](Corsac::Block* assert) {
        struct NoMove {NoMove& operator=(const NoMove&) { return *this; }};
        assert->is_false("param is int[2]",  corsac::is_move_assignable<int[2]>::value);
        assert->is_true("param is {NoMove(const NoMove&) {}",  corsac::is_move_assignable<NoMove>::value);
    });
    assert->add_block("is_trivially_move_assignable", [](Corsac::Block* assert) {
        struct A { int n; };
        assert->is_true("param is { int n; }",  corsac::is_trivially_move_assignable<A>::value);
    });
    assert->add_block("param is is_nothrow_move_assignable", [](Corsac::Block* assert) {
        struct NoMove {NoMove& operator=(const NoMove&) { return *this; }};
        assert->is_true("param is std::string",  corsac::is_nothrow_move_assignable<std::string>::value);
        assert->is_false("param is {NoMove(const NoMove&) {}}",  corsac::is_nothrow_move_assignable<NoMove>::value);
    });
    assert->add_block("param is is_destructible", [](Corsac::Block* assert) {
        assert->is_true("param is std::string",  corsac::is_destructible<std::string>::value);
    });
    assert->add_block("param is is_trivially_destructible", [](Corsac::Block* assert) {
        struct A {std::string str; ~A() noexcept {};};
        struct B {int n;~B() = default;};
        assert->is_false("param is {std::string str;}",  corsac::is_trivially_destructible<A>::value);
        assert->is_true("param is {int n;B() = default;}",  corsac::is_trivially_destructible<B>::value);
    });
    assert->add_block("is_nothrow_destructible", [](Corsac::Block* assert) {
        struct A {std::string str; ~A() noexcept {};};
        assert->is_true("param is {std::string str; ~A() noexcept {};}",  corsac::is_nothrow_destructible<A>::value);
    });
    assert->add_block("is_nothrow_move_constructible", [](Corsac::Block* assert) {
        struct NoMove {NoMove(const NoMove&) {} };
        assert->is_false("param is {int n;~B() = default;}",  corsac::is_nothrow_move_constructible<NoMove>::value);
    });
    return true;
}

#endif //CORSAC_TYPE_POD_TEST_H
