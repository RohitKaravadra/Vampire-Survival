#include "NPC.h"


void HeavyNpc::create()
{
	load_image(image, "Resources/Joker.jpg");
}

HeavyNpcSwarm::HeavyNpcSwarm(Character& _player) :player(_player)
{
	npc.create();
}

void HeavyNpcSwarm::create(unsigned int _number)
{
	for (unsigned int i = 0; i < _number; i++)
		active.add(Vector2(rand() % 2000, rand() % 2000));
}

void HeavyNpcSwarm::move(float dt)
{
	int _size = active.get_size();
	for (unsigned int i = 0; i < _size; i++)
	{
		
	}
}