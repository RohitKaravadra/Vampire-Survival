#include <iostream>
#include "Engine.h"
#include "SceneManagement.h"

using namespace Engine;
using namespace std;

const Vector2 WIN_SIZE(1024, 768);

Scene* create_game_scene();
Scene* create_editor_scene();

class Pointer :public Sprite
{
	float inpFreq;
	int value;

public:
	void set_pos()
	{
		rect.set_center(Vector2(0, value * rect.size.y + value * 20 - 50));
	}
	Pointer() :Sprite()
	{
		Vector2 size(300, 50);
		image.width = size.x;
		image.height = size.y;
		image.channels = 4;
		create_outline(image, Color::AQUA, 2);
		rect.set(size, Vector2(0));

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
				value = min(max(value, 0), 2);
				set_pos();
				inpFreq = 0.2f;
			}
		}
	}

	int get_value()
	{
		return value;
	}
};

class MainMenu : public Scene
{
	Pointer pointer;
public:
	MainMenu()
	{
		name = "Menu";
	}

	~MainMenu()
	{

	}

	void start()
	{
		pointer.reset();
	}

	void destroy()
	{

	}

	bool update(float dt)
	{

		pointer.update(dt);

		if (Inputs::ui_back())
			return true;
		return false;
	}

	void draw()
	{
		pointer.draw();
	}

	int get_choice()
	{
		return pointer.get_value();
	}
};

class Game :public App
{
	bool isRunning;
	float fps;

	Scene* curScene;
	MainMenu* menuScene;
	float inpFreq;
	SceneManager sceneManager;
public:
	// constructor to create app and camera
	Game(std::string _name, Vector2 _size, Vector2 _camPos = Vector2::zero) :App(_name, _size, _camPos)
	{
		isRunning = false;
		fps = 0;
		inpFreq = 0;

		sceneManager.create(3);
		menuScene = new MainMenu();
		sceneManager.add(menuScene);
		sceneManager.add(create_game_scene());
		sceneManager.add(create_editor_scene());
	}

	// destructor
	~Game()
	{
		destroy();
		menuScene = nullptr;
	}

	// start app and create objects
	void start()
	{
		curScene = sceneManager.change_scene("Menu");
		update_loop();
	}

	void destroy()
	{
		std::cout << "Average Fps : " << fps << std::endl;
	}

	void update_loop()
	{
		isRunning = true;
		while (isRunning)
		{
			if (inpFreq > 0)
				inpFreq -= dt;

			App::update();
			if (dt > 0)
				fps = (fps + 1 / dt) / 2;

			// update all objects
			bool change = curScene->update(dt);

			Camera::clear();

			// draw all objects on screen
			curScene->draw();

			// only to debug
			if (DEBUG_MODE)
			{
				curScene->debug();
			}

			Camera::present();

			if (change && inpFreq <= 0)
			{
				inpFreq = 2;
				if (*curScene == "MapEditor")
					curScene = sceneManager.change_scene("Menu");
				else if (*curScene == "Game")
					curScene = sceneManager.change_scene("Menu");
				else
					inpFreq = 0;
			}

			if (Inputs::ui_accept())
			{
				if (*curScene == "Menu")
				{
					int ch = menuScene->get_choice();
					if (ch == 0)
						curScene = sceneManager.change_scene("Game");
					else if (ch == 1)
						curScene = sceneManager.change_scene("MapEditor");
					else if (ch == 2)
						break;
				}
			}
		}
	}
};

int main()
{
	//DEBUG_MODE = true;
	Game app("Vapmire Survival", WIN_SIZE);
	app.start();
	return 0;
}