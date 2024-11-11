#pragma once
#include "Utilities.h"
#include "GameMath.h"

using namespace Engine;
using namespace Utilities;

static class DataManager
{
	// storing data for each Npc type
	static DArray<Pair<Vector2, float>> liteNpcData;
	static DArray<Pair<Vector2, float>> heavyNpcData;
	static DArray<Pair<Vector2, float>> staticNpcData;
	static DArray<Pair<Vector2, float>> shooterNpcData;

	// storing player data
	static Pair<Vector2, float> playerData;
	static bool isLoaded; // keeps track of data is loaded or not
	DataManager() = default;

public:
	// retunrs Npc data for given tag
	static DArray<Pair<Vector2, float>>& get_enemy_data(std::string);
	// returns player data
	static Pair<Vector2, float> get_player_data();
	// setterfor npc data with given tag
	static void set_npc_data(std::string, DArray<Pair<Vector2, float>>);
	// setter for player data
	static void set_player_data(Vector2, float);
	// clear all data
	static void clear_data();
	// save data to file (parameter specify to empty file or not)
	static void save_data(bool = false);
	// load data from the file
	static void load_data();
	// returns true if data is loaded successfully
	static bool is_loaded();
};