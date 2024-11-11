#pragma once
#include "Engine.h"

using namespace Engine;

#pragma region TileMap Methods

// class to handle tilemaps for level creation
template <unsigned int tileSize, unsigned int _setSize>
class TileMap
{
protected:
	Image tileSet[_setSize];
	Image dbgImg; // image with outline to debug colliders

	Rect rect; // rect use for collision check

	unsigned int setSize; // total tiles in tile Set
	unsigned int size; // size of data 
	Dictionary < Vector2, Pair<unsigned int, unsigned int>> data; // data (<position <layer, tile>)

	// constructorTileMap(unsigned int _tileSize)
	TileMap()
	{
		setSize = _setSize;
		rect.set(Vector2(tileSize), Vector2::zero);
		size = data.get_size();
		create_dbgImg();
	}
	// create a debug image for debugging colliders
	void create_dbgImg()
	{
		dbgImg.height = dbgImg.width = tileSize;
		dbgImg.channels = 4;
		dbgImg.free();
		create_rect_outline(dbgImg, Color::DEBUG_COLOR, 1);
	}

public:
	// update tilemap
	virtual void update(float dt) {};
	// check collision with tiles
	virtual bool is_colliding(Rect& _rect, unsigned int _layer)
	{
		if (size <= 0)
			return false;

		for (unsigned int i = 0; i < size; i++)
		{
			if (data[i].value.key == _layer)
			{
				rect.set_center(data[i].key * tileSize);
				if (rect.collide_as_rect(_rect))
					return true;
			}
		}

		return false;
	}
	// draw all tiles
	virtual void draw()
	{
		unsigned int size = data.get_size();
		for (unsigned int i = 0; i < size; i++)
		{
			rect.set_center(data[i].key * tileSize);
			Camera::draw(rect, tileSet[data[i].value.value % setSize]);
		}
	}
	// draw debug image of tiles
	virtual void debug(unsigned int _layer)
	{
		unsigned int size = data.get_size();
		for (unsigned int i = 0; i < size; i++)
		{
			if (data[i].value.key == _layer)
			{
				rect.set_center(data[i].key * tileSize);
				Camera::draw(rect, dbgImg);
			}
		}
	}
	// destructor
	virtual ~TileMap() = default;
};

#pragma endregion
