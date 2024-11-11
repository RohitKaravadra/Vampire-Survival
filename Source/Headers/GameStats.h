#pragma once
#include <string>

// struct stores current game stats
static struct GameStats
{
	static unsigned int liteKilled;
	static unsigned int heavyKilled;
	static unsigned int staticKilled;
	static unsigned int shooterKilled;
	static unsigned int wave;
	static double time;

	static void print();
	static void reset();
	// method to load stats when loading data
	static void load_stats(std::string _data);
};
