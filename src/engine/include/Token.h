//
// Created by surve on 31.10.2021.
//

#ifndef CORSAC_TOKEN_H
#define CORSAC_TOKEN_H

#include <regex>
#include <string>

class CorsacToken{
private:
    std::string regex_token;
    std::string name;

public:
    CorsacToken(std::string name, std::string reg){
        this->name = name;
        this->regex_token = reg;
    }

    bool check(std::string text){
        std::regex c(this->regex_token);

        return std::regex_match(text, c);
    }
};

#endif //CORSAC_TOKEN_H


















