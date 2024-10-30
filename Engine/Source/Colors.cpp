#include "Engine.h"

using namespace Engine;

Color Color::SILVER(192, 192, 192);
Color Color::BLACK(0, 0, 0);
Color Color::GRAY(128, 128, 128);
Color Color::WHITE(255, 255, 255);
Color Color::MAROON(128, 0, 0);
Color Color::RED(255, 0, 0);
Color Color::PURPLE(128, 0, 128);
Color Color::MAGENTA(255, 0, 255);
Color Color::GREEN(0, 128, 0);
Color Color::LIME(0, 255, 0);
Color Color::OLIVE(128, 128, 0);
Color Color::YELLOW(255, 255, 0);
Color Color::NAVY(0, 0, 128);
Color Color::BLUE(0, 0, 255);
Color Color::TEAL(0, 128, 128);
Color Color::AQUA(0, 255, 255);

bool Color::equals(Color b) const
{
	return value[0] == b.value[0] && value[1] == b.value[1] && value[2] == b.value[2];
}
