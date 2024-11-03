
#include "Utilities.h"
#include "Engine.h"

using namespace Engine;

extern const Vector2 WIN_SIZE;

class TilePointer :public Sprite
{
public:
	Vector2 gPos;
	TilePointer(Vector2 _size, Vector2 _pos) :Sprite(_size, _pos)
	{
		image.width = _size.x;
		image.height = _size.y;
		image.channels = 4;
		create_outline(image, Color::AQUA, 5);
		gPos.set(0, 0);
	}

	void update(float dt)
	{
		gPos = get_grid_pos(Camera::screen_to_world(Inputs::get_mouse_pos()), 128);
		rect.set_center(gPos * 128);
	}

	void set_image(Image& _image)
	{
		image.copy(_image);
		create_outline(image, Color::AQUA, 5);
	}
};

TilePointer* pointer;

class Tile : public Sprite
{
public:
	Tile(Color _color = Color::WHITE) :Sprite(Vector2(128), Vector2(0), _color)
	{

	}
	Tile(string _location) :Sprite(_location, Vector2::zero)
	{

	}

	Image& get_Image()
	{
		return image;
	}
};

class TileMap
{
	Tile tile[5] = { Tile("Resources/Tiles/Brick/Brick_01-128x128.png"),
		Tile("Resources/Tiles/Brick/Brick_02-128x128.png"),
		Tile("Resources/Tiles/Brick/Brick_03-128x128.png"),
		Tile("Resources/Tiles/Brick/Brick_04-128x128.png"),
		Tile("Resources/Tiles/Brick/Brick_05-128x128.png") };

	Dictionary<Vector2, int> data;

	unsigned int tileSize;
	unsigned int totalTiles;
	unsigned int curTile;

	float inpFreq;

public:
	TileMap(int _tileSize)
	{
		data = Dictionary<Vector2, int>();
		totalTiles = 5;
		curTile = 0;
		inpFreq = 0;
		tileSize = _tileSize;
		if (pointer != nullptr)
			pointer->set_image(tile[curTile].get_Image());
	}

	void add(Vector2 _pos, unsigned int tile)
	{
		Pair<Vector2, int> pair(_pos, tile % 5);
		if (data.add(pair))
			cout << data << endl;
	}

	void remove(Vector2 _pos)
	{
		if (data.remove_key(_pos))
			cout << data << endl;
	}

	void check_inputs()
	{
		inpFreq = 0.2f;
		if (Inputs::key_pressed('C'))
		{
			curTile = (curTile + 1) % totalTiles;
			pointer->set_image(tile[curTile].get_Image());
		}
		else if (Inputs::key_pressed('X'))
			remove(pointer->gPos);
		else if (Inputs::key_pressed(VK_SPACE))
			add(pointer->gPos, curTile);
		else
			inpFreq = 0;
	}

	void update(float dt)
	{
		if (inpFreq > 0)
			inpFreq -= dt;
		else
			check_inputs();
	}

	void draw()
	{
		unsigned int size = data.get_size();
		for (unsigned int i = 0; i < size; i++)
		{
			tile[data[i].value].rect.set_center(data[i].key * tileSize);
			tile[data[i].value].draw();
		}
	}

	void debug()
	{
		unsigned int size = data.get_size();
		for (unsigned int i = 0; i < size; i++)
		{
			tile[data[i].value].rect.set_center(data[i].key * tileSize);
			tile[data[i].value].debug();
		}
	}

	~TileMap()
	{
	}
};


class MapEditor : public App
{
	bool isRunning;
	float moveSpeed = 200;
	TileMap* tileMap;
public:
	MapEditor(string _name, Vector2 _size) :App(_name, _size)
	{
		isRunning = false;
		tileMap = nullptr;
	}

	~MapEditor()
	{
		destroy();
	}

	void start()
	{
		pointer = new TilePointer(Vector2(128), Vector2::zero);
		tileMap = new TileMap(128);
		update_loop();
	}

	void destroy()
	{
		delete pointer, tileMap;
	}

	void update_loop()
	{
		isRunning = true;
		while (isRunning)
		{
			App::update();

			pointer->update(deltaTime);
			tileMap->update(deltaTime);

			Camera::camRect.move(Inputs::get_axis() * deltaTime * moveSpeed);

			if (Inputs::key_pressed('E'))
				moveSpeed += 1000 * deltaTime;
			if (Inputs::key_pressed('Q'))
				moveSpeed -= 1000 * deltaTime;

			if (Inputs::key_pressed(VK_TAB))
				DEBUG_MODE = !DEBUG_MODE;

			Camera::clear();

			tileMap->draw();
			pointer->draw();

			if (DEBUG_MODE)
			{
				tileMap->debug();
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