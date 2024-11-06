#pragma once
#include "Engine.h"
#include "Character.h"
#include "Utilities.h"

using namespace Engine;
using namespace Utilities;

class HeavyNpc :public Sprite
{
public:
	HeavyNpc() = default;
	void create();
};

class HeavyNpcSwarm
{
	const float speed = 200;
	const float damage = 50;
	const float range = 100;
	HeavyNpc npc;
	DArray<Vector2> active;
	Character& player;
public:
	HeavyNpcSwarm(Character& _player);
	void create(unsigned int _number);
	void move(float dt);
	void update(float dt);
	void draw();
	void isActive();
	void collide(Character& _player);
};

class NpcManager
{
	Sprite* heavy_npc;
	Dictionary<unsigned int, DArray<Vector2>> activeNPCs;
	Character& player;
public:
	NpcManager(Character& _player);
};