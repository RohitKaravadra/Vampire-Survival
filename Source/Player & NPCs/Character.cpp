#include "Character.h"
#include "Constants.h"

void DamageArea::create(float _range, Vector2 _pos)
{
	image.width = image.height = _range;
	image.channels = 4;
	rect.set(Vector2(_range), _pos);
	create_rect_outline(image, Color::YELLOW, 2);
}

Character::Character(std::string _location, Vector2 _pos, Level& _level) :level(_level), Sprite(_location, _pos)
{
	health = 100;
	speed = 200;
	range = 400;
	damage = 50;
	tag = "Player";
	hitDamage = 0;

	healthBar.create(Vector2(50, 10), Vector2(0, 0), Color::GREEN, Color::RED);
	dmgArea.create(range, rect.get_center());

	Collisions::add_collider(*this);
}

Character::~Character()
{
	Collisions::remove_collider(*this);
}

void Character::update(float dt)
{
	if (!isActive)
		return;

	if (Inputs::key_pressed('E'))
		speed += 1000 * dt;
	if (Inputs::key_pressed('Q'))
		speed -= 1000 * dt;

	if (hitDamage > 0)
	{
		health -= hitDamage * dt;
		hitDamage = 0;
		healthBar.set_value(health / 100.f);
	}

	if (health <= 0)
		isActive = false;
	else
	{
		Vector2 delta = Inputs::get_axis() * dt * speed;
		move_and_collide(delta);
		dmgArea.rect.set_center(rect.get_center());

		if (Inputs::key_pressed(VK_SPACE))
			Collisions::rect_cast(dmgArea.rect, [](Collider* col) { col->on_collide(PlayerHeavyTag); });

		healthBar.set_pos(rect.get_center() + Vector2::up * 50);
	}
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
	if (!isActive)
		return;

	dmgArea.draw();
	Sprite::draw();
	healthBar.draw();
}

bool Character::is_alive() const
{
	return isActive;
}

void Character::on_collide(Collider& _other)
{
	if (_other.compare_tag(EnemyHeavyTag))
		hitDamage += EnemyHeavyDamage;
}