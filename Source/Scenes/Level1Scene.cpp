
#include "NPC.h"
#include "level.h"
#include "SceneManagement.h"
#include"Character.h"
#include "Constants.h"
#include "DataManager.h"

class GameScene :public Scene
{
	Character* player;
	Level* level;
	NpcManager npcManager;

	bool gameOver;
	bool gameStarted;
	float timer;
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
	void start() override
	{
		GameStats::reset();
		Scene::start();
		level = new Level();
		load_game_state();

		Camera::set_follow_target(player->rect);

		gameOver = false;
		gameStarted = false;
		timer = 3;

		// starte update loop for this scene
		update_loop();
	}

	// load saved data of game
	void load_game_state()
	{
		DataManager::load_data();
		if (DataManager::is_loaded())
		{
			Pair<Vector2, float> playerData = DataManager::get_player_data();
			player = new Character("Resources/Hero.png", playerData.key, playerData.value, *level);
		}
		else
			player = new Character("Resources/Hero.png", Vector2::zero, 100, *level);

		npcManager.create(*player, true);
	}

	// save current data of game if available
	void save_game_state()
	{
		if (gameOver)
			DataManager::save_data(true);
		else
		{
			DataManager::set_player_data(player->rect.get_center(), player->get_health());
			npcManager.save_data();
			DataManager::save_data();
		}
	}

	void destroy()override
	{
		isActive = false;

		if (App::is_active())
		{
			GameStats::time += App::sceneTimer;
			if (gameOver)
				GameStats::print();
			save_game_state();
		}

		npcManager.destroy();

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
			npcManager.update(dt);
			player->set_nearest(npcManager.get_nearest());

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
		npcManager.draw();
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

Scene* create_game_scene()
{
	return new GameScene();
}