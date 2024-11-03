#include "Engine.h" 
#include "Utilities.h"

using namespace Engine;

extern const Vector2 WIN_SIZE(1280, 720);

void load_level(Dictionary<Vector2, int>& _level);

class Level : public TileMap
{
public:
	Level() :TileMap(32)
	{
		totalTiles = 24;
		tiles = new Image[totalTiles];
		for (unsigned int i = 0; i < totalTiles; i++)
			load_image(tiles[i], "Resources/Tiles/" + std::to_string(i) + ".png");

		load_level(data);
		size = data.get_size();
	}
};

Level* level;

class Player : public Sprite
{
	float speed = 400;
public:
	Player(std::string _location, Vector2 _pos) :Sprite(_location, _pos) {}
	Player(float _rad, Vector2 _pos, Color _color) :Sprite(_rad, _pos, _color) {}
	Player(Vector2 _size, Vector2 _pos) :Sprite(_size, _pos) {}
	Player(Vector2 _size, Vector2 _pos, Color _color) :Sprite(_size, _pos, _color) {}

	void update(float dt)
	{
		if (Inputs::key_pressed('E'))
			speed += 1000 * dt;
		if (Inputs::key_pressed('Q'))
			speed -= 1000 * dt;

		Vector2 delta = Inputs::get_axis() * dt * speed;
		move_and_collide(delta);
	}

	void move(Vector2 delta)
	{
		rect.move(delta);
	}

	void move_and_collide(Vector2 delta)
	{
		// check and resolve x axis collision
		rect.move_h(delta.x);
		if (level->is_colliding(rect))
			rect.move_h(-delta.x);

		// check and resolve y axis collision
		rect.move_v(delta.y);
		if (level->is_colliding(rect))
			rect.move_v(-delta.y);
	}
};

class Game :public App
{
	bool isRunning;
	float fps;
	Player* player;

public:
	// constructor to create app and camera
	Game(std::string _name, Vector2 _size, Vector2 _camPos = Vector2::zero) :App(_name, _size, _camPos)
	{
		isRunning = false;
		fps = 0;
	}

	// destructor
	~Game()
	{
		destroy();
	}

	// start app and create objects
	void start()
	{
		// creating objects
		level = new Level();
		player = new Player(Vector2(50), Vector2(0), Color::GREEN);
		Camera::set_follow_target(player->rect);

		// starting game loop
		update_loop();
	}

	void destroy()
	{
		std::cout << "Average FPS : " << fps << std::endl;
		delete player, level;
	}

	void update_loop()
	{
		isRunning = true;
		while (isRunning)
		{
			App::update();
			fps = (fps + 1 / deltaTime) / 2;

			// update all objects
			player->update(deltaTime);

			Camera::update(deltaTime);
			Camera::clear();

			// draw all objects on screen
			level->draw();
			player->draw();

			// only to debug
			if (DEBUG_MODE)
			{
				level->debug();
				player->debug();
			}

			Camera::present();

			if (Inputs::ui_back())
				isRunning = false;
		}
	}
};

void game()
{
	DEBUG_MODE = true;
	Game app("Vampire Survival", WIN_SIZE);
	app.start();
}