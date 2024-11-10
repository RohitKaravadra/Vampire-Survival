#include "Character.h"
#include "Constants.h"

void DamageArea::create(float _range, Vector2 _pos)
{
	image.width = image.height = _range;
	image.channels = 4;
	rect.set(Vector2(_range), _pos);
	create_circle_outline(image, Color::YELLOW, 2);
}

void DamageArea::on_collide(Collider& _other)
{
	if (_other.compare_tag(HeavyNpcTag))
		player->on_collide(_other);
}

Character::Character(std::string _location, Vector2 _pos, float _health, TileMap<32U, 24U>& _level) :level(_level), Sprite(_location, _pos)
{
	health = _health;
	speed = 200;
	range = 400;
	hitDamage = 0;
	tag = PlayerTag;

	fireRate = 0;
	cdTimer = coolDown;
	target.set(0, 0);

	Vector2 camSize = Camera::camRect.size;
	healthBar.create(Vector2(200, 20), Vector2(camSize.x / 2 - 100, camSize.y - 35), Color::GREEN, Color::RED, health / 100);
	attackCharge.create(Vector2(200, 10), Vector2(camSize.x / 2 - 100, camSize.y - 50), Color::AQUA, Color::WHITE);

	pPool.create(Vector2(10, 10), 300, 300, PlayerProjectileTag, Color::BLACK);
	dmgArea.create(range, rect.get_center());

	Collisions::add_collider(*this);
}

Character::~Character()
{
	Collisions::remove_collider(*this);
}

void Character::hit(float _val)
{
	if (!isActive)
		return;

	health -= _val;
	healthBar.set_value(health / 100.f);
	if (health <= 0)
		isActive = false;
}

void Character::update(float dt)
{
	if (hitDamage > 0)
	{
		hit(hitDamage * dt);
		hitDamage = 0;
	}

	if (isActive)
	{
		if (cdTimer > 0)
		{
			cdTimer -= dt;
			attackCharge.set_value((coolDown - cdTimer) / coolDown);
			dmgArea.isActive = false;
		}
		else
			dmgArea.isActive = true;

		if (fireRate > 0)
			fireRate -= dt;

		if (Inputs::key_pressed('E'))
			speed += 1000 * dt;
		if (Inputs::key_pressed('Q'))
			speed -= 1000 * dt;

		Vector2 delta = Inputs::get_axis() * dt * speed;
		move_and_collide(delta);

		if (dmgArea.isActive)
			dmgArea.rect.set_center(rect.get_center());

		if (fireRate <= 0 && target.magnitude() > 0)
		{
			pPool.add(rect.get_center(), rect.get_center().direction(target));
			fireRate = 0.2f;
		}
		if (cdTimer <= 0 && Inputs::key_pressed(VK_SPACE))
		{
			Collisions::circle_cast(dmgArea.rect, [](Collider* col) { col->on_collide(PlayerHeavyTag); });
			cdTimer = coolDown;
		}
	}

	pPool.update(dt);
}

void Character::move(Vector2 delta)
{
	rect.move(delta);
}

void Character::move_and_collide(Vector2 delta)
{
	// check and resolve x axis collision
	rect.move_h(delta.x);
	if (level.is_colliding(rect, 1))
		rect.move_h(-delta.x);

	// check and resolve y axis collision
	rect.move_v(delta.y);
	if (level.is_colliding(rect, 1))
		rect.move_v(-delta.y);
}

void Character::attack()
{

}

void Character::draw()
{
	if (isActive)
	{
		if (dmgArea.isActive)
			dmgArea.draw();
		Sprite::draw();
	}
	pPool.draw();
}

void Character::draw_ui()
{
	attackCharge.draw_ui();
	healthBar.draw_ui();
}

bool Character::is_alive() const
{
	return isActive;
}

void Character::on_collide(Collider& _other)
{
	if (_other.compare_tag(LiteNpcTag))
		hitDamage += LiteNpcDamage;
	else if (_other.compare_tag(StaticNpcTag))
		hitDamage += StaticNpcDamage;
	else if (_other.compare_tag(HeavyNpcTag))
		hitDamage += HeavyNpcDamage;
	else if (_other.compare_tag(ShooterNpcTag))
		hitDamage += ShooterNpcDamage;
	else if (_other.compare_tag(NpcProjectileTag))
	{
		hit(NpcProjectileDamage);
		_other.on_collide(PlayerTag);
	}
}

void Character::set_nearest(Vector2 _pos)
{
	target = _pos.distance(rect.get_center()) <= range ? _pos : Vector2::zero;
}

float Character::get_health() { return health; }