#include "NPC.h"
#include "Resources.h"
#include "DataManager.h"

using namespace std;

#pragma region Base

NpcBase::NpcBase()
{
	speed = 100;
	range = 100;
	rangeDelta = 0;
	maxHealth = health = 100;
	coolDown = 0;
	otherDamage = 0;

	healthBar.create(Vector2(50, 10), Vector2(0, 0), Color::GREEN, Color::RED);
	Collisions::add_collider(*this);
}

NpcBase::~NpcBase()
{
	if (health <= 0 && otherDamage < maxHealth / 2) // update game stats when npc is killed
	{
		if (tag == LiteNpcTag)
			GameStats::liteKilled++;
		else if (tag == HeavyNpcTag)
			GameStats::heavyKilled++;
		else if (tag == StaticNpcTag)
			GameStats::staticKilled++;
		else if (tag == ShooterNpcTag)
			GameStats::shooterKilled++;
	}
	Collisions::remove_collider(*this);
}

bool NpcBase::is_alive()
{
	return isActive;
}

void NpcBase::move(Vector2 _target, float dt)
{
	Vector2 pos = rect.get_center();
	rect.set_center(pos.move_towards(_target, speed * dt, range - rangeDelta));
}

void NpcBase::update(float dt)
{
	if (!isActive)
	{
		distToTarget = 10000.f;
		return;
	}

	if (hitAmount != 0) // add acumulated damage to npc
	{
		health -= hitAmount;
		hitAmount = 0;
		healthBar.set_value(health / maxHealth);
	}

	if (distToTarget > 2000) // apply damage to npc if too far from player
	{
		health -= 10;
		otherDamage -= 10;
	}

	if (health <= 0)
		isActive = false;
}

void NpcBase::on_collide(std::string _tag)
{
	if (_tag == PlayerHeavyTag)
		hitAmount += PlayerHeavyDamage;
}

void NpcBase::on_collide(Collider& _other)
{
	if (_other.compare_tag(PlayerProjectileTag))
	{
		hitAmount += PlayerLiteDamage;
		_other.on_collide(tag);
	}
}

void NpcBase::draw()
{
	if (!isActive)
		return;
	Sprite::draw();
	healthBar.draw();
}

float NpcBase::get_health() { return health; }

void NpcBase::set_health(float _val) { health = _val; healthBar.set_value(_val / maxHealth); }

void ShooterNpcBase::update(float dt, Vector2 _target)
{
	NpcBase::update(dt);
	if (cdTimer > 0)
		cdTimer -= dt;

	if (isActive)
	{
		if (cdTimer <= 0 && distToTarget < range)
		{
			pool.add(rect.get_center(), rect.get_center().direction(_target));
			cdTimer = coolDown;
		}
	}

	pool.update(dt);
}

void ShooterNpcBase::draw()
{
	NpcBase::draw();
	pool.draw();
}

#pragma endregion

#pragma region Lite & heavy

LiteNpc::LiteNpc(Vector2 _pos)
{
	speed = 150 + rand() % 50;
	range = 30;
	maxHealth = health = 20;

	load_image(image, "Resources/Rober.png");

	rect.set(Vector2(image.width, image.height), _pos);
	tag = LiteNpcTag;
	healthBar.set_pos(rect.get_center() + Vector2::up * 50);
}

void LiteNpc::update(float dt, Vector2 _target)
{
	NpcBase::update(dt);

	if (isActive)
	{
		move(_target, dt);
		healthBar.set_pos(rect.get_center() + Vector2::up * 50);
		distToTarget = _target.distance(rect.get_center());
	}
}

HeavyNpc::HeavyNpc(Vector2 _pos) :LiteNpc(_pos)
{
	speed = 100 + rand() % 20;
	maxHealth = health = 100;

	image.free();
	load_image(image, "Resources/Joker.png");
	rect.set(Vector2(image.width, image.height), _pos);
	tag = HeavyNpcTag;
}
#pragma endregion

#pragma region StaticNpc

StaticNpc::StaticNpc(Vector2 _pos)
{
	range = 400;
	maxHealth = health = 30;
	coolDown = 2;

	load_image(image, "Resources/ScareCrow.png");
	pool.create(Vector2(20), 200, range, NpcProjectileTag, Color::RED);

	rect.set(Vector2(image.width, image.height), _pos);
	tag = StaticNpcTag;
	healthBar.set_pos(rect.get_center() + Vector2::up * 50);
}

void StaticNpc::update(float dt, Vector2 _target)
{
	ShooterNpcBase::update(dt, _target);
	if (isActive)
		distToTarget = _target.distance(rect.get_center());

}

#pragma endregion

#pragma region ShooterNpc

ShooterNpc::ShooterNpc(Vector2 _pos)
{
	speed = 130 + rand() % 50;
	range = 300;
	rangeDelta = 200;
	maxHealth = health = 70;
	coolDown = 1;

	load_image(image, "Resources/MrFreez.png");
	pool.create(Vector2(10), 300, range, NpcProjectileTag);

	rect.set(Vector2(image.width, image.height), _pos);
	tag = ShooterNpcTag;
	healthBar.set_pos(rect.get_center() + Vector2::up * 50);
}

void ShooterNpc::update(float dt, Vector2 _target)
{
	ShooterNpcBase::update(dt, _target);
	if (isActive)
	{
		move(_target, dt);
		healthBar.set_pos(rect.get_center() + Vector2::up * 50);
		distToTarget = _target.distance(rect.get_center());
	}
}
#pragma endregion

NpcManager::NpcManager()
{
	heavyNo = 2;
}

void NpcManager::create(Sprite& _player, bool _load)
{
	player = &_player;

	if (_load && DataManager::is_loaded())
	{
		wave = GameStats::wave;

		DArray<Pair<Vector2, float>> data = DataManager::get_enemy_data(LiteNpcTag);
		int size = data.get_size();
		for (unsigned int i = 0; i < size; i++)
			liteNpcs.add(data[i].key, data[i].value);

		data = DataManager::get_enemy_data(HeavyNpcTag);
		size = data.get_size();
		for (unsigned int i = 0; i < size; i++)
			heavyNpcs.add(data[i].key, data[i].value);

		data = DataManager::get_enemy_data(StaticNpcTag);
		size = data.get_size();
		for (unsigned int i = 0; i < size; i++)
			staticNpcs.add(data[i].key, data[i].value);

		data = DataManager::get_enemy_data(ShooterNpcTag);
		size = data.get_size();
		for (unsigned int i = 0; i < size; i++)
			shooterNpcs.add(data[i].key, data[i].value);
	}
	else
		update_wave();
}

void NpcManager::update_wave()
{
	if (!liteNpcs.is_active() && !shooterNpcs.is_active())
	{
		liteNpcs.create(heavyNo++, LiteNpcTag);
		shooterNpcs.create(heavyNo++, ShooterNpcTag);
		heavyNpcs.create(2 + wave, HeavyNpcTag);
		staticNpcs.create(2 + wave, StaticNpcTag);
		wave++;
		GameStats::wave = wave;
	}
}

void NpcManager::update(float dt)
{
	update_wave();

	Vector2 _target = player->rect.get_center();
	liteNpcs.update(_target, dt);
	staticNpcs.update(_target, dt);
	shooterNpcs.update(_target, dt);
	heavyNpcs.update(_target, dt);
}

Vector2 NpcManager::get_nearest()
{
	Vector2 _target = player->rect.get_center(), nearest(1000);
	float dist = _target.distance(nearest);

	Vector2 pos = liteNpcs.get_nearest();
	if (pos != Vector2::zero)
	{
		float nDist = _target.distance(pos);

		if (dist > nDist)
		{
			dist = nDist;
			nearest = pos;
		}
	}

	pos = staticNpcs.get_nearest();
	if (pos != Vector2::zero)
	{
		float nDist = _target.distance(pos);

		if (dist > nDist)
		{
			dist = nDist;
			nearest = pos;
		}
	}

	pos = shooterNpcs.get_nearest();
	if (pos != Vector2::zero)
	{
		float nDist = _target.distance(pos);

		if (dist > nDist)
		{
			dist = nDist;
			nearest = pos;
		}
	}

	pos = heavyNpcs.get_nearest();
	if (pos != Vector2::zero)
	{
		float nDist = _target.distance(pos);

		if (dist > nDist)
		{
			dist = nDist;
			nearest = pos;
		}
	}

	return nearest;
}

void NpcManager::draw()
{
	liteNpcs.draw();
	staticNpcs.draw();
	shooterNpcs.draw();
	heavyNpcs.draw();
}

void NpcManager::destroy()
{
	liteNpcs.destroy();
	staticNpcs.destroy();
	shooterNpcs.destroy();
	heavyNpcs.destroy();
}

void NpcManager::save_data()
{
	DataManager::set_npc_data(LiteNpcTag, liteNpcs.get_data());
	DataManager::set_npc_data(HeavyNpcTag, heavyNpcs.get_data());
	DataManager::set_npc_data(StaticNpcTag, staticNpcs.get_data());
	DataManager::set_npc_data(ShooterNpcTag, shooterNpcs.get_data());
}
