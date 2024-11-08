#pragma once
#include "Engine.h"
#include "Collisions.h"

using Engine::Vector2;

class Projectile :public Collider
{
	Vector2 vel;
	Vector2 pos;
	float range;
public:
	void create(Vector2 _size, float _range, std::string _tag)
	{
		vel = pos = Vector2(0);
		range = _range;
		tag = _tag;
		rect.set(_size, pos);
		isActive = false;
	}

	void set(Vector2 _pos, Vector2 _vel)
	{
		std::cout << "Set" << std::endl;
		pos = _pos;
		vel = _vel;
		rect.set_center(_pos);
		isActive = true;
		Collisions::add_collider(*this);
	}

	void reset()
	{
		Collisions::remove_collider(*this);
		isActive = false;
	}

	void on_collide(std::string _tag) override
	{
		reset();
	}

	void update(float dt)
	{
		if (!isActive)
			return;

		rect.move(vel * dt);
		if (pos.distance(rect.get_center()) > range)
			reset();
	}

	~Projectile()
	{
		reset();
	}
};

template<unsigned int poolSize>
class ProjectilePool
{
	Projectile pool[poolSize];
	Image image;
	float speed;
	Engine::Color color;

public:
	ProjectilePool() = default;

	void create(Vector2 _size, float _speed, float _range, std::string _tag, Engine::Color _color = Engine::Color::AQUA)
	{
		color = _color;
		speed = _speed;
		create_image(_size);
		for (unsigned int i = 0; i < poolSize; i++)
			pool[i].create(_size, _range, _tag);
	}

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

	void update(float dt)
	{
		for (unsigned int i = 0; i < poolSize; i++)
			if (pool[i].isActive)
				pool[i].update(dt);
	}

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

	void draw()
	{
		for (unsigned int i = 0; i < poolSize; i++)
			if (pool[i].isActive)
				Engine::Camera::draw(pool[i].rect, image);
	}
};