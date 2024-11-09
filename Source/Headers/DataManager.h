#pragma once
#include "Utilities.h"
#include "GameMath.h"

using namespace Engine;
using namespace Utilities;

static class DataManager
{
	static DArray<Pair<Vector2, float>> liteEnemyData;
	static DArray<Pair<Vector2, float>> heavyEnemyData;
	static DArray<Pair<Vector2, float>> staticEnemyData;
	static DArray<Pair<Vector2, float>> shooterEnemyData;

	static Pair<Vector2, float> playerData;
	static bool isLoaded;
	DataManager() = default;

public:
	static DArray<Pair<Vector2, float>>& get_enemy_data(std::string _tag);
	static Pair<Vector2, float> get_player_data();
	static void set_npc_data(std::string, DArray<Pair<Vector2, float>>);
	static void set_player_data(Vector2, float);
	static void clear_data();
	static void save_data(bool = false);
	static void load_data();
	static bool is_loaded();
};