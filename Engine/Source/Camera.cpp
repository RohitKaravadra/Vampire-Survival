#include "Engine.h"

using namespace Engine;

// static variables definition

bool Camera::notNull = true;
Window Camera::win;
Rect* Camera::followTarget = nullptr;
Vector2 Camera::offset;
Rect Camera::camRect;
Image Camera::background;

void Camera::create(std::string _name, Vector2 _size, Vector2 _pos, Color _bg)
{
	notNull = true;
	win.create(_size.x, _size.y, _name);
	camRect.set(_size, _pos);
	offset = _size / 2;
	set_bg_color(_bg);
}

void Camera::free()
{
	followTarget = nullptr;
}

Window& Camera::get_window()
{
	return win;
}

Vector2 Camera::world_to_screen(Vector2 _pos)
{
	return offset + _pos - camRect.center;
}

Vector2 Camera::screen_to_world(Vector2 _pos)
{
	return _pos + camRect.center - offset;
}

bool Camera::has_follow_target()
{
	return followTarget != nullptr;
}

void Camera::set_follow_target(Rect& rect)
{
	followTarget = &rect;
}

void Camera::set_bg_color(Color& _color)
{
	if (background.data == NULL)
	{
		background.width = camRect.size.x;
		background.height = camRect.size.y;
		background.channels = 3;
		background.data = new unsigned char[camRect.size.x * camRect.size.y * background.channels] {};

	}
	for (unsigned int i = 0; i < camRect.size.x * camRect.size.y; i++)
		memcpy(&background.data[i * background.channels], _color.value, background.channels);
}

void Camera::reset_follow_target()
{
	followTarget = nullptr;
}

void Camera::update(float dt)
{
	if (followTarget != nullptr && notNull)
		camRect.center = followTarget->center;
}

void Camera::clear()
{
	if (notNull)
	{
		win.clear();
		if (background.data != NULL)
			win.draw(camRect.size.x * camRect.size.y * background.channels, background.data);
	}
}

void Camera::draw(Rect& _rect, Image& _image)
{
	if (!notNull) // return if camera is not initialized
		return;

	if (!camRect.collide_as_rect(_rect)) // return if object is not in view
		return;

	Vector2 relPos = world_to_screen(_rect.get_topleft()); // relative position of object to camera

	for (unsigned int y = 0; y < _image.height; y++)
	{
		int posY = relPos.y + y; // y position of image pixel on window

		//bound check for y
		if (posY < 0)
			continue;
		if (posY > camRect.size.y)
			break;

		for (unsigned int x = 0; x < _image.width; x++)
		{
			int posX = relPos.x + x; // x position of image pixel on window

			//x bound check for x
			if (posX < 0)
				continue;
			if (posX > camRect.size.x)
				break;

			//draw pixel if alpha is greater than 0
			if (_image.alphaAt(x, y) > 0)
				win.draw(posX, posY, _image.at(x, y));
		}
	}

}

void Camera::present()
{
	win.present();
}
