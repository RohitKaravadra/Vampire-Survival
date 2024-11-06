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

void load_level(Dictionary<Vector2, Pair<unsigned int, unsigned int>>& _level)
{
	_level.clear();
	std::ifstream levelFile("level.txt");
	if (levelFile)
	{
		string line;
		unsigned int size;
		getline(levelFile, line);
		size = stoi(line);
		if (size > 0)
		{
			for (unsigned int i = 0; i < size; i++)
			{
				Pair<Vector2, Pair<unsigned int, unsigned int>> pair;

				getline(levelFile, line, ',');
				pair.key.x = stof(line);
				getline(levelFile, line, ',');
				pair.key.y = stof(line);
				getline(levelFile, line, ',');
				pair.value.key = stoi(line);
				getline(levelFile, line);
				pair.value.value = stoi(line);

				_level.add(pair);
			}
		}
	}
	levelFile.close();
}

void save_level(Dictionary<Vector2, Pair<unsigned int, unsigned int>>& _level)
{
	unsigned int size = _level.get_size();
	if (size > 0)
	{
		std::ofstream levelFile("level.txt");

		levelFile << size << "\n";
		for (unsigned int i = 0; i < size; i++)
		{
			Pair<Vector2, Pair<unsigned int, unsigned int>> pair = _level[i];
			levelFile << pair.key.x << ',';
			levelFile << pair.key.y << ',';
			levelFile << pair.value.key << ',';
			levelFile << pair.value.value << "\n";
		}

		levelFile.close();
	}
}