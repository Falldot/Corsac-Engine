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
    assert->add_block("has_trivial_constructor", [](Corsac::Block* assert) {
        struct A {int m;};
        struct B {B(B const&) {}};
        struct C {virtual void foo() {};};
        struct D {int m;D(D const&) = default; D(int x): m(x+1) {} };
        assert->is_true("param is A {int m;}",  corsac::has_trivial_constructor<A>::value);
        assert->is_false("param is B {B(B const&) {}}",  corsac::has_trivial_constructor<B>::value);
        assert->is_false("param is C {virtual void foo();}",  corsac::has_trivial_constructor<C>::value);
        assert->is_false("param is D {int m;D(D const&) = default; D(int x): m(x+1) {} };",  corsac::has_trivial_constructor<D>::value);
    });
    assert->add_block("has_trivial_copyable", [](Corsac::Block* assert) {
        struct A {int m;};
        struct B {B(B const&) {}};
        struct C {virtual void foo() {};};
        struct D {int m;D(D const&) = default; D(int x): m(x+1) {} };
        assert->is_true("param is A {int m;}",  corsac::has_trivial_copyable<A>::value);
        assert->is_false("param is B {B(B const&) {}}",  corsac::has_trivial_copyable<B>::value);
        assert->is_false("param is C {virtual void foo();}",  corsac::has_trivial_copyable<C>::value);
        assert->is_true("param is D {int m;D(D const&) = default; D(int x): m(x+1) {} };",  corsac::has_trivial_copyable<D>::value);
    });
    assert->add_block("has_trivial_assign", [](Corsac::Block* assert) {
        struct A {int m;};
        struct B {B(B const&) {}};
        struct C {virtual void foo() {};};
        struct D {int m;D(D const&) = default; D(int x): m(x+1) {} };
        assert->is_true("param is A {int m;}",  corsac::has_trivial_assign<A>::value);
        assert->is_true("param is B {B(B const&) {}}",  corsac::has_trivial_assign<B>::value);
        assert->is_false("param is C {virtual void foo();}",  corsac::has_trivial_assign<C>::value);
        assert->is_true("param is D {int m;D(D const&) = default; D(int x): m(x+1) {} };",  corsac::has_trivial_assign<D>::value);
    });
    assert->add_block("has_trivial_destructor", [](Corsac::Block* assert) {
        struct A {int m;};
        struct B {~B() noexcept {};};
        struct C {~C() = default;};
        struct D {int m; ~D() = default;};
        assert->is_true("param is A {int m;}",  corsac::has_trivial_destructor<A>::value);
        assert->is_false("param is B {~B() noexcept {};}",  corsac::has_trivial_destructor<B>::value);
        assert->is_true("param is C {~C() = default;}",  corsac::has_trivial_destructor<C>::value);
        assert->is_true("param is D {int m; ~D() = default;}",  corsac::has_trivial_destructor<D>::value);
    });
    assert->add_block("has_nothrow_constructor", [](Corsac::Block* assert) {
        struct A {int m;};
        struct B {B(B const&) {}};
        struct C {virtual void foo() {};};
        struct D {int m;D(D const&) = default; D(int x): m(x+1) {} };
        assert->is_true("param is A {int m;}",  corsac::has_nothrow_constructor<A>::value);
        assert->is_false("param is B {B() noexcept {};}",  corsac::has_nothrow_constructor<B>::value);
        assert->is_true("param is C {virtual void foo() {};}",  corsac::has_nothrow_constructor<C>::value);
        assert->is_false("param is D {int m;D(D const&) = default; D(int x): m(x+1) {} }",  corsac::has_nothrow_constructor<D>::value);
    });
    assert->add_block("has_nothrow_copy", [](Corsac::Block* assert) {
        struct A {int m;};
        struct B {B(B const&) {}};
        struct C {virtual void foo() {};};
        struct D {int m;D(D const&) = default; D(int x): m(x+1) {} };
        assert->is_true("param is A {int m;}",  corsac::has_nothrow_copy<A>::value);
        assert->is_false("param is B {B() noexcept {};}",  corsac::has_nothrow_copy<B>::value);
        assert->is_true("param is C C {virtual void foo() {};}",  corsac::has_nothrow_copy<C>::value);
        assert->is_true("param isD {int m;D(D const&) = default; D(int x): m(x+1) {} }",  corsac::has_nothrow_copy<D>::value);
    });
    assert->add_block("has_nothrow_assign", [](Corsac::Block* assert) {
        struct A {int m;};
        struct B {B(B const&) {}};
        struct C {virtual void foo() {};};
        struct D {int m;D(D const&) = default; D(int x): m(x+1) {} };
        assert->is_true("param is A {int m;}",  corsac::has_nothrow_assign<A>::value);
        assert->is_true("param is B {B() noexcept {};}",  corsac::has_nothrow_assign<B>::value);
        assert->is_true("param is C {virtual void foo() {};};",  corsac::has_nothrow_assign<C>::value);
        assert->is_true("param is D {int m;D(D const&) = default; D(int x): m(x+1) {} }",  corsac::has_nothrow_assign<D>::value);
    });
    assert->add_block("has_virtual_destructor", [](Corsac::Block* assert) {
        struct A {int m;};
        struct B {~B() noexcept {};};
        struct C {~C() = default;};
        struct D {int m; virtual ~D() {};};
        assert->is_false("param is A {int m;}",  corsac::has_virtual_destructor<A>::value);
        assert->is_false("param is B {~B() noexcept {};}",  corsac::has_virtual_destructor<B>::value);
        assert->is_false("param is C {~C() = default;}",  corsac::has_virtual_destructor<C>::value);
        assert->is_true("param is D {int m; ~D() = default;}",  corsac::has_virtual_destructor<D>::value);
    });
    assert->add_block("is_literal_type", [](Corsac::Block* assert) {
        struct A {int m;};
        struct B {virtual ~B() {};};
        assert->is_true("param is A {int m;}",  corsac::is_literal_type<A>::value);
        assert->is_false("param is B B {virtual ~B() {};}",  corsac::is_literal_type<B>::value);
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
    assert->add_block("is_trivially_default_constructible", [](Corsac::Block* assert) {
        struct A {std::string str;};
        struct B {int n;B() = default;};
        assert->is_false("param is A {std::string str;}",  corsac::is_trivially_default_constructible<A>::value);
        assert->is_true("param is B {int n;B() = default;}",  corsac::is_trivially_default_constructible<B>::value);
    });

    return true;
}

#endif //CORSAC_TYPE_POD_TEST_H
