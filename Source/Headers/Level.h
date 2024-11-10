#pragma once
#include "Resources.h"
#include "TileMap.h"

using namespace Engine;

class Level : public TileMap<32, 24>
{
public:
	Level()
	{
		for (unsigned int i = 0; i < 24; i++)
			load_image(tileSet[i], "Resources/Tiles/" + std::to_string(i) + ".png");

		load_level("level.txt", data);
		size = data.get_size();
	}
};