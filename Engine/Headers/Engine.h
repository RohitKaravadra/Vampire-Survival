#pragma once

#include "Utilities.h"
#include "Collisions.h"
#include "GamesEngineeringBase.h"
#include <iostream>

using namespace GamesEngineeringBase;
using Utilities::Dictionary;
using Utilities::Pair;

static int OBJECT_ID_COUNTER = 0;
extern bool DEBUG_MODE;

// generate id for objects created
int generate_id();

namespace Engine
{
	// create rectangular outline in an image
	void create_rect_outline(Image& _image, Color _color, int _width = 1);
	// create circular outline in an image
	void create_circle_outline(Image& _image, Color _color, int _width = 1);
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
		// returns the mouse button values
		static bool mouse_button(MouseButton _button);
		// returns scroll wheel value
		static int mouse_wheel();
		// checks if given key is pressed or not
		static bool key_pressed(int key);
		// check if accept is precced
		static bool ui_accept();
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
		// draw ui on canvas
		static void draw_ui(Vector2 _pos, Image& _image);
		// present canvas
		static void present();
	};

	// class to handle sprites 
	class Sprite :public Collider
	{
	protected:
		Image image; // sprite image
		Image dbgImage; // rect image for debugging collider

		// constructors
		Sprite() = default;
		Sprite(Vector2 _size, Vector2 _pos);
		Sprite(Vector2 _size, Vector2 _pos, Color _color);
		Sprite(std::string _location, Vector2 _pos, bool _center = true);

		// create an outline rect for debugging
		void create_debug_data();
	public:
		//method to update sprite 
		virtual void update(float dt) {}
		// method to draw sprite on window
		virtual void draw();
		// function to debug
		virtual void debug();
		// destructor
		virtual ~Sprite() = default;

		bool operator==(Sprite& other) const;
		bool operator==(Sprite* other) const;
		bool operator!=(Sprite& other) const;
		bool operator!=(Sprite* other) const;
	};

	// sample App class for easy build up
	class App
	{
	protected:
		// constructor to create an app
		App(std::string _name, Vector2 _size, Vector2 _camPos = Vector2::zero)
		{
			Camera::create(_name, _size, _camPos);
			Inputs::Init(Camera::get_window());
		}
		// destructor to destroy app
		~App()
		{
			Collisions::destroy();
			Inputs::destroy();
			Camera::destroy();
		}
	};
}

namespace UI
{
	// class creates a fill type ui image 
	class FillBar
	{
		Image image;
		Rect rect;
		float val = 1; // current value of fill
		Engine::Color fColor, bColor; // foreground and background color

		// method to fills the image
		void fill();
	public:
		// method to create image
		void create(Vector2, Vector2, Engine::Color, Engine::Color = Engine::Color::BLACK, float = 1);
		// method to set value of image fill
		void set_value(float);
		// method to set image position
		void set_pos(Vector2);
		// method to draw image relative to camera
		void draw();
		// method to draw image as ui
		void draw_ui();
	};
}