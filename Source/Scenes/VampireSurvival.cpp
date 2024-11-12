#include <iostream>
#include "Engine.h"
#include "SceneManagement.h"
#include "Resources.h"

using namespace Engine;
using namespace std;

const Vector2 WIN_SIZE(1024, 768);

// functions declaration for getting scenes
Scene* create_level_1_scene();
Scene* create_level_2_scene();
Scene* create_editor_scene();


// pointer for main menu (for scene selection) 
class Pointer :public Sprite
{
	float inpFreq;
	int value;
	Image menuImage;

	void set_pos() { rect.set_center(Vector2(0, value * rect.size.y + value * 40 - 190)); }

public:

	Pointer() :Sprite(Vector2(350, 70), Vector2(0))
	{
		load_image(menuImage, "Resources/Menu.png");
		create_rect_outline(image, Color::YELLOW, 10);
		reset();
	}

	void reset()
	{
		value = 0;
		inpFreq = 0.2f;
		set_pos();
	}

	void update(float dt)
	{
		if (inpFreq > 0)
			inpFreq -= dt;

		if (inpFreq <= 0)
		{
			float axis = Inputs::get_v_axis();
			if (axis == 0)
				inpFreq = 0;
			else
			{
				value += axis > 0 ? 1 : -1;
				value = min(max(value, 0), 3);
				set_pos();
				inpFreq = 0.2f;
			}
		}
	}

	void draw()
	{
		Camera::draw_ui(Vector2(0), menuImage);
		Sprite::draw();
	}

	int get_value() const { return value; }
};

// Menu
class MainMenu : public Scene
{
	Pointer pointer; // gives to selected scene
	float inpFreq; // for keeping inputs in a certain frequency

	string curScene; // keeps track of current scene
	SceneManager<3> sceneManager; // scene manager to change scenes
public:
	MainMenu()
	{
		name = "Menu";

		float inpFreq = 0;
		// adding scens to scene manager
		sceneManager.add(create_level_1_scene());
		sceneManager.add(create_level_2_scene());
		sceneManager.add(create_editor_scene());
	}

	void start()
	{
		inpFreq = 0;

		// start update loop
		update_loop();
	}

	void update(float dt)
	{
		pointer.update(dt);

		if (inpFreq > 0)
			inpFreq -= dt;

		if (Inputs::ui_accept() && inpFreq <= 0)
		{
			int ch = pointer.get_value();
			inpFreq = 0.2f;

			// check for scene change
			switch (ch)
			{
			case 0:curScene = sceneManager.change_scene("Level1Scene");
				break;
			case 1:curScene = sceneManager.change_scene("Level2Scene");
				break;
			case 2:curScene = sceneManager.change_scene("MapEditor");
				break;
			case 3: isActive = false;
				break;
			default:inpFreq = 0;
			}

			pointer.reset();
		}
	}

	void draw() { pointer.draw(); }
};

class Game :public App
{

	MainMenu menuScene;
public:
	// constructor to create app and camera
	Game(std::string _name, Vector2 _size, Vector2 _camPos = Vector2::zero) :App(_name, _size, _camPos) {}

	// start app and create objects
	void start()
	{
		isActive = true;
		menuScene.start();
		isActive = false;
	}
};

int main()
{
	// dont enable it (just for debugging)
	DEBUG_MODE = false;
	srand(static_cast<unsigned int>(time(NULL)));

	Game app("Vapmire Survival", WIN_SIZE);
	app.start();

	return 0;
}