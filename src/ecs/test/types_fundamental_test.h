//
// test/type_fundamental_test.h
//
// Created by Falldot on 01.11.2021.
// Copyright (c) 2021 Corsac. All rights reserved.
//
#ifndef CORSAC_TYPES_H
#define CORSAC_TYPES_H

bool types_fundamental_test(CorsacTest* assert)
{
    auto is_void = assert->add_block("is_void");
    {
        is_void->is_true("param is void", corsac::is_void<void>::value);
        is_void->is_false("param is int", corsac::is_void<int>::value);
    }
    auto has_void_arg = assert->add_block("has_void_arg");
    {
        has_void_arg->is_true("param int void int", corsac::has_void_arg<int, void, int>::value);
        has_void_arg->is_false("param int float int", corsac::has_void_arg<int, float, int>::value);
    }
    auto is_null_pointer = assert->add_block("is_null_pointer");
    {
        is_null_pointer->is_true("param nullptr", corsac::is_null_pointer<decltype(nullptr)>::value);
        is_null_pointer->is_false("param int", corsac::is_null_pointer<int>::value);
    }
    auto is_integral = assert->add_block("is_integral");
    {
        is_integral->is_true("param int", corsac::is_integral<int>::value);
        is_integral->is_false("param void", corsac::is_integral<void>::value);
    }
    auto is_floating_point = assert->add_block("is_floating_point");
    {
        is_floating_point->is_true("param float", corsac::is_floating_point<float>::value);
        is_floating_point->is_false("param void", corsac::is_floating_point<void>::value);
    }
    auto is_arithmetic = assert->add_block("is_arithmetic");
    {
        is_arithmetic->is_true("param int", corsac::is_arithmetic<int>::value);
        is_arithmetic->is_true("param float", corsac::is_arithmetic<float>::value);
        is_arithmetic->is_false("param void", corsac::is_arithmetic<void>::value);
    }
    auto is_fundamental = assert->add_block("is_fundamental");
    {
        class T{};
        is_fundamental->is_true("param int", corsac::is_fundamental<int>::value);
        is_fundamental->is_true("param float", corsac::is_fundamental<float>::value);
        is_fundamental->is_true("param nullptr", corsac::is_fundamental<decltype(nullptr)>::value);
        is_fundamental->is_true("param void", corsac::is_fundamental<void>::value);
        is_fundamental->is_false("param class", corsac::is_fundamental<T>::value);
    }
    auto is_hat_type = assert->add_block("is_hat_type");
    {
        class T{};
        #if defined(CORSAC_COMPILER_WINRTCX_ENABLED)
            is_hat_type->is_true("param int", corsac::is_hat_type<int>::value);
            is_hat_type->is_true("param float", corsac::is_hat_type<float>::value);
            is_hat_type->is_true("param nullptr", corsac::is_hat_type<decltype(nullptr)>::value);
            is_hat_type->is_true("param void", corsac::is_hat_type<void>::value);
            is_hat_type->is_true("param class", corsac::is_hat_type<T>::value);
        #else
            is_hat_type->is_false("param int", corsac::is_hat_type<int>::value);
            is_hat_type->is_false("param float", corsac::is_hat_type<float>::value);
            is_hat_type->is_false("param nullptr", corsac::is_hat_type<decltype(nullptr)>::value);
            is_hat_type->is_false("param void", corsac::is_hat_type<void>::value);
            is_hat_type->is_false("param class", corsac::is_hat_type<T>::value);
        #endif
    }
    return true;
}

#endif // Header include guard

