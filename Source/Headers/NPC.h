#pragma once
#include "Projectiles.h"
#include "Utilities.h"

using namespace Engine;
using namespace Utilities;

// template class to create swarms of enemies
template<typename T>
class NpcSwarm
{
	DArray<T*> active;

public:

	void create(unsigned int _number, Vector2 _minPos, Vector2 _maxPos)
	{
		for (unsigned int i = 0; i < _number; i++)
			add(Vector2::get_random(_minPos, _maxPos));
	}

	void destroy() { active.clear_with_elements(); }

	void add(Vector2 _pos) { active.add(new T(_pos)); }

	void update(Vector2 _target, float dt)
	{
		if (active.get_size() > 0)
		{
			active.foreach([&](T* _npc) {
				if (_npc->is_alive())
					_npc->update(dt, _target);
				else
					active.remove_and_delete(_npc);
				});
		}
	}

	void draw() { active.foreach([](T* _npc) {_npc->draw(); }); }

	bool is_active() { return active.get_size() > 0; }

	Vector2 get_nearest()
	{
		Vector2 pos(0, 0);
		float dist = 10000.f;
		active.foreach([&](T* _npc) {
			if (_npc->distToTarget < dist)
			{
				pos = _npc->rect.get_center();
				dist = _npc->distToTarget;
			}
			});
		return pos;
	}
};

static struct NpcStats
{
	static unsigned int heavyKilled;
	static unsigned int staticKilled;

	static void print();
	static void reset();
};

// base class for all Npcs
class NpcBase : public Sprite
{
protected:
	float speed;
	float damage;
	float range;
	float health;
	float coolDown;
	float hitAmount; // commulative ammout of damage to be added

	UI::FillBar healthBar;
public:
	float distToTarget;

	NpcBase();
	virtual ~NpcBase();
	void move(Vector2 _target, float dt);
	bool is_alive();
	void on_collide(std::string) override;
	void on_collide(Collider& _other) override;
};

// Heavy Enemy type (only moves towards enemies, damages on collision)
class HeavyNpc : public NpcBase
{
public:
	HeavyNpc() = default;
	HeavyNpc(Vector2);
	~HeavyNpc();
	void update(float, Vector2);
	void draw() override;
};

class StaticNpc : public NpcBase
{
	float cdTimer;
	ProjectilePool<10> pool;
public:
	StaticNpc() = default;
	StaticNpc(Vector2);
	~StaticNpc();
	void update(float, Vector2);
	void draw() override;
};

class NpcManager
{
	NpcSwarm<HeavyNpc> heavy;
	NpcSwarm<StaticNpc> staticNpcs;
	unsigned int heavyNo;
	Sprite* player;
	unsigned int wave = 0;
public:
	NpcManager();
	void create(Sprite&);
	void destroy();
	void update(float);
	void draw();
	Vector2 get_nearest();
};