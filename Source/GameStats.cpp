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