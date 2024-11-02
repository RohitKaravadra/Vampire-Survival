#include "Engine.h"

using namespace Engine;

#pragma region Sprite Methods

// constructors
Sprite::Sprite()
{
	id = generate_id();
}

Sprite::Sprite(Vector2 _size, Vector2 _pos)
{
	rect.set(_size, _pos);
}

Sprite::Sprite(Vector2 _size, Vector2 _pos, Color _color)
{
	id = generate_id();
	image.width = _size.x;
	image.height = _size.y;
	image.channels = 4;
	fill_image(image, _color);
	rect.set(_size, _pos);
}

Sprite::Sprite(string _location, Vector2 _pos, bool _center)
{
	id = generate_id();
	load_image(image, _location);
	Vector2 size = Vector2(image.width, image.height);
	rect.set(size, _center ? _pos : _pos + size / 2);
}

void Sprite::draw()
{
	if (image.data == NULL) // returns if no image
		return;

	Camera::draw(rect, image);
}

void Sprite::debug()
{
	if (DEBUG_MODE)
	{

	}
}

Sprite::~Sprite()
{

}

bool Sprite::operator==(Sprite& other) const
{
	return id == other.id;
}

bool Sprite::operator==(Sprite* other) const
{
	return id == other->id;
}

bool Sprite::operator!=(Sprite& other) const
{
	return id != other.id;
}

bool Sprite::operator!=(Sprite* other) const
{
	return id != other->id;
}

#pragma endregion

#pragma region SpritesGroup Methods

int SpriteGroup::get_size()
{
	return curIndex;
}

Sprite* SpriteGroup::get_sprite(unsigned int i)
{
	if (i < curIndex)
		return group[i];
	return nullptr;
}

SpriteGroup::SpriteGroup(unsigned int _maxSize) // (subjected to change)
{
	curIndex = 0;
	maxSize = _maxSize;
	group = new Sprite * [_maxSize]();

}

void SpriteGroup::add(Sprite* sprite)
{
	if (group == nullptr)
		return;

	if (curIndex < maxSize)
		group[curIndex++] = sprite;
	else
	{
		delete sprite;
		sprite = nullptr;
	}
}

void SpriteGroup::add(SpriteGroup* _group)
{
	if (group != nullptr && _group->group != nullptr)
	{
		int size = _group->get_size();
		for (unsigned int i = 0; i < size; i++)
		{
			if (curIndex < maxSize)
				add(_group->get_sprite(i));
		}
	}
}

void SpriteGroup::update(float dt)
{
	if (group == nullptr)
		return;

	for (int i = 0; i < curIndex; i++)
		group[i]->update(dt);
}

void SpriteGroup::draw()
{
	if (group == nullptr)
		return;

	for (int i = 0; i < curIndex; i++)
		group[i]->draw();
}

void SpriteGroup::debug()
{
	if (group == nullptr)
		return;

	for (int i = 0; i < curIndex; i++)
		group[i]->debug();
}

bool SpriteGroup::is_colliding(Rect& rect)
{
	for (int i = 0; i < curIndex; i++)
		if (rect.collide_as_rect(group[i]->rect))
			return true;
	return false;
}

void SpriteGroup::destroy()
{
	if (group != nullptr)
	{
		for (int i = 0; i < curIndex; i++)
		{
			if (group[i] != nullptr)
			{
				delete group[i];
				group[i] = nullptr;
			}
		}

		delete[] group;

		group = nullptr;
		curIndex = 0;
		maxSize = 0;
	}
}

bool SpriteGroup::remove(Sprite& _sprite)
{
	for (unsigned int i = 0; i < curIndex; i++)
	{
		if (_sprite == group[i])
		{
			group[i] = group[--curIndex];
			group[curIndex] = nullptr;
			return true;
		}
	}
	return false;
}

int SpriteGroup::remove(SpriteGroup& _group)
{
	int _size = _group.get_size();
	int removed = 0;
	for (unsigned int i = 0; i < curIndex; i++)
	{
		for (unsigned int j = 0; j < _size; j++)
		{
			if (_group.get_sprite(j) == group[i])
			{
				group[i] = group[--curIndex];
				group[curIndex] = nullptr;
				removed++;
			}
		}
	}
	return removed;
}

#pragma endregion
