
#include "Engine.h" 

using namespace Engine;

const Vector2 WIN_SIZE(1280, 720);

class Enemy : public Sprite
{
public:
	Enemy(float _rad, Vector2 _pos, Color _color) :Sprite(_rad, _pos, _color) {}
	Enemy(Vector2 _size, Vector2 _pos, Color _color) :Sprite(_size, _pos, _color) {}
};

SpriteGroup* platforms;

class Player : public Sprite
{
	float speed = 400;
	float gravity = 0;
	bool isGrounded;
public:
	Player(Vector2 _pos, std::string location) :Sprite(_pos, location) {}
	Player(float _rad, Vector2 _pos, Color _color) :Sprite(_rad, _pos, _color) {}
	Player(Vector2 _size, Vector2 _pos, Color _color) :Sprite(_size, _pos, _color) {}

	void update(float dt) {
		move(dt);
	}

	void move(float dt)
	{
		float delta = Inputs::get_h_axis() * speed * dt;

		// check and resolve x axis collision
		rect.center.x += delta;
		if (platforms->is_colliding(rect))
			rect.center.x -= delta;

		// check and resolve y axis collision
		if (isGrounded && Inputs::key_pressed(VK_SPACE))
			gravity = -2;
		else
			gravity += 9 * dt;

		rect.center.y += gravity;
		if (platforms->is_colliding(rect))
		{
			if (gravity >= 0)
				isGrounded = true;
			rect.center.y -= gravity;
			gravity = 0;
		}
		else
			isGrounded = false;
	}
};

class App
{
	bool isRunning;
	Timer timer;
	float dt;

	Sprite* player;
public:

	App()
	{
		isRunning = false;
		dt = 1;

		player = nullptr;
		platforms = new SpriteGroup(5);

		Camera::create("Vampire Survival", WIN_SIZE, Vector2::zero, Color::OLIVE);
		Inputs::Init(Camera::get_window());
	}

	~App()
	{
		destroy();
	}

	void start()
	{
		srand(static_cast<unsigned int>(time(NULL)));

		player = new Player(Vector2(0, 0), "Resources/L.png");

		platforms->add(new Enemy(Vector2(WIN_SIZE.x, 30), Vector2(0, WIN_SIZE.y / 2), Color::RED));
		platforms->add(new Enemy(Vector2(400, 30), Vector2(-300, 200), Color::RED));
		platforms->add(new Enemy(Vector2(400, 30), Vector2(300, 200), Color::RED));

		Camera::set_follow_target(player->rect);

		isRunning = true;
		update_loop();
	}

	void destroy()
	{
		Inputs::free();
		delete platforms, player;
	}

	void update_loop()
	{
		while (isRunning)
		{
			Inputs::refresh();
			dt = static_cast<float>(timer.dt());

			platforms->update(dt);
			player->update(dt);

			Camera::update(dt);
			Camera::clear();

			platforms->draw();
			player->draw();

			Camera::present();

			if (Inputs::ui_back())
				isRunning = false;

			/*Vector2 mouse(Camera::screen_to_world(Inputs::get_mouse_pos()));
			std::cout << mouse << std::endl;

			std::cout << dt << std::endl;*/
		}
	}
};

int main()
{
	srand(static_cast<unsigned int>(time(NULL)));
	App* app = new App();
	app->start();
	delete app;

	return 0;
}