//
// test/types_transformation_test.h
//
// Created by Falldot on 03.11.2021.
// Copyright (c) 2021 Corsac. All rights reserved.
//
#ifndef CORSAC_ENGINE_TYPE_TRANSFORMATION_TEST_H
#define CORSAC_ENGINE_TYPE_TRANSFORMATION_TEST_H

bool type_transformation_test(Corsac::Block* assert) {
    assert->add_block("add_const", [](Corsac::Block *assert) {
        using Type1 = corsac::add_const<int>::type;
        using Type2 = corsac::add_const<volatile int>::type;
        using Type3 = corsac::add_const<const int>::type;
        assert->is_true("param is int", corsac::is_same<const int, Type1>::value);
        assert->is_true("param is volatile int", corsac::is_same<const volatile int, Type2>::value);
        assert->is_true("param is const int",corsac::is_same<const int, Type3>::value);
    });
    assert->add_block("add_volatile", [](Corsac::Block *assert) {
        using Type1 = corsac::add_volatile<int>::type;
        using Type2 = corsac::add_volatile<volatile int>::type;
        using Type3 = corsac::add_volatile<const int>::type;
        assert->is_true("param is int", corsac::is_same<volatile int, Type1>::value);
        assert->is_true("param is volatile int", corsac::is_same<volatile int, Type2>::value);
        assert->is_true("param is const int",corsac::is_same<volatile const int, Type3>::value);
    });
    assert->add_block("add_cv", [](Corsac::Block *assert) {
        using Type1 = corsac::add_cv<int>::type;
        using Type2 = corsac::add_cv<const volatile int>::type;
        assert->is_true("param is int", corsac::is_same<const volatile int, Type1>::value);
        assert->is_true("param is const volatile int", corsac::is_same<const volatile int, Type2>::value);
    });
    assert->add_block("make_signed", [](Corsac::Block *assert) {
        using Type1 = corsac::make_signed<unsigned int>::type;
        using Type2 = corsac::make_signed<unsigned char>::type;
        using Type3 = corsac::make_signed<volatile unsigned long>::type;
        assert->is_true("param is unsigned int", corsac::is_same<signed int, Type1>::value);
        assert->is_true("param is unsigned char", corsac::is_same<signed char, Type2>::value);
        assert->is_true("param is volatile unsigned long", corsac::is_same<volatile signed long, Type3>::value);
    });
    assert->add_block("make_unsigned", [](Corsac::Block *assert) {
        using Type1 = corsac::make_unsigned<signed int>::type;
        using Type2 = corsac::make_unsigned<signed char>::type;
        using Type3 = corsac::make_unsigned<volatile signed long>::type;
        assert->is_true("param is signed int", corsac::is_same<unsigned int, Type1>::value);
        assert->is_true("param is signed char", corsac::is_same<unsigned char, Type2>::value);
        assert->is_true("param is volatile signed long", corsac::is_same<volatile unsigned long, Type3>::value);
    });
    assert->add_block("remove_pointer", [](Corsac::Block *assert) {
        using Type1 = corsac::remove_pointer<int>::type;
        using Type2 = corsac::remove_pointer<int*>::type;
        using Type3 = corsac::remove_pointer<int**>::type;
        assert->is_true("param is int", corsac::is_same<int, Type1>::value);
        assert->is_true("param is int*", corsac::is_same<int, Type2>::value);
        assert->is_true("param is int**", corsac::is_same<int*, Type3>::value);
    });
    assert->add_block("add_pointer", [](Corsac::Block *assert) {
        using Type1 = corsac::add_pointer<int>::type;
        using Type2 = corsac::add_pointer<int*>::type;
        using Type3 = corsac::add_pointer<int**>::type;
        assert->is_true("param is int", corsac::is_same<int*, Type1>::value);
        assert->is_true("param is int*", corsac::is_same<int**, Type2>::value);
        assert->is_true("param is int**", corsac::is_same<int***, Type3>::value);
    });
    assert->add_block("remove_extent", [](Corsac::Block *assert) {
        using Type1 = corsac::remove_extent<int>::type;
        using Type2 = corsac::remove_extent<int[]>::type;
        using Type3 = corsac::remove_extent<int[3]>::type;
        assert->is_true("param is int", corsac::is_same<int, Type1>::value);
        assert->is_true("param is int[]", corsac::is_same<int, Type2>::value);
        assert->is_true("param is int[3]", corsac::is_same<int, Type3>::value);
    });
    assert->add_block("aligned_storage", [](Corsac::Block *assert) {
        using Type = corsac::aligned_storage<sizeof(int), alignof(double)>::type;
        assert->equal("param is sizeof(int), alignof(double)", alignof(int), 4);
        assert->equal("param is sizeof(int), alignof(double)", alignof(Type), 8);
    });
    assert->add_block("aligned_union", [](Corsac::Block *assert) {
        union U_type
        {
            int i;
            float f;
            double d;
            U_type(float e) : f(e) {}
        };
        using aligned_U_type = std::aligned_union<16, int, float, double>::type;
        aligned_U_type au;
        U_type* u = new (&au) U_type(1.0f);
        assert->equal("param is16, int, float, double, alignof(double)", u->i, 1065353216);
        u->~U_type();
    });
    return true;
}

#endif //CORSAC_ENGINE_TYPES_TRANSFORMATION_H
