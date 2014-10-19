#include "Object.h"
#include "World.h"

Object::Object(World * world)
{
	PhysicsBody = world->m_Sim->CreateRigidBody();
}


Object::~Object()
{
}
