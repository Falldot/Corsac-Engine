#pragma once

#include <iostream>

#include "../../CRSC/CRSC.h"

#include "../Types/Vector2D.h"

#include "Sprite.h"
#include "Transform.h"

class KeyboardController : public Component
{
public:
	Transform* transform;
	Sprite* sprite;

	bool W = false, A = false, S = false, D = false;
	Vector2D UnitPosition;

	void Init() override
	{
		this->transform = &entity->getComponent<Transform>();
		this->sprite = &entity->getComponent<Sprite>();
	}

	void Update() override
	{
		if (CRSC_Scene::event.type == SDL_KEYDOWN)
		{
			switch (CRSC_Scene::event.key.keysym.sym)
			{
			case SDLK_w:
				W = true;
				break;
			case SDLK_a:
				A = true;
				break;
			case SDLK_d:
				D = true;
				break;
			case SDLK_s:
				S = true;
				break;
			default:
				break;
			}
		}
		if (CRSC_Scene::event.type == SDL_KEYUP)
		{
			switch (CRSC_Scene::event.key.keysym.sym)
			{
			case SDLK_w:
				W = false;
				break;
			case SDLK_a:
				A = false;
				break;
			case SDLK_d:
				D = false;
				break;
			case SDLK_s:
				S = false;
				break;
			default:
				break;
			}
		}

		if (!W && !S && !A && !D) {
			this->sprite->Play("Idle");

			this->transform->velocity.x = 0;
			this->transform->velocity.y = 0;
		}
		else if (D && S) {
			this->sprite->Play("DownRight");
			this->sprite->flip = SDL_FLIP_NONE;

			this->transform->velocity.x = 1;
			this->transform->velocity.y = 1;
		}
		else if (A && S) {
			this->sprite->Play("DownRight");
			this->sprite->flip = SDL_FLIP_HORIZONTAL;

			this->transform->velocity.x = -1;
			this->transform->velocity.y = 1;
		}
		else if (W && A) {
			this->sprite->Play("Right");
			this->sprite->flip = SDL_FLIP_HORIZONTAL;

			this->transform->velocity.x = -1;
			this->transform->velocity.y = -1;
		}
		else if (W && D) {
			this->sprite->Play("Right");
			this->sprite->flip = SDL_FLIP_NONE;

			this->transform->velocity.x = 1;
			this->transform->velocity.y = -1;
		}
		else if (W) {
			this->sprite->Play("Up");

			this->transform->velocity.y = -1;
			this->transform->velocity.x = 0;
		}
		else if (S) {
			this->sprite->Play("Down");

			this->transform->velocity.y = 1;
			this->transform->velocity.x = 0;
		}
		else if (A) {
			this->sprite->Play("Right");
			this->sprite->flip = SDL_FLIP_HORIZONTAL;

			this->transform->velocity.x = -1;
			this->transform->velocity.y = 0;
		}
		else if (D) {
			this->sprite->Play("Right");
			this->sprite->flip = SDL_FLIP_NONE;

			this->transform->velocity.x = 1;
			this->transform->velocity.y = 0;
		}
	}
private:

};

