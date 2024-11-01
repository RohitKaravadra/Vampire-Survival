
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
public:
	Player(Vector2 _pos, std::string location) :Sprite(_pos, location) {}
	Player(float _rad, Vector2 _pos, Color _color) :Sprite(_rad, _pos, _color) {}
	Player(Vector2 _size, Vector2 _pos, Color _color) :Sprite(_size, _pos, _color) {}

	void update(float dt) {
		move(dt);
	}

	void move(float dt)
	{
		Vector2 delta = Inputs::get_axis() * dt * speed;

		// check and resolve x axis collision
		rect.center.x += delta.x;
		if (platforms->is_colliding(rect))
			rect.center.x -= delta.x;

		// check and resolve y axis collision
		rect.center.y += delta.y;
		if (platforms->is_colliding(rect))
			rect.center.y -= delta.y;
	}
};

int main()
{
	srand(static_cast<unsigned int>(time(NULL)));
	DEBUG_MODE = true;
	App app("Vampire Survival", WIN_SIZE);

	Player* player = new Player(Vector2(0, 0), "Resources/L.png");

	platforms = new SpriteGroup(3);
	platforms->add(new Enemy(Vector2(WIN_SIZE.x, 30), Vector2(0, WIN_SIZE.y / 2), Color::RED));
	platforms->add(new Enemy(Vector2(400, 30), Vector2(-300, 200), Color::RED));
	platforms->add(new Enemy(Vector2(400, 30), Vector2(300, 200), Color::RED));

	app.gameObjects = new SpriteGroup(10);
	app.gameObjects->add(platforms);
	app.gameObjects->add(player);

	Camera::set_follow_target(player->rect);

	//app.gameObjects->remove(*player);

	app.start();

	delete platforms, player;

	return 0;
}