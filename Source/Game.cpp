
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

int main()
{
	srand(static_cast<unsigned int>(time(NULL)));
	App app("Vampire Survival", WIN_SIZE, Vector2::zero, Color::OLIVE);

	Player* player = new Player(Vector2(0, 0), "Resources/L.png");

	platforms = new SpriteGroup(3);
	platforms->add(new Enemy(Vector2(WIN_SIZE.x, 30), Vector2(0, WIN_SIZE.y / 2), Color::RED));
	platforms->add(new Enemy(Vector2(400, 30), Vector2(-300, 200), Color::RED));
	platforms->add(new Enemy(Vector2(400, 30), Vector2(300, 200), Color::RED));

	app.gameObjects = new SpriteGroup(10);

	app.gameObjects->add(platforms);
	app.gameObjects->add(player);

	//Camera::set_follow_target(player->rect);

	app.gameObjects->remove(*player);

	app.start();

	delete platforms, player;

	return 0;
}