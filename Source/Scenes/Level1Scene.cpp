
#include "NPC.h"
#include "level.h"
#include "SceneManagement.h"
#include"Character.h"
#include "Constants.h"
#include "DataManager.h"


// scene
class Level1Scene :public Scene
{
	Character* player; // character pointer
	Level* level; // level pointer
	NpcManager npcManager; // npc manager not a pointer lol

	bool gameOver; // to check game over state
	bool gameStarted; // to check game started state
	float timer; // timer for game start and over
	float fps; // fps to check performance
public:
	Level1Scene() { name = "Level1Scene"; }

	~Level1Scene() { destroy(); }

	// start app and create objects
	void start() override
	{
		Scene::start();
		level = new Level();
		load_game_state();

		Camera::set_follow_target(player->rect);

		gameOver = false;
		gameStarted = false;
		timer = 3;
		fps = 0;

		// starte update loop for this scene
		update_loop();
	}

	// load saved data of game
	void load_game_state()
	{
		// reset all data before loading new data
		GameStats::reset();
		DataManager::clear_data();
		// load data
		DataManager::load_data();

		// instantiate objects according to data loaded or not
		if (DataManager::is_loaded())
		{
			Pair<Vector2, float> playerData = DataManager::get_player_data();
			player = new Character("Resources/Hero.png", playerData.key, playerData.value, *level);
		}
		else
			player = new Character("Resources/Hero.png", Vector2::zero, 100, *level);

		// create npcs
		npcManager.create(*player, true);
	}

	// save current data of game if available
	void save_game_state()
	{
		// check if game is over or player exited the scene
		if (gameOver)
			DataManager::save_data(true);
		else
		{
			// save current gamme data including player and npcs
			DataManager::set_player_data(player->rect.get_center(), player->get_health());
			npcManager.save_data();
			DataManager::save_data();
		}
	}

	void destroy()override
	{
		isActive = false;

		// check if App is not destroyed (destroy function is called when app is destroyed)
		if (App::is_active())
		{
			GameStats::time += App::sceneTimer; // update game timer in game stats
			if (gameOver)
				GameStats::print(); // display game stats
			// display fps
			std::cout << "\nAverage FPS : " << fps << "\n\n";
			save_game_state();
		}

		npcManager.destroy();

		// always delete pointers created with new 
		delete player, level;
		player = nullptr;
		level = nullptr;
	}

	void update_game_state()
	{
		// current game stat
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

		// calculate fps
		fps = (fps + 1 / dt) / 2;

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
		// draw all objects
		level->draw();
		player->draw();
		npcManager.draw();
	}

	void draw_ui() override
	{
		// draw player health and power
		player->draw_ui();
	}

	void debug()override
	{
		// only for deugging
		level->debug(1);
		player->debug();
	}
};

// creates and return a new level 1 scene
Scene* create_level_1_scene()
{
	return new Level1Scene();
}