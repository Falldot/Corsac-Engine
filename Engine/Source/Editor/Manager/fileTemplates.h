//
// Created by Falldot on 05.02.2022.
//
#ifndef CORSAC_FILETEMPLATES_H
#define CORSAC_FILETEMPLATES_H

#pragma once

#include <string>

namespace corsac
{
    inline std::string newCmakeFile(const std::string& project)
    {
        return
        {
            "cmake_minimum_required(VERSION 3.16 FATAL_ERROR)\n"
            "\n"
            "project("+project+" VERSION 0.1.0)\n"
            "\n"
            "include_directories(\n"
            "        src\n"
            "        $ENV{ProgramFiles}/Corsac/include\n"
            ")\n"
            "file(GLOB SOURCES\n"
            "        main.cpp\n"
            "        src/*.h src/*.cpp src/*.c src/*.hpp\n"
            "        src/*/*.h src/*/*.cpp src/*/*.c src/*/*.hpp\n"
            ")\n"
            "\n"
            "set(SDL2_INCLUDE_DIR $ENV{ProgramFiles}/Corsac/include/SDL)\n"
            "set(SDL2_LIB_DIR $ENV{ProgramFiles}/Corsac/lib/x64/SDL)\n"
            "\n"
            "link_directories(${SDL2_LIB_DIR})\n"
            "\n"
            "configure_file(${SDL2_LIB_DIR}/SDL2.dll ${CMAKE_BINARY_DIR}/Debug/SDL2.dll COPYONLY)\n"
            "\n"
            "add_executable(${PROJECT_NAME} ${SOURCES})\n"
            "target_link_libraries(${PROJECT_NAME} SDL2main SDL2)"
        };
    }

    inline std::string newMainCppFile()
    {
        return
        {
            "#include \"SDL/SDL.h\"\n"
            "\n"
            "int main(int argc, char *argv[])\n"
            "{\n"
            "    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)\n"
            "        return 1;\n"
            "\n"
            "    SDL_Window* Window = SDL_CreateWindow(\"Hello Corsac!\", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);\n"
            "    if (Window == nullptr)\n"
            "        return 1;\n"
            "\n"
            "    SDL_Renderer* Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);\n"
            "    if (Renderer == nullptr)\n"
            "        return 1;\n"
            "\n"
            "    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);\n"
            "    SDL_RenderClear(Renderer);\n"
            "\n"
            "    SDL_Event e;\n"
            "    bool quit = false;\n"
            "    while(!quit)\n"
            "    {\n"
            "        while(SDL_PollEvent(&e) != 0 )\n"
            "        {\n"
            "            if( e.type == SDL_QUIT )\n"
            "            {\n"
            "                quit = true;\n"
            "            }\n"
            "        }\n"
            "        SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);\n"
            "        SDL_RenderClear(Renderer);\n"
            "        SDL_RenderPresent(Renderer);\n"
            "    }\n"
            "\n"
            "    SDL_DestroyRenderer(Renderer);\n"
            "    SDL_DestroyWindow(Window);\n"
            "    SDL_Quit();\n"
            "\n"
            "    return 0;\n"
            "}"
        };
    }
}

#endif //CORSAC_FILETEMPLATES_H
