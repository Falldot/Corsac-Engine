#pragma once

#include <vector>

#include "CRSC_App.h"
#include "CRSC_Timer.h"
#include "CRSC_Scene.h"

class CRSC_Engine
{
public:
	CRSC_Engine();
	~CRSC_Engine();

	void Init(const char* GameName, const char* OrgName);
	void Setup();

	void AddScene(CRSC_Scene* scenes);

	void Exit();
private:
	std::vector<std::unique_ptr<CRSC_Scene>> scenes;

	Uint32 time_step_ms = 1000 / 60;
	Uint32 next_game_step = SDL_GetTicks();
	bool FPS_Limit = true;

	float interpolation = 0;

	CRSC_Timer fpsTimer;
	int countedFrames = 0;

	CRSC_App* app;
	CRSC_Logs* log;

	void Destroy();


	bool run = false;
};

