

#include "type_traits.h"

#include "internal/type_fundamental.h"
#include "internal/type_transformation.h"
#include "internal/type_compound.h"
#include "internal/type_pod.h"
#include "internal/type_properties.h"


#include "iterator.h"
#include "memory.h"
#include "algorithm.h"
#include "functional.h"
#include "random.h"
#include "vector.h"
#include "sparse_set.h"

#define CORSAC_TEST_ONLY_RESULT

#define TEST_ENABLE
#include "../../test/Test.h"

#include "type_traits_test.h"
#include "type_properties_test.h"
#include "type_fundamental_test.h"
#include "type_transformation_test.h"
#include "type_pod_test.h"
#include "type_compound_test.h"
#include "vector_test.h"

int main() {
    auto assert = new Corsac::Block("STL");

    assert->add_block("type", [](Corsac::Block *assert) {
        assert->add_block("type_fundamental_test", [](Corsac::Block *assert) {
            type_fundamental_test(assert);
        });
        assert->add_block("type_compound_test", [](Corsac::Block *assert) {
            type_compound_test(assert);
        });
        assert->add_block("type_traits_test", [](Corsac::Block *assert) {
            type_traits_test(assert);
        });
        assert->add_block("type_transformation_test", [](Corsac::Block *assert) {
            type_transformation_test(assert);
        });
        assert->add_block("type_properties_test", [](Corsac::Block *assert) {
            type_properties_test(assert);
        });
        assert->add_block("type_properties_test", [](Corsac::Block *assert) {
            type_pod_test(assert);
        });
        assert->add_block("vector_test", [](Corsac::Block *assert) {
            vector_test(assert);
        });
    });
    assert->start();
// cout << "[TITLE] corsac::slice" << endl;
//	{
//		cout << "[TEST] init when not set size" << endl;
//		{
//            auto arr = corsac::slice<int>();
//			if (arr.capacity() != 2) 					FAILED()
//			if (!arr.empty())                           FAILED()
//			if (arr.size() != CORSAC_SLICE_INIT_SIZE) 	FAILED()
//		}
//
//		cout << "[TEST] init when set size" << endl;
//		{
//			size_t size = 64;
//            auto arr = corsac::slice<int>(size);
//
//			if (arr.capacity() != size) 				FAILED()
//            if (!arr.empty())                           FAILED()
//			if (arr.size() != CORSAC_SLICE_INIT_SIZE) 	FAILED()
//		}
//
//		cout << "[TITLE] add elements" << endl;
//		{
//			auto arr = corsac::slice<int>();
//
//			cout << "[TEST] push_back" << endl;
//			{
//				int el = 1;
//				arr.append(el);
//                if (arr.empty())            FAILED()
//				if (arr.capacity() != 2)    FAILED()
//				if (arr.size() != 1) 		FAILED()
//				if (arr[0] != el)           FAILED()
//			}
//
//			cout << "[TEST] push_back and auto grow" << endl;
//			{
//				int el1 = 2; int el2 = 3;
//				arr.append(el1);
//				arr.append(el2);
//                if (arr.empty())                FAILED()
//				if (arr.capacity() != 4)        FAILED()
//				if (arr.size() != 3) 			FAILED()
//                if (arr[1] != el1)              FAILED()
//                if (arr[2] != el2)              FAILED()
//			}
//
//            cout << "[TEST] push_back and auto grow" << endl;
//            {
//
//            }
//		}
//	}

//        auto obj1 = new Person(10, 30);
//        auto arr1 = corsac::slice<Person*>();
//        arr1.append(obj1);
//        const Person* last1 = arr1.last();
//        using value_type = Person*;
//        const Person** last2 = &last1;
//        last2->~value_type();
//        arr1.pop();
//        cout << last1->X << endl;
//
//        auto obj = new Person(10, 30);
//        auto arr = vector<Person*>();
//        arr.push_back(obj);
//        const Person* last = arr.back();
//        arr.pop_back();
//        cout << last->X << endl;
//
//{
//        clock_t start_time = clock();
//        double elapsed_time = (double)(clock() - start_time);
//        int play = 1;
//        int count = 0;
//
//        auto arr = vector<Position>();
//
//        Position last = arr.back();
//        arr.pop_back();
//
//        while (play)
//        {
//
//            for (size_t i = 0; i < 50000; i++)
//                arr.push_back({1, 1});
//
//            count += 1;
//            elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
//            if (elapsed_time > 1.0) {
//                play = 0;
//            }
//        }
//        printf("Vector Benchmark\t %d \t %f \n", count, elapsed_time);
//}
//
//{
//        clock_t start_time = clock();
//        double elapsed_time = (double)(clock() - start_time);
//        int play = 1;
//        int count = 0;
//
//        auto arr = corsac::slice<Position>();
//
//
//        while (play)
//        {
//
//            for (size_t i = 0; i < 50000; i++) {
//                arr.append({i,i});
//            }
//
//            count += 1;
//            elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
//            if (elapsed_time > 1.0) {
//                play = 0;
//            }
//        }
//        printf("Slicee Benchmark\t %d \t %f \n", count, elapsed_time);
//}


    return 0;
}