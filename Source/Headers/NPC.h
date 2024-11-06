#pragma once
#include "Engine.h"
#include "Character.h"
#include "Utilities.h"

using namespace Engine;
using namespace Utilities;

class HeavyNpc :public Sprite
{
	float speed;
	float damage;
	float range;
	float health;
	float coolDown;
	bool alive;

public:
	HeavyNpc() = default;
	HeavyNpc(Vector2 _size, Vector2 _pos);
	~HeavyNpc();
	void move(Vector2 _target, float dt);
	void on_collide(std::string _tag) override;
	void update(float dt, Vector2 _target);
	bool is_alive();
};

class HeavyNpcSwarm :public Sprite
{
	DArray<HeavyNpc*> active;
	Sprite* target;

	float addTime;
public:
	HeavyNpcSwarm();
	void create(unsigned int _number, Sprite& _rect);
	void destroy();
	void add();
	void update(float dt);
	void draw();
	void debug() {};
};