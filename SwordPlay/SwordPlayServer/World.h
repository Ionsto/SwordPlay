#pragma once
#include "Object.h"
#include "Player.h"
#include <vector>
#include <Tokamak\tokamak.h>
class World
{
public:
	std::vector<Player> Players;
	neSimulator *m_Sim;
	int ObjectCount;
	int RigidBodyCount;
	Object ** ObjectArray;
	World();
	~World();
};
