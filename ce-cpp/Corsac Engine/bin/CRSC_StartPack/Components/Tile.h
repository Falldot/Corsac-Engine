#pragma once

#include <SDL.h>

#include "../../CRSC/CRSC.h"

#include "../Types/Vector2D.h"

class Tile : public Component
{
public:

	SDL_Texture* texture;
	SDL_Rect src, dest;
	Vector2D position;

	Tile() = default;

	~Tile()
	{
		SDL_DestroyTexture(texture);
	}

	Tile(int srcX, int srcY, int xpos, int ypos, int tsize, int tscale ,std::string id)
	{
		this->texture = CRSC_GetTexture(id);

		src.x = srcX;
		src.y = srcY;
		src.w = src.h = tsize;
		position.x = static_cast<float>(xpos);
		position.y = static_cast<float>(ypos);
		dest.w = dest.h = tsize * tscale;
	}

	void Update() override
	{
		dest.x = static_cast<int>(position.x - CRSC_Scene::camera.x);
		dest.y = static_cast<int>(position.y - CRSC_Scene::camera.y);
	}

	void Render() override
	{
		Draw(texture, src, dest, SDL_FLIP_NONE);
	}

private:

};
