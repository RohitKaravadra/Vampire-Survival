#pragma once
#include <iostream>
#include "Level.h"
#include "Projectiles.h"

using namespace Engine;

// Damage Area for area attack
class DamageArea : public Sprite
{
public:
	DamageArea() = default;
	void create(float _range, Vector2 _pos);
};

// Character class to handle player movement and attacks
class Character : public Sprite
{
	float health;
	float speed;
	float range;
	const float coolDown = 5; // for area attack
	float hitDamage; // cumulative damage to be addet while updating frame

	float fireRate;// for continous attack
	float cdTimer; // timer for area attack
	Vector2 target; // target for continuous attack

	UI::FillBar healthBar;
	UI::FillBar attackCharge; // for area attack
	ProjectilePool<20U> pPool;

	DamageArea dmgArea; // for area attack
	TileMap<32U, 24U>& level; // reference to the level for collision check

	// method to attack enemy
	void attack();
	// move character without colliding
	void move(Vector2 delta);
	// move character and collide
	void move_and_collide(Vector2 delta);
	// apply damage to player
	void hit(float _val);
	// overriding on collide method
	void on_collide(Collider& _other) override;
public:
	Character() = default;
	Character(std::string _location, Vector2 _pos, float _health, TileMap<32U, 24U>& _level);
	// destructor
	~Character() override;
	// update character
	void update(float dt)override;
	// overriding draw methos
	void draw()override;
	// overriding ui draw of sprite
	void draw_ui() override;
	// method to check if player is alive
	bool is_alive() const;
	// overriding on collide method
	void set_nearest(Vector2 _pos);
	// returns health of the player
	float get_health();
};
