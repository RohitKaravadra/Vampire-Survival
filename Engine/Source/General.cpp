#include "Engine.h"

using namespace Engine;

bool DEBUG_MODE = false;

int generate_id()
{
	return OBJECT_ID_COUNTER++;
}