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

float Vector2::magnitude() const
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

float Vector2::distance(Vector2 v2) const
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
	return  distance(v2) > speed ? *this + direction(v2) * speed : v2;
}

Vector2 Vector2::move_towards(Vector2& v2, const float speed, float minDist)
{
	float dist = distance(v2);
	if (dist <= minDist)
		return *this;
	dist -= minDist;
	return *this + direction(v2) * min(dist, speed);
}

Vector2 Vector2::get_random(Vector2 _min, Vector2 _max)
{
	Vector2 _range = _max - _min;
	return _min + Vector2(rand() % static_cast<int>(_range.x), rand() % static_cast<int>(_range.y));
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

Vector2 Engine::get_grid_pos(Vector2 _pos, int _size, int _space)
{
	float tSize = _size + _space;
	Vector2 gPos = (_pos / tSize).to_int();

	gPos.x += _pos.x < 0 ? -0.5f : 0.5f;
	gPos.y += _pos.y < 0 ? -0.5f : 0.5f;

	return gPos;
}

std::string Engine::get_time(double _sec)
{
	std::string time = "";
	time += std::to_string(static_cast<int>(_sec / 3600)) +
		":" + std::to_string(static_cast<int>(_sec) % 3600 / 60) +
		":" + std::to_string(static_cast<int>(_sec) % 60);
	return time;
}

Vector2 Engine::get_random_exclude(Rect _rect, Vector2 _extra)
{
	// choose a random side
	int _side = rand() % 4;
	Vector2 _tl = _rect.get_topleft() - Vector2(50), _br = _rect.get_botmright() + Vector2(50);
	Vector2 _min, _max;
	switch (_side)
	{
		// left side
	case 0:_min.set(_tl.x - _extra.x, _tl.y - _extra.y); _max.set(_tl.x, _br.y + _extra.y);
		break;
		//right side
	case 1:_min.set(_br.x, _tl.y - _extra.y); _max.set(_br.x + _extra.x, _br.y + _extra.y);
		break;
		//top side
	case 2:_min.set(_tl.x - _extra.x, _tl.y - _extra.y); _max.set(_br.x + _extra.x, _tl.y);
		break;
		// bottom side
	case 3:_min.set(_tl.x - _extra.x, _br.y); _max.set(_br.x + _extra.x, _br.y + _extra.y);
		break;
	}

	// return random between 2 vectors
	return Vector2::get_random(_min, _max);
}

