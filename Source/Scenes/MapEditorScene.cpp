
#include "Level.h"
#include "SceneManagement.h"

using namespace Engine;
using namespace Utilities;
using namespace std;

class TilePointer :public Sprite
{
public:
	Vector2 gPos;
	TilePointer() :Sprite(Vector2(32), Vector2(0))
	{
		create_rect_outline(image, Color::RED, 1);
		gPos.set(0, 0);
	}

	void update(float dt)
	{
		gPos = get_grid_pos(Camera::screen_to_world(Inputs::get_mouse_pos()), 32);
		rect.set_center(gPos * 32);
	}

	void set_image(Image& _image)
	{
		image.copy(_image);
		create_rect_outline(image, Color::RED, 2);
	}
};

class EditorMap :public Level
{
	int curTile;
	unsigned int curLayer;
	float inpFreq;
	TilePointer& pointer;
public:
	EditorMap(TilePointer& _pointer) :pointer(_pointer), Level()
	{
		curTile = 0;
		curLayer = 0;
		inpFreq = 0;
		pointer.set_image(tileSet[curTile]);
	}

	void add(Vector2 _pos, unsigned int _layer, unsigned int _tile)
	{
		Pair<Vector2, Pair<unsigned int, unsigned int>> pair;
		pair.key = _pos;
		pair.value.key = _layer;
		pair.value.value = _tile;
		data.add(pair);
	}

	void remove(Vector2 _pos)
	{
		data.remove_key(_pos);
	}

	void check_inputs()
	{
		inpFreq = 0.2f;
		int wheel = Inputs::mouse_wheel();
		if (wheel < 0)
		{
			curTile--;
			if (curTile < 0)
				curTile = setSize - 1;
			pointer.set_image(tileSet[curTile]);
		}
		else if (wheel > 0)
		{
			curTile = (curTile + 1) % setSize;
			pointer.set_image(tileSet[curTile]);
		}
		else if (Inputs::key_pressed(VK_SPACE))
		{
			curLayer = curLayer == 0 ? 1 : 0;
			if (curLayer == 1)
				std::cout << "Layer changed to Solid\n";
			else
				std::cout << "Layer changed to Pass Through\n";
		}
		else if (Inputs::key_pressed('P'))
		{
			save_level("level.txt", data);
		}
		else
			inpFreq = 0;

		if (Inputs::mouse_button(MouseRight))
			remove(pointer.gPos);
		if (Inputs::mouse_button(MouseLeft))
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
	EditorMap* map;
	TilePointer pointer;
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
		map = new EditorMap(pointer);
		isActive = true;

		update_loop();
	}

	void destroy()
	{
		isActive = false;
		delete map;
		map = nullptr;
	}

	void update(float dt)
	{
		pointer.update(dt);
		map->update(dt);

		Camera::camRect.move(Inputs::get_axis() * dt * moveSpeed);

		// update camera movement speed
		if (Inputs::key_pressed('E'))
			moveSpeed += 1000 * dt;
		if (Inputs::key_pressed('Q'))
			moveSpeed -= 1000 * dt;

		// check for exiting scene condition
		if (Inputs::ui_back())
			isActive = false;
	}

	void draw()
	{
		map->draw();
		map->debug(1);
		pointer.draw();
	}

	void debug()
	{

	}
};

Scene* create_editor_scene()
{
	return new MapEditorScene();
}