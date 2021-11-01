#include <iostream>


#include <time.h>

#include "types_fundamental_test.h"

using namespace std;

int main() {
    auto assert = new CorsacTest();
    assert->equal("types_fundamental_test", types_fundamental_test(assert), true);
    assert->start();
//    cout << "[TITLE] corsac::slice" << endl;
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

	    class Person
        {
        public:
            int X, Y;

	        Person(int x, int y) : X(x), Y(y)
            {}
            ~Person()
            {
                cout << "hi" << endl;
                X = 0;
                Y = 0;
            }

        };

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

//    {
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
//    }
//
//    {
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
//    }


    return 0;
}