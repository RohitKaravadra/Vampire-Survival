#include "Engine.h"
#include "Resources.h"

using namespace Engine;

#pragma region Sprite Methods

Sprite::Sprite(Vector2 _size, Vector2 _pos)
{
	id = generate_id();
	rect.set(_size, _pos);
	image.width = _size.x;
	image.height = _size.y;
	image.channels = 4;
	create_debug_data();
}

Sprite::Sprite(Vector2 _size, Vector2 _pos, Color _color)
{
	id = generate_id();
	image.width = _size.x;
	image.height = _size.y;
	image.channels = 4;
	fill_image(image, _color);
	rect.set(_size, _pos);
	create_debug_data();
}

Sprite::Sprite(std::string _location, Vector2 _pos, bool _center)
{
	id = generate_id();
	load_image(image, _location);
	Vector2 size = Vector2(image.width, image.height);
	rect.set(size, _center ? _pos : _pos + size / 2);
	create_debug_data();
}


void Sprite::create_debug_data()
{
	if (!DEBUG_MODE)
		return;

	dbgImage.width = rect.size.x;
	dbgImage.height = rect.size.y;
	dbgImage.channels = 4;
	create_rect_outline(dbgImage, Color::DEBUG_COLOR);
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
		Camera::draw(rect, dbgImage);
}

bool Sprite::operator==(Sprite& other) const { return id == other.id; }
bool Sprite::operator==(Sprite* other) const { return id == other->id; }
bool Sprite::operator!=(Sprite& other) const { return id != other.id; }
bool Sprite::operator!=(Sprite* other) const { return id != other->id; }

#pragma endregion
