#pragma once
#include "Engine.h"
#include "Character.h"
#include "Utilities.h"

using namespace Engine;
using namespace Utilities;

struct HeavyNpcData
{
	float speed;
	float damage;
	float range;
	float health;
	float coolDown;
	Rect rect;

	HeavyNpcData() = default;
	HeavyNpcData(Vector2 _size, Vector2 _pos);
	void move(Vector2 _target, float dt);
	bool is_collide(Rect _rect);
	void update(float dt, Vector2 _target);
};

class HeavyNpcSwarm
{
	Image image;
	Vector2 rectSize;
	DArray<HeavyNpcData> active;
	Rect* target;

	float addTime;
public:
	HeavyNpcSwarm();
	void create(unsigned int _number, Rect& _rect);
	void add();
	void update(float dt);
	void draw();
	void debug() {};
};

class NpcManager
{
	Sprite* heavy_npc;
	Dictionary<unsigned int, DArray<Vector2>> activeNPCs;
	Character& player;
public:
	NpcManager(Character& _player);
};