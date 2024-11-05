#pragma once

#include "Engine.h"

using GamesEngineeringBase::Image;
using Engine::Vector2;

// copy image to the given reference og image
bool load_image(Image& _image, std::string _location);
// returns a copy of the level
void load_level(Dictionary<Vector2, unsigned int>& _level);
// save given level to the file
void save_level(Dictionary<Vector2, unsigned int>& _level);