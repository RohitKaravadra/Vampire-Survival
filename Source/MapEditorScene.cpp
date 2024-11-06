
#include "Engine.h"
#include "Resources.h"
#include "SceneManagement.h"

using namespace Engine;
using namespace Utilities;
using namespace std;

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
	unsigned int curLayer;
	float inpFreq;

public:
	Map() : TileMap(32)
	{
		totalTiles = 24;
		tiles = new Image[totalTiles];
		for (unsigned int i = 0; i < totalTiles; i++)
			load_image(tiles[i], "Resources/Tiles/" + std::to_string(i) + ".png");

		load_level(data);
		size = data.get_size();
		curTile = 0;
		curLayer = 0;
		inpFreq = 0;
		pointer.set_image(tiles[curTile]);
	}

	void load()
	{
		curTile = 0;
		inpFreq = 0;
		load_level(data);
		size = data.get_size();
		pointer.set_image(tiles[curTile]);
	}

	void add(Vector2 _pos, unsigned int _layer, unsigned int _tile)
	{
		Pair<Vector2, Pair<unsigned int, unsigned int>> pair;
		pair.key = _pos;
		pair.value.key = _layer;
		pair.value.value = _tile;
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
		else if (Inputs::key_pressed('L'))
		{
			curLayer = curLayer == 0 ? 1 : 0;
			if (curLayer == 1)
				std::cout << "Layer changed to Solid\n";
			else
				std::cout << "Layer changed to Pass Through\n";
		}
		else if (Inputs::key_pressed('P'))
		{
			std::cout << "Level Saved" << std::endl;
			save_level(data);
		}
		else
			inpFreq = 0;

		if (Inputs::key_pressed('X'))
			remove(pointer.gPos);
		if (Inputs::key_pressed(VK_SPACE))
			add(pointer.gPos, curLayer, curTile);
	}

	void update(float dt)
	{
		if (inpFreq > 0)
			inpFreq -= dt;
		else
			check_inputs();
	}
};

class MapEditorScene : public Scene
{
	float moveSpeed = 200;
	Map map;
public:
	MapEditorScene()
	{
		name = "MapEditor";
	}

	~MapEditorScene()
	{

	}

	void start()
	{
		map.load();
		isActive = true;
		std::cout << name << " started" << std::endl;
	}

	void destroy()
	{
		isActive = false;
		std::cout << name << " destroyed" << std::endl;
	}

	bool update(float dt)
	{
		pointer.update(dt);
		map.update(dt);

		Camera::camRect.move(Inputs::get_axis() * dt * moveSpeed);

		if (Inputs::key_pressed('E'))
			moveSpeed += 1000 * dt;
		if (Inputs::key_pressed('Q'))
			moveSpeed -= 1000 * dt;

		if (Inputs::ui_back())
			return true;
		return false;
	}

	void draw()
	{
		map.draw();
		pointer.draw();
	}

	void debug()
	{
		if (DEBUG_MODE)
		{
			map.debug();
		}
	}
};

Scene* create_editor_scene()
{
	return new MapEditorScene();
}