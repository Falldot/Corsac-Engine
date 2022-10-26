#pragma once

#include "SDL.h"

#include "../../CRSC/CRSC.h"

#include "Transform.h"

class FixCamera : public Component
{
private:
    Transform* transform;
public:
    SDL_Rect camera;

    FixCamera(int width, int height)
    {
        camera = { 0, 0, width, height };
    };
    ~FixCamera() {};

    void Init() override
    {
        transform = &entity->getComponent<Transform>();
    }

	void Update() override
	{
        camera.x = static_cast<int>(transform->position.x - (CRSC_App::Width / 2 - (transform->width * transform->scale) / 2));
        camera.y = static_cast<int>(transform->position.y - (CRSC_App::Height / 2 - (transform->height * transform->scale) / 2));

        if (camera.x < 0)
            camera.x = 0;
        if (camera.y < 0)
            camera.y = 0;
        if (camera.x > camera.w)
            camera.x = camera.w;
        if (camera.y > camera.h)
            camera.h = camera.h;
	}
};