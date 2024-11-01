#include "Engine.h"

using namespace Engine;

bool DEBUG_MODE = false;

int generate_id()
{
	return OBJECT_ID_COUNTER++;
}

// static variables

Vector2 Vector2::zero(0, 0);
Vector2 Vector2::one(1, 1);
Vector2 Vector2::up(0, -1);
Vector2 Vector2::down(0, 1);
Vector2 Vector2::left(-1, 0);
Vector2 Vector2::right(0, 1);

#pragma region Vector2 methods

Vector2::Vector2()
{
	set(0, 0);
}

Vector2::Vector2(float value)
{
	set(value, value);
}

Vector2::Vector2(float _x, float _y)
{
	set(_x, _y);
}

void Vector2::set(float _x, float _y)
{
	x = _x;
	y = _y;
}

float Vector2::magnitude()
{
	return sqrtf(x * x + y * y);
}

Vector2 Vector2::normalize()
{
	float mag = magnitude();
	return mag > 0 ? *this / mag : Vector2::zero;
}

Vector2 Vector2::to_int()
{
	return Vector2(static_cast<int>(x), static_cast<int>(y));
}

float Vector2::distance(Vector2& v2) const
{
	return (*this - v2).magnitude();
}

Vector2 Vector2::direction(Vector2& v2) const
{
	return (v2 - *this).normalize();
}

Vector2 Vector2::clamp(Vector2 min, Vector2 max)
{
	return Vector2(x < min.x ? min.x : x > max.x ? max.x : x, y < min.y ? min.y : y > max.y ? max.y : y);
}

float Vector2::distance(const Vector2& v1, const Vector2& v2)
{
	return (v1 - v2).magnitude();
}

Vector2 Vector2::move_towards(Vector2& v2, const float steps)
{
	return *this + direction(v2) * steps;
}

#pragma endregion

#pragma region Vector2 operators

Vector2 Vector2::operator+(const Vector2& v2) const
{
	return Vector2(x + v2.x, y + v2.y);
}

Vector2 Vector2::operator-(const Vector2& v2) const
{
	return Vector2(x - v2.x, y - v2.y);
}

Vector2 Vector2::operator*(const float& value) const
{
	return Vector2(x * value, y * value);
}

Vector2 Vector2::operator/(const float& value) const
{
	return Vector2(x / value, y / value);
}

Vector2& Vector2::operator+=(const Vector2& v2)
{
	this->x += v2.x;
	this->y += v2.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& v2)
{
	this->x -= v2.x;
	this->y -= v2.y;
	return *this;
}

Vector2& Vector2::operator*=(const float& value)
{
	this->x *= value;
	this->y *= value;
	return *this;
}

Vector2& Vector2::operator/=(const float& value)
{
	this->x /= value;
	this->y /= value;
	return *this;
}

bool Vector2::operator==(const Vector2& v2) const
{
	return x == v2.x && y == v2.y;
}

bool Vector2::operator>(const Vector2& v2) const
{
	return x > v2.x && y > v2.y;
}

bool Vector2::operator<(const Vector2& v2) const
{
	return x < v2.x && y < v2.y;
}

bool Vector2::operator>=(const Vector2& v2) const
{
	return x >= v2.x && y >= v2.y;
}

bool Vector2::operator<=(const Vector2& v2) const
{
	return x <= v2.x && y <= v2.y;
}

bool Vector2::operator!=(const Vector2& v2) const
{
	return x != v2.x || y != v2.y;
}

#pragma endregion

#pragma region Rect methods

// constructors
Rect::Rect()
{
	size = Vector2(3, 3);
	center = Vector2(0, 0);
	generate_debug_data();
}

Rect::Rect(Vector2 _size, Vector2 _center)
{
	size = _size;
	center = _center;
	generate_debug_data();
}

void Rect::generate_debug_data()
{
	if (!DEBUG_MODE)
		return;

	if (dbgImg.data != NULL)
		delete[] dbgImg.data;
	else
		dbgImg.channels = 4;

	dbgImg.width = size.x;
	dbgImg.height = size.y;
	dbgImg.data = new unsigned char[size.x * size.y * dbgImg.channels] {};
	for (unsigned int i = 0; i < size.x; i++)
	{
		memcpy(&dbgImg.data[i * dbgImg.channels], Color::DEBUG_COLOR.value, dbgImg.channels);
		memcpy(&dbgImg.data[(static_cast<int>((size.y - 1) * size.x) + i) * dbgImg.channels], Color::DEBUG_COLOR.value, dbgImg.channels);
	}
	for (unsigned int i = 0; i < size.y; i++)
	{
		memcpy(&dbgImg.data[static_cast<int>(size.x * i) * dbgImg.channels], Color::DEBUG_COLOR.value, dbgImg.channels);
		memcpy(&dbgImg.data[static_cast<int>(size.x * i + size.x - 1) * dbgImg.channels], Color::DEBUG_COLOR.value, dbgImg.channels);
	}
}

void Rect::set(Vector2 _size, Vector2 _center)
{
	size = _size;
	center = _center;
	generate_debug_data();
}

Vector2 Rect::get_topleft() const
{
	return center - size / 2;
}

Vector2 Rect::get_botmright() const
{
	return center + size / 2;
}

void Rect::set_topleft(Vector2& value)
{
	center = value + size / 2;
}

void Rect::set_botmright(Vector2& value)
{
	center = value - size / 2;
}

void Rect::clamp(const Vector2& min, const Vector2& max)
{
	center = center.clamp(min + size / 2, max - size / 2);
}

bool Rect::collide_as_rect(Rect& _rect)
{
	Vector2 atl = get_topleft();
	Vector2 btl = _rect.get_topleft();
	Vector2 abr = get_botmright();
	Vector2 bbr = _rect.get_botmright();

	return atl.x < bbr.x &&
		btl.x < abr.x &&
		atl.y < bbr.y &&
		btl.y < abr.y;
}

bool Rect::collide_as_circle(Rect& _rect)
{
	return Vector2::distance(center, _rect.center) < (size.x + _rect.size.x) / 2;
}

void Rect::debug()
{
	if (dbgImg.data != NULL)
		Camera::draw(*this, dbgImg);
}

#pragma endregion