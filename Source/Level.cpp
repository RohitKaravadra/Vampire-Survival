#include "Level.h"

Level::Level() :TileMap(32)
{
	totalTiles = 24;
	tiles = new Image[totalTiles];
	for (unsigned int i = 0; i < totalTiles; i++)
		load_image(tiles[i], "Resources/Tiles/" + std::to_string(i) + ".png");

	load_level(data);
	size = data.get_size();
}