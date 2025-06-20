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

void load_level(std::string _name, Dictionary<Vector2, Pair<unsigned int, unsigned int>>& _level)
{
	_level.clear();
	std::ifstream levelFile(_name);
	if (levelFile)
	{
		string line;
		unsigned int size;
		getline(levelFile, line);
		size = stoi(line);
		if (size > 0)
		{
			Pair<Vector2, Pair<unsigned int, unsigned int>> pair;
			for (unsigned int i = 0; i < size; i++)
			{

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
		else
			std::cout << "ERROR : Loaded level is empty" << std::endl;

		levelFile.close();
	}
	else
		std::cout << "ERROR : Can't open file " << _name << std::endl;
}

void save_level(std::string _name, Dictionary<Vector2, Pair<unsigned int, unsigned int>>& _level)
{
	unsigned int size = _level.get_size();
	if (size > 0)
	{
		std::ofstream levelFile(_name);

		if (levelFile)
		{
			levelFile << size << "\n";
			for (unsigned int i = 0; i < size; i++)
			{
				Pair<Vector2, Pair<unsigned int, unsigned int>> pair = _level[i];
				levelFile << pair.key.x << ',';
				levelFile << pair.key.y << ',';
				levelFile << pair.value.key << ',';
				levelFile << pair.value.value << "\n";
			}

			std::cout << "Level Saved in " << _name << std::endl;
		}
		else
			std::cout << "ERROR : Can't open or create file" << std::endl;

		levelFile.close();
	}
	else
		std::cout << "WARNING : Level is empty. Can't save an empty level" << std::endl;
}