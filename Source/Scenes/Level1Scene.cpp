
#include "NPC.h"
#include "level.h"
#include "SceneManagement.h"
#include"Character.h"

class GameScene :public Scene
{
	Character* player;
	Level* level;
	NpcManager npcManager;

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
	void start() override
	{
		Scene::start();
		level = new Level();
		player = new Character("Resources/Hero.png", Vector2(0), *level);
		npcManager.create(*player);

		Camera::set_follow_target(player->rect);

		gameOver = false;
		endCounter = 0;

		// starte update loop for this scene
		update_loop();
	}

	void destroy()override
	{
		isActive = false;

		if (App::is_active())
		{
			std::cout << "Survive Time : " << get_time(App::sceneTimer) << std::endl;
			NpcStats::print();
		}
		npcManager.destroy();

		delete player, level;
		player = nullptr;
		level = nullptr;
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

	void update(float dt)override
	{
		// update all objects
		player->update(dt);
		npcManager.update(dt);
		player->set_nearest(npcManager.get_nearest());
		Camera::update(dt);

		if (is_game_over(dt))
			isActive = false;

		// check for exit condition
		if (!gameOver && Inputs::ui_back())
			isActive = false;
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