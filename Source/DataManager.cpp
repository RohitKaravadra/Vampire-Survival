#include "DataManager.h"
#include "Constants.h"
#include "GameStats.h"
#include <fstream>
#include <string>

DArray<Pair<Vector2, float>> DataManager::liteEnemyData;
DArray<Pair<Vector2, float>> DataManager::heavyEnemyData;
DArray<Pair<Vector2, float>> DataManager::staticEnemyData;
DArray<Pair<Vector2, float>> DataManager::shooterEnemyData;

Pair<Vector2, float> DataManager::playerData;
bool DataManager::isLoaded = false;


void DataManager::set_npc_data(std::string _tag, DArray<Pair<Vector2, float>> _data)
{
	if (_tag == LiteNpcTag)
		liteEnemyData = _data;
	else if (_tag == HeavyNpcTag)
		heavyEnemyData = _data;
	else if (_tag == StaticNpcTag)
		staticEnemyData = _data;
	else if (_tag == ShooterNpcTag)
		shooterEnemyData = _data;
}

void DataManager::set_player_data(Vector2 _pos, float _health)
{
	playerData.key = _pos;
	playerData.value = _health;
}

DArray<Pair<Vector2, float>>& DataManager::get_enemy_data(std::string _tag)
{
	if (_tag == LiteNpcTag)
		return liteEnemyData;
	if (_tag == HeavyNpcTag)
		return heavyEnemyData;
	if (_tag == StaticNpcTag)
		return staticEnemyData;
	if (_tag == ShooterNpcTag)
		return shooterEnemyData;

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

	liteEnemyData.clear();
	heavyEnemyData.clear();
	staticEnemyData.clear();
	shooterEnemyData.clear();

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

		int size = liteEnemyData.get_size();
		saveFile << LiteNpcTag << "\n" << size << "\n";
		for (unsigned int i = 0; i < size; i++)
			saveFile << liteEnemyData[i].key.x << ',' <<
			liteEnemyData[i].key.y << ',' << liteEnemyData[i].value << "\n";

		size = heavyEnemyData.get_size();
		saveFile << HeavyNpcTag << "\n" << size << "\n";
		for (unsigned int i = 0; i < size; i++)
			saveFile << heavyEnemyData[i].key.x << ',' <<
			heavyEnemyData[i].key.y << ',' << heavyEnemyData[i].value << "\n";

		size = staticEnemyData.get_size();
		saveFile << StaticNpcTag << "\n" << size << "\n";
		for (unsigned int i = 0; i < size; i++)
			saveFile << staticEnemyData[i].key.x << ',' <<
			staticEnemyData[i].key.y << ',' << staticEnemyData[i].value << "\n";

		size = shooterEnemyData.get_size();
		saveFile << ShooterNpcTag << "\n" << size << "\n";
		for (unsigned int i = 0; i < size; i++)
			saveFile << shooterEnemyData[i].key.x << ',' <<
			shooterEnemyData[i].key.y << ',' << shooterEnemyData[i].value << "\n";
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

					liteEnemyData.add(pair);
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

					heavyEnemyData.add(pair);
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

					staticEnemyData.add(pair);
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

					shooterEnemyData.add(pair);
				}
			}
		}
	}
	saveFile.close();
}

bool DataManager::is_loaded() { return isLoaded; }
