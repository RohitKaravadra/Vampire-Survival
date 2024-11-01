#include "Engine.h"

using namespace Engine;

App::App(std::string _name, Vector2 _size, Vector2 _camPos, Color _bg)
{
	isRunning = false;
	dt = 1;
	eTime = 100;

	Camera::create(_name, _size, _camPos, _bg);
	Inputs::Init(Camera::get_window());
}

App::~App()
{
	destroy();
}

void App::start()
{
	isRunning = true;
	update_loop();
}

void App::destroy()
{
	Inputs::free();
	delete gameObjects;
	gameObjects = nullptr;
}

void App::update_loop()
{
	while (isRunning)
	{
		Inputs::refresh();
		dt = static_cast<float>(timer.dt());
		eTime += dt;

		gameObjects->update(dt);

		Camera::update(dt);
		Camera::clear();

		gameObjects->draw();
		if (DEBUG_MODE)
			gameObjects->debug();

		Camera::present();

		if (Inputs::ui_back())
			break;
	}
}