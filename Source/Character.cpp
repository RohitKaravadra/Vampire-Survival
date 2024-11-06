#include "Character.h"

void DamageArea::create(float _range, Vector2 _pos)
{
	image.width = image.height = _range;
	image.channels = 4;
	rect.set(Vector2(_range), _pos);
	create_rect_outline(image, Color::YELLOW, 2);
}

Character::Character(std::string _location, Vector2 _pos, TileMap& _level) :level(_level), Sprite(_location, _pos)
{
	health = 100;
	speed = 200;
	range = 400;
	damage = 50;
	alive = true;
	tag = "Player";
	dmgArea.create(range, rect.get_center());
	Collisions::add_collider(*this);
}

Character::~Character()
{

	Collisions::remove_collider(*this);
}

void Character::update(float dt)
{
	if (!alive)
		return;

	if (Inputs::key_pressed('E'))
		speed += 1000 * dt;
	if (Inputs::key_pressed('Q'))
		speed -= 1000 * dt;

	Vector2 delta = Inputs::get_axis() * dt * speed;
	move_and_collide(delta);
	dmgArea.rect.set_center(rect.get_center());
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
	if (!alive)
		return;

	dmgArea.draw();
	Sprite::draw();
}

bool Character::is_alive() const
{
	return alive;
}

void Character::hit(float _damage)
{
	if (!alive)
		return;

	health -= _damage;
	health = clamp(health, 0.f, 100.f);
	alive = health > 0;
	std::cout << "Health : " << health << std::endl;
}

void Character::on_collide(Collider& _other)
{
	if (_other.compare_tag("Heavy Enemy"))
		hit(5);
}