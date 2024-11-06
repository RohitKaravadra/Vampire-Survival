#include "Projectiles.h"

ProjectileData::ProjectileData(Vector2 _pos, Vector2 _size, Vector2 _vel, float _range, float _damage)
{
	rect.set(_size, _pos);
	velocity = _vel;
	range = _range;
}