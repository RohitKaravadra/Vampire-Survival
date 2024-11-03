
#include "Utilities.h"
#include "Engine.h"

using namespace Engine;

extern const Vector2 WIN_SIZE;

Vector2 get_grid_pos(Vector2 _pos, int _size, int _space = 0)
{
	float tSize = _size + _space;
	Vector2 gPos = (_pos / tSize).to_int() * tSize;

	gPos.x += _pos.x < 0 ? -tSize / 2 : tSize / 2;
	gPos.y += _pos.y < 0 ? -tSize / 2 : tSize / 2;

	return gPos;
}

class Tile : public Sprite
{
public:
	Tile(Color _color = Color::BLUE) :Sprite(Vector2(128), Vector2(0), _color)
	{

	}
};

class TileMap :public SpriteGroup
{
	Tile tile[5] = { Tile(), Tile(Color::GREEN), Tile(Color::RED), Tile(Color::YELLOW), Tile(Color::MAGENTA) };
	Dictionary<Vector2, int> data;
public:
	TileMap(unsigned int _width, unsigned int _height) :SpriteGroup(_width* _height)
	{
		data = Dictionary<Vector2, int>(_width * _height);
	}

	void add(Vector2 _pos, unsigned int tile)
	{
		Pair<Vector2, int> pair(_pos, tile % 5);
		data.add(pair);
	}

	void draw()
	{
		unsigned int size = data.get_size();
		for (unsigned int i = 0; i < size; i++)
		{
			tile[data[i].value].rect.set_center(data[i].key);
			tile[data[i].value].draw();
		}
	}

	~TileMap()
	{
	}
};

TileMap* tileMap;

class TileHighlight :public Sprite
{
	unsigned int tileNo = 0;
public:
	TileHighlight(Vector2 _size, Vector2 _pos) :Sprite(_size, _pos)
	{
		image.width = _size.x;
		image.height = _size.y;
		image.channels = 4;
		create_outline(image, Color::WHITE, 2);
	}

	void update(float dt)
	{
		rect.set_center(get_grid_pos(Camera::screen_to_world(Inputs::get_mouse_pos()), 128));

		if (Inputs::key_pressed('C'))
			tileNo += 1;
		if (Inputs::key_pressed(VK_SPACE))
			tileMap->add(rect.get_center(), tileNo);
	}
};

class MapEditor : public App
{
	bool isRunning;
	float moveSpeed = 200;
	TileHighlight* indicator;
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
		tileMap = new TileMap(5, 5);
		indicator = new TileHighlight(Vector2(128), Vector2::zero);
		update_loop();
	}

	void destroy()
	{
		delete indicator, tileMap;
	}

	void update_loop()
	{
		isRunning = true;
		while (isRunning)
		{
			App::update();

			indicator->update(deltaTime);

			Camera::camRect.move(Inputs::get_axis() * deltaTime * moveSpeed);

			if (Inputs::key_pressed('E'))
				moveSpeed += 1000 * deltaTime;
			if (Inputs::key_pressed('Q'))
				moveSpeed -= 1000 * deltaTime;

			Camera::clear();

			tileMap->draw();
			indicator->draw();

			Camera::present();

			if (Inputs::ui_back())
				isRunning = false;
		}
	}
};

void map_editor()
{
	MapEditor app("Map Editor", WIN_SIZE);
	app.start();
}