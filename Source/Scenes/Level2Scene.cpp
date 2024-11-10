
#include "NPC.h"
#include "SceneManagement.h"
#include"Character.h"
#include "Constants.h"
#include "DataManager.h"
#include "TileMap.h"

class InfiniteLevel :public TileMap<32, 24>
{
	Vector2 camPos;
	Vector2 camGPos;
	Vector2 mapSize;
	Vector2 maxBounds;
	Vector2 minBounds;
public:
	InfiniteLevel()
	{
		for (unsigned int i = 0; i < 24; i++)
			load_image(tileSet[i], "Resources/Tiles/" + std::to_string(i) + ".png");

		load_level("level2.txt", data);
		size = data.get_size();

		mapSize.set(40, 30);
		camPos = Camera::camRect.get_center();
		maxBounds = (mapSize / 2) * 32;
		minBounds = Vector2::zero - (mapSize / 2) * 32;
	}

	void update(float dt) override
	{
		camPos = Camera::camRect.get_center();
		camGPos = get_grid_pos(camPos, 32);
		update_pos();
	}

	void update_pos()
	{
		for (unsigned int i = 0; i < size; i++)
		{
			Vector2 _gPos = data[i].key;
			Vector2 _pos = _gPos * 32 - camPos;

			if (_pos.x < minBounds.x)
				_gPos.x += mapSize.x;
			else if (_pos.x > maxBounds.x)
				_gPos.x -= mapSize.x;

			if (_pos.y < minBounds.y)
				_gPos.y += mapSize.y;
			else if (_pos.y > maxBounds.y)
				_gPos.y -= mapSize.y;

			data[i].key = _gPos;
		}
	}
};

class Level2Scene :public Scene
{
	Character* player;
	InfiniteLevel* level;
	NpcManager npcManager;

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
		level = new InfiniteLevel();
		player = new Character("Resources/Hero.png", Vector2(0), 100, *level);
		npcManager.create(*player, false);
		Camera::set_follow_target(player->rect);

		gameOver = false;
		gameStarted = false;
		timer = 1;

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
			level->update(dt);

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

Scene* create_level_2_scene()
{
	return new Level2Scene();
}