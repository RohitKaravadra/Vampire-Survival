#include "Engine.h"

using namespace Engine;

extern const Vector2 WIN_SIZE;

Vector2 get_grid_pos(Vector2 _pos, int _size, int _space = 0)
{
	float tSize = _size + _space;
	Vector2 gPos = (_pos / tSize).to_int() * tSize;

	gPos.x += _pos.x < 0 ? -tSize / 2 : tSize / 2;
	gPos.y += _pos.y < 0 ? -tSize / 2 : tSize / 2;

	return gPos;
}

class TileHighlight :public Sprite
{
public:
	TileHighlight(Vector2 _size, Vector2 _pos) :Sprite(_size, _pos)
	{
		image.width = _size.x;
		image.height = _size.y;
		image.channels = 4;
		create_outline(image, Color::WHITE, 2);
	}

	void update(float dt)
	{
		rect.set_center(get_grid_pos(Camera::screen_to_world(Inputs::get_mouse_pos()), 128));
	}
};

class MapEditor : public App
{
	bool isRunning;
	float moveSpeed = 200;
	TileHighlight* indicator;
public:
	MapEditor(string _name, Vector2 _size) :App(_name, _size)
	{
		isRunning = false;
	}

	~MapEditor()
	{
		destroy();
	}

	void start()
	{
		indicator = new TileHighlight(Vector2(128), Vector2::zero);
		update_loop();
	}

	void destroy()
	{
		delete indicator;
	}

	void update_loop()
	{
		isRunning = true;
		while (isRunning)
		{
			App::update();

			indicator->update(deltaTime);

			Camera::camRect.move(Inputs::get_axis() * deltaTime * moveSpeed);

			if (Inputs::key_pressed('E'))
				moveSpeed += 1000 * deltaTime;
			if (Inputs::key_pressed('Q'))
				moveSpeed -= 1000 * deltaTime;

			Camera::clear();

			indicator->draw();

			Camera::present();

			if (Inputs::ui_back())
				isRunning = false;
		}
	}
};

void map_editor()
{
	MapEditor app("Map Editor", WIN_SIZE);
	app.start();
}