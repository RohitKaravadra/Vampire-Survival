#include "Engine.h" 

using namespace Engine;

const Vector2 WIN_SIZE(1280, 720);

class Platform : public Sprite
{
public:
	Platform(Vector2 _pos, std::string location) :Sprite(_pos, location) {}
	Platform(float _rad, Vector2 _pos, Color _color) :Sprite(_rad, _pos, _color) {}
	Platform(Vector2 _size, Vector2 _pos, Color _color) :Sprite(_size, _pos, _color) {}
};

SpriteGroup* platforms;

class Player : public Sprite
{
	float speed = 400;
public:
	Player(Vector2 _pos, std::string location) :Sprite(_pos, location) {}
	Player(float _rad, Vector2 _pos, Color _color) :Sprite(_rad, _pos, _color) {}
	Player(Vector2 _size, Vector2 _pos, Color _color) :Sprite(_size, _pos, _color) {}

	void update(float dt)
	{
		if (Inputs::key_pressed('E'))
			speed += 10;
		if (Inputs::key_pressed('Q'))
			speed -= 10;

		move_and_collide(dt);
	}

	void move(float dt)
	{
		Vector2 delta = Inputs::get_axis() * dt * speed;
		rect.move(delta);
	}

	void move_and_collide(float dt)
	{
		Vector2 delta = Inputs::get_axis() * dt * speed;

		// check and resolve x axis collision
		rect.move_h(delta.x);
		if (platforms->is_colliding(rect))
			rect.move_h(-delta.x);

		// check and resolve y axis collision
		rect.move_v(delta.y);
		if (platforms->is_colliding(rect))
			rect.move_v(-delta.y);
	}
};

class App
{
	bool isRunning;
	Timer timer;
	float deltaTime;
	float fps;

	Player* player;

public:
	// constructor to create app and camera
	App(std::string _name, Vector2 _size, Vector2 _camPos = Vector2::zero)
	{
		Camera::create(_name, _size, _camPos);
		Inputs::Init(Camera::get_window());

		isRunning = false;
		deltaTime = 1;
		fps = 0;
	}

	// destructor
	~App()
	{
		destroy();
	}

	// start app and create objects
	void start()
	{
		// creating objects
		player = new Player(Vector2(0, 0), "Resources/L.png");
		Camera::set_follow_target(player->rect);

		// starting game loop
		isRunning = true;
		update_loop();
	}

	void destroy()
	{
		std::cout << "Average FPS : " << fps << std::endl;
		delete player;
		Camera::destroy();
		Inputs::destroy();
	}

	void update_loop()
	{
		timer.reset();
		while (isRunning)
		{
			Inputs::refresh();
			deltaTime = static_cast<float>(timer.dt());
			fps = (fps + 1 / deltaTime) / 2;

			// update all objects
			player->update(deltaTime);
			platforms->update(deltaTime);

			Camera::update(deltaTime);
			Camera::clear();

			// draw all objects on screen
			platforms->draw();
			player->draw();

			// only to debug
			if (DEBUG_MODE)
			{
				platforms->debug();
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
	srand(static_cast<unsigned int>(time(NULL)));
	DEBUG_MODE = true;
	Color::DEBUG_COLOR = Color::RED;
	App app("Vampire Survival", WIN_SIZE);

	Vector2 size(10, 10);
	platforms = new SpriteGroup(static_cast<unsigned int>(size.x * size.y));
	for (unsigned int y = 0; y < size.y - 1; y++)
		platforms->add(new Platform(Vector2(128 * -size.x / 2, 128 * ((int)y - size.y / 2 + 2)), "Resources/Tiles/Brick/Brick_0" + std::to_string(1 + rand() % 9) + "-128x128.png"));
	for (unsigned int x = 0; x < size.x; x++)
		platforms->add(new Platform(Vector2(128 * ((int)x - size.x / 2), 128 * -size.y / 2), "Resources/Tiles/Brick/Brick_0" + std::to_string(1 + rand() % 9) + "-128x128.png"));
	app.start();

	delete platforms;
}