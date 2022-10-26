#pragma once
#include <string>

#include <fstream>
#include <iostream>

class CRSC_Map
{
public:

	CRSC_Map(std::string tID, int ms, int ts);
	~CRSC_Map();

	void AddTile(int srcX, int srcY, int x, int y);
	void Load(std::string path, int sizeX, int sizeY);

private:
	std::string texID;
	int scale;
	int tileSize;
	int scaledSize;
};

