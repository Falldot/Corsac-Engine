//
// Created by Falldot on 05.02.2022.
//
#ifndef CORSAC_CMAKETEMPLATES_H
#define CORSAC_CMAKETEMPLATES_H

#pragma once

#include <string>

namespace corsac
{
    inline std::string newCmakeFile(const std::string& project)
    {
        return
        {
            "cmake_minimum_required(VERSION 3.16 FATAL_ERROR)\n"
            "project(" + project +" VERSION 0.1.0)\n"
            "file(GLOB SOURCES\n"
                                  "    src/*.h\n"
                                  "    src/*.cpp\n"
                                  "    src/*/*.cpp\n"
                                  "    src/*/*.hpp\n"
                                  "    src/*/*.c\n"
                                  ")\n"
            "add_executable(${PROJECT_NAME} main.cpp)\n"
        };
    }

}

#endif //CORSAC_CMAKETEMPLATES_H
