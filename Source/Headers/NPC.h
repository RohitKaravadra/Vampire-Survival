#pragma once
#include "Engine.h"
#include "Utilities.h"

using namespace Engine;
using namespace Utilities;

// template class to create swarms of enemies
template<typename T>
class NpcSwarm
{
	DArray<T*> active;

public:

	void create(unsigned int _number)
	{
		for (unsigned int i = 0; i < _number; i++)
			add();
	}

	void destroy() { active.clear_with_elements(); }

	void add()
	{
		Vector2 _pos = Vector2(1000 - rand() % 2000, 1000 - rand() % 2000);
		active.add(new T(_pos));
	}

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

class NpcManager
{
	NpcSwarm<HeavyNpc> heavy;
	unsigned int heavyNo;
	Sprite* player;
public:
	NpcManager();
	void create(Sprite&);
	void destroy();
	void update(float);
	void draw();
	Vector2 get_nearest();
};