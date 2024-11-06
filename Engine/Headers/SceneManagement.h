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
		virtual void start() {};
		void stop();
		void update_loop();
		virtual void update(float dt) {};
		virtual void draw() {};
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
	class SceneManager
	{

		Scene nullScene; // null scene if no scene found
		Scene** scenes; // array to scene pointers
		unsigned totalScenes = 0; // total number of scenes
		unsigned int curIndex = 0; // last index of scenes array
		string curScene = ""; // name of current scene

		// get index of given string return -1 if not found
		int get_index(string _scene);

	public:
		// default constructor
		SceneManager() = default;
		// destructor
		~SceneManager();
		// create scenes array with given total size
		bool create(int _totalScenes);
		// add scene pointer to scenes
		bool add(Scene* _scene);
		// get name of current scene
		string get_current();
		// change scene to given scene
		string change_scene(string _newScene);
	};
}