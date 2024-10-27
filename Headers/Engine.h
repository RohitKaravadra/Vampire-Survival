#pragma once
#include <iostream>
#include <math.h>
#include "GamesEngineeringBase.h"

using namespace GamesEngineeringBase;

namespace Engine
{

#pragma region Math

	// vector 2d class to handle all vector operations
	class Vector2
	{
	public:
		float x, y;

		// predefined static vectors
		static Vector2 zero;
		static Vector2 one;
		static Vector2 up;
		static Vector2 down;
		static Vector2 left;
		static Vector2 right;

		// constructors
		Vector2() // default Constructor
		{
			set(0, 0);
		}

		Vector2(float value) // single parameter Constructor
		{
			set(value, value);
		}

		Vector2(float _x, float _y) // double parameter Constructor
		{
			set(_x, _y);
		}

		// setter Method
		inline void set(float _x, float _y)
		{
			x = _x;
			y = _y;
		}

		// returns magnitude of this vector
		inline float magnitude()
		{
			return sqrtf(x * x + y * y);
		}

		// returns normalized value of this vector
		inline Vector2 normalize()
		{
			float mag = magnitude();
			return mag > 0 ? *this / mag : *this;
		}

		// converts and return this vector into int
		inline Vector2 to_int()
		{
			return Vector2(static_cast<int>(x), static_cast<int>(y));
		}

		// returnd float distance between this vector and v2
		inline float distance(Vector2 v2)
		{
			return (*this - v2).magnitude();
		}

		// returns clamped value of this vector between max and min vectors
		inline Vector2 clamp(Vector2 min, Vector2 max)
		{
			return Vector2(x < min.x ? min.x : x > max.x ? max.x : x, y < min.y ? min.y : y > max.y ? max.y : y);
		}

		// method to prints this Vector (Subjected to change and replace it with operator overloading)
		void print()
		{
			std::cout << "( " << x << " , " << y << " )\n";
		}

#pragma region Operator Overloading

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

	// defining static Vectors
	Vector2 Vector2::zero(0, 0);
	Vector2 Vector2::one(1, 1);
	Vector2 Vector2::up(0, -1);
	Vector2 Vector2::down(0, 1);
	Vector2 Vector2::left(-1, 0);
	Vector2 Vector2::right(0, 1);

	// class to handle rect for drawing and collision (subjected to change)
	class Rect
	{
		Vector2 size; // size of the rectangle
	public:
		Vector2 center; // center of rect

		// constructors
		Rect()
		{

		}

		Rect(Vector2 _size, Vector2 _center)
		{
			size = _size;
			center = _center;
		}

		// setter method
		void set(Vector2 _size, Vector2 _center)
		{
			size = _size;
			center = _center;
		}

		// getter for size
		Vector2 get_size() const
		{
			return size;
		}

		// returns top left position of rect (subjected to change and replace it with bounds)
		Vector2 get_topleft() const
		{
			return center - size / 2;
		}

		// returns bottom right position of rect (subjected to change and replace it with bounds)
		Vector2 get_botmright() const
		{
			return center + size / 2;
		}

		// setter for topleft
		void set_topleft(Vector2 value)
		{
			center = value + size / 2;
		}

		// setter for bottomright
		void set_botmright(Vector2 value)
		{
			center = value - size / 2;
		}

		// method to print rect (subjected to change and replace it with operator overloading)
		void print()
		{
			Vector2 topLeft = get_topleft();
			Vector2 botmRight = get_botmright();
			std::cout << "[ " << topLeft.x << " , " << topLeft.y << " , ";
			std::cout << botmRight.x << " , " << botmRight.y << " ]\n";
		}
	};

#pragma endregion

#pragma region Colors

	// Color struct to strore color values
	struct Color
	{
		unsigned int r, g, b, a;

		// constructors
		Color() // default constructor
		{
			r = 0;
			g = 0;
			b = 0;
			a = 0;
		}

		// parameterized controctor with default value for alpha
		Color(unsigned int _r, unsigned int _g, unsigned int _b, unsigned int _a = 255)
		{
			r = _r > 255 ? 255 : _r;
			g = _g > 255 ? 255 : _g;
			b = _b > 255 ? 255 : _b;
			a = _a > 255 ? 255 : _a;
		}
	};

	// predefined constant colors
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

	// class to handle primitive shape creation
	static class Shape
	{
		// private constructor for static class
		Shape();

	public:

		// method creates rectangle shape with given size and color and return it in out parameter
		inline static void rectangle(Vector2 _size, Color _color, Color*& out)
		{
			int len = _size.x * _size.y;
			out = new Color[len]();
			for (int i = 0; i < len; i++)
				out[i] = _color;
		}

		// method creates circle shape with given radii and color and return it in out parameter
		inline static void circle(unsigned int _rad, Color _color, Color*& out)
		{
			int diam = _rad * 2, len = diam * diam;
			Vector2 center = Vector2(_rad - 1);
			out = new Color[len]();
			for (int i = 0; i < len; i++)
			{
				float dist = Vector2(i % diam, i / diam).distance(center);
				if (dist < _rad)
					out[i] = _color;
				/*else
					out[i] = Color(0, 0, 0, 0);*/
			}
		}
	};


	// method to fill background of the window
	void fill_window(Window& win, Color color)
	{
		Vector2 size(win.getWidth(), win.getHeight());
		for (unsigned int i = 0; i < size.y * size.x; i++)
			win.draw(i % (int)size.x, i / size.x, color.r, color.g, color.b);
	}

#pragma endregion

#pragma region Sprites

	// class to handle sprites 
	class Sprite
	{
	protected:

		Color* image; // sprite image as array of colors (sbjected to change and replace with Image object)

		// constructors
		Sprite() // default constructor
		{
			image = nullptr;
		}

		// constructor creates rectangle shape
		Sprite(Vector2 _size, Vector2 _pos, Color _color)
		{
			Shape::rectangle(_size, _color, image);
			rect.set(_size, _pos);
		}

		// constructor creates circle shape
		Sprite(float _rad, Vector2 _pos, Color _color)
		{
			Shape::circle(_rad, _color, image);
			rect.set(Vector2(_rad * 2), _pos);
		}

	public:

		Rect rect; // rect for the sprite to draw and collide

		// method to update sprite
		virtual void update(float dt) {}

		// method to draw sprite on window
		void draw(Window& win)
		{
			if (image == nullptr) // returns if no image
				return;

			Vector2 winSize = Vector2(win.getWidth(), win.getHeight()); // windows bounds
			Vector2 pos = rect.get_topleft(); // start point to draw image
			Vector2 size = rect.get_size(); // size of the image or rect

			for (unsigned int y = 0; y < size.y; y++)
			{
				int posY = pos.y + y; // y position of image pixel on window

				//bound check for y
				if (posY < 0)
					continue;
				if (posY > winSize.y)
					break;

				for (unsigned int x = 0; x < size.x; x++)
				{
					int posX = pos.x + x; // x position of image pixel on window

					//x bound check
					if (posX < 0)
						continue;
					if (posX > winSize.x)
						break;

					Color color = image[static_cast<int>(size.x) * y + x]; // getting color values of pixel

					//draw pixel if alpha is greater than 0
					if (color.a > 0)
						win.draw(posX, posY, color.r, color.g, color.b);
				}
			}
		}

		~Sprite()
		{
			if (image != nullptr) // free memory of image if assigned
				delete[] image;
		}
	};

	// class allows to group Sprites together
	class SpriteGroup
	{
		Sprite** group = nullptr; // sprite group pointer
		int curIndex; // index of last element (subjected to change)
		int maxSize; // max size for the group (subjected to change after changing with dynamic data structure like vector)
	public:
		// constructure
		SpriteGroup(int _maxSize) // (subjected to change)
		{
			curIndex = 0;
			maxSize = _maxSize;
			group = new Sprite * [_maxSize]();

		}

		// method to add sprite to the group
		void add(Sprite* sprite)
		{
			if (group == nullptr)
				return Debug::print_warning("Sprite Group is destroyed or not initialize");

			if (curIndex < maxSize - 1)
				group[curIndex++] = sprite;
			else
			{
				Debug::print_warning("Max Group limit reached.. Object deleting object..");
				delete sprite;
				sprite = nullptr;
			}
		}

		// method to update all sprites in this group
		void update(float dt)
		{
			if (group == nullptr)
				return Debug::print_warning("Sprite Group is destroyed or not initialize");

			for (int i = 0; i < curIndex; i++)
				group[i]->update(dt);
		}

		// method to draw all sprites of this group
		void draw(Window& win)
		{
			if (group == nullptr)
				return Debug::print_warning("Sprite Group is destroyed or not initialize");

			for (int i = 0; i < curIndex; i++)
				group[i]->draw(win);
		}

		// method to destroy all sprites and this group
		void destroy()
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

		// destructor
		~SpriteGroup()
		{
			destroy();
		}
	};

#pragma endregion

#pragma region Inputs and Debugging

	// class handles inputs 
	class Inputs
	{
		static Window* win; // canvas to get inputs

		// constructors
		Inputs() {}; // private constructor for static class

	public:
		// initializes value of window
		static void Init(Window& _win)
		{
			if (win == nullptr)
				Inputs::win = &_win;
		}

		// deinitializes value of window
		static void free()
		{
			Inputs::win = nullptr;
		}

		// returns vector2 axis for player input
		static Vector2 get_axis()
		{
			Vector2 axis;

			if (win == nullptr) // return (0,0) if win is nor assigned
				return axis;

			if (win->keyPressed('W') || win->keyPressed(VK_UP))
				axis.y--;
			if (win->keyPressed('S') || win->keyPressed(VK_DOWN))
				axis.y++;
			if (win->keyPressed('A') || win->keyPressed(VK_LEFT))
				axis.x--;
			if (win->keyPressed('D') || win->keyPressed(VK_RIGHT))
				axis.x++;

			return axis.normalize();
		}

		// checks if given key is pressed or not
		static bool key_pressed(int key)
		{
			if (win == nullptr)
				return false;
			return win->keyPressed(key);
		}

		// checks if backspace or escape is pressed for UI inputs
		static bool ui_back()
		{
			if (win == nullptr)
				return false;
			return win->keyPressed(VK_ESCAPE) || win->keyPressed(VK_BACK);
		}
	};

	// defining static variables of Input
	Window* Inputs::win = nullptr;

	// Debug class to make debugging easy
	static class Debug
	{
		Debug() {};
	public:

		//method prints error on terminal
		static void print_error(std::string msg)
		{
			std::cout << " ERROR : " << msg << std::endl;
		}

		//method warning error on terminal
		static void print_warning(std::string msg)
		{
			std::cout << " WARNING : " << msg << std::endl;
		}
	};

#pragma endregion

}
