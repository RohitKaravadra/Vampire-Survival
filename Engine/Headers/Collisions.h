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
	virtual ~Collider();
};

// collision class handles collions in game
static class Collisions
{
	static DArray<Collider*> colliders;
	static unsigned int size;

	Collisions() = default;
public:
	static void add_collider(Collider& _collider);
	static void remove_collider(Collider& _collider);
	static void update();
	static void destroy();
};