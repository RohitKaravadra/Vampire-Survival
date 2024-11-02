#pragma once
#include "Utilities.h"
#include <math.h>
#include "GamesEngineeringBase.h"

using namespace GamesEngineeringBase;
using std::ostream;

static int OBJECT_ID_COUNTER = 0;
extern bool DEBUG_MODE;

// generate id for objects created
int generate_id();
// loads image from given location relative to project directory
bool load_image(Image& image, std::string location);

namespace Engine
{
	class Color;

	// create outline in an image
	void create_outline(Image& _image, Color _color, int _width = 1);
	// fill image with color
	void fill_image(Image& _image, Color _color);

	//// class for loading resources
	//static class Resources
	//{
	//	static Dictionary<string, Image> image_collection;
	//	static int load_image(string location);
	//public:
	//	static Image& get_image(string location);
	//};

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
		float magnitude();
		// returns normalized value of this vector
		Vector2 normalize();
		// converts and return this vector into int
		Vector2 to_int();
		// returns float distance between this vector and v2
		float distance(Vector2& v2) const;
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

		inline Vector2 operator-();
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
		static void Init(Window& _win);
		// deinitializes value of window
		static void destroy();
		// refresh inputs to store values
		static void refresh();
		// returns horizontal axis input
		static int get_h_axis();
		// returns vertical axis input
		static int get_v_axis();
		// returns vector2 axis for player input
		static Vector2 get_axis();
		// returns mouse position on window
		static Vector2 get_mouse_pos();
		// checks if given key is pressed or not
		static bool key_pressed(int key);
		// checks if backspace or escape is pressed for UI inputs
		static bool ui_back();
	};

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
		static void create(std::string _name, Vector2 _size, Vector2 _pos = Vector2::zero);
		// destroy camera
		static void destroy();
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

		Image image; // sprite image
		Image dbgImage; // rect image for debugging collider

		// constructors
		Sprite();
		Sprite(Vector2 _size, Vector2 _pos);
		Sprite(Vector2 _size, Vector2 _pos, Color _color);
		Sprite(string _location, Vector2 _pos, bool _center = true);

	public:
		// rect for the sprite to draw and collide
		Rect rect;
		//method to update sprite 
		virtual void update(float dt) {}
		// method to draw sprite on window
		virtual void draw();
		// function to debug
		virtual void debug();
		// destructor
		~Sprite();

		bool operator==(Sprite& other) const;
		bool operator==(Sprite* other) const;
		bool operator!=(Sprite& other) const;
		bool operator!=(Sprite* other) const;
	};

	// class allows to group Sprites together
	class SpriteGroup
	{
		Sprite** group = nullptr; // sprite group pointer
		unsigned int curIndex; // index of last element (subjected to change)
		unsigned int maxSize; // max size for the group (subjected to change after changing with dynamic data structure like vector)
	public:
		// constructor
		SpriteGroup(unsigned int _maxSize);
		// getter for curSize
		int get_size();
		// getter for i'th element
		Sprite* get_sprite(unsigned int i);
		// method to add sprite to the group
		void add(Sprite* sprite);
		// method to add sprites of given group to this group
		void add(SpriteGroup* _group);
		// remove sprite from group
		bool remove(Sprite& _sprite);
		// remove sprite from group
		int remove(SpriteGroup& _group);
		// method to update all sprites in this group
		void update(float dt);
		// method to draw all sprites of this group
		void draw();
		// function to debug
		void debug();
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

	// Sample App class for easy build up
	class App
	{
	protected:
		Timer timer;
		float deltaTime;

		// constructor to create an app
		App(std::string _name, Vector2 _size, Vector2 _camPos = Vector2::zero)
		{
			Camera::create(_name, _size, _camPos);
			Inputs::Init(Camera::get_window());
			deltaTime = 1;
		}
		// destructor to destroy app
		~App()
		{
			Inputs::destroy();
			Camera::destroy();
		}
		// update method for app
		void update()
		{
			Inputs::refresh();
			deltaTime = timer.dt();
		}
	};
}