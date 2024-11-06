
#include "Character.h"
#include "Resources.h"
#include "SceneManagement.h"

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

class Enemy : public Sprite
{
	const float speed = 200;
	float health;
	Character& player;
public:
	Enemy(Character& _player) :player(_player), Sprite("Resources/Joker.png", Vector2(rand() % 2000, rand() % 2000))
	{
		health = 100;
	}

	void move(float dt)
	{
		Vector2 pPos = player.rect.get_center();
		Vector2 newPos = rect.get_center().move_towards(pPos, speed * dt, 100);
		rect.set_center(newPos);
	}

	void update(float dt)
	{
		move(dt);
	}
};

class GameScene :public Scene
{
	Character* player;
	TileMap* level;
	Enemy* enemy;
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
		srand(static_cast<unsigned int>(time(NULL)));
		// creating objects
		isActive = true;
		level = new Level();
		player = new Character("Resources/Hero.png", Vector2(0), *level);
		enemy = new Enemy(*player);
		Camera::set_follow_target(player->rect);
		std::cout << name << " started" << std::endl;
	}

	void destroy()
	{
		isActive = false;

		delete player, level, enemy;
		player = nullptr;
		level = nullptr;
		enemy = nullptr;

		std::cout << name << " destroyed" << std::endl;
	}

	bool update(float dt)
	{
		// update all objects
		player->update(dt);
		enemy->update(dt);
		Camera::update(dt);

		if (Inputs::ui_back())
			return true;
		return false;
	}

	void draw()
	{
		level->draw();
		player->draw();
		enemy->draw();
	}

	void debug()
	{
		level->debug();
		player->debug();
		enemy->debug();
	}
};

Scene* create_game_scene()
{
	return new GameScene();
}