#pragma once
#include "Engine.h"

using namespace Engine;

struct ProjectileData
{
	float range;
	float damage;
	Vector2 velocity;
	Rect rect;

	ProjectileData(Vector2 _pos, Vector2 _size, Vector2 _vel, float _range, float _damage);
	void update(float dt);
};

class Projectiles
{
	Image image;
};