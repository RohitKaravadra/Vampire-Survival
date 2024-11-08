#include "NPC.h"
#include "Resources.h"
#include "Constants.h"

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

#pragma endregion

#pragma region Heavy

HeavyNpc::HeavyNpc(Vector2 _pos)
{
	speed = 100 + rand() % 50;
	damage = 40;
	range = 20;
	health = 100;
	coolDown = 3;

	load_image(image, "Resources/Joker.png");

	rect.set(Vector2(image.width, image.height), _pos);
	tag = EnemyHeavyTag;

	Collisions::add_collider(*this);
}

HeavyNpc::~HeavyNpc()
{
	Collisions::remove_collider(*this);
}

void HeavyNpc::update(float dt, Vector2 _target)
{
	if (!isActive)
		return;

	if (hitAmount != 0)
	{
		health -= hitAmount * dt;
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
}

void HeavyNpc::draw()
{
	if (!isActive)
		return;
	Sprite::draw();
	healthBar.draw();
}

#pragma endregion


NpcManager::NpcManager()
{
	heavyNo = 5;
}

void NpcManager::create(Sprite& _player)
{
	player = &_player;
	heavy.create(heavyNo++);
}

void NpcManager::update(float dt)
{
	if (!heavy.is_active())
		heavy.create(heavyNo++);
	heavy.update(player->rect.get_center(), dt);
}

void NpcManager::draw()
{
	heavy.draw();
}

void NpcManager::destroy()
{
	heavy.destroy();
}