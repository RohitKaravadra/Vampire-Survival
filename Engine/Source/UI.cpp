#include "Engine.h"

using namespace UI;
using Engine::clamp;

void FillBar::fill()
{
	if (image.data == nullptr)
		return;

	int w = static_cast<float>(image.width) * val; // find current fill value

	for (int i = 0; i < image.width * image.height; i++)
		memcpy(&image.data[i * image.channels], i % image.width < w ? fColor.value : bColor.value, image.channels);
}

void FillBar::create(Vector2 _size, Vector2 _pos, Engine::Color _front, Engine::Color _back, float _val)
{
	// creating an image
	image.width = _size.x;
	image.height = _size.y;
	image.channels = 4;
	image.data = new unsigned char[image.width * image.height * image.channels] {0};

	// setting default values
	fColor = _front;
	bColor = _back;
	rect.size.set(image.width, image.height);
	rect.set_top(_pos.y);
	rect.set_left(_pos.x);
	set_value(_val);
}

void FillBar::set_value(float _val)
{
	val = clamp(_val, 0.f, 1.f);
	fill();
}

void FillBar::set_pos(Vector2 _pos)
{
	rect.set_center(_pos);
}

void FillBar::draw()
{
	Engine::Camera::draw(rect, image);
}

void FillBar::draw_ui()
{
	Engine::Camera::draw_ui(rect.get_topleft(), image);
}