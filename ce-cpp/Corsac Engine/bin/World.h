#pragma once

#include <iostream>

#include "CRSC/CRSC.h"

#include "CRSC_StartPack/CRSC_StartPack.h"

class World : public CRSC_Scene
{
public:
	void Init() override;
	void Event() override;
	void Update(float fps) override;
	void Render() override;
	void Destroy() override;
private:
	Vector2D playerPos;

	CRSC_Map* Location;
};

