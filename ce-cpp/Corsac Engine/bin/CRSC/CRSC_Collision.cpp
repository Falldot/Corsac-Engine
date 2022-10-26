#include "CRSC_Collision.h"

bool CRSC_AABB(const SDL_Rect& recA, const SDL_Rect& recB)
{
	if (
		recA.x + recA.w >= recB.x &&
		recB.x + recB.w >= recA.x &&
		recA.y + recA.h >= recB.y &&
		recB.y + recB.h >= recA.y 
		)
	{
		return true;
	}
	return false;
}

bool CRSC_MouseAABB(const SDL_Rect& obj)
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	SDL_Rect mouse;
	mouse.h = mouse.w = 1;

	mouse.x = x + (obj.x + (obj.w / 2) - (CRSC_App::Width / 2));
	mouse.y = y + (obj.y + (obj.h / 2) - (CRSC_App::Height / 2));

	if (CRSC_AABB(mouse, obj))
	{
		return true;
	}
	else {
		return false;
	}
}