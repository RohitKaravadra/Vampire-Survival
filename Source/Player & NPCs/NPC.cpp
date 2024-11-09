#include "NPC.h"
#include "Resources.h"
#include "Constants.h"

#pragma region Base

NpcBase::NpcBase()
{
	speed = 100;
	range = 100;
	rangeDelta = 0;
	maxHealth = health = 100;
	coolDown = 0;

	healthBar.create(Vector2(50, 10), Vector2(0, 0), Color::GREEN, Color::RED);
	Collisions::add_collider(*this);
}

NpcBase::~NpcBase()
{
	if (health <= 0) // update game stats when npc is killed
	{
		if (tag == LiteNpcTag)
			GameStats::liteKilled++;
		else if (tag == HeavyNpcTag)
			GameStats::heavyKilled++;
		else if (tag == StaticNpcTag)
			GameStats::staticKilled++;
		else if (tag == ShooterNpcTag)
			GameStats::shooterKilled++;

		GameStats::time = App::sceneTimer;
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

	if (distToTarget > 1500) // apply damage to npc if too far from player
		health -= 10;

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

HeavyNpc::HeavyNpc(Vector2 _pos)
{
	range = 20;
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

void NpcManager::create(Sprite& _player)
{
	player = &_player;
	liteNpcs.create(heavyNo, Vector2(-2000), Vector2(2000));
	shooterNpcs.create(heavyNo, Vector2(-2000), Vector2(2000));
	heavyNpcs.create(2, Vector2(-2000), Vector2(2000));
	staticNpcs.create(5, Vector2(-1000), Vector2(1000));
}

void NpcManager::update_wave()
{
	if (!liteNpcs.is_active() && !shooterNpcs.is_active())
	{
		liteNpcs.create(heavyNo++, Vector2(-2000), Vector2(2000));
		shooterNpcs.create(heavyNo++, Vector2(-2000), Vector2(2000));
		heavyNpcs.create(2, Vector2(-2000), Vector2(2000));
		staticNpcs.create(5 + wave, Vector2(-1000), Vector2(1000));
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