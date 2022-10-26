#pragma once
#include <fstream>
#include <iostream>

class CRSC_Logs
{
public:
	CRSC_Logs();
	~CRSC_Logs();

	static void set(const char* text, const char* error);

private:
	static std::ofstream file;
	static const char* path;
};