
#include "Engine.h" 

using namespace Engine;

const Vector2 winSize(1280, 720);

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
	Player(Vector2 _pos, std::string location) :Sprite(_pos, location) {}
	Player(float _rad, Vector2 _pos, Color _color) :Sprite(_rad, _pos, _color) {}
	Player(Vector2 _size, Vector2 _pos, Color _color) :Sprite(_size, _pos, _color) {}

	void update(float dt) {
		move(dt);
	}

	void move(float dt)
	{
		Vector2 delta = Inputs::get_axis() * 200 * dt;

		// check and resolve x axis collision
		rect.center += Vector2(delta.x, 0);
		if (Collision::rect_collide(rect, enemy->rect))
			rect.center -= Vector2(delta.x, 0);

		// check and resolve y axis collision
		rect.center += Vector2(0, delta.y);
		if (Collision::rect_collide(rect, enemy->rect))
			rect.center -= Vector2(0, delta.y);
	}
};

class App
{
	bool isRunning;
	Camera* cam;
	Timer timer;
	float dt;

	Sprite* player;

	SpriteGroup* objects;
public:

	App()
	{
		isRunning = false;
		dt = 1;

		cam = new Camera("Vampire Survival", winSize, Vector2::zero);
		objects = new SpriteGroup(2);

		Inputs::Init(cam->get_window());
	}

	~App()
	{
		destroy();
	}

	void start()
	{
		srand(static_cast<unsigned int>(time(NULL)));

		enemy = new Enemy(Vector2(20), Vector2(300, 300), RED);
		player = new Player(Vector2(0, 0), "Resources/L.png");

		objects->add(enemy);
		objects->add(player);

		cam->set_follow_target(player->rect);

		isRunning = true;
		update_loop();
	}


	void destroy()
	{
		Inputs::free();
		delete objects, cam;
	}

	void update_loop()
	{
		while (isRunning)
		{
			dt = static_cast<float>(timer.dt());

			objects->update(dt);
			cam->update(dt);

			cam->clear();

			fill_window(cam->get_window(), WHITE);

			objects->draw(*cam);

			cam->present();

			if (Inputs::ui_back())
				isRunning = false;

			if (Inputs::key_pressed(VK_SPACE))
			{
				if (cam->has_follow_target())
					cam->reset_follow_target();
				else
					cam->set_follow_target(player->rect);
			}

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