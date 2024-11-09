#pragma once
#include <string>

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
	static void load_stats(std::string _data);
};
