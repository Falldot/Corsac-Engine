//
// Created by Falldot on 17.11.2021.
//

#ifndef CORSAC_VECTOR_TEST_H
#define CORSAC_VECTOR_TEST_H

#include <iostream>

bool vector_test(Corsac::Block* assert)
{
    assert->add_block("init", [](Corsac::Block* assert)
    {
        corsac::vector<int> arr = corsac::vector<int>();
        assert->is_true("is_empty_size", arr.empty());
        assert->equal("capacity == 0", arr.capacity(), 0);
        assert->equal("size == 0", arr.size(), 0);
        assert->equal("allocator is default", arr.get_allocator().get_name(), CORSAC_DEFAULT_NAME_PREFIX);
    });
    return true;
}

#endif //CORSAC_VECTOR_TEST_H
