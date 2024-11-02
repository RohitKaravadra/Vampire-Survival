#include "Engine.h"

using namespace Engine;
// static variables

Vector2 Vector2::zero(0, 0);
Vector2 Vector2::one(1, 1);
Vector2 Vector2::up(0, -1);
Vector2 Vector2::down(0, 1);
Vector2 Vector2::left(-1, 0);
Vector2 Vector2::right(1, 0);

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

Vector2 Vector2::move_towards(Vector2& v2, const float speed)
{
	return distance(v2) > speed ? *this + direction(v2) * speed : v2;
}

Vector2 Vector2::move_towards(Vector2& v2, const float speed, float minDist)
{
	return distance(v2) > minDist ? *this + direction(v2) * speed : v2 - direction(v2) * minDist;
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
	set(Vector2(4), Vector2::zero);
}

Rect::Rect(Vector2 _size, Vector2 _center)
{
	set(_size, _center);
}

void Rect::set(Vector2 _size, Vector2 _center)
{
	l = 0;
	r = _size.x;
	t = 0;
	b = _size.y;
	size = _size;
	set_center(_center);
}

Vector2 Rect::get_center() const
{
	return Vector2(l + (r - l) / 2, t + (b - t) / 2);
}

Vector2 Rect::get_topleft() const
{
	return Vector2(l, t);
}

Vector2 Rect::get_botmright() const
{
	return Vector2(r, b);
}

void Rect::set_center(Vector2 center)
{
	Vector2 hSize = size / 2;
	l = center.x - hSize.x;
	r = center.x + hSize.x;
	t = center.y - hSize.y;
	b = center.y + hSize.y;
}

void Rect::set_top(float _value)
{
	t = _value;
	b = t + size.y;
}

void Rect::set_bottom(float _value)
{
	b = _value;
	t = b - size.y;
}

void Rect::set_left(float _value)
{
	l = _value;
	r = l + size.x;
}

void Rect::set_right(float _value)
{
	r = _value;
	l = r - size.x;
}

void Rect::move_h(float delta)
{
	l += delta;
	r += delta;
}

void Rect::move_v(float delta)
{
	t += delta;
	b += delta;
}

void Rect::move(Vector2 delta)
{
	move_h(delta.x);
	move_v(delta.y);
}

void Rect::clamp(const Vector2& min, const Vector2& max)
{
	Vector2 center = get_center();
	Vector2 hSize = size / 2;
	center = center.clamp(min + hSize, max - hSize);
	set_center(center);
}

bool Rect::collide_as_rect(Rect& _rect) const
{
	return l < _rect.r &&
		_rect.l < r &&
		t < _rect.b &&
		_rect.t < b;
}

bool Rect::collide_as_circle(Rect& _rect) const
{
	return Vector2::distance(get_center(), _rect.get_center()) < (_rect.size.x + size.x) / 2;
}

#pragma endregion
