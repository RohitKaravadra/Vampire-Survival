#include "Engine.h" 

using namespace Engine;

extern const Vector2 WIN_SIZE(1280, 720);

class Tile : public Sprite
{
public:
	Tile(string _location, Vector2 _pos) :Sprite(_location, _pos) {}
	Tile(float _rad, Vector2 _pos, Color _color) :Sprite(_rad, _pos, _color) {}
	Tile(Vector2 _size, Vector2 _pos) :Sprite(_size, _pos) {}
	Tile(Vector2 _size, Vector2 _pos, Color _color) :Sprite(_size, _pos, _color) {}
};

SpriteGroup* walls;
SpriteGroup* ground;

class Player : public Sprite
{
	float speed = 400;
public:
	Player(string _location, Vector2 _pos) :Sprite(_location, _pos) {}
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
		if (walls->is_colliding(rect))
			rect.move_h(-delta.x);

		// check and resolve y axis collision
		rect.move_v(delta.y);
		if (walls->is_colliding(rect))
			rect.move_v(-delta.y);
	}
};

void create_walls(Vector2 size)
{
	// create border
	walls = new SpriteGroup(static_cast<unsigned int>(size.x * (size.y - 1) * 4));
	Vector2 pos;
	for (unsigned int y = 1; y < size.y; y++)
	{
		// left wall
		pos.set(0, 128 * y);
		walls->add(new Tile("Resources/Tiles/Brick/Brick_01-128x128.png", pos));
		// right wall
		pos += Vector2::right * 128 * size.x;
		walls->add(new Tile("Resources/Tiles/Brick/Brick_01-128x128.png", pos));
	}
	for (unsigned int x = 0; x < size.x + 1; x++)
	{
		// top wall
		pos.set(128 * x, 0);
		walls->add(new Tile("Resources/Tiles/Brick/Brick_01-128x128.png", pos));
		// bottom wall
		pos += Vector2::down * 128 * size.y;
		walls->add(new Tile("Resources/Tiles/Brick/Brick_01-128x128.png", pos));
	}
}

void fill_ground(Vector2 size)
{
	ground = new SpriteGroup(static_cast<unsigned int>((size.x - 1) * (size.y - 1)));
	for (int y = 1; y < size.y; y++)
		for (int x = 1; x < size.x; x++)
			ground->add(new Tile("Resources/Tiles/Tile/Tile_06-128x128.png", Vector2(x, y) * 128));
}

void generate_map()
{
	//"Resources/Tiles/Brick/Brick_0" + std::to_string(1 + rand() % 9) + "-128x128.png"
	Vector2 size(10, 10);

	create_walls(size);
	fill_ground(size);
}

class Game :public App
{
	bool isRunning;
	float fps;
	Player* player;

public:
	// constructor to create app and camera
	Game(string _name, Vector2 _size, Vector2 _camPos = Vector2::zero) :App(_name, _size, _camPos)
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
		player = new Player(Vector2(50), Vector2(400, 400), Color::GREEN);
		Camera::set_follow_target(player->rect);
		generate_map();

		// starting game loop
		update_loop();
	}

	void destroy()
	{
		std::cout << "Average FPS : " << fps << std::endl;
		delete player, walls, ground;
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
			walls->update(deltaTime);

			Camera::update(deltaTime);
			Camera::clear();

			// draw all objects on screen
			ground->draw();
			walls->draw();
			player->draw();

			// only to debug
			if (DEBUG_MODE)
			{
				ground->debug();
				walls->debug();
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
	srand(234);

	DEBUG_MODE = true;
	Color::DEBUG_COLOR = Color::RED;

	Game app("Vampire Survival", WIN_SIZE);
	app.start();
}