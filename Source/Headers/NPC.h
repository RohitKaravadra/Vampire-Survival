#pragma once
#include "Projectiles.h"
#include "Utilities.h"
#include "GameStats.h"
#include "Constants.h"

using namespace Engine;
using namespace Utilities;

// template class to create swarms of enemies
template<typename T>
class NpcSwarm
{
	DArray<T*> active;

public:

	void create(unsigned int _number, std::string _tag)
	{
		if (_tag == StaticNpcTag)
		{
			Vector2 _min = Camera::camRect.get_topleft(), _max = Camera::camRect.get_botmright();
			for (unsigned int i = 0; i < _number; i++)
				add(Vector2::get_random(_min, _max));
		}
		else
		{
			for (unsigned int i = 0; i < _number; i++)
				add(get_random_exclude(Camera::camRect, Vector2(100, 100)));
		}
	}

	void destroy() { active.clear_with_elements(); }

	void add(Vector2 _pos) { active.add(new T(_pos)); }

	void add(Vector2 _pos, float _health) { active.add(new T(_pos)); active[active.get_size() - 1]->set_health(_health); }

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

	DArray<Pair<Vector2, float>> get_data()
	{
		DArray<Pair<Vector2, float>> data;
		int size = active.get_size();
		if (size > 0)
		{
			Pair<Vector2, float> pair;
			for (int i = 0; i < size; i++)
			{
				pair.key = active[i]->rect.get_center();
				pair.value = active[i]->get_health();
				data.add(pair);
			}
		}
		return data;
	}
};

// base class for all Npcs
class NpcBase : public Sprite
{
	float otherDamage; // keeps in check of automatic damage
protected:
	float speed;
	int range;
	int rangeDelta;

	float maxHealth;
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
	void update(float dt) override;
	virtual void draw() override;
	float get_health();
	void set_health(float);
};

class ShooterNpcBase :public NpcBase
{
protected:
	float cdTimer;
	ProjectilePool<10> pool;
public:
	void update(float dt, Vector2 _target);
	void draw() override;
};


// Heavy Enemy type (only moves towards enemies, damages on collision)
class LiteNpc : public NpcBase
{
public:
	LiteNpc() = default;
	LiteNpc(Vector2);
	~LiteNpc() = default;
	void update(float, Vector2);
};

class HeavyNpc : public LiteNpc
{
public:
	HeavyNpc() = default;
	HeavyNpc(Vector2);
};

class StaticNpc : public ShooterNpcBase
{
public:
	StaticNpc() = default;
	StaticNpc(Vector2);
	~StaticNpc() = default;
	void update(float, Vector2);
};

class ShooterNpc : public ShooterNpcBase
{
public:
	ShooterNpc() = default;
	ShooterNpc(Vector2);
	void update(float, Vector2);
};

class NpcManager
{
	NpcSwarm<LiteNpc> liteNpcs;
	NpcSwarm<HeavyNpc> heavyNpcs;
	NpcSwarm<StaticNpc> staticNpcs;
	NpcSwarm<ShooterNpc> shooterNpcs;
	unsigned int npcIncrease;
	Sprite* player;
	unsigned int wave = 0;
public:
	NpcManager();
	void create(Sprite&, bool = false);
	void destroy();
	void update_wave();
	void update(float);
	void draw();
	Vector2 get_nearest();
	void save_data();
};