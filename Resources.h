#pragma once

#include "Engine.h"
#include <sstream>;

using namespace Engine;

// copy image to the given reference og image
bool load_image(Image& _image, std::string _location);
// returns a copy of the level
void load_level(Dictionary<Vector2, Pair<unsigned int, unsigned int>>& _level);
// save given level to the file
void save_level(Dictionary<Vector2, Pair<unsigned int, unsigned int>>& _level);