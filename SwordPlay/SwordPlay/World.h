#pragma once
#include <vector>
#include "Object.h"
#include <Irrlicht\vector3d.h>
using namespace irr;
using namespace core;
class GameManager;
struct Movement{
public:
	int Id;
	vector3df dPos;
	vector3df dRot;
	Movement(int id, vector3df dpos, vector3df drot)
	{
		Id = id;
		dPos = dpos;
		dRot = drot;
	};
};
class World{
public:
	struct Player
	{
		int PlayerObjectIds[10];
		vector3df QuedBodyMovement;
		vector3df QuedBodyRotation;
		std::vector<Movement> QuedMovment;
	} Player;
	int MeshCount;
	IAnimatedMesh ** Meshes;
	ICameraSceneNode * Camera;
	int ObjectCount;
	Object ** ObjectArray;
	World(GameManager * gm);
	~World();
	void Update(GameManager * gm);
	void MoveCamera(vector3df pos, vector3df rot);
};