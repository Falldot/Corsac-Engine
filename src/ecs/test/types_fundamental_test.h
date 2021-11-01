// Copyright (c) 2021 Corsac Team
// License: MIT License
// MIT License web page: https://opensource.org/licenses/MIT
#ifndef CORSAC_TYPES_H
#define CORSAC_TYPES_H


#define TEST_ENABLE
#include "../../test/Test.h"

#include "internall/type_traits.h"
#include "internall/type_fundamental.h"
#include "internall/type_transformation.h"
#include "internall/type_compound.h"

void types()
{
    auto assert = new CorsacTest();

    assert->equal("is_void", 1, 1);
    {
        assert->equal("\t param is void", corsac::is_void<void>::value, 1);
        assert->equal("\t param is int", corsac::is_void<int>::value, 0);
    }
    assert->equal("has_void_arg", 1, 1);
    {
        assert->equal("\t param int void int", corsac::has_void_arg<int, void, int>::value, 1);
        assert->equal("\t param int float int", corsac::has_void_arg<int, float, int>::value, 0);
    }
    assert->equal("is_null_pointer", 1, 1);
    {
        assert->equal("\t param nullptr", corsac::is_null_pointer<decltype(nullptr)>::value, 1);
        assert->equal("\t param int", corsac::is_null_pointer<int>::value, 0);
    }
    assert->equal("is_integral", 1, 1);
    {
        assert->equal("\t param int", corsac::is_integral<int>::value, 1);
        assert->equal("\t param void", corsac::is_integral<void>::value, 0);
    }
    assert->equal("is_floating_point_helper", 1, 1);
    {
        assert->equal("\t param float", corsac::is_floating_point_helper<float>::value, 1);
        assert->equal("\t param void", corsac::is_floating_point_helper<void>::value, 0);
    }
    assert->equal("is_arithmetic", 1, 1);
    {
        assert->equal("\t param int", corsac::is_arithmetic<int>::value, 1);
        assert->equal("\t param float", corsac::is_arithmetic<float>::value, 1);
        assert->equal("\t param void", corsac::is_arithmetic<void>::value, 0);
    }
    assert->equal("is_fundamental", 1, 1);
    {
        class T{};
        assert->equal("\t param int", corsac::is_fundamental<int>::value, 1);
        assert->equal("\t param float", corsac::is_fundamental<float>::value, 1);
        assert->equal("\t param nullptr", corsac::is_fundamental<decltype(nullptr)>::value, 1);
        assert->equal("\t param void", corsac::is_fundamental<void>::value, 1);
        assert->equal("\t param class", corsac::is_fundamental<T>::value, 0);
    }
    assert->equal("is_hat_type", 1, 1);
    {
        class T{};
        #if defined(CORSAC_COMPILER_WINRTCX_ENABLED)
            assert->equal("\t param int", corsac::is_hat_type<int>::value, 1);
            assert->equal("\t param float", corsac::is_hat_type<float>::value, 1);
            assert->equal("\t param nullptr", corsac::is_hat_type<decltype(nullptr)>::value, 1);
            assert->equal("\t param void", corsac::is_hat_type<void>::value, 1);
            assert->equal("\t param class", corsac::is_hat_type<T>::value, 1);
        #else
            assert->equal("\t param int", corsac::is_hat_type<int>::value, 0);
            assert->equal("\t param float", corsac::is_hat_type<float>::value, 0);
            assert->equal("\t param nullptr", corsac::is_hat_type<decltype(nullptr)>::value, 0);
            assert->equal("\t param void", corsac::is_hat_type<void>::value, 0);
            assert->equal("\t param class", corsac::is_hat_type<T>::value, 0);
        #endif
    }

    assert->start();
}

#endif // Header include guard

