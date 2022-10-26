#pragma once

#include <cstddef>

#include "SDL.h"

class CRSC_Asset;

class CRSC_Scene
{
public:
	virtual void Init() {}
	virtual void Update(float fps) {}
	virtual void Event() {}
	virtual void Render() {}
	virtual void Destroy() {}

	static SDL_Rect camera;

	static SDL_Event event;

	enum groupLabels : std::size_t
	{
		Map,
		Players,
		Colliders,
		Projectiles,
		ClickUps,
		ClickDowns,
		Hovers
	};

	bool run = false;
private:
};


