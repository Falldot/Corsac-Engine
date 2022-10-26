#pragma once

struct Animation
{
	int index;
	int frames;
	int speed;

	Animation(){}
	Animation(int i, int f, int s)
	{
		this->index = i;
		this->frames = f;
		this->speed = s;
	}
};