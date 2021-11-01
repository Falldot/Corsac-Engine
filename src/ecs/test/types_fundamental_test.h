//
// internal/type_fundamental_test.h
//
// Created by Falldot on 01.11.2021.
// Copyright (c) 2021 Corsac. All rights reserved.
//
#ifndef CORSAC_TYPES_H
#define CORSAC_TYPES_H


#define TEST_ENABLE
#include "../../test/Test.h"

#include "internall/type_traits.h"
#include "internall/type_fundamental.h"
#include "internall/type_transformation.h"
#include "internall/type_compound.h"

bool types_fundamental_test(CorsacTest* assert)
{
    auto is_void = assert->add_block("is_void");
    {
        is_void->equal("param is void", corsac::is_void<void>::value, 1);
        is_void->equal("param is int", corsac::is_void<int>::value, 0);
    }
    auto has_void_arg = assert->add_block("has_void_arg");
    {
        has_void_arg->equal("param int void int", corsac::has_void_arg<int, void, int>::value, 1);
        has_void_arg->equal("param int float int", corsac::has_void_arg<int, float, int>::value, 0);
    }
    auto is_null_pointer = assert->add_block("is_null_pointer");
    {
        is_null_pointer->equal("param nullptr", corsac::is_null_pointer<decltype(nullptr)>::value, 1);
        is_null_pointer->equal("param int", corsac::is_null_pointer<int>::value, 0);
    }
    auto is_integral = assert->add_block("is_integral");
    {
        is_integral->equal("param int", corsac::is_integral<int>::value, 1);
        is_integral->equal("param void", corsac::is_integral<void>::value, 0);
    }
    auto is_floating_point = assert->add_block("is_floating_point");
    {
        is_floating_point->equal("param float", corsac::is_floating_point<float>::value, 1);
        is_floating_point->equal("param void", corsac::is_floating_point<void>::value, 0);
    }
    auto is_arithmetic = assert->add_block("is_arithmetic");
    {
        is_arithmetic->equal("param int", corsac::is_arithmetic<int>::value, 1);
        is_arithmetic->equal("param float", corsac::is_arithmetic<float>::value, 1);
        is_arithmetic->equal("param void", corsac::is_arithmetic<void>::value, 0);
    }
    auto is_fundamental = assert->add_block("is_fundamental");
    {
        class T{};
        is_fundamental->equal("param int", corsac::is_fundamental<int>::value, 1);
        is_fundamental->equal("param float", corsac::is_fundamental<float>::value, 1);
        is_fundamental->equal("param nullptr", corsac::is_fundamental<decltype(nullptr)>::value, 1);
        is_fundamental->equal("param void", corsac::is_fundamental<void>::value, 1);
        is_fundamental->equal("param class", corsac::is_fundamental<T>::value, 0);
    }
    auto is_hat_type = assert->add_block("is_hat_type");
    {
        class T{};
        #if defined(CORSAC_COMPILER_WINRTCX_ENABLED)
            is_hat_type->equal("\t param int", corsac::is_hat_type<int>::value, 1);
            is_hat_type->equal("\t param float", corsac::is_hat_type<float>::value, 1);
            is_hat_type->equal("\t param nullptr", corsac::is_hat_type<decltype(nullptr)>::value, 1);
            is_hat_type->equal("\t param void", corsac::is_hat_type<void>::value, 1);
            is_hat_type->equal("\t param class", corsac::is_hat_type<T>::value, 1);
        #else
            is_hat_type->equal("param int", corsac::is_hat_type<int>::value, 0);
            is_hat_type->equal("param float", corsac::is_hat_type<float>::value, 0);
            is_hat_type->equal("param nullptr", corsac::is_hat_type<decltype(nullptr)>::value, 0);
            is_hat_type->equal("param void", corsac::is_hat_type<void>::value, 0);
            is_hat_type->equal("param class", corsac::is_hat_type<T>::value, 0);
        #endif
    }
    return true;
}

#endif // Header include guard

