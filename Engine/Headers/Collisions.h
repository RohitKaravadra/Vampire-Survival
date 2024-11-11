#pragma once
#include<iostream>
#include "GameMath.h"
#include "Utilities.h"

using Engine::Rect;
using Engine::Vector2;
using Utilities::DArray;

// collider class to handle collisions
class Collider
{
protected:
	int id; // object id to identify the object
	std::string tag = "";// identify layers for collision
	Collider() = default;
public:
	bool isActive = true;
	Rect rect;
	// compare tag with given sprite
	bool compare_tag(Collider& _other) const;
	// compare tag with given tag
	bool compare_tag(std::string _tag) const;
	// returns tag of this collider
	std::string get_tag();
	// function called when collided
	virtual void on_collide(std::string _tag) {};
	//function called when collided
	virtual void on_collide(Collider& _other) {};
	// vitual destructor
	virtual ~Collider() = default;
};

// collision class handles collions in game
static class Collisions
{
	static DArray<Collider*> layer0; // Layer 0 colliders
	static DArray<Collider*> layer1; // Layer 1 colliders
	static unsigned int size0; // size of collider collection 0
	static unsigned int size1; // size of collider collection 1

	Collisions() = default;
public:
	// add collider to collisions
	static void add_collider(Collider& _collider, int = 0);
	// remove collider from collisions
	static void remove_collider(Collider& _collider, int = 0);
	// updates collisions
	static void update();
	// remove all colliders from collision
	static void destroy();
	// functions cast a rects to colliders and performs the given task to each
	template<typename Func>
	static void rect_cast(Rect& _rect, Func _todo, int _layer = 0)
	{
		if (_layer == 0)
		{
			layer0.foreach([&_rect, &_todo](Collider* col)
				{
					if (col->isActive && col->rect.collide_as_rect(_rect))
						_todo(col);
				});
		}
		else if (_layer == 1)
		{
			layer1.foreach([&_rect, &_todo](Collider* col)
				{
					if (col->isActive && col->rect.collide_as_rect(_rect))
						_todo(col);
				});
		}
	}
	// functions cast a circle to colliders and performs the given task to each
	template<typename Func>
	static void circle_cast(Rect& _rect, Func _todo, int _layer = 0)
	{
		if (_layer == 0)
		{
			layer0.foreach([&_rect, &_todo](Collider* col)
				{
					if (col->isActive && col->rect.collide_as_circle(_rect))
						_todo(col);
				});
		}
		else if (_layer == 1)
		{
			layer1.foreach([&_rect, &_todo](Collider* col)
				{
					if (col->isActive && col->rect.collide_as_circle(_rect))
						_todo(col);
				});
		}
	}
};