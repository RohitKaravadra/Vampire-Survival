#include "GameStats.h"
#include "GameMath.h"
#include <iostream>

unsigned int GameStats::liteKilled = 0;
unsigned int GameStats::heavyKilled = 0;
unsigned int GameStats::staticKilled = 0;
unsigned int GameStats::shooterKilled = 0;
unsigned int GameStats::wave = 0;
double GameStats::time = 0;

void GameStats::print()
{
	std::cout << "\n---------------------------------\n";
	std::cout << "Survive Time : " << Engine::get_time(time) << std::endl;
	std::cout << "Wave(s) Survived :" << wave << std::endl;
	std::cout << "---------------\nEnemy Killed";
	std::cout << "\nLite Killed : " << liteKilled;
	std::cout << "\nHeavy Killed : " << heavyKilled;
	std::cout << "\nStatic Killed : " << staticKilled;
	std::cout << "\nShooter Killed : " << shooterKilled;
	std::cout << "\n----------------\n";
}

void GameStats::reset()
{
	wave = 0;
	liteKilled = 0;
	heavyKilled = 0;
	staticKilled = 0;
	shooterKilled = 0;
	time = 0;
}

void GameStats::load_stats(std::string _data)
{
	int size = _data.size();
	if (size < 2)
		return;

	std::string _num = "";
	int count = 0;
	for (int i = 0; i < size; i++)
	{
		if (_data[i] == ',')
		{
			switch (++count)
			{
			case 1: time = stod(_num); break;
			case 2: wave = stoi(_num); break;
			case 3: liteKilled = stoi(_num); break;
			case 4: heavyKilled = stoi(_num); break;
			case 5: staticKilled = stoi(_num); break;
			case 6: shooterKilled = stoi(_num); break;
			default:;
			}
			_num = "";
		}
		else
			_num += _data[i];
	}
}