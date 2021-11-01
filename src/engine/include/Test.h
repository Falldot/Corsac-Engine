//
// Created by surve on 31.10.2021.
//

#ifndef CORSAC_TEST_H
#define CORSAC_TEST_H

#include <functional>
#include <vector>
#include <windows.h>
#include <string>
#include <iostream>

class CorsacTest
{
private:

    std::vector<std::function<bool()>> tests;
    std::vector<std::string> names;

public:
    CorsacTest(){};

    void add_test(std::string name, std::function<bool()> test)
    {
        this->tests.push_back(test);
        this->names.push_back(name);
    }

    void start_tests()
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        // for(auto i = 0; i < 256; i++){
        //     SetConsoleTextAttribute(hConsole, i);
        //     std::cout << i << std::endl;
        // }
        std::cout << this->tests.size() << std::endl;
        for(auto i = 0; i < this->tests.size(); i++)
        {
            if(this->tests[i]())
                SetConsoleTextAttribute(hConsole, 160);
            else
                SetConsoleTextAttribute(hConsole, 70);

            std::cout << "   " << this->names[i]<< std::endl;
        }

        SetConsoleTextAttribute(hConsole, 6);
    }

};

#endif //CORSAC_TEST_H
