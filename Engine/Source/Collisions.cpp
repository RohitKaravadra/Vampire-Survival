#include "Collisions.h"

#pragma region Collider

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

#pragma endregion

#pragma region Collisions

// static definitions
DArray<Collider*> Collisions::layer0;
DArray<Collider*> Collisions::layer1;
unsigned int Collisions::size0 = 0;
unsigned int Collisions::size1 = 0;

void Collisions::add_collider(Collider& _collider, int _layer)
{
	if (_layer == 0)
	{
		layer0.add(&_collider);
		size0 = layer0.get_size();
	}
	else if (_layer == 1)
	{
		layer1.add(&_collider);
		size1 = layer1.get_size();
	}
	//std::cout << "Collider Added " << _collider.get_tag() << std::endl;
}

void Collisions::remove_collider(Collider& _collider, int _layer)
{
	if (_layer == 0)
	{
		layer0.remove(&_collider);
		size0 = layer0.get_size();
	}
	else if (_layer == 1)
	{
		layer1.remove(&_collider);
		size1 = layer1.get_size();
	}
	//std::cout << "Collider Removed " << _collider.get_tag() << std::endl;
}

void Collisions::update()
{
	// check layer 0 collision with itself
	for (unsigned int i = 0; i < size0; i++)
	{
		if (layer0[i]->isActive)
		{
			for (unsigned int j = i + 1; j < size0 - 1; j++)
			{
				if (layer0[j]->isActive)
				{
					if (layer0[i]->rect.collide_as_rect(layer0[j]->rect))
					{
						layer0[i]->on_collide(*layer0[j]);
						layer0[j]->on_collide(*layer0[i]);
					}
				}
			}
		}
	}

	// check layer 1 collision with layer 0 (layer 1 is for projectile layers circle colliding)
	for (unsigned int i = 0; i < size0; i++)
	{
		if (layer0[i]->isActive)
		{
			for (unsigned int j = 0; j < size1; j++)
			{
				if (layer0[i]->rect.collide_as_circle(layer1[j]->rect))// mostly for projectiles
					layer0[i]->on_collide(*layer1[j]);
			}
		}
	}
}

void Collisions::destroy()
{
	layer0.clear();
	size0 = 0;
}

#pragma endregion
