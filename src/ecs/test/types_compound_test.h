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
    return true;
}

#endif //CORSAC_ENGINE_TYPES_COMPOUND_TEST_H
