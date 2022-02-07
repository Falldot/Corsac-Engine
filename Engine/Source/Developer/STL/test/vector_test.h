//
// Created by Falldot on 17.11.2021.
//

#ifndef CORSAC_VECTOR_TEST_H
#define CORSAC_VECTOR_TEST_H

#include <iostream>

#include "Corsac/vector.h"

bool vector_test(corsac::Block* assert)
{
    assert->add_block("init", [](corsac::Block* assert)
    {

    });
    return true;
}

#endif //CORSAC_VECTOR_TEST_H
