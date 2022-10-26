#pragma once

#include "../../CRSC/CRSC.h"

#include "../Types/Vector2D.h"

class Transform : public Component
{
public:
	Vector2D position;
	Vector2D velocity;

	int height = 36;
	int width = 36;
	int scale = 1;

	int speed = 6;

	Transform()
	{
		this->position.Zero();
	}

	Transform(int sc)
	{
		position.Zero();
		this->scale = sc;
	}

	Transform(float x, float y)
	{
		this->position.x = x;
		this->position.y = y;
	}

	Transform(float x, float y, int h, int w, int sc)
	{
		this->position.x = x;
		this->position.y = y;
		this->width = w;
		this->height = h;
		this->scale = sc;
	}

	void Init() override
	{
		this->velocity.Zero();
	}

	void Update() override
	{
		position.x += static_cast<int>(velocity.x * speed);
		position.y += static_cast<int>(velocity.y * speed);
	}
};

