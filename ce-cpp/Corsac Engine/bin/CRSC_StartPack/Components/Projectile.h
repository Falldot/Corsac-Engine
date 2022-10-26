#pragma once

#include "Transform.h"

#include "../../CRSC/CRSC.h"

#include "../Types/Vector2D.h"

class Projectile : public Component
{
public:
	Projectile(int rng, int sp, Vector2D vel) : range(rng), speed(sp), velocity(vel)
	{}
	~Projectile()
	{}

	void Init() override
	{
		transform = &entity->getComponent<Transform>();
		transform->velocity = velocity;
	}

	void Update() override
	{
		distance += speed;

		if (distance > range)
		{
			std::cout << "!!!!!aaa" << std::endl;
			entity->Destroy();
		}
		else if (transform->position.x > CRSC_Scene::camera.x + CRSC_Scene::camera.w ||
				transform->position.x < CRSC_Scene::camera.x ||
				transform->position.y > CRSC_Scene::camera.y + CRSC_Scene::camera.h ||
				transform->position.y < CRSC_Scene::camera.y)
		{
			std::cout << "!!!!!ddd" << std::endl;
			entity->Destroy();
		}
	}

private:

	Transform* transform;

	int range = 0;
	int speed = 0;
	int distance = 0;

	Vector2D velocity;
};