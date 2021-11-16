//
// test/type_fundamental_test.h
//
// Created by Falldot on 01.11.2021.
// Copyright (c) 2021 Corsac. All rights reserved.
//
#ifndef CORSAC_TYPE_TEST_H
#define CORSAC_TYPE_TEST_H

bool type_fundamental_test(Corsac::Block* assert)
{
    assert->add_block("is_void", [](Corsac::Block* assert)
    {
        assert->is_true("param is void", corsac::is_void<void>::value);
        assert->is_false("param is int", corsac::is_void<int>::value);
    });
    assert->add_block("has_void_arg", [](Corsac::Block* assert)
    {
        assert->is_true("param int void int", corsac::has_void_arg<int, void, int>::value);
        assert->is_false("param int float int", corsac::has_void_arg<int, float, int>::value);
    });
    assert->add_block("is_null_pointer", [](Corsac::Block* assert)
    {
        assert->is_true("param nullptr", corsac::is_null_pointer<decltype(nullptr)>::value);
        assert->is_false("param int", corsac::is_null_pointer<int>::value);
    });
    assert->add_block("is_integral", [](Corsac::Block* assert)
    {
        assert->is_true("param int", corsac::is_integral<int>::value);
        assert->is_false("param void", corsac::is_integral<void>::value);
    });
    assert->add_block("is_floating_point", [](Corsac::Block* assert)
    {
        assert->is_true("param float", corsac::is_floating_point<float>::value);
        assert->is_false("param void", corsac::is_floating_point<void>::value);
    });
    assert->add_block("is_arithmetic", [](Corsac::Block* assert)
    {
        assert->is_true("param int", corsac::is_arithmetic<int>::value);
        assert->is_true("param float", corsac::is_arithmetic<float>::value);
        assert->is_false("param void", corsac::is_arithmetic<void>::value);
    });
    assert->add_block("is_fundamental", [](Corsac::Block* assert)
    {
        class T{};
        assert->is_true("param int", corsac::is_fundamental<int>::value);
        assert->is_true("param float", corsac::is_fundamental<float>::value);
        assert->is_true("param nullptr", corsac::is_fundamental<decltype(nullptr)>::value);
        assert->is_true("param void", corsac::is_fundamental<void>::value);
        assert->is_false("param class", corsac::is_fundamental<T>::value);
    });
    assert->add_block("is_hat_type", [](Corsac::Block* assert)
    {
        class T{};
        #if defined(CORSAC_COMPILER_WINRTCX_ENABLED)
            assert->is_true("param int", corsac::is_hat_type<int>::value);
            assert->is_true("param float", corsac::is_hat_type<float>::value);
            assert->is_true("param nullptr", corsac::is_hat_type<decltype(nullptr)>::value);
            assert->is_true("param void", corsac::is_hat_type<void>::value);
            assert->is_true("param class", corsac::is_hat_type<T>::value);
        #else
            assert->is_false("param int", corsac::is_hat_type<int>::value);
            assert->is_false("param float", corsac::is_hat_type<float>::value);
            assert->is_false("param nullptr", corsac::is_hat_type<decltype(nullptr)>::value);
            assert->is_false("param void", corsac::is_hat_type<void>::value);
            assert->is_false("param class", corsac::is_hat_type<T>::value);
        #endif
    });
    return true;
}

#endif // Header include guard

