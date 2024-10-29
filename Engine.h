#pragma once
#include <iostream>
#include <ostream>
#include <math.h>
#include "GamesEngineeringBase.h"

using namespace GamesEngineeringBase;
using std::ostream;

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
		void set(float _x, float _y)
		{
			x = _x;
			y = _y;
		}

		// returns magnitude of this vector
		float magnitude()
		{
			return sqrtf(x * x + y * y);
		}

		// returns normalized value of this vector
		Vector2 normalize()
		{
			float mag = magnitude();
			return mag > 0 ? *this / mag : Vector2::zero;
		}

		// converts and return this vector into int
		Vector2 to_int()
		{
			return Vector2(static_cast<int>(x), static_cast<int>(y));
		}

		// returns float distance between this vector and v2
		float distance(Vector2& v2) const
		{
			return (*this - v2).magnitude();
		}

		// returns direct vector to target vector
		Vector2 direction(Vector2& v2) const
		{
			return (v2 - *this).normalize();
		}

		// returns clamped value of this vector between max and min vectors
		Vector2 clamp(Vector2 min, Vector2 max)
		{
			return Vector2(x < min.x ? min.x : x > max.x ? max.x : x, y < min.y ? min.y : y > max.y ? max.y : y);
		}

		// returns distance between given vector
		static float distance(const Vector2& v1, const Vector2& v2)
		{
			return (v1 - v2).magnitude();
		}

		// returns moved vector towards given vector with speed
		Vector2 move_towards(Vector2& v2, const float steps)
		{
			return *this + direction(v2) * steps;
		}

#pragma region Operator Overloading

		inline Vector2 operator+(const Vector2& v2) const
		{
			return Vector2(x + v2.x, y + v2.y);
		}

		inline Vector2 operator-(const Vector2& v2) const
		{
			return Vector2(x - v2.x, y - v2.y);
		}

		inline Vector2 operator*(const float& value) const
		{
			return Vector2(x * value, y * value);
		}

		inline Vector2 operator/(const float& value) const
		{
			return Vector2(x / value, y / value);
		}


		Vector2& operator+=(const Vector2& v2)
		{
			this->x += v2.x;
			this->y += v2.y;
			return *this;
		}

		Vector2& operator-=(const Vector2& v2)
		{
			this->x -= v2.x;
			this->y -= v2.y;
			return *this;
		}

		Vector2& operator*=(const float& value)
		{
			this->x *= value;
			this->y *= value;
			return *this;
		}

		Vector2& operator/=(const float& value)
		{
			this->x /= value;
			this->y /= value;
			return *this;
		}


		inline bool operator==(const Vector2& v2) const
		{
			return x == v2.x && y == v2.y;
		}

		inline bool operator>(const Vector2& v2) const
		{
			return x > v2.x && y > v2.y;
		}

		inline bool operator<(const Vector2& v2) const
		{
			return x < v2.x && y < v2.y;
		}

		inline bool operator>=(const Vector2& v2) const
		{
			return x >= v2.x && y >= v2.y;
		}

		inline bool operator<=(const Vector2& v2) const
		{
			return x <= v2.x && y <= v2.y;
		}

		inline bool operator!=(const Vector2& v2) const
		{
			return x != v2.x || y != v2.y;
		}

		friend ostream& operator<<(ostream& os, Vector2& v)
		{
			return os << " (" << v.x << "," << v.y << ") ";
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
	public:
		Vector2 size; // size of the rectangle
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
		void set_topleft(Vector2& value)
		{
			center = value + size / 2;
		}

		// setter for bottomright
		void set_botmright(Vector2& value)
		{
			center = value - size / 2;
		}

		// clamps the rect in max and min bounds
		void clamp(const Vector2& min, const Vector2& max)
		{
			center = center.clamp(min + size / 2, max - size / 2);
		}

		// override outstreasm operator for output
		friend ostream& operator<<(ostream& os, Rect& rect)
		{
			Vector2 topLeft = rect.get_topleft();
			Vector2 botmRight = rect.get_botmright();
			return os << "[ " << topLeft.x << " , " << topLeft.y << " , " << botmRight.x << " , " << botmRight.y << " ] ";
		}
	};

#pragma endregion

#pragma region Inputs and Debugging

	// class handles inputs 
	class Inputs
	{
		static Window* win; // canvas to get inputs
		static Vector2 mousePos; // save mouse pos per frame

		// constructors
		Inputs() {}; // private constructor for static class

	public:

		// default destructure
		~Inputs() {};

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

		static void refresh()
		{
			if (win == nullptr)
				return;

			win->checkInput();
			mousePos.set(win->getMouseInWindowX(), win->getMouseInWindowY());
		}

		// returns horizontal axis input
		static int get_h_axis()
		{
			if (win == nullptr)
				return 0;

			int axis = 0;

			if (win->keyPressed('A') || win->keyPressed(VK_LEFT))
				axis--;
			if (win->keyPressed('D') || win->keyPressed(VK_RIGHT))
				axis++;

			return axis;
		}

		// returns vertical axis input
		static int get_v_axis()
		{
			if (win == nullptr)
				return 0;

			int axis = 0;

			if (win->keyPressed('W') || win->keyPressed(VK_LEFT))
				axis--;
			if (win->keyPressed('S') || win->keyPressed(VK_RIGHT))
				axis++;

			return axis;
		}

		// returns vector2 axis for player input
		static Vector2 get_axis()
		{
			return Vector2(get_h_axis(), get_v_axis()).normalize();
		}

		// returns mouse position on window
		static Vector2 get_mouse_pos()
		{
			return mousePos;
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
	Vector2 Inputs::mousePos(0, 0);

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

		//method print warning on terminal
		static void print_warning(std::string msg)
		{
			std::cout << " WARNING : " << msg << std::endl;
		}

		//method print message on terminal
		static void print_message(std::string msg)
		{
			std::cout << " WARNING : " << msg << std::endl;
		}
	};

#pragma endregion

#pragma region Collisions

	class Collision
	{
	public:
		static bool circle_collide(const Rect& a, const Rect& b)
		{
			return Vector2::distance(a.center, b.center) < (a.size.x + b.size.x) / 2;
		}

		static bool rect_collide(const Rect& a, const Rect& b)
		{
			Vector2 atl = a.get_topleft();
			Vector2 btl = b.get_topleft();
			Vector2 abr = a.get_botmright();
			Vector2 bbr = b.get_botmright();

			return atl.x < bbr.x &&
				btl.x < abr.x &&
				atl.y < bbr.y &&
				btl.y < abr.y;
		}
	};

#pragma endregion

#pragma region Colors

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

#pragma region Camera and Rendering

	// camera class to handle camera movement and rendering 
	static class Camera
	{
		static Window win; // canvas to print on (canvas size is cameras size)
		static Rect* followTarget; // follow target for camera
		static Vector2 offset; // offset value to keep (0,0) in center of canvas

		Camera() {};
	public:
		static bool notNull; // to check if camera is not created
		static Rect camRect; // camera rect 

		// create new camera
		static void create(std::string _name, Vector2 _size, Vector2 _pos = Vector2::zero)
		{
			notNull = true;
			win.create(_size.x, _size.y, _name);
			camRect.set(_size, _pos);
			offset = _size / 2;
		}

		// destroy camera
		static void free()
		{
			followTarget = nullptr;
		}

		// get referance to window
		static Window& get_window()
		{
			return win;
		}

		// returns relative position to camera
		static Vector2 get_rel_pos(Vector2 _pos)
		{
			return offset + _pos - camRect.center;
		}

		// check if follow target is assigned or not
		static bool has_follow_target()
		{
			return followTarget != nullptr;
		}

		// setter for follow target
		static void set_follow_target(Rect& rect)
		{
			followTarget = &rect;
		}

		// resets follow target
		static void reset_follow_target()
		{
			followTarget = nullptr;
		}

		// updates cameras if follow follow target set
		static void update(float dt)
		{
			if (followTarget != nullptr && notNull)
				camRect.center = followTarget->center;
		}

		// clear canvas
		static void clear()
		{
			if (notNull)
				win.clear();
		}

		// draw object on canvas if in view
		static void draw(Rect& _rect, Image& _image)
		{
			if (!Collision::rect_collide(camRect, _rect)) // return if object is not in view
				return;

			Vector2 relPos = get_rel_pos(_rect.get_topleft()); // relative position of object to camera

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

		// present canvas
		static void present()
		{
			win.present();
		}
	};

	bool Camera::notNull = true;
	Window Camera::win;
	Rect* Camera::followTarget = nullptr;
	Vector2 Camera::offset;
	Rect Camera::camRect;

#pragma endregion

#pragma region Primitive Shapes

	// class to handle primitive shape creation
	static class Shape
	{
		// private constructor for static class
		Shape();

	public:

		// method creates rectangle shape with given size and color and return it in out parameter
		inline static void rectangle(Vector2 _size, Color _color, Image& outImage)
		{
			outImage.width = _size.x;
			outImage.height = _size.y;
			outImage.channels = 4;
			outImage.data = new unsigned char[_size.x * _size.y * outImage.channels];
			for (unsigned int i = 0; i < _size.x * _size.y; i++)
				memcpy(&outImage.data[i * outImage.channels], _color.value, outImage.channels);
		}

		// method creates circle shape with given radii and color and return it in out parameter
		inline static void circle(unsigned int _rad, Color _color, Image& outImage)
		{
			int diam = _rad * 2, len = diam * diam;
			outImage.width = outImage.height = diam;
			outImage.channels = 4;
			outImage.data = new unsigned char[diam * diam * outImage.channels] {0};
			Vector2 center = Vector2(_rad - 1);
			for (int i = 0; i < len; i++)
			{
				float dist = Vector2(i % diam, i / diam).distance(center);
				if (dist < _rad)
					memcpy(&outImage.data[i * outImage.channels], _color.value, outImage.channels);
			}
		}
	};


	// method to fill background of the window
	void fill_window(Window& win, Color color)
	{
		Vector2 size(win.getWidth(), win.getHeight());
		for (unsigned int i = 0; i < size.y * size.x; i++)
			win.draw(i % (int)size.x, i / size.x, color.value);
	}

#pragma endregion

#pragma region Sprites

	// class to handle sprites 
	class Sprite
	{
	protected:

		Image image; // sprite image as array of colors (sbjected to change and replace with Image object)

		// constructors
		Sprite() // default constructor
		{
		}

		//// constructor creates rectangle shape
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

		// constructor creates image 
		Sprite(Vector2 _pos, std::string _location, bool _center = true)
		{
			if (image.load(_location))
			{
				Vector2 size = Vector2(image.width, image.height);
				rect.set(size, _center ? _pos : _pos + size / 2);
			}
			else
			{
				Shape::rectangle(Vector2(10), RED, image);
				rect.set(Vector2(10), _pos);
			}
		}

	public:

		Rect rect; // rect for the sprite to draw and collide

		// method to update sprite
		virtual void update(float dt) {}

		// method to draw sprite on window
		void draw()
		{
			if (image.data == NULL) // returns if no image
				return;

			if (Camera::notNull) // check if camera is created
				Camera::draw(rect, image);
		}

		~Sprite()
		{
			if (image.data != NULL) // free memory of image if assigned
				image.free();
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

			if (curIndex < maxSize)
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
		void draw()
		{
			if (group == nullptr)
			{
				Debug::print_warning("Sprite Group is destroyed or not initialize");
				return;
			}

			for (int i = 0; i < curIndex; i++)
				group[i]->draw();
		}

		// checks collision of given sprite with all sprites
		bool is_colliding(Rect& rect)
		{
			for (int i = 0; i < curIndex; i++)
				if (Collision::rect_collide(rect, group[i]->rect))
					return true;
			return false;
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

}
