#pragma once
#include "Engine.h"
#include "Collisions.h"

using Engine::Vector2;

// class for projectiles
class Projectile :public Collider
{
	Vector2 vel; // velodity
	Vector2 pos; // initial position for range check
	float range; // range
public:
	// create a projectile
	void create(Vector2 _size, float _range, std::string _tag)
	{
		vel = pos = Vector2(0);
		range = _range;
		tag = _tag;
		rect.set(_size, pos);
		isActive = false;
	}

	// set projectile
	void set(Vector2 _pos, Vector2 _vel)
	{
		pos = _pos;
		vel = _vel;
		rect.set_center(_pos);
		isActive = true;
		Collisions::add_collider(*this, 1);
	}

	// reset projectile
	void reset()
	{
		Collisions::remove_collider(*this, 1);
		isActive = false;
	}

	void on_collide(std::string _tag) override { reset(); }

	void update(float dt)
	{
		if (!isActive)
			return;

		rect.move(vel * dt);
		if (pos.distance(rect.get_center()) > range)
			reset();
	}

	~Projectile() { reset(); }
};

template<unsigned int poolSize>
// template for projectile pool
class ProjectilePool
{
	Projectile pool[poolSize]; // projectile pool
	Image image; // image of projectile to draw
	float speed; // speed of projectile
	Engine::Color color; // color of projectile

public:
	ProjectilePool() = default;

	// create pool with give data and assign tab for collision
	void create(Vector2 _size, float _speed, float _range, std::string _tag, Engine::Color _color = Engine::Color::AQUA)
	{
		color = _color;
		speed = _speed;
		create_image(_size);
		for (unsigned int i = 0; i < poolSize; i++)
			pool[i].create(_size, _range, _tag);
	}

	// create a circle for projectile visual (can be swaped with an image)
	void create_image(Vector2 _size)
	{
		image.width = _size.x;
		image.height = _size.y;
		image.channels = 4;
		if (image.data != NULL)
			image.free();
		image.data = new unsigned char[image.width * image.height * image.channels] {};

		Vector2 _center = _size / 2;
		float _rad = _size.x / 2;
		for (int i = 0; i < _size.x * _size.y; i++)
			if (Vector2(i % image.width, i / image.width).distance(_center) <= _rad)
				memcpy(&image.data[i * image.channels], color.value, image.channels);
	}

	// update all projectiles in pool
	void update(float dt)
	{
		for (unsigned int i = 0; i < poolSize; i++)
			if (pool[i].isActive)
				pool[i].update(dt);
	}

	// add projectile (basically finds inactive projectile and activate it)
	void add(Vector2 _pos, Vector2 _dir)
	{
		for (int i = 0; i < poolSize; i++)
		{
			if (!pool[i].isActive)
			{
				pool[i].set(_pos, _dir * speed);
				break;
			}
		}
	}

	// draw all projectiles
	void draw()
	{
		for (unsigned int i = 0; i < poolSize; i++)
			if (pool[i].isActive)
				Engine::Camera::draw(pool[i].rect, image);
	}
};