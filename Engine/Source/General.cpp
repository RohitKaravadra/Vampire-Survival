#include "Engine.h"

using namespace Engine;

extern bool DEBUG_MODE = false;

int generate_id()
{
	return OBJECT_ID_COUNTER++;
}

void Engine::create_rect_outline(Image& _image, Color _color, int _width)
{
	int iw = _image.width, ih = _image.height;

	if (_image.data == NULL)
		_image.data = new unsigned char[iw * ih * _image.channels] {0};

	for (unsigned int w = 0; w < _width; w++)
	{
		for (unsigned int i = 0; i < iw; i++)
		{
			memcpy(&_image.data[(w * iw + i) * _image.channels], _color.value, _image.channels);
			memcpy(&_image.data[(((ih - 1 - w) * iw) + i) * _image.channels], _color.value, _image.channels);
		}

		for (unsigned int i = 0; i < ih; i++)
		{
			memcpy(&_image.data[(iw * i + w) * _image.channels], _color.value, _image.channels);
			memcpy(&_image.data[(iw * i + iw - 1 - w) * _image.channels], _color.value, _image.channels);
		}
	}
}

void Engine::create_circle_outline(Image& _image, Color _color, int _width)
{
	int iw = _image.width, ih = _image.height;
	Vector2 _center(iw / 2.f, ih / 2.f);

	if (_image.data == NULL)
		_image.data = new unsigned char[iw * ih * _image.channels] {0};

	float dist;
	for (int x = 0; x < iw; x++)
	{
		for (int y = 0; y < ih; y++)
		{
			dist = _center.distance(Vector2(x, y));

			if (dist <= _center.x && dist >= _center.x - _width)
				memcpy(&_image.data[x * y * _image.channels], _color.value, _image.channels);
		}
	}
}

void Engine::fill_image(Image& image, Color _color)
{
	Vector2 _size(image.width, image.height);
	image.data = new unsigned char[_size.x * _size.y * image.channels] {};
	for (unsigned int i = 0; i < _size.x * _size.y; i++)
		memcpy(&image.data[i * image.channels], _color.value, image.channels);
}
