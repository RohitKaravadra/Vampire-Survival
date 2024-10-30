#include "Engine.h"

bool Engine::load_image(Image& image, std::string location)
{
	return image.load(location);
}