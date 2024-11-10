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
	const float coolDown = 5; // for area attack
	float hitDamage; // cumulative damage to be addet while updating frame

	float fireRate;// for continous attack
	float cdTimer; // timer for area attack
	Vector2 target;

	UI::FillBar healthBar;
	UI::FillBar attackCharge;
	ProjectilePool<20U> pPool;

	DamageArea dmgArea;
	TileMap<32U, 24U>& level;
public:
	Character() = default;
	Character(std::string _location, Vector2 _pos, float _health, TileMap<32U, 24U>& _level);
	// destructor
	~Character() override;
	// update character
	void update(float dt)override;
	// apply damage to player
	void hit(float _val);
	// move character without colliding
	void move(Vector2 delta);
	// move character and collide
	void move_and_collide(Vector2 delta);
	// method to attack enemy
	void attack();
	// overriding draw methos
	void draw()override;
	// overriding ui draw of sprite
	void draw_ui() override;
	// method to check if player is alive
	bool is_alive() const;
	// overriding on collide method
	void on_collide(Collider& _other) override;
	// overriding on collide method
	void set_nearest(Vector2 _pos);
	// returns health of the player
	float get_health();
};
