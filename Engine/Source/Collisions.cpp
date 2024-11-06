#include "Collisions.h"

Collider::~Collider()
{

}

std::string Collider::get_tag()
{
	return tag;
}

bool Collider::compare_tag(Collider& _other) const
{
	return tag == _other.tag;
}

bool Collider::compare_tag(std::string _tag) const
{
	return tag == _tag;
}


DArray<Collider*> Collisions::colliders;
unsigned int Collisions::size = 0;

void Collisions::add_collider(Collider& _collider)
{
	colliders.add(&_collider);
	size = colliders.get_size();
	std::cout << "Collider " << _collider.get_tag() << " added" << std::endl;
}

void Collisions::remove_collider(Collider& _collider)
{
	colliders.remove(&_collider);
	size = colliders.get_size();
	std::cout << "Collider " << _collider.get_tag() << " removed" << std::endl;
}

void Collisions::update()
{
	for (unsigned int i = 0; i < size; i++)
	{
		for (unsigned int j = i + 1; j < size - 1; j++)
		{
			if (colliders[i]->rect.collide_as_rect(colliders[j]->rect))
			{
				colliders[i]->on_collide(*colliders[j]);
				colliders[j]->on_collide(*colliders[i]);
			}
		}
	}
}

void Collisions::destroy()
{
	colliders.clear();
	size = 0;
}
