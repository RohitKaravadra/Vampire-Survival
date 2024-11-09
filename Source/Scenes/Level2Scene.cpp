
#include "NPC.h"
#include "level.h"
#include "SceneManagement.h"
#include"Character.h"
#include "Constants.h"
#include "DataManager.h"

class Level2Scene :public Scene
{
	Character* player;
	Level* level;
	//NpcManager npcManager;

	bool gameOver;
	bool gameStarted;
	float timer;
public:
	Level2Scene()
	{
		name = "Level2Scene";
	}

	~Level2Scene()
	{
		destroy();
	}

	// start app and create objects
	void start() override
	{
		Scene::start();
		GameStats::reset();
		level = new Level();
		player = new Character("Resources/Hero.png", Vector2(0), 100, *level);
		Camera::set_follow_target(player->rect);

		gameOver = false;
		gameStarted = false;
		timer = 3;

		// starte update loop for this scene
		update_loop();
	}

	void destroy()override
	{
		isActive = false;

		if (App::is_active())
		{
			if (gameOver)
				GameStats::print();
		}

		//npcManager.destroy();

		delete player, level;
		player = nullptr;
		level = nullptr;
	}

	void update_game_state()
	{
		if (timer <= 0)
		{
			if (gameStarted)
			{
				if (gameOver)
					isActive = false;
				else
				{
					if (!player->is_alive())
					{
						gameOver = true;
						timer = 3;
						GameStats::time = App::sceneTimer;
					}
				}
			}
			else
				gameStarted = true;
		}
	}

	void update(float dt)override
	{
		// update timer
		if (timer > 0)
			timer -= dt;

		update_game_state();

		// check if game is started
		if (gameStarted)
		{
			// update all objects
			player->update(dt);
			//npcManager.update(dt);
			//player->set_nearest(npcManager.get_nearest());

			// check for exit condition
			if (!gameOver && Inputs::ui_back())
				isActive = false;
		}

		Camera::update(dt);
	}

	void draw() override
	{
		level->draw();
		player->draw();
		//npcManager.draw();
	}

	void draw_ui() override
	{
		player->draw_ui();
	}

	void debug()override
	{
		level->debug(1);
		player->debug();
	}
};

Scene* create_level_2_scene()
{
	return new Level2Scene();
}