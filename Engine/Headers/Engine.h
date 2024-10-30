#pragma once
#include <iostream>
#include <ostream>
#include <math.h>
#include "GamesEngineeringBase.h"

using namespace GamesEngineeringBase;
using std::ostream;

static int OBJECT_ID_COUNTER;

int generate_id();

namespace Engine
{
	class Color;

	bool load_image(Image& image, std::string location);
	void fill_window(Window& win, Color color);

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
		Vector2();
		Vector2(float value);
		Vector2(float _x, float _y);

		void set(float _x, float _y);
		// setter Method

		float magnitude();// returns magnitude of this vector

		Vector2 normalize();// returns normalized value of this vector

		Vector2 to_int();// converts and return this vector into int

		float distance(Vector2& v2) const;// returns float distance between this vector and v2

		Vector2 direction(Vector2& v2) const;// returns direct vector to target vector

		Vector2 clamp(Vector2 min, Vector2 max);// returns clamped value of this vector between max and min vectors

		static float distance(const Vector2& v1, const Vector2& v2);// returns distance between given vector

		Vector2 move_towards(Vector2& v2, const float steps);// returns moved vector towards given vector with speed

		inline Vector2 operator+(const Vector2& v2) const;
		inline Vector2 operator-(const Vector2& v2) const;
		inline Vector2 operator*(const float& value) const;
		inline Vector2 operator/(const float& value) const;

		Vector2& operator+=(const Vector2& v2);
		Vector2& operator-=(const Vector2& v2);
		Vector2& operator*=(const float& value);
		Vector2& operator/=(const float& value);

		inline bool operator==(const Vector2& v2) const;
		inline bool operator>(const Vector2& v2) const;
		inline bool operator<(const Vector2& v2) const;
		inline bool operator>=(const Vector2& v2) const;
		inline bool operator<=(const Vector2& v2) const;
		inline bool operator!=(const Vector2& v2) const;

		friend ostream& operator<<(ostream& os, Vector2& v)
		{
			return os << " (" << v.x << "," << v.y << ") ";
		}
	};

	// class to handle rect for drawing and collision (subjected to change)
	class Rect
	{
	public:
		Vector2 size; // size of the rectangle
		Vector2 center; // center of rect

		// constructors
		Rect();
		Rect(Vector2 _size, Vector2 _center);

		void set(Vector2 _size, Vector2 _center);// setter method

		Vector2 get_topleft() const;// returns top left position of rect (subjected to change and replace it with bounds)

		Vector2 get_botmright() const;// returns bottom right position of rect (subjected to change and replace it with bounds)

		void set_topleft(Vector2& value);// setter for topleft

		void set_botmright(Vector2& value);// setter for bottomright

		void clamp(const Vector2& min, const Vector2& max);// clamps the rect in max and min bounds

		// override outstreasm operator for output
		friend ostream& operator<<(ostream& os, Rect& rect)
		{
			Vector2 topLeft = rect.get_topleft();
			Vector2 botmRight = rect.get_botmright();
			return os << "[ " << topLeft.x << " , " << topLeft.y << " , " << botmRight.x << " , " << botmRight.y << " ] ";
		}
	};

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

		static void Init(Window& _win);// initializes value of window

		static void free();// deinitializes value of window

		static void refresh();

		static int get_h_axis();// returns horizontal axis input

		static int get_v_axis();// returns vertical axis input

		static Vector2 get_axis();// returns vector2 axis for player input

		static Vector2 get_mouse_pos();// returns mouse position on window

		static bool key_pressed(int key);// checks if given key is pressed or not

		static bool ui_back();// checks if backspace or escape is pressed for UI inputs
	};

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

	class Collision
	{
	public:
		static bool circle_collide(const Rect& a, const Rect& b);
		static bool rect_collide(const Rect& a, const Rect& b);
	};

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
	};

	// camera class to handle camera movement and rendering 
	static class Camera
	{
		static Window win; // canvas to print on (canvas size is cameras size)
		static Rect* followTarget; // follow target for camera
		static Vector2 offset; // offset value to keep (0,0) in center of canvas
		static Image background;

		Camera() {};
	public:
		static bool notNull; // to check if camera is not created
		static Rect camRect; // camera rect 

		// create new camera
		static void create(std::string _name, Vector2 _size, Vector2 _pos = Vector2::zero, Color _bg = Color::BLACK);
		// destroy camera
		static void free();
		// get referance to window
		static Window& get_window();
		// returns screen position
		static Vector2 world_to_screen(Vector2 _pos);
		// returns world position 
		static Vector2 screen_to_world(Vector2 _pos);
		// check if follow target is assigned or not
		static bool has_follow_target();
		// setter for follow target (does not work if window is changed i.e. zoom value changes)
		static void set_follow_target(Rect& rect);
		// setter for gackground color (does not work if window is changed i.e. zoom value changes)
		static void set_bg_color(Color& _color);
		// resets follow target
		static void reset_follow_target();
		// updates cameras if follow follow target set
		static void update(float dt);
		// clear canvas
		static void clear();
		// draw object on canvas if in view
		static void draw(Rect& _rect, Image& _image);
		// present canvas
		static void present();
	};

	// class to handle sprites 
	class Sprite
	{
		int id; // object id to identify the object
	protected:

		Image image; // sprite image as array of colors (sbjected to change and replace with Image object)

		// constructors
		Sprite();
		Sprite(Vector2 _size, Vector2 _pos, Color _color);
		Sprite(Vector2 _pos, std::string _location, bool _center = true);

	public:
		// rect for the sprite to draw and collide
		Rect rect;
		//method to update sprite 
		virtual void update(float dt) {}
		// method to draw sprite on window
		void draw();
		// destructor
		~Sprite();
	};

	// class allows to group Sprites together
	class SpriteGroup
	{
		Sprite** group = nullptr; // sprite group pointer
		int curIndex; // index of last element (subjected to change)
		int maxSize; // max size for the group (subjected to change after changing with dynamic data structure like vector)
	public:
		// constructor
		SpriteGroup(int _maxSize);
		// getter for curSize
		int get_size();
		// getter for i'th element
		Sprite* get_sprite(int i);
		// method to add sprite to the group
		void add(Sprite* sprite);
		// method to add sprites of given group to this group
		void add(SpriteGroup* _group);
		// method to update all sprites in this group
		void update(float dt);
		// method to draw all sprites of this group
		void draw();
		// checks collision of given sprite with all sprites
		bool is_colliding(Rect& rect);
		// method to destroy all sprites and this group
		void destroy();

		// destructor
		~SpriteGroup()
		{
			destroy();
		}
	};

}
