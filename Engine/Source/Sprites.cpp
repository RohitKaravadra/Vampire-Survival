#include "Engine.h"

using namespace Engine;

#pragma region Sprite Methods

// constructors
Sprite::Sprite()
{
	id = generate_id();
}

Sprite::Sprite(Vector2 _size, Vector2 _pos, Color _color)
{
	id = generate_id();
	image.width = _size.x;
	image.height = _size.y;
	image.channels = 4;
	image.data = new unsigned char[_size.x * _size.y * image.channels] {};
	for (unsigned int i = 0; i < _size.x * _size.y; i++)
		memcpy(&image.data[i * image.channels], _color.value, image.channels);
	rect.set(_size, _pos);
}

Sprite::Sprite(Vector2 _pos, std::string _location, bool _center)
{
	id = generate_id();
	if (load_image(image, _location))
	{
		Vector2 size = Vector2(image.width, image.height);
		rect.set(size, _center ? _pos : _pos + size / 2);
	}
}

void Sprite::draw()
{
	if (image.data == NULL) // returns if no image
		return;

	if (Camera::notNull) // check if camera is created
		Camera::draw(rect, image);
}

Sprite::~Sprite()
{
	if (image.data != NULL) // free memory of image if assigned
		image.free();
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

Sprite* SpriteGroup::get_sprite(int i)
{
	if (i < curIndex)
		return group[i];
	return nullptr;
}

SpriteGroup::SpriteGroup(int _maxSize) // (subjected to change)
{
	curIndex = 0;
	maxSize = _maxSize;
	group = new Sprite * [_maxSize]();

}

void SpriteGroup::add(Sprite* sprite)
{
	if (group == nullptr)
		return Debug::print_warning("Sprite Group is destroyed or not initialize");

	if (curIndex < maxSize)
		group[curIndex++] = sprite;
	else
	{
		Debug::print_warning("Max Group limit reached.. Object deleting object..");
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
		return Debug::print_warning("Sprite Group is destroyed or not initialize");

	for (int i = 0; i < curIndex; i++)
		group[i]->update(dt);
}

void SpriteGroup::draw()
{
	if (group == nullptr)
	{
		Debug::print_warning("Sprite Group is destroyed or not initialize");
		return;
	}

	for (int i = 0; i < curIndex; i++)
		group[i]->draw();
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