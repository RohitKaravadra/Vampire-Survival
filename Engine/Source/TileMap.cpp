#include "Engine.h"

using namespace Engine;

#pragma region TileMap Methods

TileMap::TileMap(unsigned int _tileSize)
{
	tileSize = _tileSize;
	collider.set(Vector2(tileSize), Vector2::zero);
	totalTiles = 0;
	tiles = nullptr;
	size = data.get_size();
	create_dbgImg();
}

void TileMap::create_dbgImg()
{
	dbgImg.height = dbgImg.width = tileSize;
	dbgImg.channels = 4;
	dbgImg.free();
	create_rect_outline(dbgImg, Color::DEBUG_COLOR, 1);
}

bool TileMap::is_colliding(Rect& _rect, unsigned int _layer)
{
	if (size <= 0)
		return false;

	for (unsigned int i = 0; i < size; i++)
	{
		if (data[i].value.key == _layer)
		{
			collider.set_center(data[i].key * tileSize);
			if (collider.collide_as_rect(_rect))
				return true;
		}
	}

	return false;
}

void TileMap::draw()
{
	unsigned int size = data.get_size();
	for (unsigned int i = 0; i < size; i++)
	{
		collider.set_center(data[i].key * tileSize);
		Camera::draw(collider, tiles[data[i].value.value % totalTiles]);
	}
}

void TileMap::debug(unsigned int _layer)
{
	unsigned int size = data.get_size();
	for (unsigned int i = 0; i < size; i++)
	{
		if (data[i].value.key == _layer)
		{
			collider.set_center(data[i].key * tileSize);
			Camera::draw(collider, dbgImg);
		}
	}
}

TileMap::~TileMap()
{
	if (tiles != nullptr)
	{
		delete[] tiles;
		tiles = nullptr;
	}
}

#pragma endregion
