#pragma once
#include <iostream>
#include "Level.h"

using namespace Engine;

class DamageArea : public Sprite
{
public:
	DamageArea() = default;
	void create(float _range, Vector2 _pos);
};

class Character : public Sprite
{
	float health;
	float speed;
	float range;
	float damage;
	float hitDamage;
	UI::FillBar healthBar;

	DamageArea dmgArea;
	Level& level;
public:
	Character() = default;
	Character(std::string _location, Vector2 _pos, Level& _level);
	// destructor
	~Character() override;
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
	// method to check if player is alive
	bool is_alive() const;
	// overriding on collide method
	void on_collide(Collider& _other) override;
};
