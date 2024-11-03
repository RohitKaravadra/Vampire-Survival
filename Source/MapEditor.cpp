
#include "Utilities.h"
#include "Engine.h"
#include <fstream>

using namespace Engine;

extern const Vector2 WIN_SIZE;

void save_level(Dictionary<Vector2, int>& _level)
{
	unsigned int size = _level.get_size();
	if (size > 0)
	{
		std::ofstream levelFile("level.txt");

		levelFile << size << "\n";
		for (unsigned int i = 0; i < size; i++)
		{
			Pair<Vector2, int> pair = _level[i];
			levelFile << pair.key.x << "\n";
			levelFile << pair.key.y << "\n";
			levelFile << pair.value << "\n";
		}

		levelFile.close();
	}
}

void load_level(Dictionary<Vector2, int>& _level)
{
	_level.clear();
	std::ifstream levelFile("level.txt");
	if (levelFile)
	{
		unsigned int size;
		levelFile >> size;
		if (size > 0)
		{
			for (unsigned int i = 0; i < size; i++)
			{
				Pair<Vector2, int> pair;
				levelFile >> pair.key.x;
				levelFile >> pair.key.y;
				levelFile >> pair.value;

				_level.add(pair);
			}
		}
	}
	levelFile.close();
}

class TilePointer :public Sprite
{
public:
	Vector2 gPos;
	TilePointer() :Sprite()
	{
		image.width = 32;
		image.height = 32;
		image.channels = 4;
		create_outline(image, Color::AQUA, 1);
		gPos.set(0, 0);
		rect.set(Vector2(32), gPos);
	}

	void update(float dt)
	{
		gPos = get_grid_pos(Camera::screen_to_world(Inputs::get_mouse_pos()), 32);
		rect.set_center(gPos * 32);
	}

	void set_image(Image& _image)
	{
		image.copy(_image);
		create_outline(image, Color::AQUA, 1);
	}
};

TilePointer pointer;

class Map :public TileMap
{
	int curTile;
	float inpFreq;

public:
	Map() : TileMap(32)
	{
		totalTiles = 24;
		tiles = new Image[totalTiles];
		for (unsigned int i = 0; i < totalTiles; i++)
			load_image(tiles[i], "Resources/Tiles/" + std::to_string(i) + ".png");

		curTile = 0;
		inpFreq = 0;
		load_level(data);
		size = data.get_size();
		pointer.set_image(tiles[curTile]);
	}

	void add(Vector2 _pos, unsigned int tile)
	{
		Pair<Vector2, int> pair(_pos, tile);
		if (data.add(pair) && DEBUG_MODE)
			cout << data << endl;
	}

	void remove(Vector2 _pos)
	{
		if (data.remove_key(_pos) && DEBUG_MODE)
			cout << data << endl;
	}

	void check_inputs()
	{
		inpFreq = 0.2f;
		if (Inputs::key_pressed('C'))
		{
			curTile--;
			if (curTile < 0)
				curTile = totalTiles - 1;
			pointer.set_image(tiles[curTile]);
		}
		else if (Inputs::key_pressed('V'))
		{
			curTile = (curTile + 1) % totalTiles;
			pointer.set_image(tiles[curTile]);
		}
		else
			inpFreq = 0;

		if (Inputs::key_pressed('X'))
			remove(pointer.gPos);
		if (Inputs::key_pressed(VK_SPACE))
			add(pointer.gPos, curTile);
	}

	void update(float dt)
	{
		if (inpFreq > 0)
			inpFreq -= dt;
		else
			check_inputs();
	}

	~Map()
	{
		save_level(data);
	}
};

class MapEditor : public App
{
	bool isRunning;
	float moveSpeed = 200;
	Map map;
public:
	MapEditor(string _name, Vector2 _size) :App(_name, _size)
	{
		isRunning = false;
	}

	~MapEditor()
	{
		destroy();
	}

	void start()
	{
		update_loop();
	}

	void destroy()
	{
	}

	void update_loop()
	{
		isRunning = true;
		while (isRunning)
		{
			App::update();

			pointer.update(deltaTime);
			map.update(deltaTime);

			Camera::camRect.move(Inputs::get_axis() * deltaTime * moveSpeed);

			if (Inputs::key_pressed('E'))
				moveSpeed += 1000 * deltaTime;
			if (Inputs::key_pressed('Q'))
				moveSpeed -= 1000 * deltaTime;

			Camera::clear();

			map.draw();
			pointer.draw();

			if (DEBUG_MODE)
			{
				map.debug();
			}

			Camera::present();

			if (Inputs::ui_back())
				isRunning = false;
		}
	}
};

void map_editor()
{
	DEBUG_MODE = true;
	MapEditor app("Map Editor", WIN_SIZE);
	app.start();
}