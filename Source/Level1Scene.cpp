
#include "NPC.h"
#include "level.h"
#include "SceneManagement.h"

class GameScene :public Scene
{
	Character* player;
	TileMap* level;
	HeavyNpcSwarm swarm;

	bool gameOver;
	float endCounter;
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
		level = new Level();
		player = new Character("Resources/Hero.png", Vector2(0), *level);
		swarm.create(5, *player);

		Camera::set_follow_target(player->rect);

		gameOver = false;
		endCounter = 0;
		std::cout << name << " started" << std::endl;

		// starte update loop for this scene
		update_loop();
	}

	void destroy()
	{
		isActive = false;
		swarm.destroy();
		delete player, level;
		player = nullptr;
		level = nullptr;

		std::cout << name << " destroyed" << std::endl;
	}

	bool is_game_over(float dt)
	{
		if (gameOver)
		{
			if (endCounter <= 0)
				return true;
			endCounter -= dt;
		}
		else
		{
			if (!player->is_alive())
			{
				gameOver = true;
				endCounter = 3;
			}
		}
		return false;
	}

	void update(float dt)
	{
		// update all objects
		player->update(dt);
		swarm.update(dt);
		Camera::update(dt);

		if (is_game_over(dt))
			isActive = false;

		// check for exit condition
		if (!gameOver && Inputs::ui_back())
			isActive = false;
	}

	void draw()
	{
		Camera::clear();
		level->draw();
		player->draw();
		swarm.draw();
	}

	void debug()
	{
		level->debug(1);
		player->debug();
		swarm.debug();
	}
};

Scene* create_game_scene()
{
	return new GameScene();
}