//
// Created by Falldot on 26.11.2021.
//

//#define CORSAC_TEST_TIME_OFF
//#define CORSAC_TEST_RESULT_OFF
#define TEST_ENABLE

#include "Test.h"

void* __cdecl operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
    return new uint8_t[size];
}

void* __cdecl operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
    return new uint8_t[size];
}

#include "type_traits_test.h"
#include "type_properties_test.h"
#include "type_fundamental_test.h"
#include "type_transformation_test.h"
#include "type_pod_test.h"
#include "type_compound_test.h"
#include "vector_test.h"


#include "Corsac/unique_ptr.h"

int main()
{
    auto assert = new corsac::Block("STL");

    assert->add_block("type", [](corsac::Block *assert) {
        assert->add_block("type_fundamental_test", [](corsac::Block *assert) {
            type_fundamental_test(assert);
        });
        assert->add_block("type_compound_test", [](corsac::Block *assert) {
            type_compound_test(assert);
        });
        assert->add_block("type_traits_test", [](corsac::Block *assert) {
            type_traits_test(assert);
        });
        assert->add_block("type_transformation_test", [](corsac::Block *assert) {
            type_transformation_test(assert);
        });
        assert->add_block("type_properties_test", [](corsac::Block *assert) {
            type_properties_test(assert);
        });
        assert->add_block("type_properties_test", [](corsac::Block *assert) {
            type_pod_test(assert);
        });
        assert->add_block("vector_test", [](corsac::Block *assert) {
            vector_test(assert);
        });
    });
    assert->start();
    return 0;
}
