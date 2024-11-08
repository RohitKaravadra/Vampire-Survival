#include "SceneManagement.h"

using namespace Engine;

void Scene::update_loop()
{
	isActive = true;

	if (Camera::notNull)
		Camera::camRect.set_center(Vector2(0));

	timer.reset();

	while (isActive)
	{
		Inputs::refresh();
		dt = timer.dt();
		App::sceneTimer += dt;

		Collisions::update();
		update(dt);

		if (Camera::notNull)
			Camera::clear();

		draw();

		if (DEBUG_MODE)
			debug();

		draw_ui();

		if (Camera::notNull)
			Camera::present();
	}

	if (Camera::notNull)
		Camera::camRect.set_center(Vector2(0));

	isActive = false;
	destroy();
}

void Scene::stop()
{
	App::sceneTimer = 0;
	Collisions::destroy();
	isActive = false;
	destroy();
}