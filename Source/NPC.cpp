#include "NPC.h"


HeavyNpcData::HeavyNpcData(Vector2 _size, Vector2 _pos)
{
	speed = 100 + rand() % 200;
	damage = 40;
	range = 200;
	health = 100;
	coolDown = 3;
	rect.set(_size, _pos);
}

void HeavyNpcData::move(Vector2 _target, float dt)
{
	Vector2 pos = rect.get_center();
	rect.set_center(pos.move_towards(_target, speed * dt, range - 20));
}

bool HeavyNpcData::is_collide(Rect _rect)
{
	return rect.collide_as_rect(_rect);
}

void HeavyNpcData::update(float dt, Vector2 _target)
{
	if (coolDown > 0)
		coolDown -= dt;

	move(_target, dt);
}

HeavyNpcSwarm::HeavyNpcSwarm()
{
	load_image(image, "Resources/Joker.png");
	rectSize.set(image.width, image.height);
	addTime = 3;
}

void HeavyNpcSwarm::add()
{
	active.add(HeavyNpcData(rectSize, Vector2(rand() % 2000, rand() % 2000)));
}

void HeavyNpcSwarm::create(unsigned int _number, Rect& _target)
{
	target = &_target;
	for (unsigned int i = 0; i < _number; i++)
		add();
}

void HeavyNpcSwarm::update(float dt)
{
	if (target == nullptr)
		return;

	if (addTime > 0)
		addTime -= dt;
	else
	{
		addTime = 3;
		add();
	}

	int _size = active.get_size();
	Vector2 pos = target->get_center();
	for (unsigned int i = 0; i < _size; i++)
		active[i].move(pos, dt);
}

void HeavyNpcSwarm::draw()
{
	int _size = active.get_size();
	for (unsigned int i = 0; i < _size; i++)
		Camera::draw(active[i].rect, image);
}