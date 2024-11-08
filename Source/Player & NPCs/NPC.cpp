#include "NPC.h"
#include "Resources.h"
#include "Constants.h"


unsigned int NpcStats::heavyKilled = 0;
unsigned int NpcStats::staticKilled = 0;

void NpcStats::print()
{
	std::cout << "---------------\nEnemy Killed\n";
	std::cout << "\nHeavy Killed : " << heavyKilled;
	std::cout << "\nStatic Killed : " << staticKilled;
	std::cout << "\n----------------\n";
}

void NpcStats::reset()
{
	heavyKilled = 0;
	staticKilled = 0;
}

#pragma region Base

NpcBase::NpcBase()
{
	speed = 100;
	damage = 0;
	range = 100;
	health = 100;
	coolDown = 0;

	healthBar.create(Vector2(50, 10), Vector2(0, 0), Color::GREEN, Color::RED);
}

NpcBase::~NpcBase()
{
	if (health <= 0)
	{
		if (tag == HeavyNpcTag)
			NpcStats::heavyKilled++;
		else if (tag == StaticNpcTag)
			NpcStats::staticKilled++;
	}
}

bool NpcBase::is_alive()
{
	return isActive;
}

void NpcBase::move(Vector2 _target, float dt)
{
	Vector2 pos = rect.get_center();
	rect.set_center(pos.move_towards(_target, speed * dt, range - 20));
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

#pragma endregion

#pragma region Heavy

HeavyNpc::HeavyNpc(Vector2 _pos)
{
	speed = 100 + rand() % 50;
	damage = 40;
	range = 40;
	health = 100;
	coolDown = 3;

	load_image(image, "Resources/Joker.png");

	rect.set(Vector2(image.width, image.height), _pos);
	tag = HeavyNpcTag;

	Collisions::add_collider(*this);
}

HeavyNpc::~HeavyNpc()
{
	Collisions::remove_collider(*this);
}

void HeavyNpc::update(float dt, Vector2 _target)
{
	if (!isActive)
	{
		distToTarget = 10000.f;
		return;
	}

	if (hitAmount != 0)
	{
		health -= hitAmount;
		hitAmount = 0;
		healthBar.set_value(health / 100.f);
	}

	if (health <= 0)
		isActive = false;
	else
	{
		move(_target, dt);
		healthBar.set_pos(rect.get_center() + Vector2::up * 50);
	}

	distToTarget = _target.distance(rect.get_center());
}

void HeavyNpc::draw()
{
	if (!isActive)
		return;
	Sprite::draw();
	healthBar.draw();
}

#pragma endregion

#pragma region StaticNpc

StaticNpc::StaticNpc(Vector2 _pos)
{
	speed = 200;
	damage = 40;
	range = 400;
	health = 100;
	coolDown = 2;

	load_image(image, "Resources/ScareCrow.png");
	pool.create(Vector2(20), speed, range, NpcProjectileTag, Color::RED);

	rect.set(Vector2(image.width, image.height), _pos);
	tag = StaticNpcTag;
	healthBar.set_pos(rect.get_center() + Vector2::up * 50);

	Collisions::add_collider(*this);
}


StaticNpc::~StaticNpc()
{
	Collisions::remove_collider(*this);
}

void StaticNpc::update(float dt, Vector2 _target)
{
	pool.update(dt);
	if (!isActive)
	{
		distToTarget = 10000.f;
		return;
	}

	if (cdTimer > 0)
		cdTimer -= dt;

	if (hitAmount != 0)
	{
		health -= hitAmount;
		hitAmount = 0;
		healthBar.set_value(health / 100.f);
	}

	distToTarget = _target.distance(rect.get_center());

	if (health <= 0)
		isActive = false;
	else
	{
		if (cdTimer <= 0 && distToTarget < range)
		{
			pool.add(rect.get_center(), rect.get_center().direction(_target));
			cdTimer = coolDown;
		}
	}
}

void StaticNpc::draw()
{
	if (!isActive)
		return;
	Sprite::draw();
	healthBar.draw();
	pool.draw();
}

#pragma endregion



NpcManager::NpcManager()
{
	heavyNo = 5;
}

void NpcManager::create(Sprite& _player)
{
	NpcStats::reset();
	player = &_player;
	heavy.create(heavyNo += wave, Vector2(-2000), Vector2(2000));
	staticNpcs.create(5 + wave, Vector2(-1000), Vector2(1000));
}

void NpcManager::update(float dt)
{
	if (!heavy.is_active())
	{
		heavy.create(heavyNo++, Vector2(-2000), Vector2(2000));
		staticNpcs.create(5 + wave, Vector2(-1000), Vector2(1000));
		wave++;
	}

	Vector2 _target = player->rect.get_center();
	heavy.update(_target, dt);
	staticNpcs.update(_target, dt);
}

Vector2 NpcManager::get_nearest()
{
	Vector2 _target = player->rect.get_center(), nearest(1000);
	float dist = _target.distance(nearest);

	Vector2 pos = heavy.get_nearest();
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

	return nearest;
}

void NpcManager::draw()
{
	heavy.draw();
	staticNpcs.draw();
}

void NpcManager::destroy()
{
	heavy.destroy();
	staticNpcs.destroy();
}