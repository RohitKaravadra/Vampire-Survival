#pragma once
#include<iostream>
#include "Engine.h"

using std::string;

namespace Engine
{
	// sample scene class for easy build
	class Scene
	{
	protected:
		bool isActive = false;
		std::string name = "";
		Timer timer;
		float dt;
	public:
		Scene() = default;
		virtual void start() { App::sceneTimer = 0; };
		void stop();
		void update_loop();
		virtual void update(float dt) {};
		virtual void draw() {};
		virtual void draw_ui() {};
		virtual void debug() {};
		virtual void destroy() {};
		virtual ~Scene() {};

		string get_name()
		{
			return name;
		}

		bool operator==(Scene& _other)
		{
			return _other.name == name;
		}

		bool operator==(std::string _other)
		{
			return _other == name;
		}

		bool operator!=(Scene& _other)
		{
			return _other.name != name;
		}

		bool operator!=(std::string _other)
		{
			return _other != name;
		}
	};

	// scene management class to handle scene change
	template<unsigned int totalScenes = 1U>
	class SceneManager
	{

		Scene nullScene; // null scene if no scene found
		Scene* scenes[totalScenes]; // array to scene pointers
		unsigned int curIndex = 0; // last index of scenes array
		string curScene = ""; // name of current scene

		// get index of given string return -1 if not found
		int get_index(string _scene)
		{
			if (_scene.empty())
				return -1;

			for (unsigned int i = 0; i < totalScenes; i++)
				if (*scenes[i] == _scene)
					return i;
		}

	public:
		// default constructor
		SceneManager() = default;
		// destructor
		~SceneManager()
		{
			if (totalScenes <= 0)
				return;

			for (unsigned int i = 0; i < curIndex; i++)
			{
				delete scenes[i];
				scenes[i] = nullptr;
			}
		}

		// add scene pointer to scenes
		bool add(Scene* _scene)
		{
			if (curIndex >= totalScenes)
			{
				delete _scene;
				return false;
			}

			scenes[curIndex++] = _scene;
			return true;
		}

		// get name of current scene
		string get_current() { return curScene; }

		// change scene to given scene
		string change_scene(string _newScene)
		{
			int i = get_index(_newScene);
			if (i == -1)
				return "";

			curScene = scenes[i]->get_name();
			scenes[i]->start();
			return scenes[i]->get_name();
		}
	};
}