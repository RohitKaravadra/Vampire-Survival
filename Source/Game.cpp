
#include "Engine.h" 

using namespace Engine;

Vector2 minBound = Vector2(25, 25);
Vector2 maxBound = Vector2(1255, 695);

class Enemy : public Sprite
{
public:
	Enemy(float _rad, Vector2 _pos, Color _color) :Sprite(_rad, _pos, _color) {}
	Enemy(Vector2 _size, Vector2 _pos, Color _color) :Sprite(_size, _pos, _color) {}
};


Sprite* enemy;

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
	Player(float _rad, Vector2 _pos, Color _color) :Sprite(_rad, _pos, _color) {}
	Player(Vector2 _size, Vector2 _pos, Color _color) :Sprite(_size, _pos, _color) {}

	void update(float dt) {
		move(dt);
	}

	void move(float dt)
	{
		Vector2 delta = Inputs::get_axis() * 200 * dt;

		rect.center += Vector2(delta.x, 0);
		if (Collision::rect_collide(rect, enemy->rect))
			rect.center -= Vector2(delta.x, 0);
		rect.center += Vector2(0, delta.y);
		if (Collision::rect_collide(rect, enemy->rect))
			rect.center -= Vector2(0, delta.y);

		rect.center = rect.center.clamp(minBound, maxBound);
	}
};

class App
{
	bool isRunning;
	Window win;
	Timer timer;
	float deltaTime;

	Sprite* player;

	//SpriteGroup* objects;
public:

	App()
	{
		isRunning = false;
		win.create(1280, 720, "Vampire Survival");
		deltaTime = 1;
		player = nullptr;

		Inputs::Init(win);
		//objects = new SpriteGroup(10);
	}

	~App()
	{
		destroy();
	}

	void start()
	{
		srand(static_cast<unsigned int>(time(NULL)));

		/*objects->add(new Enemy(Vector2(50), Vector2(500, 400), RED));
		objects->add(new Player(Vector2(50), Vector2(600, 700), GREEN));*/

		enemy = new Enemy(50.f, Vector2(500, 400), RED);
		player = new Player(25.f, Vector2(600, 700), GREEN);

		isRunning = true;
		update_loop();
	}


	void destroy()
	{
		Inputs::free();
		delete player, enemy;
		//delete objects;
	}

	void update_loop()
	{
		while (isRunning)
		{
			deltaTime = static_cast<float>(timer.dt());

			//objects->update(deltaTime);
			player->update(deltaTime);

			win.clear();

			//fill_window(win, WHITE);

			//objects->draw(win);
			player->draw(win);
			enemy->draw(win);

			win.present();

			if (Inputs::ui_back())
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