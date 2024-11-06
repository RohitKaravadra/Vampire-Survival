#include "SceneManagement.h"
#include "Engine.h"

using namespace Engine;

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

Scene* SceneManager::change_scene(string _newScene)
{
	int i = get_index(_newScene);
	if (i == -1)
		return &nullScene;

	if (!curScene.empty())
	{
		int ci = get_index(curScene);
		if (ci != -1)
			scenes[ci]->destroy();
	}

	if (Camera::notNull)
		Camera::camRect.set_center(Vector2(0));

	curScene = scenes[i]->get_name();
	scenes[i]->start();
	return scenes[i];
}
