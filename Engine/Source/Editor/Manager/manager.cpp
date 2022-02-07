//
// Created by Falldot on 05.02.2022.
//
#include <iostream>
#include <filesystem>
#include <string>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: <ProjectName>" << '\n';
        exit(1);
    }

    std::string CurrentPath(std::filesystem::current_path().generic_string());
    std::string projectName = argv[1];

    std::cout << CurrentPath << std::endl;
    std::cout << projectName << std::endl;

    return 0;
}

