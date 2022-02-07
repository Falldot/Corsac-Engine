//
// Created by Falldot on 05.02.2022.
//
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

#include "fileTemplates.h"

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        std::cout << "Please specify the project directory:" << std::endl << std::endl;
        std::cout << "corsac <project-directory>" << std::endl;
        exit(1);
    }

    std::string currentPath(std::filesystem::current_path().generic_string());
    std::string command(argv[1]);

    std::cout << command << std::endl;
    if (command == "build")
    {
        if (!std::filesystem::exists(currentPath + "/build"))
        {
            std::filesystem::create_directories(currentPath + "/build");
        }
        std::string com = "cd build && cmake -S .. && cmake --build .";
        system(com.c_str());
        return 0;
    }

    const std::string& projectName(command);
    std::string projectPath(currentPath + "/" + projectName);

    if (std::filesystem::exists(projectPath))
    {
        std::cout << "Error: this project is already exist!" << std::endl;
        exit(1);
    }

    std::filesystem::create_directories(projectPath);
    std::cout << "Creating a new Corsac project in " + projectPath << std::endl << std::endl;

    std::filesystem::path cmakeFile(projectPath);
    cmakeFile /= "CMakeLists.txt";
    std::ofstream ofsCmakeFile(cmakeFile);
    ofsCmakeFile << corsac::newCmakeFile(projectName);

    std::filesystem::path mainFile(projectPath);
    mainFile /= "main.cpp";
    std::ofstream ofsMainFile(mainFile);
    ofsMainFile << corsac::newMainCppFile();

    std::filesystem::create_directories(projectPath + "/src");

    std::cout << "Success! Created " + projectName + " at " + projectPath << std::endl;

    return 0;
}

