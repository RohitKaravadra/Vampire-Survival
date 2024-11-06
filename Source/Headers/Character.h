#pragma once
#include <iostream>
#include "Engine.h"

using namespace Engine;

class DamageArea : public Sprite
{
public:
	DamageArea() = default;
	void create(float _range, Vector2 _pos);
};

class Character : public Sprite
{
	float health = 100;
	float speed = 200;
	float range = 400;
	float damage = 50;
	DamageArea dmgArea;
	TileMap& level;
public:
	Character() = default;
	Character(std::string _location, Vector2 _pos, TileMap& _level);
	// destructor
	~Character() = default;
	// update character
	void update(float dt);
	// move character without colliding
	void move(Vector2 delta);
	// move character and collide
	void move_and_collide(Vector2 delta);
	// method to attack enemy
	void attack();
	// overriding draw methos
	void draw();
};
