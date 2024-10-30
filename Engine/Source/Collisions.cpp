#include "Engine.h"

using namespace Engine;

bool Collision::circle_collide(const Rect& a, const Rect& b)
{
	return Vector2::distance(a.center, b.center) < (a.size.x + b.size.x) / 2;
}

bool Collision::rect_collide(const Rect& a, const Rect& b)
{
	Vector2 atl = a.get_topleft();
	Vector2 btl = b.get_topleft();
	Vector2 abr = a.get_botmright();
	Vector2 bbr = b.get_botmright();

	return atl.x < bbr.x &&
		btl.x < abr.x &&
		atl.y < bbr.y &&
		btl.y < abr.y;
}