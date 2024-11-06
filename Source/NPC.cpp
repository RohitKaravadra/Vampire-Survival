#include "NPC.h"
#include "Resources.h"

HeavyNpc::HeavyNpc(Vector2 _size, Vector2 _pos)
{
	speed = 100 + rand() % 50;
	damage = 40;
	range = 20;
	health = 100;
	coolDown = 3;
	alive = true;
	load_image(image, "Resources/Joker.png");
	rect.set(_size, _pos);
	tag = "Heavy Enemy";
	Collisions::add_collider(*this);
}

HeavyNpc::~HeavyNpc()
{
	Collisions::remove_collider(*this);
}

bool HeavyNpc::is_alive()
{
	return alive;
}

void HeavyNpc::move(Vector2 _target, float dt)
{
	Vector2 pos = rect.get_center();
	rect.set_center(pos.move_towards(_target, speed * dt, range - 20));
}

void HeavyNpc::on_collide(std::string _tag)
{
	if (_tag.compare("Player"))
		std::cout << tag << " Collided " << _tag << std::endl;
}

void HeavyNpc::update(float dt, Vector2 _target)
{
	if (coolDown > 0)
		coolDown -= dt;

	move(_target, dt);
}

HeavyNpcSwarm::HeavyNpcSwarm()
{
	load_image(image, "Resources/Joker.png");
	rect.set(image.width, image.height);
	addTime = 3;
}

void HeavyNpcSwarm::add()
{
	Vector2 _pos = Vector2(1000 - rand() % 2000, 1000 - rand() % 2000);
	active.add(new HeavyNpc(rect.size, _pos));
}

void HeavyNpcSwarm::create(unsigned int _number, Sprite& _target)
{
	target = &_target;
	for (unsigned int i = 0; i < _number; i++)
		add();
}

void HeavyNpcSwarm::destroy()
{
	target = nullptr;
	active.clear_with_elements();
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
	Vector2 _target = target->rect.get_center();
	active.foreach([&](HeavyNpc* _npc) {
		if (_npc->is_alive())
			_npc->update(dt, _target);
		else
			active.remove_and_delete(_npc);
		});
}

void HeavyNpcSwarm::draw()
{
	active.foreach([](HeavyNpc* _npc) {_npc->draw(); });
}