//
// Created by Falldot on 08.01.2022.
//

#ifndef ECS_SPARSE_SET_TEST_H
#define ECS_SPARSE_SET_TEST_H

#include "Corsac/sparse_set.h"

bool sparse_set_test(corsac::Block* assert) {

    assert->add_block("dynamic", [](corsac::Block *assert) {

        const uint32_t element = 33;
        corsac::fixed_vector<uint32_t, 5> some_element = {33, 44, 1, 0, 176};
        corsac::fixed_vector<uint32_t, 3> some_element_rem = {33, 1, 176};

        corsac::sparse_set<uint32_t> set;

        assert->add_block("init", [set](corsac::Block *assert) {
            assert->is_true("empty()", set.empty());
            assert->equal("size()", set.size(), 0);
            assert->equal("capacity()", set.capacity(), 0);
        });
        assert->add_block("add", [&set, element](corsac::Block *assert) {
            set.add(element);
            assert->is_false("empty()", set.empty());
            assert->equal("size()", set.size(), 1);
            assert->equal("capacity()", set.capacity(), 1);
            assert->is_true("has(element)", set.has(element));
        });
        assert->add_block("remove", [&set, element](corsac::Block *assert) {
            set.remove(element);
            assert->is_true("empty()", set.empty());
            assert->equal("size()", set.size(), 0);
            assert->equal("capacity()", set.capacity(), 1);
            assert->is_false("has(element)", set.has(element));
        });
        assert->add_block("add some elements", [&set, some_element](corsac::Block *assert) {
            for (int i = 0; i < some_element.size(); ++i)
            {
                set.add(some_element[i]);
            }
            assert->is_false("empty()", set.empty());
            assert->equal("size()", set.size(), 5);
            assert->equal("capacity()", set.capacity(), 8);
            for (int i = 0; i < some_element.size(); ++i)
            {
                assert->is_true("has(element)", set.has(some_element[i]));
            }
        });
        assert->add_block("remove some elements", [&set, some_element_rem](corsac::Block *assert) {
            for (int i = 0; i < some_element_rem.size(); ++i)
            {
                set.remove(some_element_rem[i]);
            }
            assert->is_false("empty()", set.empty());
            assert->equal("size()", set.size(), 2);
            assert->equal("capacity()", set.capacity(), 8);
            for (int i = 0; i < some_element_rem.size(); ++i)
            {
                assert->is_false("has(element)", set.has(some_element_rem[i]));
            }
        });
    });


    assert->add_block("init fixed", [](corsac::Block *assert) {
        corsac::sparse_set<uint32_t, 10> set;
        assert->is_true("empty()", set.empty());
        assert->is_false("full()", set.full());
        assert->is_true("can_overflow()", set.can_overflow());
        assert->is_false("has_overflowed()", set.has_overflowed());
        assert->equal("size()", set.size(), 0);
        assert->equal("capacity()", set.capacity(), 10);
        assert->equal("max_size()", set.max_size(), 10);
    });
    assert->add_block("init static", [](corsac::Block *assert) {
        corsac::sparse_set<uint32_t, 10, false> set;
        assert->is_true("empty()", set.empty());
        assert->is_false("full()", set.full());
        assert->is_false("can_overflow()", set.can_overflow());
        assert->is_false("has_overflowed()", set.has_overflowed());
        assert->equal("size()", set.size(), 0);
        assert->equal("capacity()", set.capacity(), 10);
        assert->equal("max_size()", set.max_size(), 10);
    });
    return true;
}

#endif //ECS_SPARSE_SET_TEST_H
