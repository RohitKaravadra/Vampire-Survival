
#include "Engine.h" 

using namespace Engine;

Vector2 minBound = Vector2(25, 25);
Vector2 maxBound = Vector2(1255, 695);

class Enemy : public Sprite
{
public:
	Enemy(float _rad, Vector2 _pos, Color _color) :Sprite(_rad, _pos, _color) {}
};

//class Sworm
//{
//	Enemy** list;
//	int curSize = 0;
//	float eTime = 0;
//	float gTime = 0.5f;
//	float delta = 0.1f;
//	int size = 100;
//
//public:
//	Sworm()
//	{
//		list = new Enemy * [size];
//		create_enemy();
//	}
//
//	void create_enemy()
//	{
//		if (curSize < size - 1)
//		{
//			list[curSize++] = new Enemy(25.f, Vector2(25 + rand() % 1000, 50), RED);
//			//print_msg("Enemy Created");
//		}
//	}
//
//	void destroy_enemy(int index)
//	{
//		if (list[index] != nullptr)
//		{
//			Enemy* n = list[index];
//			list[index] = list[--curSize];
//			list[curSize + 1] = nullptr;
//			n->destroy();
//			//print_msg("Enemy Destroyed");
//		}
//	}
//
//	void update(float dt)
//	{
//		Vector2 dir(0, 1);
//
//		for (unsigned int i = 0; i < curSize; i++)
//		{
//			list[i]->update(dt);
//			if (list[i]->rect.center.y > 720)
//			{
//				destroy_enemy(i);
//				i--;
//			}
//		}
//
//		eTime += dt;
//		if (eTime > gTime)
//		{
//			create_enemy();
//			eTime = 0;
//			if (gTime > 0.1f)
//				gTime -= dt;
//			else
//				gTime = 0.1f;
//		}
//	}
//
//	void Draw(GamesEngineeringBase::Window& win)
//	{
//		for (int i = 0; i < curSize; i++)
//		{
//			list[i]->draw(win);
//		}
//	}
//
//	void print_msg(std::string msg)
//	{
//		std::cout << msg << std::endl;
//	}
//
//	~Sworm()
//	{
//		for (unsigned int i = 0; i < curSize; i++)
//		{
//			list[i]->destroy();
//			//print_msg("Enemy Destroyed");
//		}
//
//		delete[] list;
//	}
//};

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
	SpriteGroup* Objects;
public:

	App()
	{
		isRunning = false;
		deltaTime = 1;

		win.create(1280, 720, "Vampire Survival");
		Inputs::Init(win);
		Objects = new SpriteGroup(10);
	}

	void start()
	{
		srand(static_cast<unsigned int>(time(NULL)));

		Objects->add(new Enemy(25.f, Vector2(500, 400), RED));
		Objects->add(new Player(25.0f, Vector2(600, 700), GREEN));

		isRunning = true;
		update_loop();
	}

	~App()
	{
		destroy();
	}

	void destroy()
	{
		Inputs::free();
		delete Objects;
	}

	void update_loop()
	{
		while (isRunning)
		{
			deltaTime = static_cast<float>(timer.dt());

			Objects->update(deltaTime);

			win.clear();

			fill_window(win, WHITE);

			Objects->draw(win);

			win.present();

			if (win.keyPressed(VK_ESCAPE))
				isRunning = false;

			//std::cout << deltaTime << std::endl;
		}
	}
};

int main()
{
	App* app = new App();
	app->start();
	delete app;

	return 0;
}