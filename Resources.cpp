#include "Resources.h"
#include <fstream>
#include <Utilities.h>

using namespace std;
using namespace Utilities;

// function loads image to given reference
bool load_image(Image& image, std::string location)
{
	image.free();
	return image.load(location);
}

void load_level(Dictionary<Vector2, unsigned int>& _level)
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
				Pair<Vector2, unsigned int> pair;
				levelFile >> pair.key.x;
				levelFile >> pair.key.y;
				levelFile >> pair.value;

				_level.add(pair);
			}
		}
	}
	levelFile.close();
}

void save_level(Dictionary<Vector2, unsigned int>& _level)
{
	unsigned int size = _level.get_size();
	if (size > 0)
	{
		std::ofstream levelFile("level.txt");

		levelFile << size << "\n";
		for (unsigned int i = 0; i < size; i++)
		{
			Pair<Vector2, unsigned int> pair = _level[i];
			levelFile << pair.key.x << "\n";
			levelFile << pair.key.y << "\n";
			levelFile << pair.value << "\n";
		}

		levelFile.close();
	}
}