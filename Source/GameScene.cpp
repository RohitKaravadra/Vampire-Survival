#include "Engine.h" 
#include "Resources.h"
#include "SceneManagement.h"

using namespace Engine;

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
	Player() = default;
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
		move(delta);
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

class GameScene :public Scene
{
	Player* player;

public:
	GameScene()
	{
		name = "Game";
	}

	~GameScene()
	{
		std::cout << name << " destroyed" << std::endl;
		destroy();
	}

	// start app and create objects
	void start()
	{
		// creating objects
		isActive = true;
		level = new Level();
		player = new Player("Resources/L.png", Vector2(0));
		Camera::set_follow_target(player->rect);
		std::cout << name << " started" << std::endl;
	}

	void destroy()
	{
		isActive = false;

		delete player, level;
		player = nullptr;
		level = nullptr;

		std::cout << name << " destroyed" << std::endl;
	}

	bool update(float dt)
	{
		// update all objects
		player->update(dt);
		Camera::update(dt);

		if (Inputs::ui_back())
			return true;
		return false;
	}

	void draw()
	{
		level->draw();
		player->draw();
	}

	void debug()
	{
		level->debug();
		player->debug();
	}
};

Scene* create_game_scene()
{
	return new GameScene();
}