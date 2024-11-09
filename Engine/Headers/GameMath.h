#pragma once

#include <ostream>
#include <iostream>
#include <math.h>

using std::ostream;

namespace Engine
{
	// Color struct to strore color values with alpha (4 channels)
	struct Color
	{
		unsigned char value[4]{ 0 };

		// constructors
		Color() {} // default constructor

		// parameterized controctor with default value for alpha
		Color(unsigned int _r, unsigned int _g, unsigned int _b, unsigned int _a = 255)
		{
			value[0] = _r > 255 ? 255 : _r;
			value[1] = _g > 255 ? 255 : _g;
			value[2] = _b > 255 ? 255 : _b;
			value[3] = _a > 255 ? 255 : _a;
		}

		bool equals(Color b) const;

		static Color BLACK;
		static Color SILVER;
		static Color GRAY;
		static Color WHITE;
		static Color MAROON;
		static Color RED;
		static Color PURPLE;
		static Color MAGENTA;
		static Color GREEN;
		static Color LIME;
		static Color OLIVE;
		static Color YELLOW;
		static Color NAVY;
		static Color BLUE;
		static Color TEAL;
		static Color AQUA;
		static Color DEBUG_COLOR;
	};

	// vector 2d class to handle all vector operations
	class Vector2
	{
	public:
		float x, y;

		static Vector2 zero; // (0,0)
		static Vector2 one; // (1,1)
		static Vector2 up; // (0,-1)
		static Vector2 down; // (0,1)
		static Vector2 left; // (-1,0)
		static Vector2 right; // (1,0)

		// constructors
		Vector2();
		Vector2(float value);
		Vector2(float _x, float _y);

		// setter Method
		void set(float _x, float _y);
		// returns magnitude of this vector
		float magnitude() const;
		// returns normalized value of this vector
		Vector2 normalize();
		// converts and return this vector into int
		Vector2 to_int();
		// returns float distance between this vector and v2
		float distance(Vector2 v2) const;
		// returns direct vector to target vector
		Vector2 direction(Vector2& v2) const;
		// returns clamped value of this vector between max and min vectors
		Vector2 clamp(Vector2 min, Vector2 max);
		// returns distance between given vector
		static float distance(const Vector2& v1, const Vector2& v2);
		// returns moved vector towards given vector with speed
		Vector2 move_towards(Vector2& v2, const float speed);
		// returns moved vector towards given vector with speed keeping minimum distance in check
		Vector2 move_towards(Vector2& v2, const float speed, float minDist);
		// get a random Vector2 between gin=ven range
		static Vector2 get_random(Vector2 _min, Vector2 _max);

		Vector2 operator-();
		Vector2 operator+(const Vector2& v2) const;
		Vector2 operator-(const Vector2& v2) const;
		Vector2 operator*(const float& value) const;
		Vector2 operator/(const float& value) const;

		Vector2& operator+=(const Vector2& v2);
		Vector2& operator-=(const Vector2& v2);
		Vector2& operator*=(const float& value);
		Vector2& operator/=(const float& value);

		bool operator==(const Vector2& v2) const;
		bool operator>(const Vector2& v2) const;
		bool operator<(const Vector2& v2) const;
		bool operator>=(const Vector2& v2) const;
		bool operator<=(const Vector2& v2) const;
		bool operator!=(const Vector2& v2) const;

		friend ostream& operator<<(ostream& os, Vector2& v)
		{
			return os << " (" << v.x << "," << v.y << ") ";
		}
	};

	// class to handle rect for drawing and collision (subjected to change)
	class Rect
	{
		float l, r, t, b;
	public:
		Vector2 size; // size of the rectangle
		//Vector2 center; // center of rect

		// constructors
		Rect();
		Rect(Vector2 _size, Vector2 _center);
		// setter method
		void set(Vector2 _size, Vector2 _center);
		// returns center position of rect 
		Vector2 get_center() const;
		// returns bottom right position of rect 
		Vector2 get_topleft() const;
		// returns bottom right position of rect
		Vector2 get_botmright() const;

		// set center of rect
		void set_center(Vector2 center);
		// set top of rect
		void set_top(float _value);
		// set bottom of rect
		void set_bottom(float _value);
		// set right of rect
		void set_right(float _value);
		// set left of rect
		void set_left(float _value);
		// moves rect in horizontal direction
		void move_h(float delta);
		// moves rect in verticle direction
		void move_v(float delta);
		// moves rect in given velocity
		void move(Vector2 delta);
		// clamps the rect in max and min bounds
		void clamp(const Vector2& min, const Vector2& max);
		// collide with other rect
		bool collide_as_rect(Rect& _rect) const;
		// collide this rect as circle with other rect
		bool collide_as_circle(Rect& _rect) const;

		// override outstreasm operator for output
		friend ostream& operator<<(ostream& os, Rect& rect)
		{
			return os << "[ " << rect.l << " , " << rect.t << " , " << rect.b << " , " << rect.r << " ] ";
		}
	};

	// converts given position into grid position
	Vector2 get_grid_pos(Vector2 _pos, int _size, int _space = 0);

	template<typename T>
	T clamp(T _val, T _min, T _max)
	{
		return std::max(_min, std::min(_max, _val));
	}
	// generic definition of simple functions
	template <typename T>
	T lerp(T _a, T _b, float _t)
	{
		_t = clamp(_t, 0.f, 1.f);
		return _a + _t * (_b - _a);
	}

	// function to convert time into HH::MM::SS format
	std::string get_time(double _sec);
}
