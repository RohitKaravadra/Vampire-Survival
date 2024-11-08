#pragma once
#include <iostream>
#include "Level.h"
#include "Projectiles.h"

using namespace Engine;

class DamageArea : public Sprite
{
	Sprite* player = nullptr;
public:
	DamageArea() = default;
	void create(float _range, Vector2 _pos);
	void set_player(Sprite*);
	void on_collide(Collider& _other);
};

class Character : public Sprite
{
	float health;
	float speed;
	float range;
	float hitDamage; // cumulative damage to be addet while updating frame

	float fireRate;
	Vector2 target;

	UI::FillBar healthBar;
	ProjectilePool<20U> pPool;

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
	// overriding on collide method
	void set_nearest(Vector2 _pos);
};
