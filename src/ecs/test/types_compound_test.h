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
    auto extent = assert->add_block("extent");
    {
        extent->equal("param is int[10]", corsac::extent<int[10]>::value, 10);
        extent->equal("param is int[]", corsac::extent<int[]>::value, 0);
    }
    auto is_array = assert->add_block("is_array");
    {
        is_array->is_true("param is int[10]", corsac::is_array<int[10]>::value);
        is_array->is_true("param is int[]", corsac::is_array<int[]>::value);
        is_array->is_false("param is int", corsac::is_array<int>::value);
    }
    return true;
}

#endif //CORSAC_ENGINE_TYPES_COMPOUND_TEST_H
