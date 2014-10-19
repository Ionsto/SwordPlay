#pragma once
#include <Tokamak\tokamak.h>
class World;
class Object
{
public:
	neRigidBody * PhysicsBody;
	Object(World * world);
	~Object();
};

