#include "Engine.h"


Vector2 minBound = Vector2(25, 25);
Vector2 maxBound = Vector2(1255, 695);

class Enemy : public Sprite
{
	float speed;
public:
	Enemy(float _rad, Vector2 _pos, Color _color) :Sprite(_rad, _pos, _color)
	{
		speed = 200 + rand() % 100;
	}

	void update(float dt) {
		rect.center += Vector2::down * speed * dt;
	}

	void destroy()
	{
		delete this;
	}
};

class Sworm
{
	Enemy** list;
	int curSize = 0;
	float eTime = 0;
	float gTime = 0.5f;
	float delta = 0.1f;
	int size = 100;

public:
	Sworm()
	{
		list = new Enemy * [size];
		create_enemy();
	}

	void create_enemy()
	{
		if (curSize < size - 1)
		{
			list[curSize++] = new Enemy(25.f, Vector2(25 + rand() % 1000, 50), RED);
			//print_msg("Enemy Created");
		}
	}

	void destroy_enemy(int index)
	{
		if (list[index] != nullptr)
		{
			Enemy* n = list[index];
			list[index] = list[--curSize];
			list[curSize + 1] = nullptr;
			n->destroy();
			//print_msg("Enemy Destroyed");
		}
	}

	void update(float dt)
	{
		Vector2 dir(0, 1);

		for (unsigned int i = 0; i < curSize; i++)
		{
			list[i]->update(dt);
			if (list[i]->rect.center.y > 720)
			{
				destroy_enemy(i);
				i--;
			}
		}

		eTime += dt;
		if (eTime > gTime)
		{
			create_enemy();
			eTime = 0;
			if (gTime > 0.1f)
				gTime -= dt;
			else
				gTime = 0.1f;
		}
	}

	void Draw(GamesEngineeringBase::Window& win)
	{
		for (int i = 0; i < curSize; i++)
		{
			list[i]->draw(win);
		}
	}

	void print_msg(std::string msg)
	{
		std::cout << msg << std::endl;
	}

	~Sworm()
	{
		for (unsigned int i = 0; i < curSize; i++)
		{
			list[i]->destroy();
			//print_msg("Enemy Destroyed");
		}

		delete[] list;
	}
};

class Player : public Sprite
{
public:
	Player(float _rad, Vector2 _pos, Color _color) :Sprite(_rad, _pos, _color)
	{

	}

	void update(float dt) {
		move(dt);
	}

	void move(float dt)
	{
		Vector2 delta = Inputs::get_axis() * 200 * dt;
		rect.center = (rect.center + delta).clamp(minBound, maxBound);
	}
};


class App
{
	bool isRunning;
	Window win;
	Timer timer;
	float deltaTime;
	Sworm enemies;
	Player player = Player(25.0f, Vector2(600, 700), GREEN);
public:
	Sprite** actors;

	App()
	{
		isRunning = false;
		actors = new Sprite * [20];
		win.create(1280, 720, "Game");
		Inputs::Init(win);
	}

	void start()
	{
		srand(static_cast<unsigned int>(time(NULL)));

		isRunning = true;
		update_loop();
	}

	void update_loop()
	{
		while (isRunning)
		{
			deltaTime = static_cast<float>(timer.dt());

			player.update(deltaTime);
			enemies.update(deltaTime);

			win.clear();

			fill_window(win, WHITE);

			player.draw(win);
			enemies.Draw(win);

			win.present();

			if (win.keyPressed(VK_ESCAPE))
				stop();

			//std::cout << deltaTime << std::endl;
		}
	}

	void stop()
	{
		isRunning = false;
		Inputs::free();
	}

	~App()
	{
		delete[] actors;
	}
};

int main()
{
	App app;
	app.start();
}