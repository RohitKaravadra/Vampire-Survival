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

		Collisions::update();
		update(dt);

		if (Camera::notNull)
			Camera::clear();

		draw();

		if (DEBUG_MODE)
			debug();

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
	Collisions::destroy();
	isActive = false;
	destroy();
}

SceneManager::~SceneManager()
{
	if (scenes == nullptr)
		return;

	for (unsigned int i = 0; i < curIndex; i++)
	{
		delete scenes[i];
		scenes[i] = nullptr;
	}

	delete[] scenes;
	scenes = nullptr;
}

int SceneManager::get_index(string _scene)
{
	if (_scene.empty())
		return -1;

	for (unsigned int i = 0; i < totalScenes; i++)
		if (*scenes[i] == _scene)
			return i;
}

bool SceneManager::create(int _totalScenes)
{
	if (scenes != nullptr)
		return false;

	scenes = new Scene * [_totalScenes];
	totalScenes = _totalScenes;
	return true;
}

bool SceneManager::add(Scene* _scene)
{
	if (curIndex >= totalScenes)
	{
		delete _scene;
		return false;
	}

	scenes[curIndex++] = _scene;
	return true;
}

string SceneManager::get_current()
{
	return curScene;
}

string SceneManager::change_scene(string _newScene)
{
	int i = get_index(_newScene);
	if (i == -1)
		return "";

	curScene = scenes[i]->get_name();
	scenes[i]->start();
	return scenes[i]->get_name();
}
