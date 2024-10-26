#pragma once
#include <iostream>
#include <math.h>
#include "GamesEngineeringBase.h"

using namespace GamesEngineeringBase;


class Vector2
{
public:
	float x, y;

	static Vector2 zero;
	static Vector2 one;
	static Vector2 up;
	static Vector2 down;
	static Vector2 left;
	static Vector2 right;

	Vector2()
	{
		set(0, 0);
	}

	Vector2(float value)
	{
		set(value, value);
	}

	Vector2(float _x, float _y)
	{
		set(_x, _y);
	}

	inline void set(float _x, float _y)
	{
		x = _x;
		y = _y;
	}

	inline float magnitude()
	{
		return sqrtf(x * x + y * y);
	}

	inline Vector2 normalize()
	{
		float mag = magnitude();
		return mag > 0 ? *this / mag : *this;
	}

	inline Vector2 to_int()
	{
		return Vector2(static_cast<int>(x), static_cast<int>(y));
	}

	inline float distance(Vector2 v2)
	{
		return (*this - v2).magnitude();
	}

	inline Vector2 clamp(Vector2 min, Vector2 max)
	{
		return Vector2(x < min.x ? min.x : x > max.x ? max.x : x, y < min.y ? min.y : y > max.y ? max.y : y);
	}

	void print()
	{
		std::cout << "( " << x << " , " << y << " )\n";
	}

#pragma region Operators

	inline Vector2 operator+(Vector2 v2) const
	{
		return Vector2(x + v2.x, y + v2.y);
	}

	inline Vector2 operator-(Vector2 v2) const
	{
		return Vector2(x - v2.x, y - v2.y);
	}

	inline Vector2 operator*(float value) const
	{
		return Vector2(x * value, y * value);
	}

	inline Vector2 operator/(float value) const
	{
		return Vector2(x / value, y / value);
	}

	inline void operator+=(Vector2 v2)
	{
		*this = *this + v2;
	}

	inline void operator-=(Vector2 v2)
	{
		*this = *this - v2;
	}

	inline bool operator==(Vector2 v2) const
	{
		return x == v2.x && y == v2.y;
	}

	inline bool operator>(Vector2 v2) const
	{
		return x > v2.x && y > v2.y;
	}

	inline bool operator<(Vector2 v2) const
	{
		return x < v2.x && y < v2.y;
	}

	inline bool operator>=(Vector2 v2) const
	{
		return x >= v2.x && y >= v2.y;
	}

	inline bool operator<=(Vector2 v2) const
	{
		return x <= v2.x && y <= v2.y;
	}

	inline bool operator!=(Vector2 v2) const
	{
		return x != v2.x || y != v2.y;
	}

#pragma endregion
};

Vector2 Vector2::zero(0, 0);
Vector2 Vector2::one(1, 1);
Vector2 Vector2::up(0, -1);
Vector2 Vector2::down(0, 1);
Vector2 Vector2::left(-1, 0);
Vector2 Vector2::right(0, 1);

class Rect
{
	Vector2 size;
public:
	Vector2 center;

	Rect()
	{

	}

	Rect(Vector2 _size, Vector2 _center)
	{
		size = _size;
		center = _center;
	}

	void set(Vector2 _size, Vector2 _center)
	{
		size = _size;
		center = _center;
	}

	Vector2 get_size() const
	{
		return size;
	}

	Vector2 get_topleft() const
	{
		return center - size / 2;
	}

	Vector2 get_botmright() const
	{
		return center + size / 2;
	}

	void set_topleft(Vector2 value)
	{
		center = value + size / 2;
	}

	void set_botmright(Vector2 value)
	{
		center = value - size / 2;
	}

	void print()
	{
		Vector2 topLeft = get_topleft();
		Vector2 botmRight = get_botmright();
		std::cout << "[ " << topLeft.x << " , " << topLeft.y << " , ";
		std::cout << botmRight.x << " , " << botmRight.y << " ]\n";
	}
};


#pragma region Colors

struct Color
{
	unsigned int r, g, b, a;
	Color()
	{
		r = 0;
		g = 0;
		b = 0;
		a = 0;
	}

	Color(unsigned int _r, unsigned int _g, unsigned int _b, unsigned int _a = 255)
	{
		r = _r > 255 ? 255 : _r;
		g = _g > 255 ? 255 : _g;
		b = _b > 255 ? 255 : _b;
		a = _a > 255 ? 255 : _a;
	}
};

const Color BLACK(0, 0, 0);
const Color SILVER(192, 192, 192);
const Color GRAY(128, 128, 128);
const Color WHITE(255, 255, 255);
const Color MAROON(128, 0, 0);
const Color RED(255, 0, 0);
const Color PURPLE(128, 0, 128);
const Color FUCHSIA(255, 0, 255);
const Color GREEN(0, 128, 0);
const Color LIME(0, 255, 0);
const Color OLIVE(128, 128, 0);
const Color YELLOW(255, 255, 0);
const Color NAVY(0, 0, 128);
const Color BLUE(0, 0, 255);
const Color TEAL(0, 128, 128);
const Color AQUA(0, 255, 255);

#pragma endregion 

#pragma region Primitive Shapes

class Shape
{
	Shape();

public:

	inline static void rectangle(Vector2 _size, Color _color, Color*& out)
	{
		int len = _size.x * _size.y;
		out = new Color[len];
		for (int i = 0; i < len; i++)
			out[i] = _color;
	}

	inline static void circle(unsigned int _rad, Color _color, Color*& out)
	{
		int diam = _rad * 2, len = diam * diam;
		Vector2 center = Vector2(_rad - 1);
		out = new Color[len];
		for (int i = 0; i < len; i++)
		{
			float dist = Vector2(i % diam, i / diam).distance(center);
			if (dist < _rad)
				out[i] = _color;
			else
				out[i] = Color(0, 0, 0, 0);
		}
	}
};

void fill_window(Window& win, Color color)
{
	Vector2 size(win.getWidth(), win.getHeight());
	for (unsigned int i = 0; i < size.y * size.x; i++)
		win.draw(i % (int)size.x, i / size.x, color.r, color.g, color.b);
}

#pragma endregion


class Sprite
{
protected:

	Color* image;

	Sprite()
	{
		image = nullptr;
	}

	Sprite(Vector2 _size, Vector2 _pos, Color _color)
	{
		Shape::rectangle(_size, _color, image);
		rect.set(_size, _pos);
	}

	Sprite(float _rad, Vector2 _pos, Color _color)
	{
		Shape::circle(_rad, _color, image);
		rect.set(Vector2(_rad * 2), _pos);
	}

public:

	Rect rect;

	void draw(Window& win)
	{
		if (image == nullptr)
			return;

		Vector2 winSize = Vector2(win.getWidth(), win.getHeight());
		Vector2 pos = rect.get_topleft();
		Vector2 size = rect.get_size();

		for (unsigned int y = 0; y < size.y; y++)
		{
			int posY = pos.y + y;
			if (posY < 0)
				continue;
			if (posY > winSize.y)
				break;

			for (unsigned int x = 0; x < size.x; x++)
			{
				int posX = pos.x + x;
				if (posX < 0)
					continue;
				if (posX > winSize.x)
					break;

				Color color = image[static_cast<int>(size.x) * y + x];
				if (color.a > 0)
					win.draw(posX, posY, color.r, color.g, color.b);
			}
		}
	}

	~Sprite()
	{
		if (image != nullptr)
			delete[] image;
	}
};

static class Inputs
{
	static Window* win;

	Inputs();
	~Inputs();

public:
	static void Init(Window& _win)
	{
		if (win == nullptr)
			Inputs::win = &_win;
	}

	static void free()
	{
		Inputs::win = nullptr;
	}

	static Vector2 get_axis()
	{
		Vector2 dir;

		if (win == nullptr)
			return dir;

		if (win->keyPressed('W') || win->keyPressed(VK_UP))
			dir.y--;
		if (win->keyPressed('S') || win->keyPressed(VK_DOWN))
			dir.y++;
		if (win->keyPressed('A') || win->keyPressed(VK_LEFT))
			dir.x--;
		if (win->keyPressed('D') || win->keyPressed(VK_RIGHT))
			dir.x++;

		return dir.normalize();
	}

	static bool key_pressed(int key)
	{
		if (win == nullptr)
			return false;
		return win->keyPressed(key);
	}

	static bool ui_back()
	{
		if (win == nullptr)
			return false;
		return win->keyPressed(VK_ESCAPE) || win->keyPressed(VK_BACK);
	}
};

Window* Inputs::win = nullptr;
