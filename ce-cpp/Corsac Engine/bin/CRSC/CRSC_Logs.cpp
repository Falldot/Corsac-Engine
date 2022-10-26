#include "CRSC_Logs.h"

const char* CRSC_Logs::path = "logs.txt";
std::ofstream CRSC_Logs::file;

CRSC_Logs::CRSC_Logs()
{
    // Создаем текстовый файл в который будем писать логи ошибок
    file.open(path, std::ofstream::app);
    file.close();
}

CRSC_Logs::~CRSC_Logs(){}

void CRSC_Logs::set(const char* text, const char* error)
{
    file.open(path, std::ofstream::app);
    file << text << std::endl;
    std::cout << text << std::endl;
    file.close();
}