#include "DataManager.h"
#include "Constants.h"
#include "GameStats.h"
#include <fstream>
#include <string>

DArray<Pair<Vector2, float>> DataManager::liteNpcData;
DArray<Pair<Vector2, float>> DataManager::heavyNpcData;
DArray<Pair<Vector2, float>> DataManager::staticNpcData;
DArray<Pair<Vector2, float>> DataManager::shooterNpcData;

Pair<Vector2, float> DataManager::playerData;
bool DataManager::isLoaded = false;


void DataManager::set_npc_data(std::string _tag, DArray<Pair<Vector2, float>> _data)
{
	if (_tag == LiteNpcTag)
		liteNpcData = _data;
	else if (_tag == HeavyNpcTag)
		heavyNpcData = _data;
	else if (_tag == StaticNpcTag)
		staticNpcData = _data;
	else if (_tag == ShooterNpcTag)
		shooterNpcData = _data;
}

void DataManager::set_player_data(Vector2 _pos, float _health)
{
	playerData.key = _pos;
	playerData.value = _health;
}

DArray<Pair<Vector2, float>>& DataManager::get_enemy_data(std::string _tag)
{
	if (_tag == LiteNpcTag)
		return liteNpcData;
	if (_tag == HeavyNpcTag)
		return heavyNpcData;
	if (_tag == StaticNpcTag)
		return staticNpcData;
	if (_tag == ShooterNpcTag)
		return shooterNpcData;

	DArray<Pair<Vector2, float>> data;
	return data;
}

Pair<Vector2, float> DataManager::get_player_data()
{
	return playerData;
}

void DataManager::clear_data()
{
	playerData.key = Vector2::zero;
	playerData.value = 100;

	liteNpcData.clear();
	heavyNpcData.clear();
	staticNpcData.clear();
	shooterNpcData.clear();

	isLoaded = false;
}

void DataManager::save_data(bool _null)
{
	std::ofstream saveFile("save.txt");

	if (!_null)
	{
		saveFile << GameStats::time << "," << GameStats::wave << ","
			<< GameStats::liteKilled << "," << GameStats::heavyKilled << ","
			<< GameStats::staticKilled << "," << GameStats::shooterKilled << "\n";

		saveFile << PlayerTag << "\n" << playerData.key.x << ','
			<< playerData.key.y << ',' << playerData.value << "\n";

		int size = liteNpcData.get_size();
		saveFile << LiteNpcTag << "\n" << size << "\n";
		for (unsigned int i = 0; i < size; i++)
			saveFile << liteNpcData[i].key.x << ',' <<
			liteNpcData[i].key.y << ',' << liteNpcData[i].value << "\n";

		size = heavyNpcData.get_size();
		saveFile << HeavyNpcTag << "\n" << size << "\n";
		for (unsigned int i = 0; i < size; i++)
			saveFile << heavyNpcData[i].key.x << ',' <<
			heavyNpcData[i].key.y << ',' << heavyNpcData[i].value << "\n";

		size = staticNpcData.get_size();
		saveFile << StaticNpcTag << "\n" << size << "\n";
		for (unsigned int i = 0; i < size; i++)
			saveFile << staticNpcData[i].key.x << ',' <<
			staticNpcData[i].key.y << ',' << staticNpcData[i].value << "\n";

		size = shooterNpcData.get_size();
		saveFile << ShooterNpcTag << "\n" << size << "\n";
		for (unsigned int i = 0; i < size; i++)
			saveFile << shooterNpcData[i].key.x << ',' <<
			shooterNpcData[i].key.y << ',' << shooterNpcData[i].value << "\n";
	}

	saveFile.close();
}

void DataManager::load_data()
{
	std::ifstream saveFile("save.txt");

	if (saveFile)
	{
		std::string line;
		std::getline(saveFile, line);

		if (!saveFile.eof())
			isLoaded = true;

		GameStats::load_stats(line);

		while (std::getline(saveFile, line))
		{
			if (line == PlayerTag)
			{
				getline(saveFile, line, ',');
				playerData.key.x = stof(line);
				getline(saveFile, line, ',');
				playerData.key.y = stof(line);
				getline(saveFile, line);
				playerData.value = stof(line);
			}

			else if (line == LiteNpcTag)
			{
				getline(saveFile, line);
				int size = stoi(line);
				Pair<Vector2, float> pair;
				for (unsigned int i = 0; i < size; i++)
				{
					std::getline(saveFile, line, ',');
					pair.key.x = stof(line);
					std::getline(saveFile, line, ',');
					pair.key.y = stof(line);
					std::getline(saveFile, line);
					pair.value = stof(line);

					liteNpcData.add(pair);
				}
			}

			else if (line == HeavyNpcTag)
			{
				getline(saveFile, line);
				int size = stoi(line);
				Pair<Vector2, float> pair;
				for (unsigned int i = 0; i < size; i++)
				{
					std::getline(saveFile, line, ',');
					pair.key.x = stof(line);
					std::getline(saveFile, line, ',');
					pair.key.y = stof(line);
					std::getline(saveFile, line);
					pair.value = stof(line);

					heavyNpcData.add(pair);
				}
			}

			else if (line == StaticNpcTag)
			{
				getline(saveFile, line);
				int size = stoi(line);
				Pair<Vector2, float> pair;
				for (unsigned int i = 0; i < size; i++)
				{
					std::getline(saveFile, line, ',');
					pair.key.x = stof(line);
					std::getline(saveFile, line, ',');
					pair.key.y = stof(line);
					std::getline(saveFile, line);
					pair.value = stof(line);

					staticNpcData.add(pair);
				}
			}

			else if (line == ShooterNpcTag)
			{
				getline(saveFile, line);
				int size = stoi(line);
				Pair<Vector2, float> pair;
				for (unsigned int i = 0; i < size; i++)
				{
					std::getline(saveFile, line, ',');
					pair.key.x = stof(line);
					std::getline(saveFile, line, ',');
					pair.key.y = stof(line);
					std::getline(saveFile, line);
					pair.value = stof(line);

					shooterNpcData.add(pair);
				}
			}
		}
	}
	saveFile.close();
}

bool DataManager::is_loaded() { return isLoaded; }
